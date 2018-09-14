/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/pose_stamped.h"

namespace simple_msgs {
PoseStamped::PoseStamped(const Header& header, const Pose& pose) : header_{header}, pose_{pose} {}

PoseStamped::PoseStamped(Header&& header, Pose&& pose) : header_{std::move(header)}, pose_{std::move(pose)} {}

PoseStamped::PoseStamped(const void* data)
  : header_{GetPoseStampedFbs(data)->header()->data()}  // namespace simple_msgs
  , pose_{GetPoseStampedFbs(data)->pose()->data()} {}

PoseStamped::PoseStamped(const PoseStamped& other, const std::lock_guard<std::mutex>&)
  : PoseStamped{other.header_, other.pose_} {}

PoseStamped::PoseStamped(PoseStamped&& other, const std::lock_guard<std::mutex>&) noexcept
  : PoseStamped{std::move(other.header_), std::move(other.pose_)} {}

PoseStamped::PoseStamped(const PoseStamped& other) : PoseStamped{other, std::lock_guard<std::mutex>(other.mutex_)} {}

PoseStamped::PoseStamped(PoseStamped&& other) noexcept
  : PoseStamped{std::forward<PoseStamped>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

PoseStamped& PoseStamped::operator=(const PoseStamped& other) {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = other.header_;
    pose_ = other.pose_;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(PoseStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = std::move(other.header_);
    pose_ = std::move(other.pose_);
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
  std::lock_guard<std::mutex> lock{p.mutex_};
  out << p.header_ << p.pose_;
  return out;
}
}  // namespace simple_msgs
