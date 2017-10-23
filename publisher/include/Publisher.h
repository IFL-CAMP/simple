#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>


class Publisher{
public:

	Publisher(std::string port);
	~Publisher();
	void publish(const SIMPLE::BASEMSG& msg);

	std::unique_ptr<SIMPLE::BASEMSG> createTRANSFORM(SIMPLE::HEADER& header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);
	std::unique_ptr<SIMPLE::BASEMSG> createPOSITION(SIMPLE::HEADER& header, double px, double py, double pz, double qi, double qj, double qk, double qr);
	std::unique_ptr<SIMPLE::BASEMSG> createSTATUS(SIMPLE::HEADER& header, int code, int subcode, std::string errorName, std::string errorMsg);
	std::unique_ptr<SIMPLE::BASEMSG> createCAPABILITY(SIMPLE::HEADER& header, std::vector<std::string> msgNames);
	std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_BOOL(SIMPLE::HEADER& header, bool data);
	std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_INT(SIMPLE::HEADER& header, int data);
	std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_FLOAT(SIMPLE::HEADER& header, float data);
	std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_DOUBLE(SIMPLE::HEADER& header, double data);
	std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_STR(SIMPLE::HEADER& header, std::string data);

	std::unique_ptr<SIMPLE::HEADER> createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, int timeStamp);

private:

	std::unique_ptr<zmq::socket_t> socket;
	std::unique_ptr<zmq::context_t> context;

};

