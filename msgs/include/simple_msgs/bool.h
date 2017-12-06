#pragma once

#include "generic_message.h"
#include "bool_generated.h"

namespace simple_msgs
{
class Bool : public GenericMessage
{
public:
  Bool()
    : GenericMessage()
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Bool(bool data)
    : data_(data)
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

  void set(bool data)
  {
    data_ = data;
    modified_ = true;
  }

  bool get() const { return data_; }
  static const char* getTopic() { return BoolFbsIdentifier(); }

private:
  bool data_{false};
};
}  // Namespace simple_msgs.
