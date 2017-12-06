
#include "simple_msgs/pose.h"

namespace simple_msgs
{
uint8_t* Pose::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_)
  {
    builder_->Clear();
    // all flatbuffer strings and vectors must be created before the creation of the table builder!
    auto positionVec = builder_->CreateVector(position_.getBufferData(), position_.getBufferSize());
    auto quaternionVec = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
    PoseFbsBuilder pBuilder(*builder_);
    pBuilder.add_position(positionVec);
    pBuilder.add_quaternion(quaternionVec);
    auto p = pBuilder.Finish();
    FinishPoseFbsBuffer(*builder_,
                        p);  // we have to explicitly call this method if we want the file_identifier to be set
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

Pose::Pose(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPoseFbs(data);
  quaternion_ = Quaternion(p->quaternion()->data());
  position_ = Point(p->position()->data());
  modified_ = true;
}

void Pose::setQuaternion(uint8_t* quaternion)
{
  std::lock_guard<std::mutex> lock(mutex_);
  quaternion_ = Quaternion(quaternion);
  modified_ = true;
}

void Pose::setPosition(uint8_t* position)
{
  std::lock_guard<std::mutex> lock(mutex_);
  position_ = Point(position);
  modified_ = true;
}
}
