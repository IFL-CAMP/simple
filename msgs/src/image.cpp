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

namespace simple_msgs
{
template <>
data Image<uint8_t>::getDataUnionType() const
{
  return data_uint8_type;
}

template <>
data Image<int16_t>::getDataUnionType() const
{
  return data_int16_type;
}

template <>
data Image<float>::getDataUnionType() const
{
  return data_float_type;
}

template <>
data Image<double>::getDataUnionType() const
{
  return data_double_type;
}

template <>
flatbuffers::Offset<void>
Image<uint8_t>::getDataUnionElem(const std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const
{
  return Createuint8_type(*builder, builder->CreateVector(static_cast<const uint8_t*>(*data_), data_size_)).Union();
}

template <>
flatbuffers::Offset<void>
Image<int16_t>::getDataUnionElem(const std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const
{
  return Createint16_type(*builder, builder->CreateVector(static_cast<const int16_t*>(*data_), data_size_)).Union();
}

template <>
flatbuffers::Offset<void>
Image<float>::getDataUnionElem(const std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const
{
  return Createfloat_type(*builder, builder->CreateVector(static_cast<const float*>(*data_), data_size_)).Union();
}

template <>
flatbuffers::Offset<void>
Image<double>::getDataUnionElem(const std::shared_ptr<flatbuffers::FlatBufferBuilder> builder) const
{
  return Createdouble_type(*builder, builder->CreateVector(static_cast<const double*>(*data_), data_size_)).Union();
}

template <>
Image<uint8_t>& Image<uint8_t>::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto image_data = GetImageFbs(data);

  Image<uint8_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const uint8_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const uint8_t*>(mydata);
  }
  return *this;
}

template <>
Image<int16_t>& Image<int16_t>::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto image_data = GetImageFbs(data);

  Image<int16_t>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const int16_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const int16_t*>(mydata);
  }
  return *this;
}

template <>
Image<double>& Image<double>::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto image_data = GetImageFbs(data);

  Image<double>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const double_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const double*>(mydata);
  }
  return *this;
}

template <>
Image<float>& Image<float>::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto image_data = GetImageFbs(data);

  Image<float>::fillPartialImage(image_data);

  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const float_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const float*>(mydata);
  }
  return *this;
}

template <>
Image<uint8_t>::Image(const uint8_t* data)
  : GenericMessage()
{
  auto image_data = GetImageFbs(data);

  Image<uint8_t>::fillPartialImage(image_data);
  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const uint8_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const uint8_t*>(mydata);
  }
}

template <>
Image<int16_t>::Image(const uint8_t* data)
  : GenericMessage()
{
  auto image_data = GetImageFbs(data);

  Image<int16_t>::fillPartialImage(image_data);
  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const int16_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const int16_t*>(mydata);
  }
}

template <>
Image<double>::Image(const uint8_t* data)
  : GenericMessage()
{
  auto image_data = GetImageFbs(data);

  Image<double>::fillPartialImage(image_data);
  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const double_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const double*>(mydata);
  }
}

template <>
Image<float>::Image(const uint8_t* data)
  : GenericMessage()
{
  auto image_data = GetImageFbs(data);

  Image<float>::fillPartialImage(image_data);
  // Set the Image data according to the right date type.
  if (flatbuffers::IsFieldPresent(image_data, ImageFbs::VT_IMAGE))
  {
    auto mydata = static_cast<const float_type*>(image_data->image())->raw()->data();
    data_ = std::make_shared<const float*>(mydata);
  }
}
}  // namespace simple_msgs
