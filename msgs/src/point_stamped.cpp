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

#include "simple_msgs/point_stamped.h"

namespace simple_msgs
{
PointStamped::PointStamped(Header header, Point point)
  : point_(std::move(point))
  , header_(std::move(header))
{
}

PointStamped::PointStamped(const uint8_t* data)
  : point_(GetPointStampedFbs(data)->point()->data())
  , header_(GetPointStampedFbs(data)->header()->data())
{
}

PointStamped::PointStamped(const PointStamped& other)
  : PointStamped(other.header_, other.point_)
{
}

PointStamped::PointStamped(PointStamped&& other) noexcept
  : point_(std::move(other.point_))
  , header_(std::move(other.header_))
{
}

PointStamped& PointStamped::operator=(const PointStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    point_ = other.point_;
    header_ = other.header_;
  }
  return *this;
}

PointStamped& PointStamped::operator=(PointStamped&& other) noexcept
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    point_ = std::move(other.point_);
    header_ = std::move(other.header_);
  }
  return *this;
}

PointStamped& PointStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPointStampedFbs(data);
  point_ = p->point()->data();
  header_ = p->header()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> PointStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto builder = std::unique_ptr<flatbuffers::FlatBufferBuilder>(new flatbuffers::FlatBufferBuilder(1024));

  auto point_data = point_.getBufferData();
  auto point_vector = builder->CreateVector(point_data->data(), point_data->size());

  auto header_data = header_.getBufferData();
  auto header_vector = builder->CreateVector(header_data->data(), header_data->size());

  PointStampedFbsBuilder tmp_builder(*builder);
  tmp_builder.add_point(point_vector);
  tmp_builder.add_header(header_vector);
  FinishPointStampedFbsBuffer(*builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
}

void PointStamped::setHeader(const Header& h)
{
  std::lock_guard<std::mutex> lock(mutex_);
  header_ = h;
}

void PointStamped::setPoint(const Point& p)
{
  std::lock_guard<std::mutex> lock(mutex_);
  point_ = p;
}

std::ostream& operator<<(std::ostream& out, const PointStamped& p)
{
  out << p.header_ << p.point_;

  return out;
}
}  // namespace simple_msgs
