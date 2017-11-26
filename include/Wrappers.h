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
		virtual uint8_t* getBufferData();
		virtual int getBufferSize();
		
	};

	class HeaderW:GenericMessage{
		HeaderW();
		HeaderW(const std::string& frame_id, const int seq, const double timeStamp);
		HeaderW(const simple_msgs::header& headerFBS);
		HeaderW(const uint8_t* data);

	};
	class ImageW:GenericMessage{
		ImageW();
		ImageW(const uint8_t* data, const uint8_t* header, const double width, const double height, const double depth, const int resX, const int resY, const int resZ, const uint8_t* pose, const std::string& enconding);
		ImageW(const simple_msgs::image& imageFBS);
		ImageW(const uint8_t* data);
	};


}
