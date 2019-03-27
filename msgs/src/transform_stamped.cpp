/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/transform_stamped.h"

namespace simple_msgs {
TransformStamped::TransformStamped(const Header& header, const Transform& transform)
  : header_{header}, transform_{transform} {}

TransformStamped::TransformStamped(Header&& header, Transform&& transform)
  : header_{std::move(header)}, transform_{std::move(transform)} {}

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

TransformStamped& TransformStamped::operator=(const TransformStamped& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    header_ = rhs.header_;
    transform_ = rhs.transform_;
  }
  return *this;
}

TransformStamped& TransformStamped::operator=(TransformStamped&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    header_ = std::move(rhs.header_);
    transform_ = std::move(rhs.transform_);
  }
  return *this;
}

TransformStamped& TransformStamped::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto p = GetTransformStampedFbs(*rhs);
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
