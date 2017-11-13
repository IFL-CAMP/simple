#pragma once

#define ZMQ_STATIC

#include <zmq_utils.h>
#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "MSGreader.h"
#include "simple_msgs/simple.pb.h"

namespace simple {
template <typename T>
class Subscriber {
 public:
  ///@brief Class constructor: opens a socket of type ZMQ_SUB and connects it to
  ///the port. Context shall be provided
  Subscriber(std::string port, zmq::context_t& context);
  ~Subscriber();
  ///@brief Returns a message from the data received through the socket. The
  ///message type depends on the template type
  /// of the class instance. Filter for the subscriber socket shall be taken
  /// from the message type
  ///@return Protobuf-type message, matching the instance type
  std::unique_ptr<T> subscribe();
private:
  ///@brief set the socket option to match the message type according to the class instance
  ///@param msg Reference to the message instance
  void filterSubscription(const T& msg);
  ///@brief Receive a message from the connected publisher. The subscription
  ///filter is already set on the instance
  /// socket
  std::unique_ptr<zmq::socket_t> socket;
};
}  // namespace simple

template <typename T>
void simple::Subscriber<T>::filterSubscription(const T& msg) {
  std::string topic = msg.GetTypeName();

  socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
}

template <typename T>
std::unique_ptr<T> simple::Subscriber<T>::subscribe() {
  ///

  zmq::message_t ZMQmessage;

  try {
    socket->recv(
        &ZMQmessage);  // receive messages that fit the filter of the socket
  } catch (zmq::error_t& e) {
    std::cerr << "Could not receive message: " << e.what();
  }

  std::string strMessage(
      static_cast<char*>(ZMQmessage.data()),
      ZMQmessage.size());  // copy data from ZMQ message into string

  std::unique_ptr<T> SIMPLEmsg = std::make_unique<T>();

  // remove the topic string in front of the message
  strMessage.erase(0, SIMPLEmsg->GetTypeName().length());

  SIMPLEmsg->ParseFromString(strMessage);  // copy data from string to protobuf message

  return SIMPLEmsg;
}

template <typename T>
simple::Subscriber<T>::Subscriber(std::string port, zmq::context_t& context) {
  socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);

  try {
    socket->connect(port);
  } catch (zmq::error_t& e) {
    std::cerr << "could not connect to socket:" << e.what();
  }

  T BASEmsg;
  // filter the type of messages this subscriber will receive. Filter type depends on the message type
  filterSubscription(BASEmsg);
}

template <typename T>
simple::Subscriber<T>::~Subscriber() {
  // close the socket and destroy the context
  socket->close();
}

