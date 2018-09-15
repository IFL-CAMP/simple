/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/point_stamped.h"

namespace simple_msgs {
PointStamped::PointStamped(const Header& header, const Point& point) : header_{header}, point_{point} {}

PointStamped::PointStamped(Header&& header, Point&& point) : header_{std::move(header)}, point_{std::move(point)} {}

PointStamped::PointStamped(const void* data)
  : header_{GetPointStampedFbs(data)->header()->data()}, point_{GetPointStampedFbs(data)->point()->data()} {}

PointStamped::PointStamped(const PointStamped& other, const std::lock_guard<std::mutex>&)
  : PointStamped{other.header_, other.point_} {}

PointStamped::PointStamped(PointStamped&& other, const std::lock_guard<std::mutex>&) noexcept
  : PointStamped{std::move(other.header_), std::move(other.point_)} {}

PointStamped::PointStamped(const PointStamped& other)
  : PointStamped{other, std::lock_guard<std::mutex>(other.mutex_)} {}

PointStamped::PointStamped(PointStamped&& other) noexcept
  : PointStamped{std::forward<PointStamped>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

PointStamped& PointStamped::operator=(const PointStamped& other) {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = other.header_;
    point_ = other.point_;
  }
  return *this;
}

PointStamped& PointStamped::operator=(PointStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = std::move(other.header_);
    point_ = std::move(other.point_);
  }
  return *this;
}

PointStamped& PointStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetPointStampedFbs(*data);
  header_ = p->header()->data();
  point_ = p->point()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> PointStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto header_vector = builder.CreateVector(header_data->data(), header_data->size());

  auto point_data = point_.getBufferData();
  auto point_vector = builder.CreateVector(point_data->data(), point_data->size());

  PointStampedFbsBuilder tmp_builder{builder};
  tmp_builder.add_header(header_vector);
  tmp_builder.add_point(point_vector);
  FinishPointStampedFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void PointStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void PointStamped::setPoint(const Point& point) {
  std::lock_guard<std::mutex> lock{mutex_};
  point_ = point;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const PointStamped& p) {
  std::lock_guard<std::mutex> lock{p.mutex_};
  out << p.header_ << p.point_;
  return out;
}
}  // namespace simple_msgs
