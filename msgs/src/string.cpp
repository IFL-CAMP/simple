
#include <algorithm>

#include "simple_msgs/string.h"

namespace simple_msgs
{
String::String()
  : GenericMessage()
{
}

String::String(const std::string& data)
  : GenericMessage()
  , data_(data)
{
}

String::String(std::string&& data)
  : GenericMessage()
  , data_(std::move(data))
{
  data.clear();
}

String::String(const char* data)
  : GenericMessage()
  , data_(data)
{
}

String::String(const uint8_t* data)
  : GenericMessage()
  , data_(GetStringFbs(data)->data()->c_str())
{
}

String::String(const String& other)
  : String(other.data_)
{
}

String::String(String&& other)
  : GenericMessage()
  , data_(std::move(other.data_))
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
    data_ = std::move(other.data_);
    modified_ = true;
    other.data_.clear();
  }
  return *this;
}

bool String::operator==(const String& rhs) const
{
  return (data_ == rhs.data_);
}

bool String::operator!=(const String& rhs) const
{
  return !(*this == rhs);
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

void String::set(const std::string& data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = data;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const String& s)
{
  out << s.data_;
  return out;
}
}
