#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Client.h"
#include "myContext.h"
#include "simple_msgs/simple.pb.h"
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
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // start context
  simple::myContext globalContext;

  // start a client for each type of message, for testing

  simple::Client<simple::position> clientGen("tcp://localhost:5555",
                                               *globalContext.context);

 //create the generic message to be requested
  simple::MSGcreator msgCreator;

  simple::header* header = msgCreator.createHEADER(1, "generic", "PC");

  std::unique_ptr<simple::position> positionMsg = msgCreator.createPOSITION(header, 0, 0, 0, 0, 0, 0, 0);

    try {
		if (clientGen.request(*positionMsg))
		{
			std::cout << "Request sent and reply received" << "\n";
		}
		else
		{
			std::cout << "No reply received" << "\n";
		}
      
    } catch (zmq::error_t& e) {
      std::cout << "Something went wrong with the request!"
                << "\n";
    }

    

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
