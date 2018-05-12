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

#include "simple_msgs/bool.h"

namespace simple_msgs {
Bool::Bool(bool data) : data_{data} {}

Bool::Bool(const uint8_t* data) : data_{GetBoolFbs(data)->data()} {}

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
  data_ = data;
  data_ = GetBoolFbs(data.get())->data();
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
