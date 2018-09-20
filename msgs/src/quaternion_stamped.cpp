/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/quaternion_stamped.h"

namespace simple_msgs {
QuaternionStamped::QuaternionStamped(const Header& header, const Quaternion& quaternion)
  : header_{header}, quaternion_{quaternion} {}

QuaternionStamped::QuaternionStamped(Header&& header, Quaternion&& quaternion)
  : header_{std::move(header)}, quaternion_{std::move(quaternion)} {}

QuaternionStamped::QuaternionStamped(const void* data)
  : header_{GetQuaternionStampedFbs(data)->header()->data()}
  , quaternion_{GetQuaternionStampedFbs(data)->quaternion()->data()} {}

QuaternionStamped::QuaternionStamped(const QuaternionStamped& other, const std::lock_guard<std::mutex>&)
  : QuaternionStamped{other.header_, other.quaternion_} {}

QuaternionStamped::QuaternionStamped(QuaternionStamped&& other, const std::lock_guard<std::mutex>&) noexcept
  : QuaternionStamped{std::move(other.header_), std::move(other.quaternion_)} {}

QuaternionStamped::QuaternionStamped(const QuaternionStamped& other)
  : QuaternionStamped{other, std::lock_guard<std::mutex>(other.mutex_)} {}

QuaternionStamped::QuaternionStamped(QuaternionStamped&& other) noexcept
  : QuaternionStamped{std::forward<QuaternionStamped>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

QuaternionStamped& QuaternionStamped::operator=(const QuaternionStamped& other) {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = other.header_;
    quaternion_ = other.quaternion_;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(QuaternionStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
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
  std::lock_guard<std::mutex> lock{q.mutex_};
  out << q.header_ << q.quaternion_;
  return out;
}
}  // namespace simple_msgs
