#pragma once

#include <memory>
#include <zmq.h>
#include <string>
#include <thread>

#include "simple_msgs/generic_message.h"

namespace simple
{
/**
 * @brief Creates a client socket for a specific type of message.
 */
template <typename T>
class Client
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_REQ socket and connects it to the port. The user defined callback function
   * is responsible for using the received reply the way it wants
   * @param port string for the connection port.
   */
  Client<T>(const std::string& port)
    : socket_(zmq_socket(context_, ZMQ_REQ))
  {
    auto success = zmq_connect(socket_, port.c_str());
    if (success != 0)
    {
      throw std::runtime_error("SIMPLE Client: cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
    const char* topic = T::getTopic();
    zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic, strlen(topic));
  }

  ~Client<T>()
  {
    zmq_close(socket_);
    zmq_ctx_term(context_);
  }
  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: Flatbuffer-type message to be sent as request.
   */
  T request(const flatbuffers::FlatBufferBuilder& msg)
  {
    uint8_t* buffer = msg.GetBufferPointer();  //< Get the data from the message.
    int buffer_size = msg.GetSize();
    return request(buffer, buffer_size);
  }

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  T request(const simple_msgs::GenericMessage& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    return request(buffer, buffer_size);
  }

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be published.
   */
  T request(const uint8_t* msg, const int msg_size)
  {
    const char* identifier = flatbuffers::GetBufferIdentifier(msg);  //< Get the message identifier.
    auto identifier_size = strlen(identifier);                       //< Size of the message identifier.

    zmq_msg_t topic;
    zmq_msg_init_size(&topic, identifier_size);
    memcpy(zmq_msg_data(&topic), identifier, identifier_size);

    zmq_msg_t request;
    zmq_msg_init_size(&request, msg_size);
    memcpy(zmq_msg_data(&request), msg, msg_size);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_sendmsg(socket_, &topic, ZMQ_SNDMORE);
    auto request_sent = zmq_sendmsg(socket_, &request, 0);

    if (topic_sent == -1 || request_sent == -1)
    {
      throw std::runtime_error("The client could not send the request to the server. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }

    zmq_msg_close(&topic);
    zmq_msg_close(&request);

    int data_past_topic;
    auto data_past_topic_size = sizeof(data_past_topic);

    zmq_msg_t reply;
    zmq_msg_init(&reply);

    int reply_received = zmq_msg_recv(&reply, socket_, 0);
    zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
    if (data_past_topic)
    {
      reply_received = zmq_msg_recv(&reply, socket_, 0);
    }

    if (reply_received == -1)
    {
      throw std::runtime_error("The client could not receive the reply from the server ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }

    T reply_data(static_cast<uint8_t*>(zmq_msg_data(&reply)));  //< Build a T object from the server reply.
    zmq_msg_close(&reply);
    return reply_data;
  }

private:
  static void* context_;
  void* socket_;
};

template <typename T>
void* Client<T>::context_(zmq_ctx_new());
}  // Namespace simple.
