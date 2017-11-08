#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Client.h"
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

  // start a client for each type of message, for testing

  simple::Client<simple::CAPABILITY> clientCap("tcp://localhost:5555",
                                               *globalContext.context);
  simple::Client<simple::TRANSFORM> clientTrans("tcp://localhost:5556",
                                                *globalContext.context);
  simple::Client<simple::POSITION> clientPos("tcp://localhost:5557",
                                             *globalContext.context);
  simple::Client<simple::STATUS> clientStat("tcp://localhost:5558",
                                            *globalContext.context);
  simple::Client<simple::GENERIC> clientGen("tcp://localhost:5559",
                                            *globalContext.context);

  // create the holders for the incoming data

  std::vector<std::string> Capabilities;
  simple::HEADER CapHeader;

  simple::HEADER TransHeader;
  double px, py, pz, r11, r12, r13, r21, r22, r23, r31, r32, r33;

  simple::HEADER PosHeader;
  double pospx, pospy, pospz, e1, e2, e3, e4;

  simple::HEADER StatHeader;
  int code, subcode;
  std::string errorName, errorMsg;

  simple::HEADER GenHeader;
  bool data;

  s_catch_signals();
  while (!s_interrupted) {
    try {
      std::unique_ptr<simple::CAPABILITY> recvCap = clientCap.request();
      clientCap.readMsg(*recvCap, CapHeader, Capabilities);

      std::unique_ptr<simple::TRANSFORM> recvTrans = clientTrans.request();
      clientTrans.readMsg(*recvTrans, TransHeader, px, py, pz, r11, r12, r13,
                          r21, r22, r23, r31, r32, r33);

      std::unique_ptr<simple::POSITION> recvPos = clientPos.request();
      clientPos.readMsg(*recvPos, PosHeader, pospx, pospy, pospz, e1, e2, e3,
                        e4);

      std::unique_ptr<simple::STATUS> recvStat = clientStat.request();
      clientStat.readMsg(*recvStat, StatHeader, code, subcode, errorName,
                         errorMsg);

      std::unique_ptr<simple::GENERIC> recvGen = clientGen.request();
      clientGen.readMsg(*recvGen, GenHeader, data);
    } catch (zmq::error_t& e) {
      std::cout << "Something went wrong with the request!"
                << "\n";
    }

    // print the received data
    std::cout << "Capability received: " << Capabilities.at(0) << ", "
              << Capabilities.at(1) << ", " << Capabilities.at(2) << "\n";
    std::cout << "Transform received: px=" << px << ", py=" << py
              << ", pz=" << pz << ", r11=" << r11 << "\n";
    std::cout << "Position received: px=" << pospx << ", py=" << pospy
              << ", pz=" << pospz << ", e1=" << e1 << ", e2=" << e2 << "\n";
    std::cout << "Status received: code=" << code << ", subcode=" << subcode
              << ", errorName=" << errorName << ", errorMsg=" << errorMsg
              << "\n";
    std::cout << "Generic received: bool=" << data << "\n";
  }

  std::cout << "Interruption received, killing server"
            << "\n";

  // delete all global objects allocated by libprotobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
