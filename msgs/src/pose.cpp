/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/pose.h"

namespace simple_msgs {
Pose::Pose(const Point& position, const Quaternion& quaternion) : position_{position}, quaternion_{quaternion} {}

Pose::Pose(Point&& position, Quaternion&& quaternion)
  : position_{std::move(position)}, quaternion_{std::move(quaternion)} {}

Pose::Pose(const void* data)
  : position_{GetPoseFbs(data)->position()->data()}, quaternion_{GetPoseFbs(data)->quaternion()->data()} {}

Pose::Pose(const Pose& other, const std::lock_guard<std::mutex>&) : Pose{other.position_, other.quaternion_} {}

Pose::Pose(Pose&& other, const std::lock_guard<std::mutex>&) noexcept
  : Pose{std::move(other.position_), std::move(other.quaternion_)} {}

Pose::Pose(const Pose& other) : Pose{other, std::lock_guard<std::mutex>(other.mutex_)} {}

Pose::Pose(Pose&& other) noexcept : Pose{std::forward<Pose>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

Pose& Pose::operator=(const Pose& other) {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    position_ = other.position_;
    quaternion_ = other.quaternion_;
  }
  return *this;
}

Pose& Pose::operator=(Pose&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    position_ = std::move(other.position_);
    quaternion_ = std::move(other.quaternion_);
  }
  return *this;
}

Pose& Pose::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  position_ = GetPoseFbs(*data)->position()->data();
  quaternion_ = GetPoseFbs(*data)->quaternion()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Pose::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto position_data = position_.getBufferData();
  auto position_vector = builder.CreateVector(position_data->data(), position_data->size());

  auto quaternion_data = quaternion_.getBufferData();
  auto quaternion_vector = builder.CreateVector(quaternion_data->data(), quaternion_data->size());

  PoseFbsBuilder tmp_builder{builder};
  tmp_builder.add_position(position_vector);
  tmp_builder.add_quaternion(quaternion_vector);
  FinishPoseFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Pose::setQuaternion(const Quaternion& quaternion) {
  std::lock_guard<std::mutex> lock{mutex_};
  quaternion_ = quaternion;
}

void Pose::setPosition(const Point& position) {
  std::lock_guard<std::mutex> lock{mutex_};
  position_ = position;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const Pose& p) {
  std::lock_guard<std::mutex> lock{p.mutex_};
  out << "Pose \n \t" << p.position_ << p.quaternion_;
  return out;
}
}  // namespace simple_msgs
