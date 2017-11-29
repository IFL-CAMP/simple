#pragma once

#include <mutex>
#include "generic_message.h"
#include "header_generated.h"
#include <typeinfo>

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
    : seq_n_(seq_n), frame_id_(frame_id), timestamp_(timestamp), field_mofified_(true)
  {
  }

  /**
   * @brief Constructor from buffer data
   * @param data acquired from flatbuffer, for example from GetRoot method
   */
  Header(const uint8_t* data);

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;

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
    std::lock_guard<std::mutex> lock(mutex_);
    seq_n_ = seq_n;
    field_mofified_ = true;
  }

  /**
   * @brief TODO
   * @param frame_id
   */
  void setFrameID(const std::string& frame_id)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    frame_id_ = frame_id;
    field_mofified_ = true;
  }

  /**
   * @brief Modifies the data for time stamp. Keep in mind creating new messages from scratch can be more efficient than
   * mutating existing ones
   * @param timestamp Seconds since the epoch, when the data in the message was generated
   */
  void setTimestamp(const double timestamp)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    timestamp_ = timestamp;
    field_mofified_ = true;
  }

private:
  int seq_n_{ 0 };
  std::string frame_id_{ "" };
  double timestamp_{ 0.0 };
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
