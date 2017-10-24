#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>


class Subscriber{
public:

	Subscriber(std::string port);
	~Subscriber();
	std::unique_ptr<SIMPLE::BASEMSG> subscribe();

	//STATUS_Params getSTATUS(SIMPLE::BASEMSG& msg);
	//POSITION_Params getPOSITION(SIMPLE::BASEMSG& msg);
	//TRANSFORM_Params getTRANSFORM(SIMPLE::BASEMSG& msg);

private:

	std::unique_ptr<zmq::socket_t> socket;
	std::unique_ptr<zmq::context_t> context;

};
struct STATUS_Params{
	int code;
	int subcode;
	std::string errorName;
	std::string errorMsg;
};
struct POSITION_Params{
	double px;
	double py;
	double pz;
	double qi;
	double qj;
	double qk;
	double qr;
};
struct TRANSFORM_Params{
	double px;
	double py;
	double pz;
	double r11;
	double r12;
	double r13;
	double r21;
	double r22;
	double r23;
	double r31;
	double r32;
	double r33;
};