#pragma once

#include "generic_message.h"
#include <mutex>
#include "rotation_matrix_generated.h"

namespace simple_msgs
{
class RotationMatrix : public GenericMessage<RotationMatrix>
{
public:
	using GenericMessage::GenericMessage;

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	RotationMatrix(double r11,double r12,double r13,double r21,double r22,double r23,double r31,double r32,double r33);

	/**
	* @brief TODO
	* @param bufferPointer
	*/
	RotationMatrix(const uint8_t* bufferPointer);
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

	void setMatrixCoefs(std::vector<double> coefs);

	std::vector<double> getMatrixCoefs();

	static const char* derivedTopic(){ return RotationMatrixFbsIdentifier(); }
private:
	double r11_, r12_, r13_, r21_, r22_, r23_, r31_, r32_, r33_;
	mutable bool field_mofified_{ false };
	mutable std::mutex mutex_;
};
}
