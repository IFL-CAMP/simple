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

#include "simple_msgs/pose.h"

namespace simple_msgs {
Pose::Pose(const Point& position, const Quaternion& quaternion) : position_{position}, quaternion_{quaternion} {}

Pose::Pose(Point&& position, Quaternion&& quaternion)
  : position_{std::move(position)}, quaternion_{std::move(quaternion)} {}

Pose::Pose(const void* data)
  : position_{GetPoseFbs(data)->position()->data()}, quaternion_{GetPoseFbs(data)->quaternion()->data()} {}

Pose::Pose(const Pose& other) : Pose{other.position_, other.quaternion_} {}

Pose::Pose(Pose&& other) noexcept : Pose{std::move(other.position_), std::move(other.quaternion_)} {}

Pose& Pose::operator=(const Pose& p) {
  if (this != std::addressof(p)) {
    std::lock_guard<std::mutex> lock{mutex_};
    position_ = p.position_;
    quaternion_ = p.quaternion_;
  }
  return *this;
}

Pose& Pose::operator=(Pose&& p) noexcept {
  if (this != std::addressof(p)) {
    std::lock_guard<std::mutex> lock{mutex_};
    position_ = std::move(p.position_);
    quaternion_ = std::move(p.quaternion_);
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

std::ostream& operator<<(std::ostream& out, const Pose& p) {
  out << "Pose \n \t" << p.position_ << p.quaternion_;
  return out;
}
}  // namespace simple_msgs
