
#include "simple_msgs/pose_stamped.h"

namespace simple_msgs
{
PoseStamped::PoseStamped()
  : GenericMessage()
{
}

PoseStamped::PoseStamped(const Header& header, const Pose& pose)
  : GenericMessage()
  , pose_(pose)
  , header_(header)
{
}

PoseStamped::PoseStamped(const uint8_t* data)
  : GenericMessage()
  , pose_(GetPoseStampedFbs(data)->pose()->data())
  , header_(GetPoseStampedFbs(data)->header()->data())
{
}

PoseStamped::PoseStamped(const PoseStamped& other)
  : PoseStamped(other.header_, other.pose_)
{
}

PoseStamped::PoseStamped(PoseStamped&& other)
  : PoseStamped(std::move(other.header_), std::move(other.pose_))
{
}

PoseStamped& PoseStamped::operator=(const PoseStamped& p)
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pose_ = p.pose_;
    header_ = p.header_;
    modified_ = true;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(PoseStamped&& p)
{
  if (this != std::addressof(p))
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pose_ = std::move(p.pose_);
    header_ = std::move(p.header_);
    modified_ = true;
  }
  return *this;
}

PoseStamped& PoseStamped::operator=(const uint8_t* data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  auto p = GetPoseStampedFbs(data);
  pose_ = Pose(p->pose()->data());
  header_ = Header(p->header()->data());
  modified_ = true;
  return *this;
}

uint8_t* PoseStamped::getBufferData() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (modified_ || pose_.isModified() || header_.isModified())
  {
    builder_->Clear();
    auto poseVec = builder_->CreateVector(pose_.getBufferData(), pose_.getBufferSize());
    auto headerVec = builder_->CreateVector(header_.getBufferData(), header_.getBufferSize());
    PoseStampedFbsBuilder tmp_builder(*builder_);
    tmp_builder.add_pose(poseVec);
    tmp_builder.add_header(headerVec);
    FinishPoseStampedFbsBuffer(*builder_, tmp_builder.Finish());
    modified_ = false;
  }
  return builder_->GetBufferPointer();
}

void PoseStamped::setPose(const Pose& pose)
{
  std::lock_guard<std::mutex> lock(mutex_);
  pose_ = pose;
  modified_ = true;
}

void PoseStamped::setHeader(const Header& header)
{
  std::lock_guard<std::mutex> lock(mutex_);
  header_ = header;
  modified_ = true;
}

std::ostream& operator<<(std::ostream& out, const PoseStamped& p)
{
  out << p.header_ << p.pose_;
  return out;
}
}
