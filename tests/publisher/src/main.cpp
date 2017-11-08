#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Publisher.h"
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

  // start one instance of publisher for each message type

  simple::Publisher<simple::CAPABILITY> pubCap("tcp://*:5555",
                                               *globalContext.context);
  simple::Publisher<simple::TRANSFORM> pubTrans("tcp://*:5556",
                                                *globalContext.context);
  simple::Publisher<simple::POSITION> pubPos("tcp://*:5557",
                                             *globalContext.context);
  simple::Publisher<simple::STATUS> pubStat("tcp://*:5558",
                                            *globalContext.context);
  simple::Publisher<simple::GENERIC> pubGen("tcp://*:5559",
                                            *globalContext.context);

  // put random stuff on each message, for testing

  simple::HEADER* headerCap = pubCap.createHEADER(1, "CAPABILITY", "My PC");
  simple::HEADER* headerTrans = pubTrans.createHEADER(1, "TRANSFORM", "My PC");
  simple::HEADER* headerPos = pubPos.createHEADER(1, "POSITION", "My PC");
  simple::HEADER* headerStat = pubStat.createHEADER(1, "STATUS", "My PC");
  simple::HEADER* headerGen = pubGen.createHEADER(1, "GENERIC", "My PC");

  std::vector<std::string> vec = {"POSITION", "STATUS", "TRANSFORM"};
  std::unique_ptr<simple::CAPABILITY> capMSG = pubCap.createMSG(headerCap, vec);

  std::unique_ptr<simple::TRANSFORM> transMSG = pubTrans.createMSG(
      headerTrans, 2.1, 2.2, 2.3, 1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3);

  std::unique_ptr<simple::POSITION> posMSG =
      pubPos.createMSG(headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);

  std::unique_ptr<simple::STATUS> statMSG =
      pubStat.createMSG(headerStat, 1, 2, "Error Name", "Random message");

  std::unique_ptr<simple::GENERIC> genMSG = pubGen.createMSG(headerGen, true);

  s_catch_signals();
  while (!s_interrupted) {
    try {  // send all messages one after the other
      pubCap.publish(*capMSG);
      std::cout << "Capability Message published"
                << "\n";
      pubGen.publish(*genMSG);
      std::cout << "Generic Message published"
                << "\n";
      pubPos.publish(*posMSG);
      std::cout << "Position Message published"
                << "\n";
      pubStat.publish(*statMSG);
      std::cout << "Status Message published"
                << "\n";
      pubTrans.publish(*transMSG);
      std::cout << "Transform Message published"
                << "\n";
    } catch (zmq::error_t& e) {
      std::cout << "Something went wrong with the publishing..."
                << "\n";
    }
  }

  std::cout << "Interruption received, killing server"
            << "\n";

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
