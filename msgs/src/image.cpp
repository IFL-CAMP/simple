
#include "simple_msgs/image.h"

namespace simple_msgs {

  template <>
  data Image<uint8_t>::getDataUnionType() const
  {
    return data_dataUInt8;
  }

  template <>
  data Image<int16_t>::getDataUnionType() const
  {
    return data_dataInt16;
  }

  template <>
  data Image<double>::getDataUnionType() const
  {
    return data_dataDouble;
  }

  template <>
  data Image<float>::getDataUnionType() const
  {
    return data_dataFloat;
  }

  template <>
  flatbuffers::Offset<void> Image<uint8_t>::getDataUnionElem() const
  {
    return CreatedataUInt8(*builder_, builder_->CreateVector(data_, dataLength_)).Union();
  }

  template <>
  flatbuffers::Offset<void> Image<int16_t>::getDataUnionElem() const
  {
    return CreatedataInt16(*builder_, builder_->CreateVector(data_, dataLength_)).Union();
  }

  template <>
  flatbuffers::Offset<void> Image<float>::getDataUnionElem() const
  {
    return CreatedataFloat(*builder_, builder_->CreateVector(data_, dataLength_)).Union();
  }

  template <>
  flatbuffers::Offset<void> Image<double>::getDataUnionElem() const
  {
    return CreatedataDouble(*builder_, builder_->CreateVector(data_, dataLength_)).Union();
  }

}


