/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <utility>

#include "simple_msgs/pose_stamped.h"

namespace simple_msgs
{
PoseStamped::PoseStamped(Header header, Pose pose)
  : pose_(std::move(pose))
  , header_(std::move(header))
{
}

PoseStamped::PoseStamped(const uint8_t* data)
  : pose_(GetPoseStampedFbs(data)->pose()->data())
  , header_(GetPoseStampedFbs(data)->header()->data())
{
}

PoseStamped::PoseStamped(const PoseStamped& other)
  : PoseStamped(other.header_, other.pose_)
{
}

PoseStamped::PoseStamped(PoseStamped&& other) noexcept
  : PoseStamped(other.header_, other.pose_)
{
}

PoseStamped& PoseStamped::operator=(const PoseStamped& p)
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pose_ = p.pose_;
    header_ = p.header_;
    modified_ = true;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(PoseStamped&& p) noexcept
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pose_ = std::move(p.pose_);
    header_ = std::move(p.header_);
    modified_ = true;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPoseStampedFbs(data);
  pose_ = Pose(p->pose()->data());
  header_ = Header(p->header()->data());
  modified_ = true;
  return *this;
}

uint8_t* PoseStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_ || pose_.isModified() || header_.isModified())
  {
    builder_->Clear();
    auto poseVec = builder_->CreateVector(pose_.getBufferData(), pose_.getBufferSize());
    auto headerVec = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    PoseStampedFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_pose(poseVec);
    tmp_builder.add_header(headerVec);
    FinishPoseStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void PoseStamped::setPose(const Pose& pose)
{
  std::lock_guard<std::mutex> lock(mutex_);
  pose_ = pose;
  modified_ = true;
}

void PoseStamped::setHeader(const Header& header)
{
  std::lock_guard<std::mutex> lock(mutex_);
  header_ = header;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const PoseStamped& p)
{
  out << p.header_ << p.pose_;
  return out;
}
}  // namespace simple_msgs
