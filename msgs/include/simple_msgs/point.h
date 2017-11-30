#pragma once

#include <mutex>
#include "generic_message.h"
#include "point_generated.h"

namespace simple_msgs
{
class Point : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param x X coordinate
   * @param y Y coordinate
   * @param z Z coordinate
   */
  Point(const double x, const double y, const double z) : x_(x), y_(y), z_(z), field_mofified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Point(const uint8_t* bufferPointer);

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;

  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const{ return builder_->GetSize(); }

  static const char* topic_;

private:
	double x_, y_, z_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
const char* Point::topic_ = simple_msgs::PointFbsIdentifier();
}  // namespace simple_msgs
