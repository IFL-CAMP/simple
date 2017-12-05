#pragma once

#include "generic_message.h"
#include "quaternion_generated.h"
#include <mutex>

namespace simple_msgs
{
class Quaternion : public GenericMessage
{
public:
  Quaternion()
    : GenericMessage()
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Quaternion(double x, double y, double z, double w)
    : x_(x)
    , y_(y)
    , z_(z)
    , w_(w)
    , mofified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Quaternion(const uint8_t* bufferPointer);

  Quaternion(const Quaternion& q)
    : Quaternion(q.x_, q.y_, q.z_, q.w_)
  {
  }

  Quaternion& operator=(const Quaternion& q)
  {
    if (this != std::addressof(q))
    {
      x_ = q.x_;
      y_ = q.y_;
      z_ = q.z_;
      w_ = q.w_;
      mofified_ = true;
    }
    return *this;
  }

  /**
   * @brief TODO
   * @return
   */
  uint8_t* getBufferData() const;

  /**
   * @brief TODO
   * @return
   */
  int getBufferSize() const { return builder_->GetSize(); }

  /**
   * @brief TODO
   * @return
   */
  std::vector<double> getQuaternion() const { return std::vector<double>{x_, y_, z_, w_}; }

  /**
   * @brief TODO
   * @return
   */
  void setQuaternion(std::vector<double> quat);

  /**
   * @brief TODO
   * @return
   */
  static const char* getTopic() { return QuaternionFbsIdentifier(); }

private:
  double x_, y_, z_, w_;
  mutable bool mofified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
