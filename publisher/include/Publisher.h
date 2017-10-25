#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>

namespace simple{

	class Publisher{
		///Publisher class. Each instance of this class opens a socket of type ZMQ_PUB and binds it to the port (port is the input argument for the constructor).
		///Creates messages of any type and publishes it through the socket. Closes socket upon object destruction.
	public:

		Publisher(std::string port,zmq::context_t& context);
		~Publisher();
		void publish(const SIMPLE::BASEMSG& msg);

		std::unique_ptr<SIMPLE::BASEMSG> createTRANSFORM(SIMPLE::HEADER* header, double px, double py, double pz, double r11, double r12, double r13, double r21, double r22, double r23, double r31, double r32, double r33);
		std::unique_ptr<SIMPLE::BASEMSG> createPOSITION(SIMPLE::HEADER* header, double px, double py, double pz, double qi, double qj, double qk, double qr);
		std::unique_ptr<SIMPLE::BASEMSG> createSTATUS(SIMPLE::HEADER* header, int code, int subcode, std::string errorName, std::string errorMsg);
		std::unique_ptr<SIMPLE::BASEMSG> createCAPABILITY(SIMPLE::HEADER* header, std::vector<std::string> msgNames);
		std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_BOOL(SIMPLE::HEADER* header, bool data);
		std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_INT(SIMPLE::HEADER* header, int data);
		std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_FLOAT(SIMPLE::HEADER* header, float data);
		std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_DOUBLE(SIMPLE::HEADER* header, double data);
		std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_STR(SIMPLE::HEADER* header, std::string data);

		SIMPLE::HEADER* createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);

	private:

		std::unique_ptr<zmq::socket_t> socket;

	};
	class myContext{
	public: 
		zmq::context_t context;
		myContext(){ 
			context = zmq::context_t(1); 
		};
		~myContext(){
			context.close();
		};
	};

}