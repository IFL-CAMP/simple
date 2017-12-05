
#include "simple_msgs/rotation_matrix.h"

void simple_msgs::RotationMatrix::setMatrixCoefs(std::vector<double> coefs)
{
  r11_ = coefs.at(0);
  r12_ = coefs.at(1);
  r13_ = coefs.at(2);
  r21_ = coefs.at(3);
  r22_ = coefs.at(4);
  r23_ = coefs.at(5);
  r31_ = coefs.at(6);
  r32_ = coefs.at(7);
  r33_ = coefs.at(8);
  mofified_ = true;
}

uint8_t* simple_msgs::RotationMatrix::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (mofified_)
  {
    builder_->Clear();
    simple_msgs::RotationMatrixFbsBuilder rBuilder(*builder_);
    rBuilder.add_r11(r11_);
    rBuilder.add_r12(r12_);
    rBuilder.add_r13(r13_);
    rBuilder.add_r21(r21_);
    rBuilder.add_r22(r22_);
    rBuilder.add_r23(r23_);
    rBuilder.add_r31(r31_);
    rBuilder.add_r32(r32_);
    rBuilder.add_r33(r33_);
    auto r = rBuilder.Finish();
    simple_msgs::FinishRotationMatrixFbsBuffer(
        *builder_, r);  // we have to explicitly call this method if we want the file_identifier to be set
    mofified_ = false;
  }
  return builder_->GetBufferPointer();
}

simple_msgs::RotationMatrix::RotationMatrix(const uint8_t* bufferPointer)
{
  auto r = GetRotationMatrixFbs(bufferPointer);
  r11_ = r->r11();
  r12_ = r->r12();
  r13_ = r->r13();
  r21_ = r->r21();
  r22_ = r->r22();
  r23_ = r->r23();
  r31_ = r->r31();
  r32_ = r->r32();
  r33_ = r->r33();
  mofified_ = true;
}
