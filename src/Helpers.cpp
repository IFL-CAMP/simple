#include "Helpers.h"



simple::HeaderH::HeaderH(const int versionNum, const std::string& dataTypeName, const std::string& deviceName):h_(std::make_unique<simple::header>()){
	h_->set_timestamp(getCurrentTime());
	h_->set_datatypename(dataTypeName);
	h_->set_versionnumber(versionNum);
	h_->set_devicename(deviceName);
}
double simple::HeaderH::getCurrentTime()
{
	std::chrono::time_point<std::chrono::system_clock> nowTime = std::chrono::system_clock::now();
	std::chrono::duration<double, std::ratio<1>> duration =
		std::chrono::duration_cast<std::chrono::seconds>(nowTime.time_since_epoch());
	return duration.count();
}
const std::string& simple::HeaderH::getString(){
	return h_->datatypename();
}

const std::string& simple::HeaderH::getString2(){
	return h_->devicename();
}
simple::GenericIntH::GenericIntH(const int data, const HeaderH& header) :g_(std::make_unique<simple::generic>()){
	g_->set_basicint(data);
	g_->se (header.h_.get());
}
int simple::GenericIntH::getData(){
	return g_->basicint();
}
const std::string& simple::GenericIntH::getDeviceName(){
	return(g_->head().devicename());
}