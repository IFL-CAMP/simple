
#include "simple_msgs/pose.h"

uint8_t* simple_msgs::Pose::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (mofified_)
  {
    builder_->Clear();
    // all flatbuffer strings and vectors must be created before the creation of the table builder!
    auto positionVec = builder_->CreateVector(position_.getBufferData(), position_.getBufferSize());
    auto quaternionVec = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
    simple_msgs::PoseFbsBuilder pBuilder(*builder_);
    pBuilder.add_position(positionVec);
    pBuilder.add_quaternion(quaternionVec);
    auto p = pBuilder.Finish();
    simple_msgs::FinishPoseFbsBuffer(
        *builder_, p);  // we have to explicitly call this method if we want the file_identifier to be set
    mofified_ = false;
  }
  return builder_->GetBufferPointer();
}

simple_msgs::Pose::Pose(const uint8_t* bufferPointer)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPoseFbs(bufferPointer);
  quaternion_ = simple_msgs::Quaternion(p->quaternion()->data());
  position_ = simple_msgs::Point(p->position()->data());
  mofified_ = true;
}

void simple_msgs::Pose::setQuaternion(uint8_t* quaternion)
{
  std::lock_guard<std::mutex> lock(mutex_);
  quaternion_ = simple_msgs::Quaternion(quaternion);
  mofified_ = true;
}

void simple_msgs::Pose::setPosition(uint8_t* position)
{
  std::lock_guard<std::mutex> lock(mutex_);
  position_ = simple_msgs::Point(position);
  mofified_ = true;
}
