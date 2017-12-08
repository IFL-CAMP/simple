#pragma once

#include "numeric_type.hpp"
#include "int_generated.h"

namespace simple_msgs
{
using Int = NumericType<int>;

template <>
NumericType<int>::NumericType(const uint8_t* data)
{
  data_ = GetIntFbs(data)->data();
  modified_ = true;
}

template <>
uint8_t* NumericType<int>::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    IntFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_);
    FinishIntFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

template <>
inline const char* NumericType<int>::getTopic()
{
  return IntFbsIdentifier();
}
}
