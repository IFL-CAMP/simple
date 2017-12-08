
#include "simple_msgs/bool.h"

namespace simple_msgs
{
Bool::Bool(const uint8_t* data)
{
  data_ = GetBoolFbs(data)->data();
  modified_ = true;
}

Bool& Bool::operator=(const Bool& b)
{
  if (this != std::addressof(b))
  {
    data_ = b.data_;
  }
  return *this;
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
