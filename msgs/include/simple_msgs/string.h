#pragma once

#include "generic_message.h"
#include <mutex>
#include "string_generated.h"

namespace simple_msgs
{
class String : public GenericMessage
{
public:
  String()
    : GenericMessage()
  {
  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  String(const std::string& data)
    : data_(data)
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

  std::string get() const { return data_; }

  void set(const std::string& data)
  {
    data_ = data;
    modified_ = true;
  }

  static const char* getTopic() { return StringFbsIdentifier(); }

private:
  std::string data_{""};
};
}  // Namespace simple_msgs.
