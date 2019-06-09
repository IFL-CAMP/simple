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
#include "simple_msgs/generated/image_generated.h"

namespace simple_msgs {

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
Image<uint8_t>& Image<uint8_t>::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*rhs);

  Image<uint8_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_uint8_type())->raw()->data();
    data_.setData({rhs, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<int16_t>& Image<int16_t>::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*rhs);

  Image<int16_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_int16_type())->raw()->data();
    data_.setData({rhs, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<float>& Image<float>::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*rhs);

  Image<float>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_float_type())->raw()->data();
    data_.setData({rhs, local_data});
  }
  return *this;
}

/**
 * @brief Copy assignment operator that uses raw memory coming from the network.
 */
template <>
Image<double>& Image<double>::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  auto image_data = GetImageFbs(*rhs);

  Image<double>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE)) {
    auto local_data = (image_data->image_as_double_type())->raw()->data();
    data_.setData({rhs, local_data});
  }
  return *this;
}

template <typename T>
std::shared_ptr<flatbuffers::DetachedBuffer> Image<T>::getBufferData(
    const std::shared_ptr<flatbuffers::FlatBufferBuilder>& builder, const simple_msgs::dataTraits<T>& data_type,
    flatbuffers::Offset<void> offset) const {
  auto encoding_string = builder->CreateString(encoding_);

  auto header_data = header_.getBufferData();
  auto header_vector = builder->CreateVector(header_data->data(), header_data->size());

  auto origin_data = origin_.getBufferData();
  auto origin_vector = builder->CreateVector(origin_data->data(), origin_data->size());

  ImageFbsBuilder tmp_builder{*builder};
  // add the information
  tmp_builder.add_encoding(encoding_string);
  tmp_builder.add_header(header_vector);
  tmp_builder.add_origin(origin_vector);
  if (!data_.empty()) { tmp_builder.add_image(offset); }
  tmp_builder.add_image_type(data_type.enum_value);
  tmp_builder.add_image_size(data_size_);
  tmp_builder.add_spacing_x(spacing_x_);
  tmp_builder.add_spacing_y(spacing_y_);
  tmp_builder.add_spacing_z(spacing_z_);
  tmp_builder.add_height(height_);
  tmp_builder.add_width(width_);
  tmp_builder.add_depth(depth_);
  tmp_builder.add_num_channels(num_channels_);
  FinishImageFbsBuffer(*builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder->Release());
}

template <>
std::shared_ptr<flatbuffers::DetachedBuffer> Image<uint8_t>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  auto builder = std::make_shared<flatbuffers::FlatBufferBuilder>(1024);

  simple_msgs::dataTraits<uint8_t> data_type;
  flatbuffers::Offset<void> offset{};
  if (!data_.empty()) { offset = getDataUnionElem(builder); }

  return getBufferData(builder, data_type, offset);
}

template <>
std::shared_ptr<flatbuffers::DetachedBuffer> Image<int16_t>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  auto builder = std::make_shared<flatbuffers::FlatBufferBuilder>(1024);

  simple_msgs::dataTraits<int16_t> data_type;
  flatbuffers::Offset<void> offset{};
  if (!data_.empty()) { offset = getDataUnionElem(builder); }

  return getBufferData(builder, data_type, offset);
}

template <>
std::shared_ptr<flatbuffers::DetachedBuffer> Image<float>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  auto builder = std::make_shared<flatbuffers::FlatBufferBuilder>(1024);

  simple_msgs::dataTraits<float> data_type;
  flatbuffers::Offset<void> offset{};
  if (!data_.empty()) { offset = getDataUnionElem(builder); }

  return getBufferData(builder, data_type, offset);
}

template <>
std::shared_ptr<flatbuffers::DetachedBuffer> Image<double>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  auto builder = std::make_shared<flatbuffers::FlatBufferBuilder>(1024);

  simple_msgs::dataTraits<double> data_type;
  flatbuffers::Offset<void> offset{};
  if (!data_.empty()) { offset = getDataUnionElem(builder); }

  return getBufferData(builder, data_type, offset);
}

template <typename T>
void Image<T>::fillPartialImage(const simple_msgs::ImageFbs* imageData) {
  // Set Header.
  header_ = imageData->header()->data();
  // Set Origin.
  origin_ = imageData->origin()->data();
  // Set Image Spacing.
  spacing_x_ = imageData->spacing_x();
  spacing_y_ = imageData->spacing_y();
  spacing_z_ = imageData->spacing_z();
  // Set Image Dimensions.
  width_ = imageData->width();
  height_ = imageData->height();
  depth_ = imageData->depth();
  // Set Encoding.
  encoding_ = imageData->encoding()->c_str();
  data_size_ = imageData->image_size();
  num_channels_ = imageData->num_channels();
}

/**
 * @brief Returns an identifier of the message type generated by the flatbuffers.
 */
template <>
std::string Image<uint8_t>::getTopic() {
  return ImageFbsIdentifier();
}

/**
 * @brief Returns an identifier of the message type generated by the flatbuffers.
 */
template <>
std::string Image<int16_t>::getTopic() {
  return ImageFbsIdentifier();
}

/**
 * @brief Returns an identifier of the message type generated by the flatbuffers.
 */
template <>
std::string Image<float>::getTopic() {
  return ImageFbsIdentifier();
}

/**
 * @brief Returns an identifier of the message type generated by the flatbuffers.
 */
template <>
std::string Image<double>::getTopic() {
  return ImageFbsIdentifier();
}

}  // namespace simple_msgs
