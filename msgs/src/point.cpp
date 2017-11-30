
#include "simple_msgs/point.h"

simple_msgs::Point::Point(const uint8_t* data)
{
	auto p = GetPointFbs(data);
	x_ = p->x();
	y_ = p->y();
	z_ = p->z();
	topic_ = simple_msgs::PointFbsIdentifier();
}
uint8_t* simple_msgs::Point::getBufferData() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (field_mofified_)
	{
		builder_->Clear();
		simple_msgs::PointFbsBuilder ptBuilder(*builder_);
		ptBuilder.add_x(x_);
		ptBuilder.add_y(y_);
		ptBuilder.add_z(z_);
		auto p = ptBuilder.Finish();
		FinishPointFbsBuffer(*builder_, p);//we have to explicitly call this method if we want the file_identifier to be set
		field_mofified_ = false;
	}
	return builder_->GetBufferPointer();
}