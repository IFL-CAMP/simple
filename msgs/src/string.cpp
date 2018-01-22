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

#include <algorithm>
#include <utility>

#include "simple_msgs/string.h"

namespace simple_msgs
{
String::String(std::string data)
  : data_(std::move(data))
{
}

String::String(std::string&& data)
  : data_(std::move(data))
{
  data.clear();
}

String::String(const char* data)
  : data_(data)
{
}

String::String(const uint8_t* data)
  : data_(GetStringFbs(data)->data()->c_str())
{
}

String::String(const String& other)
  : String(other.data_)
{
}

String::String(String&& other) noexcept
  : data_(std::move(other.data_))
{
  other.clear();
}

String& String::operator=(const String& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
    modified_ = true;
  }
  return *this;
}

String& String::operator=(String&& other) noexcept
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = std::move(other.data_);
    modified_ = true;
    other.data_.clear();
  }
  return *this;
}

String& String::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto s = GetStringFbs(data);
  data_ = s->data()->c_str();
  modified_ = true;

  return *this;
}

String& String::operator+=(const String& rhs)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ += rhs.data_;
  modified_ = true;
  return *this;
}

String operator+(String lhs, const String& rhs)
{
  lhs += rhs;
  return lhs;
}

uint8_t* String::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    auto string_data = builder_->CreateString(data_);
    StringFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(string_data);
    FinishStringFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

std::ostream& operator<<(std::ostream& out, const String& s)
{
  out << s.data_;
  return out;
}
}  // namespace simple_msgs
