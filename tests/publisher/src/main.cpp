#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Publisher.h"
#include "myContext.h"
#include "MSGcreator.h"

// handle interruptions
static int s_interrupted = 0;
static void s_signal_handler(int signal_value) { s_interrupted = 1; }
static void s_catch_signals() {
  // struct signal action;
  // action.sa_handler = s_signal_handler;
  // action.sa_flags = 0;
  // sigemptyset(&action.sa_mask);
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[]) {
  
	//create global context
	simple::myContext globalContext;

	//create a transform message
	std::shared_ptr<flatbuffers::FlatBufferBuilder> msgPtr = simple::createTRANSFORM(*simple::createHeader("TRANSFORM", "PC", 1), *simple::createVec3(1.0, 2.0, 3.0), *simple::createQuaternion(2.1, 2.2, 2.3, 2.4));

	//create a publisher
	simple::Publisher pub("tcp://*:5555", *globalContext.context);

	//s_catch_signals();
	int num = 3;
	while (num>0)
	{
		try{//send all messages one after the other
			pub.publish(*msgPtr);
			std::cout << "Capability Message published" << "\n";
		}
		catch (zmq::error_t& e){
			std::cout << "Something went wrong with the publishing..." << "\n";
		}
		num--;
	}

  return 0;
}
