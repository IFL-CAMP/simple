#pragma once

#include <mutex>
#include "generic_message.h"
#include "pose_generated.h"
#include "quaternion.h"
#include "point.h"

namespace simple_msgs
{
class Pose : public GenericMessage<Pose>
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param position
   * @param quaternion
   */
  Pose(uint8_t* position, uint8_t* quaternion) : quaternion_(quaternion), position_(position), field_mofified_(true)
  {
  }

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
  int getBufferSize() const
  {
    return builder_->GetSize();
  }

  /**
   * @brief TODO
   * @param quaternion
   */
  void setQuaternion(uint8_t* quaternion);

  /**
   * @brief TODO
   * @param position
   */
  void setPosition(uint8_t* position);

  /**
   * @brief TODO
   * @return
   */
  const simple_msgs::Point* getPosition() const
  {
    return &position_;
  }

  /**
   * @brief TODO
   * @return
   */
  const simple_msgs::Quaternion* getQuaternion() const
  {
    return &quaternion_;
  }

  static const char* derivedTopic()
  {
    return PoseFbsIdentifier();
  }

private:
  simple_msgs::Quaternion quaternion_;
  simple_msgs::Point position_;
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
