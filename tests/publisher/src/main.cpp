#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Publisher.h"
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

  // start one instance of publisher for each message type

  simple::Publisher<simple::capability> pubCap("tcp://*:5555",
                                               *globalContext.context);
  simple::Publisher<simple::transform> pubTrans("tcp://*:5556",
                                                *globalContext.context);
  simple::Publisher<simple::position> pubPos("tcp://*:5557",
                                             *globalContext.context);
  simple::Publisher<simple::status> pubStat("tcp://*:5558",
                                            *globalContext.context);
  simple::Publisher<simple::generic> pubGen("tcp://*:5559",
                                            *globalContext.context);
  //start message creator
  simple::MSGcreator msgCreator;

  // put random stuff on each message, for testing

  simple::header* headerCap = new simple::header;
  simple::header* headerTrans = new simple::header;
  simple::header* headerPos = new simple::header;
  simple::header* headerStat = new simple::header;
  simple::header* headerGen = new simple::header;
  msgCreator.createHEADER(headerCap,1, "CAPABILITY", "My PC");
  msgCreator.createHEADER(headerTrans,1, "TRANSFORM", "My PC");
  msgCreator.createHEADER(headerPos,1, "POSITION", "My PC");
  msgCreator.createHEADER(headerStat, 1, "STATUS", "My PC");
  msgCreator.createHEADER(headerGen, 1, "GENERIC", "My PC");
  
  std::shared_ptr<simple::transform> transMSG(new simple::transform);
  msgCreator.createTRANSFORM(transMSG,
	  headerTrans, 2.1, 2.2, 2.3, 1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3);

  std::vector<std::string> vec = { "POSITION", "STATUS", "TRANSFORM" };
  std::shared_ptr<simple::capability> capMSG(new simple::capability);
  msgCreator.createCAPABILITY(capMSG,headerCap, vec);

  std::shared_ptr<simple::position> posMSG(new simple::position);
  msgCreator.createPOSITION(posMSG,headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);

  std::shared_ptr<simple::status> statMSG(new simple::status);
  msgCreator.createSTATUS(statMSG,headerStat, 1, 2, "Error Name", "Random message");

  std::shared_ptr<simple::generic> genMSG(new simple::generic);
  msgCreator.createGENERIC_BOOL(genMSG, headerGen, true);
  
  
  s_catch_signals();
  while (!s_interrupted) {
    try {  // send all messages one after the other
      
      pubPos.publish(*posMSG);
      std::cout << "Position Message published"
                << "\n";
      pubGen.publish(*genMSG);
      std::cout << "Generic Message published"
                << "\n";
      pubStat.publish(*statMSG);
      std::cout << "Status Message published"
                << "\n";
      pubTrans.publish(*transMSG);
      std::cout << "Transform Message published"
                << "\n";
	  pubCap.publish(*capMSG);
	  std::cout << "Capability Message published"
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
