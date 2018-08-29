/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/bool.h"

namespace simple_msgs {
Bool::Bool(bool data) : data_{data} {}

Bool::Bool(const void* data) : data_{GetBoolFbs(data)->data()} {}

Bool::Bool(const Bool& other) : Bool{other.data_} {}

Bool::Bool(Bool&& other) noexcept : data_{std::move(other.data_)} {}

Bool& Bool::operator=(const Bool& other) {
  if (this != std::addressof(other)) {
    std::lock_guard<std::mutex> lock{mutex_};
    data_ = other.data_;
  }
  return *this;
}

Bool& Bool::operator=(Bool&& other) noexcept {
  if (this != std::addressof(other)) { data_ = std::move(other.data_); }
  return *this;
}

Bool& Bool::operator=(std::shared_ptr<void*> data) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = GetBoolFbs(*data)->data();
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Bool::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  BoolFbsBuilder tmp_builder{builder};
  tmp_builder.add_data(data_);
  FinishBoolFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

void Bool::set(bool data) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = data;
}

std::ostream& operator<<(std::ostream& out, const Bool& b) {
  out << b.data_;
  return out;
}
}  // namespace simple_msgs
