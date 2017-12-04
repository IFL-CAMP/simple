#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "simple_msgs/generic_message.h"
#include "simple\contextCloser.h"

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Publisher<T>(const std::string& port) : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_PUB))
  {
    try
    {
      socket_->bind(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }
  }

  ~Publisher<T>()
  {
    socket_->close();
    context_->close();
  }

  /**
   * @brief publishes the message through the open socket.
   * @param msg Protobuf-type message to be published.
   */
  void publish(const flatbuffers::FlatBufferBuilder& msg)
  {
    // Get the data from the message.
    uint8_t* buffer = msg.GetBufferPointer();
    int buffer_size = msg.GetSize();
    publish(buffer, buffer_size);
  }
  void publish(const simple_msgs::GenericMessage<T>& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    publish(buffer, buffer_size);
  }
  void publish(const uint8_t* msg, const int size)
  {
    // get message topic to allow subscription filter
    const char* topic = flatbuffers::GetBufferIdentifier(msg);
    // get the topic size
    int s = strlen(topic);
    // create new array
    uint8_t* prefixedMsg;
    // copy the topic and the buffer into the new array
    memcpy(prefixedMsg, topic, s);
    memcpy(prefixedMsg + s, msg, size);
    // create ZMQ message of size (buffer + prefixed topic)
    zmq::message_t ZMQ_message(size + s);
    // put the data into the ZMQ message
    memcpy(ZMQ_message.data(), prefixedMsg, size + s);

    try
    {
      socket_->send(ZMQ_message);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not send the message: " << e.what();
    }
  }

private:
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};
template<typename T>
std::unique_ptr<zmq::context_t, contextCloser> Publisher<T>::context_(new zmq::context_t(1));
}  // namespace simple



