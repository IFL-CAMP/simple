#pragma once

#include "generic_message.h"
#include "quaternion_generated.h"
#include <mutex>

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
  Quaternion(double x, double y, double z, double w);

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

  std::vector<double> getQuaternion();

  void setQuaternion(std::vector<double> quat);
private:
	double x_, y_, z_, w_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}  // namespace simple_msgs
