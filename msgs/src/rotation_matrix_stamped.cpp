/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/rotation_matrix_stamped.hpp"
#include "simple_msgs/generated/rotation_matrix_stamped_generated.h"

namespace simple_msgs {
RotationMatrixStamped::RotationMatrixStamped(const Header& header, const RotationMatrix& rotation_matrix)
  : header_{header}, rotation_matrix_{rotation_matrix} {}

RotationMatrixStamped::RotationMatrixStamped(Header&& header, RotationMatrix&& rotation_matrix)
  : header_{std::move(header)}, rotation_matrix_{std::move(rotation_matrix)} {}

RotationMatrixStamped::RotationMatrixStamped(const void* data)
  : header_{GetRotationMatrixStampedFbs(data)->header()->data()}
  , rotation_matrix_{GetRotationMatrixStampedFbs(data)->rotation_matrix()->data()} {}

RotationMatrixStamped::RotationMatrixStamped(const RotationMatrixStamped& other, const std::lock_guard<std::mutex>&)
  : RotationMatrixStamped{other.header_, other.rotation_matrix_} {}

RotationMatrixStamped::RotationMatrixStamped(RotationMatrixStamped&& other, const std::lock_guard<std::mutex>&) noexcept
  : RotationMatrixStamped{std::move(other.header_), std::move(other.rotation_matrix_)} {}

RotationMatrixStamped::RotationMatrixStamped(const RotationMatrixStamped& other)
  : RotationMatrixStamped{other, std::lock_guard<std::mutex>(other.mutex_)} {}

RotationMatrixStamped::RotationMatrixStamped(RotationMatrixStamped&& other) noexcept
  : RotationMatrixStamped{std::forward<RotationMatrixStamped>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

RotationMatrixStamped& RotationMatrixStamped::operator=(const RotationMatrixStamped& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    rotation_matrix_ = rhs.rotation_matrix_;
    header_ = rhs.header_;
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(RotationMatrixStamped&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    rotation_matrix_ = std::move(rhs.rotation_matrix_);
    header_ = std::move(rhs.header_);
  }
  return *this;
}

RotationMatrixStamped& RotationMatrixStamped::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto matrix = GetRotationMatrixStampedFbs(*rhs);
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

std::string RotationMatrixStamped::getTopic() { return RotationMatrixStampedFbsIdentifier(); }

void RotationMatrixStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void RotationMatrixStamped::setRotationMatrix(const RotationMatrix& rotation_matrix) {
  std::lock_guard<std::mutex> lock{mutex_};
  rotation_matrix_ = rotation_matrix;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const RotationMatrixStamped& matrix) {
  std::lock_guard<std::mutex> lock{matrix.mutex_};
  out << matrix.header_ << matrix.rotation_matrix_;
  return out;
}
}  // namespace simple_msgs
