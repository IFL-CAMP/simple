/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_INT_H
#define SIMPLE_MSGS_INT_H

#include "generated/int_generated.h"
#include "numeric_type.hpp"

namespace simple_msgs {
using Int = NumericType<int>;

template <>
inline NumericType<int>::NumericType(const void* data) : data_{GetIntFbs(data)->data()} {}

template <>
inline NumericType<int>& NumericType<int>::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = GetIntFbs(*data)->data();
  return *this;
}

template <>
inline std::shared_ptr<flatbuffers::DetachedBuffer> NumericType<int>::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};
  IntFbsBuilder tmp_builder{builder};
  tmp_builder.add_data(data_);
  FinishIntFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

template <>
inline std::string NumericType<int>::getTopic() {
  return IntFbsIdentifier();
}
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_INT_H
