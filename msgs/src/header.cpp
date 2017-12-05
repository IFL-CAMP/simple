
#include "simple_msgs/header.h"

simple_msgs::Header::Header(const uint8_t* data)
{
  auto h = GetHeaderFbs(data);
  seq_n_ = h->sequence_number();
  frame_id_ = h->frame_id()->c_str();
  timestamp_ = h->timestamp();
  field_mofified_ = true;
}
uint8_t* simple_msgs::Header::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (field_mofified_)
  {
    builder_->Clear();
    // all flatbuffer string must be created before the creation of the table builder!
    auto frameIdStr = builder_->CreateString(frame_id_);
    simple_msgs::HeaderFbsBuilder hBuilder(*builder_);
    hBuilder.add_frame_id(frameIdStr);
    hBuilder.add_sequence_number(seq_n_);
    hBuilder.add_timestamp(timestamp_);
    auto h = hBuilder.Finish();
    simple_msgs::FinishHeaderFbsBuffer(
        *builder_, h);  // we have to explicitly call this method if we want the file_identifier to be set
    field_mofified_ = false;
  }
  return builder_->GetBufferPointer();
}

void simple_msgs::Header::setSequenceNumber(const int seq_n)
{
  std::lock_guard<std::mutex> lock(mutex_);
  seq_n_ = seq_n;
  field_mofified_ = true;
}

void simple_msgs::Header::setFrameID(const std::string& frame_id)
{
  std::lock_guard<std::mutex> lock(mutex_);
  frame_id_ = frame_id;
  field_mofified_ = true;
}

void simple_msgs::Header::setTimestamp(const double timestamp)
{
  std::lock_guard<std::mutex> lock(mutex_);
  timestamp_ = timestamp;
  field_mofified_ = true;
}
