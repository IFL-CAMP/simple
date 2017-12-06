#pragma once

#include <mutex>
#include "generic_message.h"
#include "pose_generated.h"
#include "quaternion.h"
#include "point.h"

namespace simple_msgs
{
class Pose : public GenericMessage
{
public:
  Pose()
    : GenericMessage()
  {
  }

  /**
   * @brief TODO
   * @param position
   * @param quaternion
   */
  Pose(const Point& position, const Quaternion& quaternion)
    : quaternion_(quaternion)
    , position_(position)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Pose(const uint8_t* data);

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const override;

  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const { return builder_->GetSize(); }

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
  const Point* getPosition() const { return &position_; }

  /**
   * @brief TODO
   * @return
   */
  const Quaternion* getQuaternion() const { return &quaternion_; }

  static const char* getTopic() { return PoseFbsIdentifier(); }

private:
  Quaternion quaternion_;
  Point position_;
};
}  // namespace simple_msgs
