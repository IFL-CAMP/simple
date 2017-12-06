
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
    HeaderFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_frame_id(frame_id_string);
    tmp_builder.add_sequence_number(seq_n_);
    tmp_builder.add_timestamp(timestamp_);
    FinishHeaderFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Header::setSequenceNumber(int seq_n)
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

void Header::setTimestamp(double timestamp)
{
  std::lock_guard<std::mutex> lock(mutex_);
  timestamp_ = timestamp;
  modified_ = true;
}

std::ofstream& operator<<(std::ostream& out, const Header& h)
{
  out << "Header \n \t"
      << "seq_n: " << h.seq_n_ << "\n"
      << "frame_id: " << h.frame_id_ << "\n"
      << "timestamp: " << h.timestamp << "\n";

  return out;
}
}
