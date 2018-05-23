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

#ifndef SIMPLE_MSGS_DOUBLE_H
#define SIMPLE_MSGS_DOUBLE_H

#include "generated/double_generated.h"
#include "numeric_type.hpp"

namespace simple_msgs {
using Double = NumericType<double>;

template <>
NumericType<double>::NumericType(const void* data) : data_{GetDoubleFbs(data)->data()} {}

template <>
NumericType<double>& NumericType<double>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = GetDoubleFbs(*data)->data();
  return *this;
}

template <>
std::shared_ptr<flatbuffers::DetachedBuffer> NumericType<double>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};
  DoubleFbsBuilder tmp_builder{builder};
  tmp_builder.add_data(data_);
  FinishDoubleFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

template <>
inline std::string NumericType<double>::getTopic() {
  return DoubleFbsIdentifier();
}
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_DOUBLE_H
