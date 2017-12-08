#pragma once

#include "numeric_type.hpp"
#include "double_generated.h"

namespace simple_msgs
{
using Double = NumericType<double>;

template <>
NumericType<double>::NumericType(const uint8_t* data)
  : GenericMessage()
  , data_(GetDoubleFbs(data)->data())
{
}

template <>
uint8_t* NumericType<double>::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    DoubleFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_);
    FinishDoubleFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

template <>
inline const char* NumericType<double>::getTopic()
{
  return DoubleFbsIdentifier();
}
}
