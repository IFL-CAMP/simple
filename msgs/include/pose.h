#pragma once

#include "generic_message.h"
#include "quaternion.h"
#include "point.h"

namespace simple_msgs
{
class Pose : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;
  /**
   * @brief TODO
   * @param position
   * @param quaternion
   */
  Pose(uint8_t* position, uint8_t* quaternion):quaternion_(simple_msgs::Quaternion(quaternion)),position_(simple_msgs::Point(position)){  }
  /**
   * @brief TODO
   * @param bufferPointer
   */
  Pose(const uint8_t* bufferPointer);
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

private:
  simple_msgs::Quaternion quaternion_;
  simple_msgs::Point position_;
};
}  // namespace simple_msgs
