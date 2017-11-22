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

  // create a Server 
  simple::Server<simple::generic> genServer("tcp://*:5555",
                                               *globalContext.context);

  //start message creator
  simple::MSGcreator msgCreator;

  simple::header* header;
  msgCreator.createHEADER(header,1, "Any", "me");

  std::shared_ptr<simple::generic> msg(new simple::generic);
  msgCreator.createGENERIC_BOOL(msg,header, true);

    //receive a request but don't reply
    try {
      if (genServer.receivedRequest(*msg)) {
        std::cout << "Received request: Capability"
                  << "\n";
      }

    } catch (zmq::error_t& e) {
    }
  
	
  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
