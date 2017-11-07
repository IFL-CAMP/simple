#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include "SIMPLE.pb.h"
#include "Publisher.h"
#include "myContext.h"

//handle interruptions
static int s_interrupted = 0;
static void s_signal_handler(int signal_value)
{
	s_interrupted = 1;
}
static void s_catch_signals(){
	//struct signal action;
	//action.sa_handler = s_signal_handler;
	//action.sa_flags = 0;
	//sigemptyset(&action.sa_mask);
	signal(SIGINT, s_signal_handler);
	signal(SIGTERM, s_signal_handler);
}



int main(int argc, char* argv[]) {

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	//start context
	simple::myContext globalContext;

	//start one instance of publisher for each message type

	simple::Publisher<SIMPLE::CAPABILITY> pubCap("tcp://*:5555", *globalContext.context);
	simple::Publisher<SIMPLE::TRANSFORM> pubTrans("tcp://*:5556", *globalContext.context);
	simple::Publisher<SIMPLE::POSITION> pubPos("tcp://*:5557", *globalContext.context);
	simple::Publisher<SIMPLE::STATUS> pubStat("tcp://*:5558", *globalContext.context);
	simple::Publisher<SIMPLE::GENERIC> pubGen("tcp://*:5559", *globalContext.context);

	//put random stuff on each message, for testing

	SIMPLE::HEADER* headerCap = pubCap.createHEADER(1, "CAPABILITY", "My PC");
	SIMPLE::HEADER* headerTrans = pubTrans.createHEADER(1, "TRANSFORM", "My PC");
	SIMPLE::HEADER* headerPos = pubPos.createHEADER(1, "POSITION", "My PC");
	SIMPLE::HEADER* headerStat = pubStat.createHEADER(1, "STATUS", "My PC");
	SIMPLE::HEADER* headerGen = pubGen.createHEADER(1, "GENERIC", "My PC");

	std::vector<std::string> vec = { "POSITION", "STATUS", "TRANSFORM" };
	std::unique_ptr<SIMPLE::CAPABILITY> capMSG = pubCap.createMSG(headerCap, vec);

	std::unique_ptr<SIMPLE::TRANSFORM> transMSG = pubTrans.createMSG(headerTrans, 2.1, 2.2, 2.3, 1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3);

	std::unique_ptr<SIMPLE::POSITION> posMSG = pubPos.createMSG(headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);

	std::unique_ptr<SIMPLE::STATUS> statMSG = pubStat.createMSG(headerStat, 1, 2, "Error Name", "Random message");

	std::unique_ptr<SIMPLE::GENERIC> genMSG = pubGen.createMSG(headerGen, true);

	s_catch_signals();
	while (!s_interrupted)
	{
		try{//send all messages one after the other
			pubCap.publish(*capMSG);
			std::cout << "Capability Message published" << "\n";
			pubGen.publish(*genMSG);
			std::cout << "Generic Message published" << "\n";
			pubPos.publish(*posMSG);
			std::cout << "Position Message published" << "\n";
			pubStat.publish(*statMSG);
			std::cout << "Status Message published" << "\n";
			pubTrans.publish(*transMSG);
			std::cout << "Transform Message published" << "\n";
		}
		catch (zmq::error_t& e){
			std::cout << "Something went wrong with the publishing..." << "\n";
		}
	}

	std::cout << "Interruption received, killing server" << "\n";
	
	//delete all global objects allocated by libprotobuf
	google::protobuf::ShutdownProtobufLibrary();

	

	return 0;
}