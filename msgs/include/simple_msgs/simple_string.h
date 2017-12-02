#pragma once

#include "generic_message.h"
#include <mutex>
#include "string_generated.h"

namespace simple_msgs
{
class String : GenericMessage
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	String(std::string data);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	String(const uint8_t* bufferPointer);
	/**
	* @brief TODO
	* @return
	*/
	uint8_t* getBufferData() const;
	/**
	* @brief TODO
	* @return
	*/
	int getBufferSize() const;

	std::string getString(){ return data_; }

	void setString(std::string data){ data_ = data; }
private:
	std::string data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}
