#pragma once

#define ZMQ_STATIC

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "simple_msgs/simple.pb.h"
#include <chrono>

namespace simple {

class HeaderH {
  /// Header helper class.
	friend class GenericIntH;
 public:
	 HeaderH(const int versionNum,const std::string& dataTypeName,const std::string& deviceName);
	 HeaderH(){};
  const std::string& getString();
  const std::string& getString2();
 protected:
  std::unique_ptr<simple::header> h_;
  double getCurrentTime();
};
class GenericIntH{
	///GenericInt helper class
public:
	GenericIntH(){};
	GenericIntH(const int data, const HeaderH& header);
	int getData();
	const std::string& getDeviceName();
private:
	simple::generic g_;
};


}  // namespace simple
