
#include "header.h"

simple_msgs::Header::Header(const uint8_t* data)
{
	auto h = GetHeaderFbs(data);
	seq_n_ = h->sequence_number();
	frame_id_ = h->frame_id()->c_str();
	timestamp_ = h->timestamp();
}
uint8_t* simple_msgs::Header::getBufferData() const
{
	auto h = CreateHeaderFbs(*builder_, seq_n_, builder_->CreateString(frame_id_), timestamp_);
	builder_->Finish(h);
	return builder_->GetBufferPointer();
}