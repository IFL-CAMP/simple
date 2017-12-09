#pragma once

#include <memory>
#include <zmq.h>
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
    : socket_(zmq_socket(context_, ZMQ_PUB))
  {
    auto success = zmq_bind(socket_, port.c_str());
    if (success != 0)
    {
      throw std::runtime_error("Cannot bind to the given address/port. ZMQ Error: " + zmq_errno());
    }
  }

  ~Publisher<T>()
  {
    zmq_close(socket_);
    zmq_ctx_term(context_);
  }
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
  void publish(const uint8_t* msg, const int msg_size)
  {
    const char* identifier = flatbuffers::GetBufferIdentifier(msg);  //< Get the message identifier.
    auto identifier_size = strlen(identifier);                       //< Size of the message identifier.

    // Create a two part message,

    // The first part contains the message 'topic' which is the Flatbuffer identifier.
    zmq_msg_t topic;
    zmq_msg_init_size(&topic, identifier_size);
    memcpy(zmq_msg_data(&topic), identifier, identifier_size);

    // The second part contains the message itself.
    zmq_msg_t message;
    zmq_msg_init_size(&message, msg_size);
    memcpy(zmq_msg_data(&message), msg, msg_size);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_sendmsg(socket_, &topic, ZMQ_SNDMORE);
    auto message_sent = zmq_sendmsg(socket_, &message, 0);

    if (topic_sent == -1 || message_sent == -1)
    {
      throw std::runtime_error("The publisher could not send the message. ZMQ Error: " + zmq_errno());
    }
  }

private:
  static void* context_;
  void* socket_;
};

template <typename T>
void* Publisher<T>::context_(zmq_ctx_new());
}  // Namespace simple.
