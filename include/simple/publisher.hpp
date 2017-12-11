#pragma once

#include <zmq.h>
#include <string>

#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher : public GenericSocket<T>
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Publisher<T>(const std::string& address)
    : GenericSocket<T>(zmq_socket(context_, ZMQ_PUB))
  {
    GenericSocket<T>::bind(address);
  }

  ~Publisher<T>() { zmq_ctx_term(context_); }
  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void publish(const T& msg)
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
    GenericSocket<T>::sendMsg(msg, msg_size, "[Simple Publisher] - ");
  }

private:
  static void* context_;
};

template <typename T>
void* Publisher<T>::context_(zmq_ctx_new());
}  // Namespace simple.
