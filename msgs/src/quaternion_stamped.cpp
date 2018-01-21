/**
 * S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
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

namespace simple_msgs
{
QuaternionStamped::QuaternionStamped(Header header, Quaternion quaternion)
  : quaternion_(std::move(quaternion))
  , header_(std::move(header))
{
}

QuaternionStamped::QuaternionStamped(const uint8_t* data)
  : quaternion_(GetQuaternionStampedFbs(data)->quaternion()->data())
  , header_(GetQuaternionStampedFbs(data)->header()->data())
{
}

QuaternionStamped::QuaternionStamped(const QuaternionStamped& other)
  : QuaternionStamped(other.header_, other.quaternion_)
{
}

QuaternionStamped::QuaternionStamped(QuaternionStamped&& other) noexcept
  : QuaternionStamped(other.header_, other.quaternion_)
{
}

QuaternionStamped& QuaternionStamped::operator=(const QuaternionStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    quaternion_ = other.quaternion_;
    header_ = other.header_;
    modified_ = true;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(QuaternionStamped&& other) noexcept
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    quaternion_ = std::move(other.quaternion_);
    header_ = std::move(other.header_);
    modified_ = true;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto matrix = GetQuaternionStampedFbs(data);
  quaternion_ = Quaternion(matrix->quaternion()->data());
  header_ = Header(matrix->header()->data());
  modified_ = true;

  return *this;
}

uint8_t* QuaternionStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_ || header_.isModified() || quaternion_.isModified())
  {
    builder_->Clear();
    auto quaternion_vector = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
    auto header_vector = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    QuaternionStampedFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_quaternion(quaternion_vector);
    tmp_builder.add_header(header_vector);
    FinishQuaternionStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void QuaternionStamped::setQuaternion(const Quaternion& quaternion)
{
  std::lock_guard<std::mutex> lock(mutex_);
  quaternion_ = quaternion;
  modified_ = true;
}

void QuaternionStamped::setHeader(const Header& header)
{
  std::lock_guard<std::mutex> lock(mutex_);
  header_ = header;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const QuaternionStamped& q)
{
  out << q.header_ << q.quaternion_;

  return out;
}
}  // namespace simple_msgs
