#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Server.h"
#include "myContext.h"
#include "simple_msgs/simple.pb.h"

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

  // create a Server for each type of message, for testing
  simple::Server<simple::CAPABILITY> capServer("tcp://*:5555",
                                               *globalContext.context);
  simple::Server<simple::TRANSFORM> transServer("tcp://*:5556",
                                                *globalContext.context);
  simple::Server<simple::POSITION> posServer("tcp://*:5557",
                                             *globalContext.context);
  simple::Server<simple::STATUS> statServer("tcp://*:5558",
                                            *globalContext.context);
  simple::Server<simple::GENERIC> genServer("tcp://*:5559",
                                            *globalContext.context);

  // create messages for each server to reply in case of request
  simple::HEADER* headerCap = capServer.createHEADER(1, "CAPABILITY", "My PC");
  simple::HEADER* headerTrans =
      transServer.createHEADER(1, "TRANSFORM", "My PC");
  simple::HEADER* headerPos = posServer.createHEADER(1, "POSITION", "My PC");
  simple::HEADER* headerStat = statServer.createHEADER(1, "STATUS", "My PC");
  simple::HEADER* headerGen = genServer.createHEADER(1, "GENERIC", "My PC");

  std::vector<std::string> vec = {"POSITION", "STATUS", "TRANSFORM"};
  std::unique_ptr<simple::CAPABILITY> capMSG =
      capServer.createMSG(headerCap, vec);
  std::unique_ptr<simple::TRANSFORM> transMSG = transServer.createMSG(
      headerTrans, 2.1, 2.2, 2.3, 1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3);
  std::unique_ptr<simple::POSITION> posMSG =
      posServer.createMSG(headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);
  std::unique_ptr<simple::STATUS> statMSG =
      statServer.createMSG(headerStat, 1, 2, "Error Name", "Random message");
  std::unique_ptr<simple::GENERIC> genMSG =
      genServer.createMSG(headerGen, true);

  s_catch_signals();
  while (!s_interrupted) {
    try {
      // wait for requests for each server
      if (capServer.receivedRequest()) {
        capServer.reply(*capMSG);
        std::cout << "Replied Capability"
                  << "\n";
      }
      if (transServer.receivedRequest()) {
        transServer.reply(*transMSG);
        std::cout << "Replied Transform"
                  << "\n";
      }
      if (posServer.receivedRequest()) {
        posServer.reply(*posMSG);
        std::cout << "Replied Position"
                  << "\n";
      }
      if (statServer.receivedRequest()) {
        statServer.reply(*statMSG);
        std::cout << "Replied Status"
                  << "\n";
      }
      if (genServer.receivedRequest()) {
        genServer.reply(*genMSG);
        std::cout << "Replied Generic"
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
