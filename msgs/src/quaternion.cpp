
#include "simple_msgs/quaternion.h"

namespace simple_msgs
{
Quaternion::Quaternion(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto q = GetQuaternionFbs(data);
  x_ = q->x();
  y_ = q->y();
  z_ = q->z();
  w_ = q->w();
  modified_ = true;
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
  if (this != std::addressof(q))
  {
    x_ = q.x_;
    y_ = q.y_;
    z_ = q.z_;
    w_ = q.w_;
    modified_ = true;
  }
  return *this;
}

bool Quaternion::operator==(const Quaternion& q) const
{
  return (x_ == q.x_ && y_ == q.y_ && z_ == q.z_ && w_ == q.w_);
}

bool Quaternion::operator!=(const Quaternion& q) const
{
  return !(*this == q);
}

uint8_t* Quaternion::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    QuaternionFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_x(x_);
    tmp_builder.add_y(y_);
    tmp_builder.add_z(z_);
    tmp_builder.add_w(w_);
    FinishQuaternionFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Quaternion::fromVector(const std::array<double, 4>& q)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = q[0];
  y_ = q[1];
  z_ = q[2];
  w_ = q[3];
  modified_ = true;
}

void Quaternion::setX(double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = x;
  modified_ = true;
}

void Quaternion::setY(double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  y_ = y;
  modified_ = true;
}

void Quaternion::setZ(double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  z_ = z;
  modified_ = true;
}

void Quaternion::setW(double w)
{
  std::lock_guard<std::mutex> lock(mutex_);
  w_ = w;
  modified_ = true;
}

std::ofstream& operator<<(std::ostream& out, const Quaternion& q)
{
  out << "Quaternion \n \t"
      << "x: " << q.x_ << "\n"
      << "y: " << q.y_ << "\n"
      << "z: " << q.z_ << "\n"
      << "w: " << q.w_ << "\n";

  return out;
}
}
