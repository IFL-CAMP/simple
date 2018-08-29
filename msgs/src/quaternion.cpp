/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/quaternion.h"

namespace simple_msgs {
Quaternion::Quaternion(double x, double y, double z, double w) : data_{{x, y, z, w}} {}

Quaternion::Quaternion(const std::array<double, 4>& array) : data_{array} {}

Quaternion::Quaternion(std::array<double, 4>&& array) noexcept : data_{std::move(array)} {}

Quaternion::Quaternion(const void* data) {
  auto q = GetQuaternionFbs(data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();
}

Quaternion::Quaternion(const Quaternion& other) : Quaternion{other.data_} {}

Quaternion::Quaternion(Quaternion&& other) noexcept : data_{std::move(other.data_)} {}

Quaternion& Quaternion::operator=(const Quaternion& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = other.data_;
  }
  return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = other.data_;
  }
  return *this;
}

Quaternion& Quaternion::operator=(const std::array<double, 4>& array) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = array;
  return *this;
}

Quaternion& Quaternion::operator=(std::array<double, 4>&& array) noexcept {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = array;
  return *this;
}

Quaternion& Quaternion::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto q = GetQuaternionFbs(*data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Quaternion::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};
  QuaternionFbsBuilder tmp_builder{builder};
  tmp_builder.add_x(data_[0]);
  tmp_builder.add_y(data_[1]);
  tmp_builder.add_z(data_[2]);
  tmp_builder.add_w(data_[3]);
  FinishQuaternionFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Quaternion::setX(double x) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[0] = x;
}

void Quaternion::setY(double y) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[1] = y;
}

void Quaternion::setZ(double z) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[2] = z;
}

void Quaternion::setW(double w) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[3] = w;
}

std::ostream& operator<<(std::ostream& out, const Quaternion& q) {
  out << "Quaternion \n \t"
      << "x: " << std::to_string(q.data_[0]) << "\n \t"
      << "y: " << std::to_string(q.data_[1]) << "\n \t"
      << "z: " << std::to_string(q.data_[2]) << "\n \t"
      << "w: " << std::to_string(q.data_[3]) << "\n";
  return out;
}
}  // namespace simple_msgs
