#pragma once

#include "generic_message.h"
#include "int_generated.h"
#include <mutex>

namespace simple_msgs
{
class Int : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Int(int data)
    : data_(data)
    , mofified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Int(const uint8_t* bufferPointer);
  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;
  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const { return builder_->GetSize(); }

  void setInt(int data)
  {
    data_ = data;
    mofified_ = true;
  }

  int getInt() const { return data_; }

  static const char* getTopic() { return IntFbsIdentifier(); }

private:
  int data_;
  mutable bool mofified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
