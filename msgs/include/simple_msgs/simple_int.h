#pragma once

#include "generic_message.h"
#include "int_generated.h"
#include <mutex>

namespace simple_msgs
{
class Int : public GenericMessage<Int>
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Int(int data);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Int(const uint8_t* bufferPointer);
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

	void setInt(int data){ data_ = data; }

	int getInt(){ return data_; }

	static const char* derivedTopic(){ return IntFbsIdentifier(); }
private:
	int data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}
