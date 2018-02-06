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

#include "simple_msgs/rotation_matrix.h"

namespace simple_msgs
{
RotationMatrix::RotationMatrix(double value)
  : data_{{value, value, value, value, value, value, value, value, value}}
{
}

RotationMatrix::RotationMatrix(double r11, double r12, double r13, double r21, double r22, double r23, double r31,
                               double r32, double r33)
  : data_{{r11, r12, r13, r21, r22, r23, r31, r32, r33}}
{
}

RotationMatrix::RotationMatrix(const std::array<double, 9>& array)
  : data_(array)
{
}

RotationMatrix::RotationMatrix(std::array<double, 9>&& array) noexcept
  : data_(array)
{
}

RotationMatrix::RotationMatrix(const uint8_t* data)
{
  auto r = GetRotationMatrixFbs(data);
  data_ =
      std::array<double, 9>{{r->r11(), r->r12(), r->r13(), r->r21(), r->r22(), r->r23(), r->r31(), r->r32(), r->r33()}};
}

RotationMatrix::RotationMatrix(const RotationMatrix& m)
  : RotationMatrix(m.data_)
{
}

RotationMatrix::RotationMatrix(RotationMatrix&& m) noexcept
  : RotationMatrix(m.data_)
{
}

RotationMatrix& RotationMatrix::operator=(const RotationMatrix& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
  }
  return *this;
}

RotationMatrix& RotationMatrix::operator=(RotationMatrix&& other) noexcept
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
  }
  return *this;
}

RotationMatrix& RotationMatrix::operator=(const std::array<double, 9>& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  return *this;
}

RotationMatrix& RotationMatrix::operator=(std::array<double, 9>&& array) noexcept
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  return *this;
}

RotationMatrix& RotationMatrix::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto matrix = GetRotationMatrixFbs(data);
  data_ = std::array<double, 9>{{matrix->r11(), matrix->r12(), matrix->r13(), matrix->r21(), matrix->r22(),
                                 matrix->r23(), matrix->r31(), matrix->r32(), matrix->r33()}};
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> RotationMatrix::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto builder = make_unique<flatbuffers::FlatBufferBuilder>(1024);

  RotationMatrixFbsBuilder tmp_builder(*builder);
  tmp_builder.add_r11(data_[0]);
  tmp_builder.add_r12(data_[1]);
  tmp_builder.add_r13(data_[2]);
  tmp_builder.add_r21(data_[3]);
  tmp_builder.add_r22(data_[4]);
  tmp_builder.add_r23(data_[5]);
  tmp_builder.add_r31(data_[6]);
  tmp_builder.add_r32(data_[7]);
  tmp_builder.add_r33(data_[8]);
  FinishRotationMatrixFbsBuffer(*builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
}

RotationMatrix RotationMatrix::getTranspose() const
{
  return RotationMatrix(data_[0], data_[3], data_[6], data_[1], data_[4], data_[7], data_[2], data_[5], data_[8]);
}

std::array<double, 3> RotationMatrix::getRow(int row_index) const
{
  if (0 <= row_index && row_index <= 2)
  {
    return std::array<double, 3>{{data_.at(row_index * 3), data_.at(row_index * 3 + 1), data_.at(row_index * 3 + 2)}};
  }

  throw std::out_of_range("Index out of range [0,2]");
}

std::array<double, 3> RotationMatrix::getColumn(int column_index) const
{
  if (0 <= column_index && column_index <= 2)
  {
    return std::array<double, 3>{
        {data_.at(column_index), data_.at(3 * 1 + column_index), data_.at(3 * 2 + column_index)}};
  }

  throw std::out_of_range("Index out of range [0,2]");
}
void RotationMatrix::setRow(int row_index, const std::array<double, 3>& values)
{
  for (size_t i = 0; i < values.size(); ++i)
  {
    data_.at(row_index * 3 + i) = values.at(i);
  }
}

void RotationMatrix::setColumn(int column_index, const std::array<double, 3>& values)
{
  for (size_t i = 0; i < values.size(); ++i)
  {
    data_.at(3 * i + column_index) = values.at(i);
  }
}

std::ostream& operator<<(std::ostream& out, const RotationMatrix& q)
{
  out << "RotationMatrix \n \t" << q.data_[0] << " " << q.data_[1] << " " << q.data_[2] << "\n \t" << q.data_[3] << " "
      << q.data_[4] << " " << q.data_[5] << "\n \t" << q.data_[6] << " " << q.data_[7] << " " << q.data_[8] << "\n";

  return out;
}

const RotationMatrix& RotationMatrix::Identity()
{
  static const RotationMatrix identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
  return identity;
}
}  // namespace simple_msgs
