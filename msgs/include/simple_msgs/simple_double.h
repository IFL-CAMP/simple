#pragma once

#include "generic_message.h"
#include "double_generated"
#include <mutex>

namespace simple_msgs
{
class Double : public GenericMessage<Double>
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Double(double data);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	Double(const uint8_t* bufferPointer);
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

	double getDouble();

	void setDouble(double data);
	static const char* derivedTopic(){ return DoubleFbsIdentifier(); }
private:
	double data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}
