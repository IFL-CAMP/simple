#pragma once

#include "generic_message.h"
#include "int_generated.h"
#include <mutex>

namespace simple_msgs
{
class Int : public GenericMessage
{
public:
  Int()
    : GenericMessage()
  {
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Int(int data)
    : data_(data)
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

  void set(int data)
  {
    data_ = data;
    modified_ = true;
  }

  int get() const { return data_; }

  static const char* getTopic() { return IntFbsIdentifier(); }

private:
  int data_{0};
};
}  // Namespace simple_msgs.
