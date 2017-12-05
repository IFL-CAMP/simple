
#include "simple_msgs/simple_bool.h"

simple_msgs::Bool::Bool(const uint8_t* bufferPointer){
	auto b = GetBoolFbs(bufferPointer);
	data_ = b->data();
	field_mofified_ = true;
}

uint8_t* simple_msgs::Bool::getBufferData() const{
	std::lock_guard<std::mutex> lock(mutex_);
	if (field_mofified_)
	{
		builder_->Clear();
		simple_msgs::BoolFbsBuilder bBuilder(*builder_);
		bBuilder.add_data(data_);
		auto b = bBuilder.Finish();
		simple_msgs::FinishBoolFbsBuffer(*builder_, b);//we have to explicitly call this method if we want the file_identifier to be set
		field_mofified_ = false;
	}
	return builder_->GetBufferPointer();
}