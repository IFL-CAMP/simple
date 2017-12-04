
#include "simple_msgs/pose.h"

const char* simple_msgs::Pose::derivedTopic_ = PoseFbsIdentifier();
uint8_t* simple_msgs::Pose::getBufferData() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (field_mofified_)
	{
		builder_->Clear();
		//all flatbuffer strings and vectors must be created before the creation of the table builder!
		auto positionVec = builder_->CreateVector(position_.getBufferData(), position_.getBufferSize());
		auto quaternionVec = builder_->CreateVector(quaternion_.getBufferData(), quaternion_.getBufferSize());
		simple_msgs::PoseFbsBuilder pBuilder(*builder_);
		pBuilder.add_position(positionVec);
		pBuilder.add_quaternion(quaternionVec);
		auto p = pBuilder.Finish();
		simple_msgs::FinishPoseFbsBuffer(*builder_, p);	//we have to explicitly call this method if we want the file_identifier to be set
		field_mofified_ = false;
	}
	return builder_->GetBufferPointer();
}
