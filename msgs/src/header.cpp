
#include "simple_msgs/header.h"

namespace simple_msgs
{
Header::Header()
  : GenericMessage()
{
}

Header::Header(int seq_n, const std::string& frame_id, double timestamp)
  : GenericMessage()
  , seq_n_(seq_n)
  , frame_id_(frame_id)
  , timestamp_(timestamp)
{
}

Header::Header(const uint8_t* data)
  : GenericMessage()
  , seq_n_(GetHeaderFbs(data)->sequence_number())
  , frame_id_(GetHeaderFbs(data)->frame_id()->c_str())
  , timestamp_(GetHeaderFbs(data)->timestamp())
{
}

Header::Header(const Header& h)
  : Header(h.seq_n_, h.frame_id_, h.timestamp_)
{
}

Header::Header(Header&& other)
  : GenericMessage()
  , seq_n_(std::move(other.seq_n_))
  , frame_id_(std::move(other.frame_id_))
  , timestamp_(std::move(other.timestamp_))
{
}

Header& Header::operator=(const Header& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    seq_n_ = other.seq_n_;
    frame_id_ = other.frame_id_;
    timestamp_ = other.timestamp_;
    modified_ = true;
  }
  return *this;
}

Header& Header::operator=(Header&& other)
{
  if (this != std::addressof(other))
  {
    seq_n_ = std::move(other.seq_n_);
    frame_id_ = std::move(other.frame_id_);
    timestamp_ = std::move(other.timestamp_);
    modified_ = true;
  }
  return *this;
}

Header& Header::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  seq_n_ = GetHeaderFbs(data)->sequence_number();
  frame_id_ = GetHeaderFbs(data)->frame_id()->c_str();
  timestamp_ = GetHeaderFbs(data)->timestamp();
  modified_ = true;

  return *this;
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

std::ostream& operator<<(std::ostream& out, const Header& h)
{
  out << "Header\n \t"
      << "seq_n: " << h.seq_n_ << "\n \t"
      << "frame_id: " << h.frame_id_ << "\n \t"
      << "timestamp: " << h.timestamp_ << "\n";

  return out;
}
}
