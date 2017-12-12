
#include "simple_msgs/pose.h"

namespace simple_msgs
{
Pose::Pose()
  : GenericMessage()
{
}

Pose::Pose(const Point& position, const Quaternion& quaternion)
  : GenericMessage()
  , quaternion_(quaternion)
  , position_(position)
{
}

Pose::Pose(const uint8_t* data)
  : GenericMessage()
{
  auto p = GetPoseFbs(data);
  quaternion_ = Quaternion(p->quaternion()->data());
  position_ = Point(p->position()->data());
}

Pose::Pose(const Pose& other)
  : Pose(other.position_, other.quaternion_)
{
}

Pose::Pose(Pose&& other)
  : Pose(std::move(other.position_), std::move(other.quaternion_))
{
}

Pose& Pose::operator=(const Pose& p)
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    position_ = p.position_;
    quaternion_ = p.quaternion_;
    modified_ = true;
  }
  return *this;
}

Pose& Pose::operator=(Pose&& p)
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    position_ = std::move(p.position_);
    quaternion_ = std::move(p.quaternion_);
    modified_ = true;
  }
  return *this;
}

Pose& Pose::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPoseFbs(data);
  quaternion_ = Quaternion(p->quaternion()->data());
  position_ = Point(p->position()->data());
  modified_ = true;
  return *this;
}

bool Pose::operator==(const Pose& p) const
{
  return (position_ == p.position_ && quaternion_ == p.quaternion_);
}

bool Pose::operator!=(const Pose& p) const
{
  return !(*this == p);
}

uint8_t* Pose::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    auto positionVec = builder_->CreateVector(position_.getBufferData(), position_.getBufferSize());
    auto quaternionVec = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
    PoseFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_position(positionVec);
    tmp_builder.add_quaternion(quaternionVec);
    FinishPoseFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void Pose::setQuaternion(const Quaternion& quaternion)
{
  std::lock_guard<std::mutex> lock(mutex_);
  quaternion_ = quaternion;
  modified_ = true;
}

void Pose::setPosition(const Point& position)
{
  std::lock_guard<std::mutex> lock(mutex_);
  position_ = position;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const Pose& p)
{
  out << "Pose \n \t" << p.position_ << p.quaternion_;
  return out;
}
}
