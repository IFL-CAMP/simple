
#include "simple_msgs/image.h"

template <>
simple_msgs::data simple_msgs::Image<uint8_t>::getDataUnionType() const
{
  return simple_msgs::data_dataUInt8;
}

template <>
simple_msgs::data simple_msgs::Image<int16_t>::getDataUnionType() const
{
  return simple_msgs::data_dataInt16;
}

template <>
simple_msgs::data simple_msgs::Image<double>::getDataUnionType() const
{
  return simple_msgs::data_dataDouble;
}

template <>
simple_msgs::data simple_msgs::Image<float>::getDataUnionType() const
{
  return simple_msgs::data_dataFloat;
}

template <>
flatbuffers::Offset<void> simple_msgs::Image<uint8_t>::getDataUnionElem() const
{
  return simple_msgs::CreatedataUInt8(*builder_, builder_->CreateVector(data_)).Union();
}

template <>
flatbuffers::Offset<void> simple_msgs::Image<int16_t>::getDataUnionElem() const
{
  return simple_msgs::CreatedataInt16(*builder_, builder_->CreateVector(data_)).Union();
}

template <>
flatbuffers::Offset<void> simple_msgs::Image<float>::getDataUnionElem() const
{
  return simple_msgs::CreatedataFloat(*builder_, builder_->CreateVector(data_)).Union();
}

template <>
flatbuffers::Offset<void> simple_msgs::Image<double>::getDataUnionElem() const
{
  return simple_msgs::CreatedataDouble(*builder_, builder_->CreateVector(data_)).Union();
}
