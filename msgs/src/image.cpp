
#include "image.h"

simple_msgs::data simple_msgs::Image<uint8_t>::getDataUnionType(){
	return simple_msgs::data_dataUInt8;
}
simple_msgs::data simple_msgs::Image<int16_t>::getDataUnionType(){
	return simple_msgs::data_dataInt16;
}
simple_msgs::data simple_msgs::Image<double>::getDataUnionType(){
	return simple_msgs::data_dataDouble;
}
simple_msgs::data simple_msgs::Image<float>::getDataUnionType(){
	return simple_msgs::data_dataFloat;
}
flatbuffers::Offset<void> simple_msgs::Image<uint8_t>::getDataUnionElem(){
	return simple_msgs::CreatedataUInt8(*builder_, builder_->CreateVector(data_)).Union();
}
flatbuffers::Offset<void> simple_msgs::Image<int16_t>::getDataUnionElem(){
	return simple_msgs::CreatedataInt16(*builder_, builder_->CreateVector(data_)).Union();
}
flatbuffers::Offset<void> simple_msgs::Image<float>::getDataUnionElem(){
	return simple_msgs::CreatedataFloat(*builder_, builder_->CreateVector(data_)).Union();
}
flatbuffers::Offset<void> simple_msgs::Image<double>::getDataUnionElem(){
	return simple_msgs::CreatedataDouble(*builder_, builder_->CreateVector(data_)).Union();
}