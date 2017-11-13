#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Subscriber.h"
#include "myContext.h"
#include "simple_msgs/simple.pb.h"
#include "MSGreader.h"

static int s_interrupted = 0;
static void s_signal_handler(int signal_value) { s_interrupted = 1; }

static void s_catch_signals(void) {
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // create context
  simple::myContext globalContext;

  // create one type of subscriber for each message type, for testing

  simple::Subscriber<simple::capability> subCap("tcp://localhost:5555",
                                                *globalContext.context);
  simple::Subscriber<simple::transform> subTrans("tcp://localhost:5556",
                                                 *globalContext.context);
  simple::Subscriber<simple::position> subPos("tcp://localhost:5557",
                                              *globalContext.context);
  simple::Subscriber<simple::status> subStat("tcp://localhost:5558",
                                             *globalContext.context);
  simple::Subscriber<simple::generic> subGen("tcp://localhost:5559",
                                             *globalContext.context);
											 
  // create the holders for the incoming data

  std::vector<std::string> Capabilities;
  simple::header CapHeader;

  simple::header TransHeader;
  double px, py, pz, r11, r12, r13, r21, r22, r23, r31, r32, r33;

  simple::header PosHeader;
  double pospx, pospy, pospz, e1, e2, e3, e4;

  simple::header StatHeader;
  int code, subcode;
  std::string errorName, errorMsg;

  simple::header GenHeader;
  bool data;

  //start message reader
  simple::MSGreader msgReader;
  
  s_catch_signals();
  while (!s_interrupted) {
    try {
      std::unique_ptr<simple::capability> recvCap = subCap.subscribe();
	  msgReader.readCAPABILITY(*recvCap, CapHeader, Capabilities);

      std::unique_ptr<simple::transform> recvTrans = subTrans.subscribe();
	  msgReader.readTRANSFORM(*recvTrans, TransHeader, px, py, pz, r11, r12, r13, r21,
                       r22, r23, r31, r32, r33);

      std::unique_ptr<simple::position> recvPos = subPos.subscribe();
	  msgReader.readPOSITION(*recvPos, PosHeader, pospx, pospy, pospz, e1, e2, e3, e4);

      std::unique_ptr<simple::status> recvStat = subStat.subscribe();
	  msgReader.readSTATUS(*recvStat, StatHeader, code, subcode, errorName,
                      errorMsg);

      std::unique_ptr<simple::generic> recvGen = subGen.subscribe();
	  msgReader.readGENERIC_BOOL(*recvGen, GenHeader, data);
    }

    catch (zmq::error_t& e) {
      std::cout << "Something went wrong with the subscription"
                << "\n";
    }
  }

  std::cout << "Interruption received"
	  << "\n";

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
