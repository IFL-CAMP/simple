/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/point.h"

namespace simple_msgs {
Point::Point(double value) : data_{{value, value, value}} {}

Point::Point(double x, double y, double z) : data_{{x, y, z}} {}

Point::Point(const std::array<double, 3>& array) : data_{array} {}

Point::Point(std::array<double, 3>&& array) noexcept : data_{std::move(array)} {}

Point::Point(const void* data) {
  auto p = GetPointFbs(data);
  data_[0] = p->x();
  data_[1] = p->y();
  data_[2] = p->z();
}

Point::Point(const Point& other, const std::lock_guard<std::mutex>&) : Point{other.data_} {}

Point::Point(Point&& other, const std::lock_guard<std::mutex>&) noexcept : data_{std::move(other.data_)} {}

Point::Point(const Point& other) : Point{other, std::lock_guard<std::mutex>(other.mutex_)} {}

Point::Point(Point&& other) noexcept : Point{std::forward<Point>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

Point& Point::operator=(const Point& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = rhs.data_;
  }
  return *this;
}

Point& Point::operator=(Point&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = std::move(rhs.data_);
  }
  return *this;
}

Point& Point::operator=(const std::array<double, 3>& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = rhs;
  return *this;
}

Point& Point::operator=(std::array<double, 3>&& rhs) noexcept {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = std::move(rhs);
  return *this;
}

Point& Point::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetPointFbs(*rhs);
  data_ = std::array<double, 3>{{p->x(), p->y(), p->z()}};
  return *this;
}

Point& Point::operator++() {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(data_), [](double e) { return ++e; });
  return *this;
}

Point Point::operator++(int) {
  Point tmp(*this);
  operator++();
  return tmp;
}

Point& Point::operator--() {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(data_), [](double e) { return --e; });
  return *this;
}

Point Point::operator--(int) {
  Point tmp(*this);
  operator--();
  return tmp;
}

Point& Point::operator+=(const Point& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::plus<double>());
  return *this;
}

/**
 * @brief Addition operator.
 */
Point operator+(Point lhs, const Point& rhs) {
  lhs += rhs;
  return lhs;
}

Point& Point::operator-=(const Point& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::minus<double>());
  return *this;
}

/**
 * @brief Subtraction operator.
 */
Point operator-(Point lhs, const Point& rhs) {
  lhs -= rhs;
  return lhs;
}

Point& Point::operator*=(const Point& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_),
                 std::multiplies<double>());
  return *this;
}

/**
 * @brief Multiplication operator.
 */
Point operator*(Point lhs, const Point& rhs) {
  lhs *= rhs;
  return lhs;
}

Point& Point::operator/=(const Point& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  std::transform(std::begin(data_), std::end(data_), std::begin(rhs.data_), std::begin(data_), std::divides<double>());
  return *this;
}

/**
 * @brief Division operator.
 */
Point operator/(Point lhs, const Point& rhs) {
  lhs /= rhs;
  return lhs;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Point::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};
  PointFbsBuilder tmp_builder{builder};
  tmp_builder.add_x(data_[0]);
  tmp_builder.add_y(data_[1]);
  tmp_builder.add_z(data_[2]);
  FinishPointFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Point::setX(double x) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[0] = x;
}

void Point::setY(double y) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[1] = y;
}
void Point::setZ(double z) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_[2] = z;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const Point& p) {
  std::lock_guard<std::mutex> lock{p.mutex_};
  out << "Point \n \t"
      << "x: " << std::to_string(p.data_[0]) << "\n \t"
      << "y: " << std::to_string(p.data_[1]) << "\n \t"
      << "z: " << std::to_string(p.data_[2]) << "\n";

  return out;
}
}  // namespace simple_msgs
