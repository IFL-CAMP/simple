#pragma once

#include "generic_message.h"
#include <mutex>
#include "float_generated"

namespace simple_msgs
{
class Float : public GenericMessage<Float>
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Float(float data);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Float(const uint8_t* bufferPointer);
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

	void setFloat(float data){ data_ = data; }

	float getFloat(){ return data_; }
	static const char* derivedTopic_;
private:
	float data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
const char* Float::derivedTopic_ = FloatFbsIdentifier();
}
