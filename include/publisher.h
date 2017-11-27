#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "generic_message.h"

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
class Publisher
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Publisher(const std::string& port);

  ~Publisher();

  /**
   * @brief publishes the message through the open socket.
   * @param msg Protobuf-type message to be published.
   */
  void publish(const flatbuffers::FlatBufferBuilder& msg);
  void publish(const simple_msgs::GenericMessage& msg);
  void publish(const uint8_t* msg, const int size);

private:
  static std::unique_ptr<zmq::context_t> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

}  // namespace simple
