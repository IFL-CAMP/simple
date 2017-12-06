#pragma once

#include "generic_message.h"
#include <mutex>
#include "float_generated.h"

namespace simple_msgs
{
class Float : public GenericMessage
{
public:
  Float()
    : GenericMessage()
  {
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Float(float data)
    : data_(data)
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

  void set(float data)
  {
    data_ = data;
    modified_ = true;
  }

  float get() const { return data_; }

  static const char* getTopic() { return FloatFbsIdentifier(); }

private:
  float data_{0.0};
};
}  // Namespace simple_msgs.
