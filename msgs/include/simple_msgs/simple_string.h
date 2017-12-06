#pragma once

#include "generic_message.h"
#include <mutex>
#include "string_generated.h"

namespace simple_msgs
{
class String : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  String(std::string data)
    : data_(data)
    , modified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  String(const uint8_t* bufferPointer);
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

  std::string getString() const { return data_; }

  void setString(std::string data)
  {
    data_ = data;
    modified_ = true;
  }

  static const char* getTopic() { return StringFbsIdentifier(); }

private:
  std::string data_;
  mutable bool modified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
