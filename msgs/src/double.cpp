
#include "simple_msgs/double.h"

simple_msgs::Double::Double(const uint8_t* bufferPointer)
{
  auto d = GetDoubleFbs(bufferPointer);
  data_ = d->data();
  modified_ = true;
}

uint8_t* simple_msgs::Double::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    simple_msgs::DoubleFbsBuilder dBuilder(*builder_);
    dBuilder.add_data(data_);
    auto d = dBuilder.Finish();
    simple_msgs::FinishDoubleFbsBuffer(
        *builder_, d);  // we have to explicitly call this method if we want the file_identifier to be set
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}
