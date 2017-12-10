#pragma once

#include <zmq.h>
#include <string>
#include <string.h>

namespace simple
{
template <typename T>
class GenericSocket
{
public:
  virtual ~GenericSocket() { zmq_close(socket_); }
  GenericSocket(const GenericSocket&) = delete;

protected:
  GenericSocket() = default;
  GenericSocket(void* socket)
    : socket_(socket)
  {
  }

  void bind(const std::string& address)
  {
    auto success = zmq_bind(socket_, address.c_str());
    if (success != 0)
    {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
  }

  void connect(const std::string& address)
  {
    auto success = zmq_connect(socket_, address.c_str());
    if (success != 0)
    {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
  }

  bool sendMsg(const uint8_t* msg, int msg_size, const std::string& custom_error = "[SIMPLE Error] - ")
  {
    zmq_msg_t topic;
    zmq_msg_init_size(&topic, topic_size_);
    memcpy(zmq_msg_data(&topic), topic_, topic_size_);

    zmq_msg_t message;
    zmq_msg_init_size(&message, msg_size);
    memcpy(zmq_msg_data(&message), msg, msg_size);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_sendmsg(socket_, &topic, ZMQ_SNDMORE);
    auto message_sent = zmq_sendmsg(socket_, &message, 0);

    zmq_msg_close(&topic);
    zmq_msg_close(&message);

    if (topic_sent == -1 || message_sent == -1)
    {
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " + std::to_string(zmq_errno()) << std::endl;
      return false;
    }
    return true;
  }

  bool receiveMsg(T& msg, const std::string& custom_error = "")
  {
    bool success{false};
    int data_past_topic{0};
    auto data_past_topic_size = sizeof(data_past_topic);

    zmq_msg_t message;
    zmq_msg_init(&message);

    int message_received = zmq_msg_recv(&message, socket_, 0);

    if (message_received != -1)
    {
      if (strncmp(static_cast<char*>(zmq_msg_data(&message)), topic_, topic_size_) == 0)
      {
        zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
        if (data_past_topic)
        {
          message_received = zmq_msg_recv(&message, socket_, 0);
          if (message_received != -1 && zmq_msg_size(&message) != 0)
          {
            msg = static_cast<uint8_t*>(zmq_msg_data(&message));  //< Build a T object from the server reply.
            success = true;
            zmq_msg_close(&message);
          }
          else
          {
            std::cerr << custom_error << "Failed to receive the message. ZMQ Error:" << std::to_string(zmq_errno())
                      << std::endl;
          }
        }
      }
      else
      {
        std::cerr << custom_error << "Received the wrong message type." << std::endl;
      }
    }
    return success;
  }

  void filter() { zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_, topic_size_); }
  void setTimeout(int timeout) { zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout)); }
  void* socket_;
  const char* topic_{T::getTopic()};
  const size_t topic_size_{strlen(topic_)};
};

}  // Namespace simple.
