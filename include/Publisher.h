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
	///
  ///@brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port
  ///@param port string for the connection port. Ex: "tcp://*:5556"
  ///@param context reference to the existing context
	///
  Publisher(std::string port, zmq::context_t& context);
  ///@brief Class destructor. Closes the existing socket
  ~Publisher();
  ///@brief publishes the message through the open socket
  ///@param msg Protobuf-type message to be published
  void publish(const T& msg);
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
  
  SIMPLE::HEADER* createHEADER(int versionNum, std::string dataTypeName, std::string deviceName, double timeStamp);

private:
  MSGcreator msgCreator;
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple

template <typename T>
void simple::Publisher<T>::publish(const T& msg)
{
  /// Sends the message by the open socket of the publisher. Any type of message
  /// is supported

  std::string strMSG;

  msg.SerializeToString(&strMSG);  // serialize the protobuf message into a
  // string

  std::string topic = msg.GetTypeName();//add message topic to allow subscription filter

  topic.append(strMSG);

  zmq::message_t ZMQmsg(topic.size());

  memcpy(ZMQmsg.data(), topic.c_str(), topic.size());

  try
  {
    socket->send(ZMQmsg);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not send message: " << e.what();
  }
}
template <typename T>
simple::Publisher<T>::Publisher(std::string port, zmq::context_t& context)
{
  /// Class constructor: opens a socket of type ZMQ_PUB and binds it to port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUB);
  try
  {
    socket->bind(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not bind to socket:" << e.what();
  }
}
template <typename T>
simple::Publisher<T>::~Publisher()
{
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
SIMPLE::HEADER* simple::Publisher<T>::createHEADER(int versionNum, std::string dataTypeName, std::string deviceName,
                                                   double timeStamp)
{
  /// Creates the header of the message, including version number,type of the
  /// data, name of the transmiting device and time stamp of the message.
	SIMPLE::HEADER* header =msgCreator.createHEADER(versionNum, dataTypeName, deviceName, timeStamp);

  return header;
}
