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

namespace simple_msgs {
PoseStamped::PoseStamped(const Header& header, const Pose& pose) : header_{header}, pose_{pose} {}

PoseStamped::PoseStamped(Header&& header, Pose&& pose) : header_{std::move(header)}, pose_{std::move(pose)} {}

PoseStamped::PoseStamped(const void* data)
  : header_{GetPoseStampedFbs(data)->header()->data()}  // namespace simple_msgs
  , pose_{GetPoseStampedFbs(data)->pose()->data()} {}

PoseStamped::PoseStamped(const PoseStamped& other) : PoseStamped{other.header_, other.pose_} {}

PoseStamped::PoseStamped(PoseStamped&& other) noexcept
  : PoseStamped{std::move(other.header_), std::move(other.pose_)} {}

PoseStamped& PoseStamped::operator=(const PoseStamped& p) {
  if (this != std::addressof(p)) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = p.header_;
    pose_ = p.pose_;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(PoseStamped&& p) noexcept {
  if (this != std::addressof(p)) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = std::move(p.header_);
    pose_ = std::move(p.pose_);
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetPoseStampedFbs(*data);
  header_ = p->header()->data();
  pose_ = p->pose()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> PoseStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto headerVec = builder.CreateVector(header_data->data(), header_data->size());

  auto pose_data = pose_.getBufferData();
  auto poseVec = builder.CreateVector(pose_data->data(), pose_data->size());

  PoseStampedFbsBuilder tmp_builder{builder};
  tmp_builder.add_header(headerVec);
  tmp_builder.add_pose(poseVec);
  FinishPoseStampedFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void PoseStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void PoseStamped::setPose(const Pose& pose) {
  std::lock_guard<std::mutex> lock{mutex_};
  pose_ = pose;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const PoseStamped& p) {
  out << p.header_ << p.pose_;
  return out;
}
}  // namespace simple_msgs
