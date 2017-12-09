#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>

#include "simple_msgs/generic_message.h"

namespace simple
{
/**
 * @brief Creates a reply socket for a specific type of message.
 */
template <typename T>
class Server
{
public:
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it to the port. The user defined callback
   * function is responsible for taking the received request and filling it with the reply data
   */
  Server(const std::string& port, const std::function<void(T&)>& callback, int timeout = 100)
    : socket_(zmq_socket(context_, ZMQ_REP))
    , callback_(callback)
  {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    auto success = zmq_bind(socket_, port.c_str());
    if (success != 0)
    {
      throw std::runtime_error("SIMPLE Server: cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }

    // Start the thread of the server: wait for requests on the dedicated thread.
    server_thread_ = std::thread(&Server::awaitRequest, this);
  }

  ~Server()
  {
    alive_ = false;          //< Stop the request/reply loop.
    server_thread_.join();   //< Wait for the server thead.
    zmq_close(socket_);      //< Close the socket.
    zmq_ctx_term(context_);  //< Terminate the context.
  }

private:
  /**
   * @brief TODO
   */
  void awaitRequest()
  {
    while (alive_)
    {
      int data_past_topic;
      auto data_past_topic_size = sizeof(data_past_topic);

      zmq_msg_t request;
      zmq_msg_init(&request);
      zmq_msg_recv(&request, socket_, 0);
      zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
      if (data_past_topic)
      {
        auto success = zmq_msg_recv(&request, socket_, 0);
        if (success != -1)
        {
          T request_data(static_cast<uint8_t*>(zmq_msg_data(&request)));
          callback_(request_data);
          reply(request_data);
        }
      }
      zmq_msg_close(&request);
    }
  }

  /**
   * @brief TODO
   * @param msg
   */
  void reply(const T& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    reply(buffer, buffer_size);
  }

  /**
   * @brief TODO
   * @param msg
   * @param size
   */
  void reply(const uint8_t* msg, const int msg_size)
  {
    const char* identifier = flatbuffers::GetBufferIdentifier(msg);  //< Get the message identifier.
    auto identifier_size = strlen(identifier);                       //< Size of the message identifier.

    zmq_msg_t topic;
    zmq_msg_init_size(&topic, identifier_size);
    memcpy(zmq_msg_data(&topic), identifier, identifier_size);

    zmq_msg_t reply;
    zmq_msg_init_size(&reply, msg_size);
    memcpy(zmq_msg_data(&reply), msg, msg_size);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_sendmsg(socket_, &topic, ZMQ_SNDMORE);
    auto reply_sent = zmq_sendmsg(socket_, &reply, 0);

    if (topic_sent == -1 || reply_sent == -1)
    {
      throw std::runtime_error("The server could not send the reply to the server. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
    zmq_msg_close(&topic);
    zmq_msg_close(&reply);
  }

  std::thread server_thread_;
  bool alive_{true};
  static void* context_;
  void* socket_;
  std::function<void(T&)> callback_;
};

template <typename T>
void* Server<T>::context_(zmq_ctx_new());
}  // Namespace simple.
