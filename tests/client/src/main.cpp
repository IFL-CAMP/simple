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

  simple::Client<simple::capability> clientCap("tcp://localhost:5555",
                                               *globalContext.context);
  simple::Client<simple::transform> clientTrans("tcp://localhost:5556",
                                                *globalContext.context);
  simple::Client<simple::position> clientPos("tcp://localhost:5557",
                                             *globalContext.context);
  simple::Client<simple::status> clientStat("tcp://localhost:5558",
                                            *globalContext.context);
  simple::Client<simple::generic> clientGen("tcp://localhost:5559",
                                            *globalContext.context);
  //start message creator
  simple::MSGcreator msgCreator;

  // create the holders for the incoming data
  simple::header* headerCap = new simple::header();
  msgCreator.createHEADER(headerCap,1, "Capability", "PC");
  simple::header* headerTrans = new simple::header(); 
  msgCreator.createHEADER(headerTrans,1, "Transform", "PC");
  simple::header* headerPos = new simple::header(); 
  msgCreator.createHEADER(headerPos,1, "Position", "PC");
  simple::header* headerStat = new simple::header(); 
  msgCreator.createHEADER(headerStat,1, "Status", "PC");
  simple::header* headerGen = new simple::header(); 
  msgCreator.createHEADER(headerGen,1, "Generic", "PC");

  std::vector<std::string> vec{ "status", "transform" };

  std::shared_ptr<simple::capability> msgCap(new simple::capability);
  msgCreator.createCAPABILITY(msgCap,headerCap, vec);
  std::shared_ptr<simple::transform> msgTrans(new simple::transform);
  msgCreator.createTRANSFORM(msgTrans,headerTrans, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  std::shared_ptr<simple::position> msgPos(new simple::position);
  msgCreator.createPOSITION(msgPos,headerPos, 0, 0, 0, 0, 0, 0, 0);
  std::shared_ptr<simple::status> msgStat(new simple::status);
  msgCreator.createSTATUS(msgStat,headerStat, 1, 1, "", "");
  std::shared_ptr<simple::generic> msgGen(new simple::generic);
  msgCreator.createGENERIC_BOOL(msgGen,headerGen, true);

  s_catch_signals();
  while (!s_interrupted) {
		if (clientCap.request(*msgCap))
		{
			std::cout << "Message capability requested and received" << "\n";
		}
		else
		{
			std::cout << "Message capability not received" << "\n";
		}
		if (clientTrans.request(*msgTrans))
		{
			std::cout << "Message transform requested and received" << "\n";
		}
		else
		{
			std::cout << "Message transform not received" << "\n";
		}
		if (clientPos.request(*msgPos))
		{
			std::cout << "Message position requested and received" << "\n";
		}
		else
		{
			std::cout << "Message position not received" << "\n";
		}
		if (clientStat.request(*msgStat))
		{
			std::cout << "Message status requested and received" << "\n";
		}
		else
		{
			std::cout << "Message status not received" << "\n";
		}
		if (clientGen.request(*msgGen))
		{
			std::cout << "Message generic requested and received" << "\n";
		}
		else
		{
			std::cout << "Message generic not received" << "\n";
		}
    
  }

  std::cout << "Interruption received, killing server"
            << "\n";

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
