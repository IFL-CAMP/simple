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

#include "simple_msgs/quaternion_stamped.h"

namespace simple_msgs {
QuaternionStamped::QuaternionStamped(const Header& header, const Quaternion& quaternion)
  : header_{header}, quaternion_{quaternion} {}

QuaternionStamped::QuaternionStamped(Header&& header, Quaternion&& quaternion)
  : header_{std::move(header)}, quaternion_{std::move(quaternion)} {}

QuaternionStamped::QuaternionStamped(const void* data)
  : header_{GetQuaternionStampedFbs(data)->header()->data()}
  , quaternion_{GetQuaternionStampedFbs(data)->quaternion()->data()} {}

QuaternionStamped::QuaternionStamped(const QuaternionStamped& other)
  : QuaternionStamped{other.header_, other.quaternion_} {}

QuaternionStamped::QuaternionStamped(QuaternionStamped&& other) noexcept
  : QuaternionStamped{std::move(other.header_), std::move(other.quaternion_)} {}

QuaternionStamped& QuaternionStamped::operator=(const QuaternionStamped& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    header_ = other.header_;
    quaternion_ = other.quaternion_;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(QuaternionStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    quaternion_ = std::move(other.quaternion_);
    header_ = std::move(other.header_);
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto matrix = GetQuaternionStampedFbs(*data);
  header_ = matrix->header()->data();
  quaternion_ = matrix->quaternion()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> QuaternionStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto header_vector = builder.CreateVector(header_data->data(), header_data->size());

  auto quaternion_data = quaternion_.getBufferData();
  auto quaternion_vector = builder.CreateVector(quaternion_data->data(), quaternion_data->size());

  QuaternionStampedFbsBuilder tmp_builder(builder);
  tmp_builder.add_header(header_vector);
  tmp_builder.add_quaternion(quaternion_vector);
  FinishQuaternionStampedFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void QuaternionStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void QuaternionStamped::setQuaternion(const Quaternion& quaternion) {
  std::lock_guard<std::mutex> lock{mutex_};
  quaternion_ = quaternion;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const QuaternionStamped& q) {
  out << q.header_ << q.quaternion_;

  return out;
}
}  // namespace simple_msgs
