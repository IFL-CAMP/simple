/**
 * S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
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

#ifndef SIMPLE_MSGS_FLOAT_H
#define SIMPLE_MSGS_FLOAT_H

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

template <>
NumericType<float>& NumericType<float>::operator=(const uint8_t* data)
{
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
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_FLOAT_H
