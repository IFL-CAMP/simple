#pragma once

#include <iostream>

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
   * @brief Contructor from a Point and a Quaternion.
   */
  Pose(const Point& position, const Quaternion& quaternion)
    : quaternion_(quaternion)
    , position_(position)
  {
  }

  /**
   * @brief Constructor from the buffer data.
   * @param data acquired from flatbuffer.
   */
  Pose(const uint8_t* data);

  /**
   * @brief Copy Constructor.
   */
  Pose(const Pose& p)
    : Pose(p.position_, p.quaternion_)
  {
  }

  /**
   * @brief Copy assignment.
   */
  Pose& operator=(const Pose& p);

  /**
   * @brief operator==
   */
  bool operator==(const Pose& p) const;

  /**
   * @brief operator!=
   */
  bool operator!=(const Pose& p) const;

  /**
   * @brief Builds and returns the buffer accordingly to the values currently stored.
   * @return the buffer data.
   */
  uint8_t* getBufferData() const override;

  /**
   * @brief Returns the translational part of the Pose as a Point message.
   * @return
   */
  Point getPosition() const { return position_; }

  /**
   * @brief Returns the rotational part of the Pose as a Quaternion message.
   * @return
   */
  Quaternion getQuaternion() const { return quaternion_; }

  /**
   * @brief Modifies the rotational part of the Pose.
   */
  void setPosition(const Point& position);

  /**
   * @brief Modifies the translational part of the Pose.
   */
  void setQuaternion(const Quaternion& quaternion);

  static const char* getTopic() { return PoseFbsIdentifier(); }

  friend std::ostream& operator<<(std::ostream& out, const Pose& q);

private:
  Quaternion quaternion_;
  Point position_;
};
}  // Namespace simple_msgs.
