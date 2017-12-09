
#include "simple_msgs/point.h"

namespace simple_msgs
{
Point::Point(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPointFbs(data);
  x_ = p->x();
  y_ = p->y();
  z_ = p->z();
  modified_ = true;
}

Point& Point::operator=(const Point& p)
{
  if (this != std::addressof(p))
  {
    x_ = p.x_;
    y_ = p.y_;
    z_ = p.z_;
    modified_ = true;
  }
  return *this;
}

bool Point::operator==(const Point& p) const
{
  return (x_ == p.x_ && y_ == p.y_ && z_ == p.z_);
}

bool Point::operator!=(const Point& p) const
{
  return !(*this == p);
}

uint8_t* Point::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    PointFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_x(x_);
    tmp_builder.add_y(y_);
    tmp_builder.add_z(z_);
    FinishPointFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Point::fromVector(const std::array<double, 3>& p)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = p[0];
  y_ = p[1];
  z_ = p[2];
  modified_ = true;
}

void Point::setX(double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = x;
  modified_ = true;
}

void Point::setY(double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  y_ = y;
  modified_ = true;
}
void Point::setZ(double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  z_ = z;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const Point& p)
{
  out << "Point \n \t"
      << "x: " << p.x_ << "\n \t"
      << "y: " << p.y_ << "\n \t"
      << "z: " << p.z_ << "\n";

  return out;
}
}
