
#include "simple_msgs/point.h"

simple_msgs::Point::Point(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPointFbs(data);
  x_ = p->x();
  y_ = p->y();
  z_ = p->z();
  mofified_ = true;
}

uint8_t* simple_msgs::Point::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (mofified_)
  {
    builder_->Clear();
    simple_msgs::PointFbsBuilder ptBuilder(*builder_);
    ptBuilder.add_x(x_);
    ptBuilder.add_y(y_);
    ptBuilder.add_z(z_);
    auto p = ptBuilder.Finish();
    FinishPointFbsBuffer(*builder_,
                         p);  // we have to explicitly call this method if we want the file_identifier to be set
    mofified_ = false;
  }
  return builder_->GetBufferPointer();
}

void simple_msgs::Point::setPoint(std::vector<double> pt)
{
  std::lock_guard<std::mutex> lock(mutex_);
  x_ = pt.at(0);
  y_ = pt.at(1);
  z_ = pt.at(2);
  mofified_ = true;
}
