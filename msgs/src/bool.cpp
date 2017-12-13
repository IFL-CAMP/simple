
#include "simple_msgs/bool.h"

namespace simple_msgs
{
Bool::Bool()
  : GenericMessage()
{
}

Bool::Bool(bool data)
  : GenericMessage()
  , data_(data)
{
}

Bool::Bool(const uint8_t* data)
  : GenericMessage()
  , data_(GetBoolFbs(data)->data())
{
}

Bool::Bool(const Bool& other)
  : Bool(other.data_)
{
}

Bool::Bool(Bool&& other)
  : GenericMessage()
  , data_(std::move(other.data_))
{
}

Bool& Bool::operator=(const Bool& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
    modified_ = true;
  }
  return *this;
}

Bool& Bool::operator=(Bool&& other)
{
  if (this != std::addressof(other))
  {
    data_ = std::move(other.data_);
    modified_ = true;
  }
  return *this;
}


Bool& Bool::operator=(const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto b = GetBoolFbs(data);
	data_ = b->data();
	modified_ = true;

	return *this;
}

bool Bool::operator==(const Bool& rhs) const
{
  return (data_ == rhs.data_);
}

bool Bool::operator!=(const Bool& rhs) const
{
  return !(*this == rhs);
}

uint8_t* Bool::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    BoolFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_data(data_);
    FinishBoolFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Bool::set(bool data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = data;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const Bool& b)
{
  out << b.data_;
  return out;
}
}
