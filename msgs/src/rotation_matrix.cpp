/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/rotation_matrix.hpp"

namespace simple_msgs {
RotationMatrix::RotationMatrix(double value) : data_{{value, value, value, value, value, value, value, value, value}} {}

RotationMatrix::RotationMatrix(double r11, double r12, double r13, double r21, double r22, double r23, double r31,
                               double r32, double r33)
  : data_{{r11, r12, r13, r21, r22, r23, r31, r32, r33}} {}

RotationMatrix::RotationMatrix(const std::array<double, 9>& array) : data_{array} {}

RotationMatrix::RotationMatrix(std::array<double, 9>&& array) noexcept : data_{std::move(array)} {}

RotationMatrix::RotationMatrix(const void* data)
  : data_{{
        GetRotationMatrixFbs(data)->r11(),
        GetRotationMatrixFbs(data)->r12(),
        GetRotationMatrixFbs(data)->r13(),
        GetRotationMatrixFbs(data)->r21(),
        GetRotationMatrixFbs(data)->r22(),
        GetRotationMatrixFbs(data)->r23(),
        GetRotationMatrixFbs(data)->r31(),
        GetRotationMatrixFbs(data)->r32(),
        GetRotationMatrixFbs(data)->r33(),
    }} {}

RotationMatrix::RotationMatrix(const RotationMatrix& other, const std::lock_guard<std::mutex>&)
  : RotationMatrix{other.data_} {}

RotationMatrix::RotationMatrix(RotationMatrix&& other, const std::lock_guard<std::mutex>&) noexcept
  : data_{std::move(other.data_)} {}

RotationMatrix::RotationMatrix(const RotationMatrix& other)
  : RotationMatrix{other, std::lock_guard<std::mutex>(other.mutex_)} {}

RotationMatrix::RotationMatrix(RotationMatrix&& other) noexcept
  : RotationMatrix{std::forward<RotationMatrix>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

RotationMatrix& RotationMatrix::operator=(const RotationMatrix& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = rhs.data_;
  }
  return *this;
}

RotationMatrix& RotationMatrix::operator=(RotationMatrix&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = rhs.data_;
  }
  return *this;
}

RotationMatrix& RotationMatrix::operator=(const std::array<double, 9>& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = rhs;
  return *this;
}

RotationMatrix& RotationMatrix::operator=(std::array<double, 9>&& rhs) noexcept {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = std::move(rhs);
  return *this;
}

RotationMatrix& RotationMatrix::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto matrix = GetRotationMatrixFbs(*data);
  data_ = std::array<double, 9>{{matrix->r11(), matrix->r12(), matrix->r13(), matrix->r21(), matrix->r22(),
                                 matrix->r23(), matrix->r31(), matrix->r32(), matrix->r33()}};
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> RotationMatrix::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  RotationMatrixFbsBuilder tmp_builder{builder};
  tmp_builder.add_r11(data_[0]);
  tmp_builder.add_r12(data_[1]);
  tmp_builder.add_r13(data_[2]);
  tmp_builder.add_r21(data_[3]);
  tmp_builder.add_r22(data_[4]);
  tmp_builder.add_r23(data_[5]);
  tmp_builder.add_r31(data_[6]);
  tmp_builder.add_r32(data_[7]);
  tmp_builder.add_r33(data_[8]);
  FinishRotationMatrixFbsBuffer(builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

RotationMatrix RotationMatrix::getTranspose() const {
  std::lock_guard<std::mutex> lock{mutex_};
  return {data_[0], data_[3], data_[6], data_[1], data_[4], data_[7], data_[2], data_[5], data_[8]};
}

std::array<double, 3> RotationMatrix::getRow(size_t row_index) const {
  std::lock_guard<std::mutex> lock{mutex_};
  if (row_index <= 2) { return {{data_.at(row_index * 3), data_.at(row_index * 3 + 1), data_.at(row_index * 3 + 2)}}; }
  throw std::out_of_range("Index out of range [0,2]");
}

std::array<double, 3> RotationMatrix::getColumn(size_t column_index) const {
  std::lock_guard<std::mutex> lock{mutex_};
  if (column_index <= 2) {
    return {{data_.at(column_index), data_.at(3 * 1 + column_index), data_.at(3 * 2 + column_index)}};
  }
  throw std::out_of_range("Index out of range [0,2]");
}

void RotationMatrix::setRow(size_t row_index, const std::array<double, 3>& values) {
  std::lock_guard<std::mutex> lock{mutex_};
  if (row_index <= 2) {
    for (size_t i = 0; i < values.size(); ++i) { data_.at(row_index * 3 + i) = values.at(i); }
  } else {
    throw std::out_of_range("Index out of range [0,2]");
  }
}

void RotationMatrix::setColumn(size_t column_index, const std::array<double, 3>& values) {
  std::lock_guard<std::mutex> lock{mutex_};
  if (column_index <= 2) {
    for (size_t i = 0; i < values.size(); ++i) { data_.at(3 * i + column_index) = values.at(i); }
  } else {
    throw std::out_of_range("Index out of range [0,2]");
  }
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const RotationMatrix& matrix) {
  std::lock_guard<std::mutex> lock{matrix.mutex_};
  out << "RotationMatrix \n \t" << std::to_string(matrix.data_[0]) << " " << std::to_string(matrix.data_[1]) << " "
      << std::to_string(matrix.data_[2]) << "\n \t" << std::to_string(matrix.data_[3]) << " "
      << std::to_string(matrix.data_[4]) << " " << std::to_string(matrix.data_[5]) << "\n \t"
      << std::to_string(matrix.data_[6]) << " " << std::to_string(matrix.data_[7]) << " "
      << std::to_string(matrix.data_[8]) << "\n";

  return out;
}
}  // namespace simple_msgs
