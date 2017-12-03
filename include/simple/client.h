#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "simple_msgs/generic_message.h"
#include "simple\contextCloser.h"
#include <thread>

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Client
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Client<T>(const std::string& port);

  ~Client<T>();

  /**
   * @brief publishes the message through the open socket.
   * @param msg Protobuf-type message to be published.
   */
  void request(const flatbuffers::FlatBufferBuilder& msg);
  void request(const simple_msgs::GenericMessage& msg);
  void request(const uint8_t* msg, const int size);

private:
  std::thread t_;
  bool alive_{ true };
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};
template <typename T>
std::unique_ptr<zmq::context_t, contextCloser> Client<T>::context_(new zmq::context_t(1));
}  // namespace simple
