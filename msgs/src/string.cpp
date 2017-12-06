
#include "simple_msgs/string.h"

simple_msgs::String::String(const uint8_t* bufferPointer)
{
  auto s = GetStringFbs(bufferPointer);
  data_ = s->data()->c_str();
  modified_ = true;
}

uint8_t* simple_msgs::String::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    // all flatbuffer string must be created before the creation of the table builder!
    auto dataStr = builder_->CreateString(data_);
    simple_msgs::StringFbsBuilder sBuilder(*builder_);
    sBuilder.add_data(dataStr);
    auto s = sBuilder.Finish();
    simple_msgs::FinishStringFbsBuffer(
        *builder_, s);  // we have to explicitly call this method if we want the file_identifier to be set
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}
