
#include "header.h"

simple_msgs::Header::Header(const uint8_t* data)
{
  auto h = GetHeaderFbs(data);
  seq_n_ = h->sequence_number();
  frame_id_ = h->frame_id()->c_str();
  timestamp_ = h->timestamp();
}
uint8_t* simple_msgs::Header::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (field_mofified_)
  {
    builder_->Clear();
    auto h = CreateHeaderFbs(*builder_, seq_n_, builder_->CreateString(frame_id_), timestamp_);
    builder_->Finish(h);
    field_mofified_ = false;
  }
  return builder_->GetBufferPointer();
}
