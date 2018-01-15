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

#include "simple_msgs/point.h"

namespace simple_msgs
{
	Point::Point()
		: GenericMessage()
	{
	}

	Point::Point(double value)
		: GenericMessage()
		, data_{{ value, value, value }}
{
}

Point::Point(double x, double y, double z)
: GenericMessage()
, data_{ { x, y, z } }
{
}

Point::Point(const std::array<double, 3>& array)
  : GenericMessage()
  , data_(array)
{
}

Point::Point(std::array<double, 3>&& array)
  : GenericMessage()
  , data_(std::move(array))
{
}

Point::Point(const uint8_t* data)
  : GenericMessage()
{
  auto p = GetPointFbs(data);
  data_[0] = p->x();
  data_[1] = p->y();
  data_[2] = p->z();
}

Point::Point(const Point& other)
  : Point(other.data_)
{
}

Point::Point(Point&& other)
  : GenericMessage()
  , data_(std::move(other.data_))
{
}

Point& Point::operator=(const Point& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
    modified_ = true;
  }
  return *this;
}

Point& Point::operator=(Point&& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = std::move(other.data_);
    modified_ = true;
  }
  return *this;
}

Point& Point::operator=(const std::array<double, 3>& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  modified_ = true;
  return *this;
}

Point& Point::operator=(std::array<double, 3>&& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = std::move(array);
  modified_ = true;
  return *this;
}

Point& Point::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPointFbs(data);
  data_[0] = p->x();
  data_[1] = p->y();
  data_[2] = p->z();
  modified_ = true;

  return *this;
}

Point& Point::operator+=(const Point& rhs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::plus<double>());
  modified_ = true;
  return *this;
}

Point operator+(Point lhs, const Point& rhs)
{
  lhs += rhs;
  return lhs;
}

Point& Point::operator-=(const Point& rhs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::minus<double>());
  modified_ = true;
  return *this;
}

Point operator-(Point lhs, const Point& rhs)
{
  lhs -= rhs;
  return lhs;
}

Point& Point::operator*=(const Point& rhs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_),
                 std::multiplies<double>());
  modified_ = true;
  return *this;
}

Point operator*(Point lhs, const Point& rhs)
{
  lhs *= rhs;
  return lhs;
}

Point& Point::operator/=(const Point& rhs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::divides<double>());
  modified_ = true;
  return *this;
}

Point operator/(Point lhs, const Point& rhs)
{
  lhs /= rhs;
  return lhs;
}

uint8_t* Point::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    PointFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_x(data_[0]);
    tmp_builder.add_y(data_[1]);
    tmp_builder.add_z(data_[2]);
    FinishPointFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Point::setX(double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[0] = x;
  modified_ = true;
}

void Point::setY(double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[1] = y;
  modified_ = true;
}
void Point::setZ(double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[2] = z;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
  out << "Point \n \t"
      << "x: " << p.data_[0] << "\n \t"
      << "y: " << p.data_[1] << "\n \t"
      << "z: " << p.data_[2] << "\n";

  return out;
}
}
