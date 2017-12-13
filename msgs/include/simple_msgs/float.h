#pragma once

#include "numeric_type.hpp"
#include "generated/float_generated.h"

namespace simple_msgs
{
using Float = NumericType<float>;

template <>
NumericType<float>::NumericType(const uint8_t* data)
  : GenericMessage()
  , data_(GetFloatFbs(data)->data())
{
}

template<>
NumericType<float>& NumericType<float>::operator=(const uint8_t* data) {
	std::lock_guard<std::mutex> lock(mutex_);
	data_ = GetFloatFbs(data)->data();
	modified_ = true;

	return *this;
}

template <>
uint8_t* NumericType<float>::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    FloatFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_);
    FinishFloatFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

template <>
inline const char* NumericType<float>::getTopic()
{
  return FloatFbsIdentifier();
}
}
