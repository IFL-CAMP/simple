#pragma once

#include "generic_message.h"
#include <mutex>
#include "float_generated.h"

namespace simple_msgs
{
class Float : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Float(float data)
    : data_(data)
    , modified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Float(const uint8_t* bufferPointer);

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

  void setFloat(float data)
  {
    data_ = data;
    modified_ = true;
  }

  float getFloat() const { return data_; }

  static const char* getTopic() { return FloatFbsIdentifier(); }

private:
  float data_;
  mutable bool modified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
