
#include "simple_msgs/quaternion.h"

simple_msgs::Quaternion::Quaternion(const uint8_t* bufferPointer)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto q = GetQuaternionFbs(bufferPointer);
  x_ = q->x();
  y_ = q->y();
  z_ = q->z();
  w_ = q->w();
  field_mofified_ = true;
}

uint8_t* simple_msgs::Quaternion::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (field_mofified_)
  {
    builder_->Clear();
    simple_msgs::QuaternionFbsBuilder qBuilder(*builder_);
    qBuilder.add_x(x_);
    qBuilder.add_y(y_);
    qBuilder.add_z(z_);
    qBuilder.add_w(w_);
    auto q = qBuilder.Finish();
    simple_msgs::FinishQuaternionFbsBuffer(
        *builder_, q);  // we have to explicitly call this method if we want the file_identifier to be set
    field_mofified_ = false;
  }
  return builder_->GetBufferPointer();
}

void simple_msgs::Quaternion::setQuaternion(std::vector<double> quat)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = quat.at(0);
  y_ = quat.at(1);
  z_ = quat.at(2);
  w_ = quat.at(3);
  field_mofified_ = false;
}
