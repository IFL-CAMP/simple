#pragma once

#include "generic_message.h"
#include "double_generated.h"
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
	Double(double data) :data_(data), field_mofified_(true){}

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
	int getBufferSize() const{
		return builder_->GetSize();
	}

	double getDouble() const{
		return data_;
	}

	void setDouble(double data){
		data_ = data;
		field_mofified_ = true;
	}

	static const char* derivedTopic(){ return DoubleFbsIdentifier(); }
private:
	double data_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}
