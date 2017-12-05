
#include "simple_msgs/header.h"

namespace simple_msgs
{
Header::Header(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto h = GetHeaderFbs(data);
  seq_n_ = h->sequence_number();
  frame_id_ = h->frame_id()->c_str();
  timestamp_ = h->timestamp();
  modified_ = true;
}

Header& Header::operator=(const Header& h)
{
  if (this != std::addressof(h))
  {
    seq_n_ = h.seq_n_;
    frame_id_ = h.frame_id_;
    timestamp_ = h.timestamp_;
    modified_ = true;
  }
  return *this;
}

bool Header::operator==(const Header& h) const
{
  return (seq_n_ == h.seq_n_ && frame_id_ == h.frame_id_ && timestamp_ == h.timestamp_);
}

bool Header::operator!=(const Header& h) const
{
  return !(*this == h);
}

uint8_t* Header::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    auto frame_id_string = builder_->CreateString(frame_id_);
    HeaderFbsBuilder hBuilder(*builder_);
    hBuilder.add_frame_id(frame_id_string);
    hBuilder.add_sequence_number(seq_n_);
    hBuilder.add_timestamp(timestamp_);
    FinishHeaderFbsBuffer(*builder_, hBuilder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Header::setSequenceNumber(const int seq_n)
{
  std::lock_guard<std::mutex> lock(mutex_);
  seq_n_ = seq_n;
  modified_ = true;
}

void Header::setFrameID(const std::string& frame_id)
{
  std::lock_guard<std::mutex> lock(mutex_);
  frame_id_ = frame_id;
  modified_ = true;
}

void Header::setTimestamp(const double timestamp)
{
  std::lock_guard<std::mutex> lock(mutex_);
  timestamp_ = timestamp;
  modified_ = true;
}
}
