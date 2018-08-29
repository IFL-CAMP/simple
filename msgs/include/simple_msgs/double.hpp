/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_DOUBLE_H
#define SIMPLE_MSGS_DOUBLE_H

#include "generated/double_generated.h"
#include "numeric_type.hpp"

namespace simple_msgs {
using Double = NumericType<double>;

template <>
inline NumericType<double>::NumericType(const void* data) : data_{GetDoubleFbs(data)->data()} {}

template <>
inline NumericType<double>& NumericType<double>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = GetDoubleFbs(*data)->data();
  return *this;
}

template <>
inline std::shared_ptr<flatbuffers::DetachedBuffer> NumericType<double>::getBufferData() const {
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
