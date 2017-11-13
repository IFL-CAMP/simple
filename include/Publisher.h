#pragma once

#define ZMQ_STATIC

#include <zmq_utils.h>
#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "MSGcreator.h"
#include "simple_msgs/simple.pb.h"

namespace simple {
///@brief Creates a publisher socket for a specific type of message
template <typename T>
class Publisher {
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

private:
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple

template <typename T>
void simple::Publisher<T>::publish(const T& msg) {
  /// Sends the message by the open socket of the publisher. Any type of message
  /// is supported

  std::string strMSG;

  msg.SerializeToString(&strMSG);  // serialize the protobuf message into a
  // string

  std::string topic =
      msg.GetTypeName();  // add message topic to allow subscription filter

  topic.append(strMSG);

  zmq::message_t ZMQmsg(topic.size());

  memcpy(ZMQmsg.data(), topic.c_str(), topic.size());

  try {
    socket->send(ZMQmsg);
  } catch (zmq::error_t& e) {
    std::cerr << "Could not send message: " << e.what();
  }
}
template <typename T>
simple::Publisher<T>::Publisher(std::string port, zmq::context_t& context) {
  /// Class constructor: opens a socket of type ZMQ_PUB and binds it to port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUB);
  try {
    socket->bind(port);
  } catch (zmq::error_t& e) {
    std::cerr << "could not bind to socket:" << e.what();
  }
}
template <typename T>
simple::Publisher<T>::~Publisher() {
  /// Class destructor: Closes the socket
  socket->close();
}
