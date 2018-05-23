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
  return Createuint8_type(*builder, builder->CreateVector(data_.getData(), data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<int16_t>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createint16_type(*builder, builder->CreateVector(data_.getData(), data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<float>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createfloat_type(*builder, builder->CreateVector(data_.getData(), data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<double>::getDataUnionElem(
    std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const {
  return Createdouble_type(*builder, builder->CreateVector(data_.getData(), data_size_)).Union();
}

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
