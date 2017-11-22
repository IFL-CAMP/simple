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
static void s_signal_handler(int signal_value)
{
  s_interrupted = 1;
}
static void s_catch_signals()
{
  // struct signal action;
  // action.sa_handler = s_signal_handler;
  // action.sa_flags = 0;
  // sigemptyset(&action.sa_mask);
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[])
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // start context
  simple::myContext globalContext;

  // create a Server for each type of message, for testing
  simple::Server<simple::capability> capServer("tcp://*:5555", *globalContext.context);
  simple::Server<simple::transform> transServer("tcp://*:5556", *globalContext.context);
  simple::Server<simple::position> posServer("tcp://*:5557", *globalContext.context);
  simple::Server<simple::status> statServer("tcp://*:5558", *globalContext.context);
  simple::Server<simple::generic> genServer("tcp://*:5559", *globalContext.context);
  // start message creator
  simple::MSGcreator msgCreator;

  // create messages for each server to reply in case of request
  simple::header* headerCap = new simple::header();
  msgCreator.createHEADER(headerCap, 1, "CAPABILITY", "My PC");
  simple::header* headerTrans = new simple::header();

  msgCreator.createHEADER(headerTrans, 1, "TRANSFORM", "My PC");
  simple::header* headerPos = new simple::header();
  msgCreator.createHEADER(headerPos, 1, "POSITION", "My PC");
  simple::header* headerStat = new simple::header();
  msgCreator.createHEADER(headerStat, 1, "STATUS", "My PC");
  simple::header* headerGen = new simple::header();
  msgCreator.createHEADER(headerGen, 1, "GENERIC", "My PC");

  std::vector<std::string> vec = { "POSITION", "STATUS", "TRANSFORM" };
  std::shared_ptr<simple::capability> capMSG(new simple::capability);
  msgCreator.createCAPABILITY(capMSG, headerCap, vec);
  std::shared_ptr<simple::transform> transMSG(new simple::transform);

  msgCreator.createTRANSFORM(transMSG, headerTrans, 2.1, 2.2, 2.3, 1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3);
  std::shared_ptr<simple::position> posMSG(new simple::position);
  msgCreator.createPOSITION(posMSG,headerPos, 1.0, 1.1, 1.2, 2.1, 2.2, 2.3, 2.4);
  std::shared_ptr<simple::status> statMSG(new simple::status);
  msgCreator.createSTATUS(statMSG,headerStat, 1, 2, "Error Name", "Random message");
  std::shared_ptr<simple::generic> genMSG(new simple::generic);
  msgCreator.createGENERIC_BOOL(genMSG,headerGen, true);

  s_catch_signals();
  while (!s_interrupted)
  {
    try
    {
      // wait for requests for each server
      if (capServer.receivedRequest(*capMSG))
      {
        capServer.reply(*capMSG);
        std::cout << "Replied Capability"
                  << "\n";
      }
      if (transServer.receivedRequest(*transMSG))
      {
        transServer.reply(*transMSG);
        std::cout << "Replied Transform"
                  << "\n";
      }
      if (posServer.receivedRequest(*posMSG))
      {
        posServer.reply(*posMSG);
        std::cout << "Replied Position"
                  << "\n";
      }
      if (statServer.receivedRequest(*statMSG))
      {
        statServer.reply(*statMSG);
        std::cout << "Replied Status"
                  << "\n";
      }
      if (genServer.receivedRequest(*genMSG))
      {
        genServer.reply(*genMSG);
        std::cout << "Replied Generic"
                  << "\n";
      }
    }
    catch (zmq::error_t& e)
    {
    }
  }

  std::cout << "Interruption received, killing server"
            << "\n";

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
