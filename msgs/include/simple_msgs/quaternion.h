#pragma once

#include "generic_message.h"
#include "quaternion_generated.h"

namespace simple_msgs
{
class Quaternion : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Quaternion(const uint8_t* bufferPointer);
  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;
  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const;
};
}  // namespace simple_msgs
