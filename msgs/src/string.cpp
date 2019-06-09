/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple_msgs/string.hpp"
#include "simple_msgs/generated/string_generated.h"

namespace simple_msgs {
String::String(const std::string& data) : data_{data} {}

String::String(std::string&& data) : data_{std::move(data)} {}

String::String(const char* data) : data_{data} {}

String::String(const void* data) : data_{GetStringFbs(data)->data()->c_str()} {}

String::String(const String& other, const std::lock_guard<std::mutex>&) : String{other.data_} {}

String::String(String&& other, const std::lock_guard<std::mutex>&) noexcept : data_{std::move(other.data_)} {}

String::String(const String& other) : String{other, std::lock_guard<std::mutex>(other.mutex_)} {}

String::String(String&& other) noexcept
  : String{std::forward<String>(other), std::lock_guard<std::mutex>(other.mutex_)} {}

String& String::operator=(const String& rhs) {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = rhs.data_;
  }
  return *this;
}

String& String::operator=(String&& rhs) noexcept {
  if (this != std::addressof(rhs)) {
    std::lock(mutex_, rhs.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{rhs.mutex_, std::adopt_lock};
    data_ = std::move(rhs.data_);
    rhs.data_.clear();
  }
  return *this;
}

String& String::operator=(std::shared_ptr<void*> rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ = GetStringFbs(*rhs)->data()->c_str();
  return *this;
}

String& String::operator+=(const String& rhs) {
  std::lock_guard<std::mutex> lock{mutex_};
  data_ += rhs.data_;
  return *this;
}

/**
 * @brief Concatenates two Strings.
 */
String operator+(String lhs, const String& rhs) {
  lhs += rhs;
  return lhs;
}

std::shared_ptr<flatbuffers::DetachedBuffer> String::getBufferData() const {
  std::lock_guard<std::mutex> lock{mutex_};
  flatbuffers::FlatBufferBuilder builder{1024};

  auto string_data = builder.CreateString(data_);
  StringFbsBuilder tmp_builder{builder};
  tmp_builder.add_data(string_data);
  FinishStringFbsBuffer(builder, tmp_builder.Finish());

  return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

std::string String::getTopic() { return StringFbsIdentifier(); }

/**
 * @brief Stream extraction operator.
 */
std::ostream& operator<<(std::ostream& out, const String& s) {
  std::lock_guard<std::mutex> lock{s.mutex_};
  out << s.data_;
  return out;
}
}  // namespace simple_msgs
