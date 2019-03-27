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

Bool::Bool(const Bool& other) : Bool{other.data_.load()} {}

Bool::Bool(Bool&& other) noexcept : data_{other.data_.load()} {}

Bool& Bool::operator=(const Bool& rhs) {
  if (this != std::addressof(rhs)) { data_.exchange(rhs.data_.load()); }
  return *this;
}

Bool& Bool::operator=(Bool&& rhs) noexcept {
  if (this != std::addressof(rhs)) { data_.exchange(rhs.data_.load()); }
  return *this;
}

Bool& Bool::operator=(std::shared_ptr<void*> rhs) {
  data_.store(GetBoolFbs(*rhs)->data());
  return *this;
}

std::shared_ptr<flatbuffers::DetachedBuffer> Bool::getBufferData() const {
  flatbuffers::FlatBufferBuilder builder{1024};
  BoolFbsBuilder tmp_builder{builder};
  tmp_builder.add_data(data_.load());
  FinishBoolFbsBuffer(builder, tmp_builder.Finish());
  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const Bool& b) {
  out << b.data_.load();
  return out;
}
}  // Namespace simple_msgs.
