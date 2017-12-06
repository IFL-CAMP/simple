#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>

#include "simple_msgs/generic_message.h"
#include "simple/context_deleter.h"

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
  Publisher<T>(const std::string& port)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_PUB))
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

  ~Publisher<T>() { socket_->close(); }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: Flatbuffer-type message to be published.
   */
  void publish(const flatbuffers::FlatBufferBuilder& msg)
  {
    uint8_t* buffer = msg.GetBufferPointer();  //< Get the data from the message.
    int buffer_size = msg.GetSize();
    publish(buffer, buffer_size);
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void publish(const simple_msgs::GenericMessage& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    publish(buffer, buffer_size);
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be publish.
   */
  void publish(const uint8_t* msg, const int size)
  {
    // TODO
    // get message topic to allow subscription filter
    const char* topic = flatbuffers::GetBufferIdentifier(msg);
    // get the topic size
    int s = strlen(topic);

    // create ZMQ message of size (buffer + prefixed topic)
    int totalSize = size + s;
    zmq::message_t ZMQ_message(totalSize);
    // put the data into the ZMQ message
    memcpy(ZMQ_message.data(), topic, s);
    memcpy(static_cast<uint8_t*>(ZMQ_message.data()) + s, msg, size);

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
  static std::unique_ptr<zmq::context_t, contextDeleter> context_;
  std::unique_ptr<zmq::socket_t> socket_;
};

template <typename T>
std::unique_ptr<zmq::context_t, contextDeleter> Publisher<T>::context_(new zmq::context_t(1));
}  // Namespace simple.
