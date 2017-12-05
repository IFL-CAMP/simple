#pragma once

#include "generic_message.h"
#include <mutex>
#include "rotation_matrix_generated.h"

namespace simple_msgs
{
class RotationMatrix : public GenericMessage
{
public:
  using GenericMessage::GenericMessage;

  /**
   * @brief TODO
   * @param bufferPointer
   */
  RotationMatrix(double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32,
                 double r33)
    : r11_(r11)
    , r12_(r12)
    , r13_(r13)
    , r21_(r21)
    , r22_(r22)
    , r23_(r23)
    , r31_(r31)
    , r32_(r32)
    , r33_(r33)
    , mofified_(true)
  {
  }

  /**
   * @brief TODO
   * @param bufferPointer
   */
  RotationMatrix(const uint8_t* bufferPointer);

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
   * @brief Sets the rotation matrix coeficients to the desired values
   * @param coefs Vector containing the matrix coeficients in this order: row1-col1, row1-col2, row1-col3, row2-col1,
   * row2-col2, row2-col3, row3-col1, row3-col2, row3-col3
   */
  void setMatrixCoefs(std::vector<double> coefs);

  /**
   * @brief Gets the rotation matrix coeficients
   * @return Vector containing the matrix coeficients in this order: row1-col1, row1-col2, row1-col3, row2-col1,
   * row2-col2, row2-col3, row3-col1, row3-col2, row3-col3
   */
  std::vector<double> getMatrixCoefs() const
  {
    return std::vector<double>{r11_, r12_, r13_, r21_, r22_, r23_, r31_, r32_, r33_};
  }

  static const char* getTopic() { return RotationMatrixFbsIdentifier(); }

private:
  double r11_, r12_, r13_, r21_, r22_, r23_, r31_, r32_, r33_;
  mutable bool mofified_{false};
  mutable std::mutex mutex_;
};
}  // namespace simple_msgs
