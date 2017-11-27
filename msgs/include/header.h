#pragma once

#include "generic_message.h"
#include "header_generated.h"

namespace simple_msgs
{
/**
 * @brief TODO
 */
class Header : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param seq_n
   * @param frame_id
   * @param timestamp
   */
  Header(const int seq_n, const std::string& frame_id, const double timestamp)
    : seq_n_(seq_n), frame_id_(frame_id), timestamp_(timestamp)
  {
  }

  /**
   * @brief TODO
   * @param data
   */
  Header(const uint8_t* data)
  {
    auto h = GetHeaderFbs(data);
    seq_n_ = h->sequence_number();
    frame_id_ = h->frame_id()->c_str();
    timestamp_ = h->timestamp();
  }

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const
  {
    auto h = CreateHeaderFbs(*builder_, seq_n_, builder_->CreateString(frame_id_), timestamp_);
    builder_->Finish(h);
    return builder_->GetBufferPointer();
  }

  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const
  {
    return builder_->GetSize();
  }

  /**
   * @brief TODO
   * @return
   */
  int getSequenceNumber() const
  {
    return seq_n_;
  }

  /**
   * @brief TODO
   * @return
   */
  std::string getFrameID() const
  {
    return frame_id_;
  }

  /**
   * @brief TODO
   * @return
   */
  double getTimestamp() const
  {
    return timestamp_;
  }

  /**
   * @brief TODO
   * @param seq_n
   */
  void setSequenceNumber(const int seq_n)
  {
    seq_n_ = seq_n;
  }

  /**
   * @brief TODO
   * @param frame_id
   */
  void setFrameID(const std::string& frame_id)
  {
    frame_id_ = frame_id;
  }

  /**
   * @brief TODO
   * @param timestamp
   */
  void setTimestamp(const double timestamp)
  {
    timestamp_ = timestamp;
  }

private:
  int seq_n_{ 0 };
  std::string frame_id_{ "" };
  double timestamp_{ 0.0 };
};
}
