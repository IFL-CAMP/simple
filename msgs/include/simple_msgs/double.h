#pragma once

#include "generic_message.h"
#include "double_generated.h"
#include <mutex>

namespace simple_msgs
{
class Double : public GenericMessage
{
public:
  Double()
    : GenericMessage()
  {
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Double(double data)
    : data_(data)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Double(const uint8_t* bufferPointer);

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;

  double get() const { return data_; }

  void set(double data)
  {
    data_ = data;
    modified_ = true;
  }

  static const char* getTopic() { return DoubleFbsIdentifier(); }

private:
  double data_{0.0};
};
}  // Namespace simple_msgs.
