
#include "simple_msgs/simple_int.h"

simple_msgs::Int::Int(const uint8_t* bufferPointer)
{
  auto i = GetIntFbs(bufferPointer);
  data_ = i->data();
  modified_ = true;
}

uint8_t* simple_msgs::Int::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    simple_msgs::IntFbsBuilder iBuilder(*builder_);
    iBuilder.add_data(data_);
    auto i = iBuilder.Finish();
    simple_msgs::FinishIntFbsBuffer(
        *builder_, i);  // we have to explicitly call this method if we want the file_identifier to be set
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}
