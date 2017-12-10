
#include "simple_msgs/quaternion.h"

namespace simple_msgs
{
Quaternion::Quaternion()
  : GenericMessage()
  , data_{0, 0, 0, 1}
{
}

Quaternion::Quaternion(double x, double y, double z, double w)
  : GenericMessage()
  , data_{x, y, z, w}
{
}

Quaternion::Quaternion(const std::array<double, 4>& array)
  : GenericMessage()
  , data_(array)
{
}

Quaternion::Quaternion(std::array<double, 4>&& array)
  : GenericMessage()
  , data_(std::move(array))
{
}

Quaternion::Quaternion(const uint8_t* data)
  : GenericMessage()
{
  auto q = GetQuaternionFbs(data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();
}

Quaternion::Quaternion(const Quaternion& other)
  : Quaternion(other.data_)
{
}

Quaternion::Quaternion(Quaternion&& other)
  : GenericMessage()
  , data_(std::move(other.data_))
{
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = other.data_;
    modified_ = true;
  }
  return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    data_ = std::move(other.data_);
    modified_ = true;
  }
  return *this;
}

Quaternion& Quaternion::operator=(const std::array<double, 4>& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = array;
  modified_ = true;
  return *this;
}

Quaternion& Quaternion::operator=(std::array<double, 4>&& array)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_ = std::move(array);
  modified_ = true;
  return *this;
}

Quaternion& Quaternion::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto q = GetQuaternionFbs(data);
  data_[0] = q->x();
  data_[1] = q->y();
  data_[2] = q->z();
  data_[3] = q->w();
  modified_ = true;

  return *this;
}

bool Quaternion::operator==(const Quaternion& rhs) const
{
  return data_ == rhs.data_;
}

bool Quaternion::operator!=(const Quaternion& rhs) const
{
  return !(*this == rhs);
}

uint8_t* Quaternion::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    QuaternionFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_x(data_[0]);
    tmp_builder.add_y(data_[1]);
    tmp_builder.add_z(data_[2]);
    tmp_builder.add_w(data_[3]);
    FinishQuaternionFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}
void Quaternion::setX(double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[0] = x;
  modified_ = true;
}

void Quaternion::setY(double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[1] = y;
  modified_ = true;
}

void Quaternion::setZ(double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[2] = z;
  modified_ = true;
}

void Quaternion::setW(double w)
{
  std::lock_guard<std::mutex> lock(mutex_);
  data_[3] = w;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const Quaternion& q)
{
  out << "Quaternion \n \t"
      << "x: " << q.data_[0] << "\n \t"
      << "y: " << q.data_[1] << "\n \t"
      << "z: " << q.data_[2] << "\n \t"
      << "w: " << q.data_[3] << "\n";

  return out;
}
}
