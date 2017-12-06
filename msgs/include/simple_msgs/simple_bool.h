#pragma once

#include "generic_message.h"
#include <mutex>
#include "bool_generated.h"

namespace simple_msgs
{
class Bool : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Bool(bool data)
    : data_(data)
    , modified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Bool(const uint8_t* bufferPointer);
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

  void setBool(bool data)
  {
    data_ = data;
    modified_ = true;
  }

  bool getBool() const { return data_; }
  static const char* getTopic() { return BoolFbsIdentifier(); }

private:
  bool data_;
  mutable bool modified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
