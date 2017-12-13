
#include "simple_msgs/point_stamped.h"

namespace simple_msgs
{
PointStamped::PointStamped()
  : GenericMessage()
{
}

PointStamped::PointStamped(const Point& point, const Header& header)
  : GenericMessage()
	, point_(point), header_(header)
{
}

PointStamped::PointStamped(const uint8_t* data)
  : GenericMessage(), point_(GetPointStampedFbs(data)->point()->data()), header_(GetPointStampedFbs(data)->header()->data())
{
}

PointStamped::PointStamped(const PointStamped& other)
  : PointStamped(other.point_, other.header_)
{
}

PointStamped::PointStamped(PointStamped&& other)
  : GenericMessage()
  , point_(std::move(other.point_)), header_(std::move(other.header_))
{
}

PointStamped& PointStamped::operator=(const PointStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
	point_ = other.point_;
	header_ = other.header_;
    modified_ = true;
  }
  return *this;
}

PointStamped& PointStamped::operator=(PointStamped&& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
	point_ = std::move(other.point_);
	header_ = std::move(other.header_);
    modified_ = true;
  }
  return *this;
}

PointStamped& PointStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPointStampedFbs(data);
  point_ = p->point()->data();
  header_ = p->header()->data();
  modified_ = true;

  return *this;
}

uint8_t* PointStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
	auto point_vector = builder_->CreateVector(point_.getBufferData(), point_.getBufferSize());
	auto header_vector = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    PointStampedFbsBuilder tmp_builder(*builder_);
	tmp_builder.add_point(point_vector);
	tmp_builder.add_header(header_vector);
    FinishPointStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

std::ostream& operator<<(std::ostream& out, const PointStamped& p)
{
  out << p.header_ << p.point_;

  return out;
}
}
