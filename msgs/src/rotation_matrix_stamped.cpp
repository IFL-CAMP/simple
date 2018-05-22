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

#include "simple_msgs/rotation_matrix_stamped.h"

namespace simple_msgs {
RotationMatrixStamped::RotationMatrixStamped(const Header& header, const RotationMatrix& rotation_matrix)
  : header_{header}, rotation_matrix_{rotation_matrix} {}

RotationMatrixStamped::RotationMatrixStamped(Header&& header, RotationMatrix&& rotation_matrix)
  : header_{std::move(header)}, rotation_matrix_{std::move(rotation_matrix)} {}

RotationMatrixStamped::RotationMatrixStamped(const void* data)
  : header_{GetRotationMatrixStampedFbs(data)->header()->data()}
  , rotation_matrix_{GetRotationMatrixStampedFbs(data)->rotation_matrix()->data()} {}

RotationMatrixStamped::RotationMatrixStamped(const RotationMatrixStamped& other)
  : RotationMatrixStamped{other.header_, other.rotation_matrix_} {}

RotationMatrixStamped::RotationMatrixStamped(RotationMatrixStamped&& other) noexcept
  : RotationMatrixStamped{std::move(other.header_), std::move(other.rotation_matrix_)} {}

RotationMatrixStamped& RotationMatrixStamped::operator=(const RotationMatrixStamped& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    rotation_matrix_ = other.rotation_matrix_;
    header_ = other.header_;
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(RotationMatrixStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    rotation_matrix_ = std::move(other.rotation_matrix_);
    header_ = std::move(other.header_);
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto matrix = GetRotationMatrixStampedFbs(*data);
  rotation_matrix_ = matrix->rotation_matrix()->data();
  header_ = matrix->header()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> RotationMatrixStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto header_vector = builder.CreateVector(header_data->data(), header_data->size());

  auto matrix_data = rotation_matrix_.getBufferData();
  auto matrix_vector = builder.CreateVector(matrix_data->data(), matrix_data->size());

  RotationMatrixStampedFbsBuilder tmp_builder{builder};
  tmp_builder.add_header(header_vector);
  tmp_builder.add_rotation_matrix(matrix_vector);
  FinishRotationMatrixStampedFbsBuffer(builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void RotationMatrixStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void RotationMatrixStamped::setRotationMatrix(const RotationMatrix& rotation_matrix) {
  std::lock_guard<std::mutex> lock{mutex_};
  rotation_matrix_ = rotation_matrix;
}

std::ostream& operator<<(std::ostream& out, const RotationMatrixStamped& q) {
  out << q.header_ << q.rotation_matrix_;
  return out;
}
}  // namespace simple_msgs
