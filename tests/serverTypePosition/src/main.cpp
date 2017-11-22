#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Server.h"
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

  // create a Server of type position
  simple::Server<simple::position> posServer("tcp://*:5555",
                                             *globalContext.context);

  //create a message creator
  simple::MSGcreator msgCreator;

  // create messages for each server to reply in case of request
  simple::header* headerPos;
  msgCreator.createHEADER(headerPos,1, "POSITION", "My PC");

  
  std::shared_ptr<simple::position> posMSG(new simple::position);
  msgCreator.createPOSITION(posMSG,headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);

  s_catch_signals();
  while (!s_interrupted) {
    try {
      // wait for requests for each server
      
      if (posServer.receivedRequest(*posMSG)) {
		  std::cout << "Received request for position" << "\n";
        posServer.reply(*posMSG);
        std::cout << "Replied Position"
                  << "\n";
      }

    } catch (zmq::error_t& e) {
    }
  }

  std::cout << "Interruption received, killing server"
            << "\n";

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
