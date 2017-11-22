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

  simple::Client<simple::status> clientStat("tcp://localhost:5558",
                                            *globalContext.context);

  //start the message creator
  simple::MSGcreator msgCreator;

  simple::header* header = new simple::header();
  msgCreator.createHEADER(header,1, "Status", "PC");

  std::shared_ptr<simple::status> msg(new simple::status);
  msgCreator.createSTATUS(msg,header, 1, 1, "", "");

  if (clientStat.request(*msg))
  {
	  std::cout << "Request sent and reply received" << "\n";
  }
  else
  {
	  std::cout << "Request-reply failed" << "\n";
  }


  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
