#pragma once

#define ZMQ_STATIC

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "MSGreader.h"
#include "flatbuffers/flatbuffers.h"
#include <iostream>
#include "transform_generated.h"

namespace simple
{
template <typename T>
class Subscriber
{
public:
  ///@brief Class constructor: opens a socket of type ZMQ_SUB and connects it to
  /// the port. Context shall be provided
  Subscriber<T>(std::string port, zmq::context_t& context);
  ~Subscriber<T>();
  ///@brief Returns a message from the data received through the socket. The
  /// message type depends on the template type
  /// of the class instance. Filter for the subscriber socket shall be taken
  /// from the message type
  ///@return Protobuf-type message, matching the instance type
  std::unique_ptr<T> subscribe();

private:
  ///@brief set the socket option to match the message type according to the
  /// class instance
  ///@param msg Reference to the message instance
  void filterSubscription();
  ///@brief Receive a message from the connected publisher. The subscription
  /// filter is already set on the instance
  /// socket
  std::unique_ptr<zmq::socket_t> socket;
};
}  // namespace simple
template <typename T>
void simple::Subscriber<T>::filterSubscription()
{
  socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
}
template <typename T>
std::unique_ptr<T> simple::Subscriber<T>::subscribe()
{
  ///

  zmq::message_t ZMQmessage;

  try
  {
    socket->recv(&ZMQmessage);  // receive messages that fit the filter of the socket
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Could not receive message: " << e.what();
  }

  // put the received data into the buffer
  std::unique_ptr<T> SIMPLEmsg = std::make_unique<T>(flatbuffers::GetRoot(*ZMQmessage.data());

  return SIMPLEmsg;
}
template <typename T>
simple::Subscriber<T>::Subscriber(std::string port, zmq::context_t& context)
{
  socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);

  try
  {
    socket->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "could not connect to socket:" << e.what();
  }
  // filter the type of messages this subscriber will receive. Filter type
  // depends on the message type
  filterSubscription();
}
template <typename T>
simple::Subscriber<T>::~Subscriber()
{
  // close the socket and destroy the context
  socket->close();
}
