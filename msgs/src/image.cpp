
#include "simple_msgs/image.h"

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
flatbuffers::Offset<void> Image<uint8_t>::getDataUnionElem() const
{
  return Createuint8_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<int16_t>::getDataUnionElem() const
{
  return Createint16_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<float>::getDataUnionElem() const
{
  return Createfloat_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}

template <>
flatbuffers::Offset<void> Image<double>::getDataUnionElem() const
{
  return Createdouble_type(*builder_, builder_->CreateVector(data_, data_size_)).Union();
}
}
