
#include "simple_msgs/quaternion_stamped.h"

namespace simple_msgs
{
QuaternionStamped::QuaternionStamped()
  : GenericMessage()
{
}

QuaternionStamped::QuaternionStamped(const Header& header, const Quaternion& quaternion)
  : GenericMessage()
  , quaternion_(quaternion)
  , header_(header)
{
}

QuaternionStamped::QuaternionStamped(const uint8_t* data)
  : GenericMessage()
  , quaternion_(GetQuaternionStampedFbs(data)->quaternion()->data())
  , header_(GetQuaternionStampedFbs(data)->header()->data())
{
}

QuaternionStamped::QuaternionStamped(const QuaternionStamped& other)
  : QuaternionStamped(other.header_, other.quaternion_)
{
}

QuaternionStamped::QuaternionStamped(QuaternionStamped&& other)
  : QuaternionStamped(std::move(other.header_), std::move(other.quaternion_))
{
}

QuaternionStamped& QuaternionStamped::operator=(const QuaternionStamped& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    quaternion_ = other.quaternion_;
    header_ = other.header_;
    modified_ = true;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(QuaternionStamped&& other)
{
  if (this != std::addressof(other))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    quaternion_ = std::move(other.quaternion_);
    header_ = std::move(other.header_);
    modified_ = true;
  }
  return *this;
}

QuaternionStamped& QuaternionStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto matrix = GetQuaternionStampedFbs(data);
  quaternion_ = Quaternion(matrix->quaternion()->data());
  header_ = Header(matrix->header()->data());
  modified_ = true;

  return *this;
}

uint8_t* QuaternionStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_ || header_.isModified() || quaternion_.isModified())
  {
    builder_->Clear();
    auto quaternion_vector = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
    auto header_vector = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    QuaternionStampedFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_quaternion(quaternion_vector);
    tmp_builder.add_header(header_vector);
    FinishQuaternionStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

inline void QuaternionStamped::setQuaternion(const Quaternion& quaternion)
{
  std::lock_guard<std::mutex> lock(mutex_);
  quaternion_ = quaternion;
  modified_ = true;
}

inline void QuaternionStamped::setHeader(const Header& header)
{
  std::lock_guard<std::mutex> lock(mutex_);
  header_ = header;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const QuaternionStamped& q)
{
  out << q.header_ << q.quaternion_;

  return out;
}
}  // namespace simple_msgs
