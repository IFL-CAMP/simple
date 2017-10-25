#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include "SIMPLE.pb.h"
#include "Publisher.h"

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

	simple::Publisher pub("tcp://*:5556", globalContext.context);

	SIMPLE::HEADER* header = pub.createHEADER(1, "GENERIC", "My PC", 0);

	std::unique_ptr<SIMPLE::BASEMSG> baseMSG = pub.createGENERIC_INT(header, 5);

	s_catch_signals();
	while (!s_interrupted)
	{
		try{
			pub.publish(*baseMSG);
			std::cout << "Message published" << "\n";
		}
		catch (zmq::error_t& e){
			std::cout << "Interruption received" << "\n";
		}
	}

	std::cout << "Interruption received, killing server" << "\n";

	//delete all global objects allocated by libprotobuf
	google::protobuf::ShutdownProtobufLibrary();

	

	return 0;
}