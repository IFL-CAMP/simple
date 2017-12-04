#pragma once

#include "generic_message.h"
#include <mutex>
#include "bool_generated"

namespace simple_msgs
{
class Bool : public GenericMessage<Bool>
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Bool(bool data);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Bool(const uint8_t* bufferPointer);
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

	void setBool(bool data);

	bool getBool();
	static const char* derivedTopic_;
private:
	bool data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
const char* Bool::derivedTopic_ = BoolFbsIdentifier();
}
