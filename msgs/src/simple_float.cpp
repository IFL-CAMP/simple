
#include "simple_msgs/simple_float.h"

simple_msgs::Float::Float(const uint8_t* bufferPointer)
{
  auto f = GetFloatFbs(bufferPointer);
  data_ = f->data();
  modified_ = true;
}

uint8_t* simple_msgs::Float::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    simple_msgs::FloatFbsBuilder fBuilder(*builder_);
    fBuilder.add_data(data_);
    auto f = fBuilder.Finish();
    simple_msgs::FinishFloatFbsBuffer(
        *builder_, f);  // we have to explicitly call this method if we want the file_identifier to be set
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}
