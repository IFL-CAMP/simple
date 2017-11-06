#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>
#include "MSGcreator.h"

namespace simple
{
///@brief Creates a publisher socket for a specific type of message
template <typename T>
class Publisher
{
public:
  ///@brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port
  ///@param port string for the connection port. Ex: "tcp://*:5556"
  ///@param context reference to the existing context
  Publisher(std::string port, zmq::context_t& context);
  ///@brief Class destructor. Closes the existing socket
  ~Publisher();
  ///@brief publishes the message through the open socket
  ///@param msg Protobuf-type message to be published
  void publish(T msg);

  std::unique_ptr<SIMPLE::BASEMSG> createTRANSFORM(SIMPLE::HEADER* header, double px, double py, double pz, double r11,
                                                   double r12, double r13, double r21, double r22, double r23,
                                                   double r31, double r32, double r33);
  std::unique_ptr<SIMPLE::BASEMSG> createPOSITION(SIMPLE::HEADER* header, double px, double py, double pz, double qi,
                                                  double qj, double qk, double qr);
  std::unique_ptr<SIMPLE::BASEMSG> createSTATUS(SIMPLE::HEADER* header, int code, int subcode, std::string errorName,
                                                std::string errorMsg);
  std::unique_ptr<SIMPLE::BASEMSG> createCAPABILITY(SIMPLE::HEADER* header, std::vector<std::string> msgNames);
  std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_BOOL(SIMPLE::HEADER* header, bool data);
  std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_INT(SIMPLE::HEADER* header, int data);
  std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_FLOAT(SIMPLE::HEADER* header, float data);
  std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_DOUBLE(SIMPLE::HEADER* header, double data);
  std::unique_ptr<SIMPLE::BASEMSG> createGENERIC_STR(SIMPLE::HEADER* header, std::string data);

  SIMPLE::HEADER* createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);

private:
  MSGcreator msgCreator;
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple
