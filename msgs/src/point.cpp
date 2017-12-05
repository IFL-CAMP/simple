
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
    PointFbsBuilder ptBuilder(*builder_);
    ptBuilder.add_x(x_);
    ptBuilder.add_y(y_);
    ptBuilder.add_z(z_);
    FinishPointFbsBuffer(*builder_, ptBuilder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Point::setPoint(const std::array<double, 3>& pt)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = pt[0];
  y_ = pt[1];
  z_ = pt[2];
  modified_ = true;
}

void Point::setX(const double x)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = x;
  modified_ = true;
}

void Point::setY(const double y)
{
  std::lock_guard<std::mutex> lock(mutex_);
  y_ = y;
  modified_ = true;
}
void Point::setZ(const double z)
{
  std::lock_guard<std::mutex> lock(mutex_);
  z_ = z;
  modified_ = true;
}
}
