#pragma once

#define ZMQ_STATIC

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "image_generated.h"
#include "header_generated.h"


namespace simple_msgs{

	class GenericMessage{
	public:
		GenericMessage();
		virtual ~GenericMessage();
		virtual uint8_t* getBufferData()=0;
		virtual int getBufferSize()=0;
		
	};

	class HeaderW:GenericMessage{
	public:
		HeaderW();
		HeaderW(const std::string& frame_id, const int seq, const double timeStamp);
		HeaderW(const simple_msgs::header& headerFBS);
		HeaderW(const uint8_t* data);
		virtual uint8_t* getBufferData() override;
		virtual int getBufferSize() override;
	};
	class ImageW:GenericMessage{
	public:
		ImageW();
		ImageW(const std::vector<uint8_t>& data, const uint8_t* header, const double width, const double height, const double depth, const int resX, const int resY, const int resZ, const uint8_t* pose, const std::string& enconding);
		ImageW(const std::vector<int>& data, const uint8_t* header, const double width, const double height, const double depth, const int resX, const int resY, const int resZ, const uint8_t* pose, const std::string& enconding);
		ImageW(const std::vector<float>& data, const uint8_t* header, const double width, const double height, const double depth, const int resX, const int resY, const int resZ, const uint8_t* pose, const std::string& enconding);
		ImageW(const std::vector<double>& data, const uint8_t* header, const double width, const double height, const double depth, const int resX, const int resY, const int resZ, const uint8_t* pose, const std::string& enconding);
		ImageW(const simple_msgs::image& imageFBS);
		ImageW(const uint8_t* data);
		virtual uint8_t* getBufferData() override;
		virtual int getBufferSize() override;
	};


}
