/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/image.hpp"

namespace simple_msgs {
template <>
data Image<uint8_t>::getDataUnionType() const {
  return data_uint8_type;
}

template <>
data Image<int16_t>::getDataUnionType() const {
  return data_int16_type;
}

template <>
data Image<float>::getDataUnionType() const {
  return data_float_type;
}

template <>
data Image<double>::getDataUnionType() const {
  return data_double_type;
}

template <>
flatbuffers::Offset<void> Image<uint8_t>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createuint8_type(*builder, builder->CreateVector(data_.getData(), static_cast<size_t>(data_size_))).Union();
}

template <>
flatbuffers::Offset<void> Image<int16_t>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createint16_type(*builder, builder->CreateVector(data_.getData(), static_cast<size_t>(data_size_))).Union();
}

template <>
flatbuffers::Offset<void> Image<float>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createfloat_type(*builder, builder->CreateVector(data_.getData(), static_cast<size_t>(data_size_))).Union();
}

template <>
flatbuffers::Offset<void> Image<double>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createdouble_type(*builder, builder->CreateVector(data_.getData(), static_cast<size_t>(data_size_))).Union();
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<uint8_t>& Image<uint8_t>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*data);

  Image<uint8_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_uint8_type())->raw()->data();
    data_.setData({data, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<int16_t>& Image<int16_t>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*data);

  Image<int16_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_int16_type())->raw()->data();
    data_.setData({data, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<float>& Image<float>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*data);

  Image<float>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_float_type())->raw()->data();
    data_.setData({data, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<double>& Image<double>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*data);

  Image<double>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_double_type())->raw()->data();
    data_.setData({data, local_data});
  }
  return *this;
}
}  // namespace simple_msgs
