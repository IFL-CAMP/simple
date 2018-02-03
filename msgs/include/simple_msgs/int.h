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

#ifndef SIMPLE_MSGS_INT_H
#define SIMPLE_MSGS_INT_H

#include "numeric_type.hpp"
#include "generated/int_generated.h"

namespace simple_msgs
{
using Int = NumericType<int>;

template <>
NumericType<int>::NumericType(const uint8_t* data)
  : GenericMessage()
  , data_(GetIntFbs(data)->data())
{
}

template <>
NumericType<int>& NumericType<int>::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = GetIntFbs(data)->data();
  modified_ = true;

  return *this;
}

template <>
flatbuffers::DetachedBuffer NumericType<int>::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    if (builder_->GetSize())
    {
      builder_->Clear();
    }
    IntFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_);
    FinishIntFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->Release();
}

template <>
inline const char* NumericType<int>::getTopic()
{
  return IntFbsIdentifier();
}
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_INT_H
