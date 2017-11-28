#pragma once

#include "generic_message.h"

namespace simple_msgs
{
class Image : public GenericMessage
{public:
	using GenericMessage::GenericMessage;
	/**
	* @brief TODO
	* @param seq_n
	* @param frame_id
	* @param timestamp
	*/
	Image(const std::string& encoding, const int resX, const int resY, const int resZ, const double width, const double height, const double depth, const std::vector<uint8_t>& data, const uint8_t* header, const uint8_t* origin);
	/**
	* @brief TODO
	* @param seq_n
	* @param frame_id
	* @param timestamp
	*/
	Image(const std::string& encoding, const int resX, const int resY, const int resZ, const double width, const double height, const double depth, const std::vector<double>& data, const uint8_t* header, const uint8_t* origin);
	/**
	* @brief TODO
	* @param seq_n
	* @param frame_id
	* @param timestamp
	*/
	Image(const std::string& encoding, const int resX, const int resY, const int resZ, const double width, const double height, const double depth, const std::vector<int16_t>& data, const uint8_t* header, const uint8_t* origin);
	/**
	* @brief TODO
	* @param seq_n
	* @param frame_id
	* @param timestamp
	*/
	Image(const std::string& encoding, const int resX, const int resY, const int resZ, const double width, const double height, const double depth, const std::vector<float>& data, const uint8_t* header, const uint8_t* origin);


private:
	int resX_{ 0 }, resY_{ 0 }, resZ_{ 0 };
	double width_{ 0.0 }, height_{ 0.0 }, depth_{ 0.0 };
	std::string encoding_{ "" };

};
}
