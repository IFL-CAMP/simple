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

#include "simple_msgs/transform_stamped.h"

namespace simple_msgs {
TransformStamped::TransformStamped(const Header& header, const Transform& matrix)
  : header_{header}, transform_{matrix} {}

TransformStamped::TransformStamped(Header&& header, Transform&& matrix)
  : header_{std::move(header)}, transform_{std::move(matrix)} {}

TransformStamped::TransformStamped(const void* data)
  : header_{GetTransformStampedFbs(data)->header()->data()}
  , transform_{GetTransformStampedFbs(data)->transform()->data()} {}

TransformStamped::TransformStamped(const TransformStamped& other, const std::lock_guard<std::mutex>&)
  : TransformStamped{other.header_, other.transform_} {}

TransformStamped::TransformStamped(TransformStamped&& other, const std::lock_guard<std::mutex>&) noexcept
  : TransformStamped{std::move(other.header_), std::move(other.transform_)} {}

TransformStamped::TransformStamped(const TransformStamped& other)
  : TransformStamped{other, std::lock_guard<std::mutex>(other.mutex_)} {}

TransformStamped::TransformStamped(TransformStamped&& other) noexcept
  : TransformStamped{std::forward<TransformStamped>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

TransformStamped& TransformStamped::operator=(const TransformStamped& other) {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = other.header_;
    transform_ = other.transform_;
  }
  return *this;
}

TransformStamped& TransformStamped::operator=(TransformStamped&& other) noexcept {
  if (this != std::addressof(other)) {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    header_ = std::move(other.header_);
    transform_ = std::move(other.transform_);
  }
  return *this;
}

TransformStamped& TransformStamped::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetTransformStampedFbs(*data);
  header_ = p->header()->data();
  transform_ = p->transform()->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> TransformStamped::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto header_data = header_.getBufferData();
  auto header_vector = builder.CreateVector(header_data->data(), header_data->size());

  auto transform_data = transform_.getBufferData();
  auto transform_vector = builder.CreateVector(transform_data->data(), transform_data->size());

  TransformStampedFbsBuilder tmp_builder{builder};
  tmp_builder.add_header(header_vector);
  tmp_builder.add_transform(transform_vector);
  FinishTransformStampedFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void TransformStamped::setHeader(const Header& header) {
  std::lock_guard<std::mutex> lock{mutex_};
  header_ = header;
}

void TransformStamped::setTransform(const Transform& transform) {
  std::lock_guard<std::mutex> lock{mutex_};
  transform_ = transform;
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const TransformStamped& t) {
  std::lock_guard<std::mutex> lock{t.mutex_};
  out << t.header_ << t.transform_;
  return out;
}
}  // namespace simple_msgs
