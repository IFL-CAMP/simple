#pragma once

#include "generic_message.h"
#include "quaternion_generated.h"
#include <mutex>

namespace simple_msgs
{
class Quaternion : public GenericMessage<Quaternion>
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  Quaternion(double x, double y, double z, double w) : x_(x), y_(y), z_(z), w_(w), field_mofified_(true)
  {
  }

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
  int getBufferSize() const
  {
    return builder_->GetSize();
  }

  /**
   * @brief TODO
   * @return
   */
  std::vector<double> getQuaternion() const{
	  return std::vector<double>{x_, y_, z_, w_};
  }

  /**
   * @brief TODO
   * @return
   */
  void setQuaternion(std::vector<double> quat);

  /**
   * @brief TODO
   * @return
   */
  static const char* derivedTopic()
  {
    return QuaternionFbsIdentifier();
  }

private:
  double x_, y_, z_, w_;
  mutable bool field_mofified_{ false };
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
