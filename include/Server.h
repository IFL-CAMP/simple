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
template <typename T>
class Server
{
  /// Server class.
public:
  Server(std::string port, zmq::context_t& context);
  ~Server();
  ///@brief Replies with the content provided
  ///@param msg Protobuf message containing the data to be replied
  void reply(const T& msg);
  ///@brief Receives a request from a client through the socket and checks if the type requested matches the server
  /// instance type
  ///@return Returns True if the request can be answered, False otherwise
  bool receiveRequest();
  ///@brief Creates a message of type TRANSFORM
  ///@param header pointer to the header of the message
  ///@param px element e14 of 4x4 transformation matrix
  ///@param py element e24 of 4x4 transformation matrix
  ///@param pz element e34 of 4x4 transformation matrix
  ///@param r11 element e11 of 4x4 transformation matrix
  ///@param r12 element e12 of 4x4 transformation matrix
  ///@param r13 element e13 of 4x4 transformation matrix
  ///@param r21 element e21 of 4x4 transformation matrix
  ///@param r22 element e22 of 4x4 transformation matrix
  ///@param r23 element e23 of 4x4 transformation matrix
  ///@param r31 element e31 of 4x4 transformation matrix
  ///@param r32 element e32 of 4x4 transformation matrix
  ///@param r33 element e33 of 4x4 transformation matrix
  ///@return pointer to message of type TRANSFORM
  std::unique_ptr<SIMPLE::TRANSFORM> createMSG(SIMPLE::HEADER* header, double px, double py, double pz, double r11,
	  double r12, double r13, double r21, double r22, double r23, double r31,
	  double r32, double r33);
  ///@brief Creates a message of type POSITION
  ///@param header pointer to the header of the message
  ///@param px first element of 3D vector for the position
  ///@param py second element of 3D vector for the position
  ///@param pz third element of 3D vector for the position
  ///@param e1 first quaternion: e1 = k_x*sin(theta/2)
  ///@param e2 second quaternion: e2 = k_y*sin(theta/2)
  ///@param e3 third quaternion: e3 = k_z*sin(theta/2)
  ///@param e4 fourth quaternion: e4 = cos(theta/2)
  ///@return pointer to message of type POSITION
  std::unique_ptr<SIMPLE::POSITION> createMSG(SIMPLE::HEADER* header, double px, double py, double pz, double e1,
	  double e2, double e3, double e4);
  ///@brief Creates a message of type STATUS
  ///@param header pointer to the header of the message
  ///@param code device status code - 0 to 19
  ///@param subcode device-specific code, defined by developer
  ///@param errorName Name of the error
  ///@param errorMsg Message detailing the error
  ///@return pointer to message of type STATUS
  std::unique_ptr<SIMPLE::STATUS> createMSG(SIMPLE::HEADER* header, int code, int subcode, std::string errorName,
	  std::string errorMsg);
  ///@brief Creates a message of type CAPABILITY
  ///@param header pointer to the header of the message
  ///@param msgNames Vector containing the name of the types of messages supported by the device
  ///@return pointer to message of type CAPABILITY
  std::unique_ptr<SIMPLE::CAPABILITY> createMSG(SIMPLE::HEADER* header, std::vector<std::string> msgNames);
  ///@brief Creates a message of type GENERIC containing a BOOL
  ///@param header pointer to the header of the message
  ///@param data bool to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<SIMPLE::GENERIC> createMSG(SIMPLE::HEADER* header, bool data);
  ///@brief Creates a message of type GENERIC containing a INT
  ///@param header pointer to the header of the message
  ///@param data int to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<SIMPLE::GENERIC> createMSG(SIMPLE::HEADER* header, int data);
  ///@brief Creates a message of type GENERIC containing a FLOAT
  ///@param header pointer to the header of the message
  ///@param data float to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<SIMPLE::GENERIC> createMSG(SIMPLE::HEADER* header, float data);
  ///@brief Creates a message of type GENERIC containing a DOUBLE
  ///@param header pointer to the header of the message
  ///@param data double to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<SIMPLE::GENERIC> createMSG(SIMPLE::HEADER* header, double data);
  ///@brief Creates a message of type GENERIC containing a STRING
  ///@param header pointer to the header of the message
  ///@param data string to be sent in the message
  ///@return pointer to message of type GENERIC
  std::unique_ptr<SIMPLE::GENERIC> createMSG(SIMPLE::HEADER* header, std::string data);
private:
  std::unique_ptr<zmq::socket_t> socket;
  MSGcreator msgCreator;
};

}  // namespace simple
template <typename T>
simple::Server<T>::Server(std::string port, zmq::context_t& context)
{
  /// Class constructor: opens a socket of type REP and connects it to the port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_REP);

  try
  {
    socket->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not bind socket:" << e.what();
  }
}
template <typename T>
simple::Server<T>::~Server()
{
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
bool simple::Server<T>::receiveRequest()
{
  // wait for next request.
  zmq::message_t recvREQ;

  try
  {
    socket->recv(&recvREQ);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not receive message: " << e.what();
  }

  // create reply message
  T msgReply;

  // check requested type matches the reply type
  std::string replyTopic = msgReply.GetTypeName();

  std::string strMessage(static_cast<char*>(recvREQ.data()),
                         recvREQ.size());  // copy data from ZMQ message into string

  return strMessage == replyTopic;
}
template <typename T>
void simple::Server<T>::reply(const T& msg)
{
  // Reply with the data

  // serialize data to string
  std::string strReply;
  msg.SerializeoString(&strReply);
  // create ZMQ message for reply
  zmq::message_t ZMQreply(strReply.size());
  // copy data to ZMQ message
  memcpy(ZMQreply.data(), strReply.c_str(), strReply.size());
  // send the reply through the socket
  try
  {
    socket->send(ZMQreply);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not send message: " << e.what();
  }
}
