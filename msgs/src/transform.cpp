/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/transform.hpp"

namespace simple_msgs {
Transform::Transform(const Point& point, const RotationMatrix& rotation_matrix)
  : point_{point}, matrix_{rotation_matrix} {}

Transform::Transform(Point&& point, RotationMatrix&& rotation_matrix)
  : point_{std::move(point)}, matrix_{std::move(rotation_matrix)} {}

Transform::Transform(const std::array<double, 16>& data)
  : point_{data[3], data[7], data[11]}
  , matrix_{data[0], data[1], data[2], data[4], data[5], data[6], data[8], data[9], data[10]} {}

Transform::Transform(std::array<double, 16>&& data) noexcept
  : point_{data[3], data[7], data[11]}
  , matrix_{data[0], data[1], data[2], data[4], data[5], data[6], data[8], data[9], data[10]} {}

Transform::Transform(const void* data)
  : point_{GetTransformFbs(data)->point()->data()}, matrix_{GetTransformFbs(data)->matrix()->data()} {}

Transform::Transform(const Transform& other, const std::lock_guard<std::mutex>&)
  : Transform{other.point_, other.matrix_} {}

Transform::Transform(Transform&& other, const std::lock_guard<std::mutex>&) noexcept
  : point_{std::move(other.point_)}, matrix_{std::move(other.matrix_)} {}

Transform::Transform(const Transform& other) : Transform{other, std::lock_guard<std::mutex>(other.mutex_)} {}

Transform::Transform(Transform&& other) noexcept
  : Transform{std::forward<Transform>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

Transform& Transform::operator=(const Transform& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    point_ = rhs.point_;
    matrix_ = rhs.matrix_;
  }
  return *this;
}

Transform& Transform::operator=(Transform&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    point_ = std::move(rhs.point_);
    matrix_ = std::move(rhs.matrix_);
  }
  return *this;
}

Transform& Transform::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetTransformFbs(*rhs);
  point_ = p->point()->data();
  matrix_ = p->matrix()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Transform::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto point_data = point_.getBufferData();
  auto point_vector = builder.CreateVector(point_data->data(), point_data->size());

  auto matrix_data = matrix_.getBufferData();
  auto matrix_vector = builder.CreateVector(matrix_data->data(), matrix_data->size());

  TransformFbsBuilder tmp_builder{builder};
  tmp_builder.add_point(point_vector);
  tmp_builder.add_matrix(matrix_vector);
  FinishTransformFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Transform::setTranslation(const Point& point) {
  std::lock_guard<std::mutex> lock{mutex_};
  point_ = point;
}

void Transform::setRotation(const RotationMatrix& rotation_matrix) {
  std::lock_guard<std::mutex> lock{mutex_};
  matrix_ = rotation_matrix;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const Transform& t) {
  std::lock_guard<std::mutex> lock{t.mutex_};
  auto vectorized_matrix = t.matrix_.toVector();
  out << "Transform \n \t" << std::to_string(vectorized_matrix[0]) << " " << std::to_string(vectorized_matrix[1]) << " "
      << std::to_string(vectorized_matrix[2]) << " " << std::to_string(t.point_.getX()) << "\n \t"
      << std::to_string(vectorized_matrix[3]) << " " << std::to_string(vectorized_matrix[4]) << " "
      << std::to_string(vectorized_matrix[5]) << " " << std::to_string(t.point_.getY()) << "\n \t"
      << std::to_string(vectorized_matrix[6]) << " " << std::to_string(vectorized_matrix[7]) << " "
      << std::to_string(vectorized_matrix[8]) << " " << std::to_string(t.point_.getZ()) << "\n"
      << "0 0 0 1 \n";
  return out;
}
}  // namespace simple_msgs
