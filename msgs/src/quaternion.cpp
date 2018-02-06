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

#include "simple_msgs/quaternion.h"

namespace simple_msgs
{
Quaternion::Quaternion(double x, double y, double z, double w)
  : data_{{x, y, z, w}}
{
}

Quaternion::Quaternion(const std::array<double, 4>& array)
  : data_(array)
{
}

Quaternion::Quaternion(std::array<double, 4>&& array) noexcept
  : data_(array)
{
}

Quaternion::Quaternion(const uint8_t* data)

{
  auto q = GetQuaternionFbs(data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();
}

Quaternion::Quaternion(const Quaternion& other)
  : Quaternion(other.data_)
{
}

Quaternion::Quaternion(Quaternion&& other) noexcept
  : data_(other.data_)
{
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
  }
  return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& other) noexcept
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
  }
  return *this;
}

Quaternion& Quaternion::operator=(const std::array<double, 4>& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  return *this;
}

Quaternion& Quaternion::operator=(std::array<double, 4>&& array) noexcept
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  return *this;
}

Quaternion& Quaternion::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto q = GetQuaternionFbs(data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();

  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Quaternion::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto builder = make_unique<flatbuffers::FlatBufferBuilder>(1024);

  QuaternionFbsBuilder tmp_builder(*builder);
  tmp_builder.add_x(data_[0]);
  tmp_builder.add_y(data_[1]);
  tmp_builder.add_z(data_[2]);
  tmp_builder.add_w(data_[3]);
  FinishQuaternionFbsBuffer(*builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
}
void Quaternion::setX(double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[0] = x;
}

void Quaternion::setY(double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[1] = y;
}

void Quaternion::setZ(double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[2] = z;
}

void Quaternion::setW(double w)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[3] = w;
}

std::ostream& operator<<(std::ostream& out, const Quaternion& q)
{
  out << "Quaternion \n \t"
      << "x: " << q.data_[0] << "\n \t"
      << "y: " << q.data_[1] << "\n \t"
      << "z: " << q.data_[2] << "\n \t"
      << "w: " << q.data_[3] << "\n";

  return out;
}
}  // namespace simple_msgs
