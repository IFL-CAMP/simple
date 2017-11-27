#pragma once

//#define ZMQ_STATIC  // TODO: is this necessary now?

#include <memory>
#include <zmq.hpp>
#include <string>
#include "generic_message.h"

namespace simple
{
/**
 * @brief Creates a subscriber socket.
 */
template <typename T>
class Subscriber
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_SUB socket and connects it to the port.
   * @param port string for the connection port.
   * @param context reference to the existing context.
   */
	Subscriber<T>(const std::string& port, zmq::context_t& context_);

  ~Subscriber<T>();

  /**
   * @brief publishes the message through the open socket.
   * @return TODO
   */
  const T* subscribe();
  //static zmq::context_t context_;

private:
  void filterSubscription();
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

}  // namespace simple
template <typename T>
simple::Subscriber<T>::Subscriber(const std::string& port, zmq::context_t& context_)
{
  socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_SUB);
  try
  {
    socket_->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Error - Could not bind to the socket:" << e.what();
  }
}
template <typename T>
simple::Subscriber<T>::~Subscriber()
{
  socket_->close();
}
template <typename T>
const T* simple::Subscriber<T>::subscribe()
{
  zmq::message_t ZMQmessage;

  try
  {
    socket_->recv(&ZMQmessage);  // receive messages that fit the filter of the socket
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Could not receive message: " << e.what();
  }

  // return the received data as buffer
  return flatbuffers::GetRoot<T>(ZMQmessage.data());
}
template <typename T>
void simple::Subscriber<T>::filterSubscription()
{
  // No topic for now
  socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
}
