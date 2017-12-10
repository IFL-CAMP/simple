#pragma once

#include <zmq.h>
#include <string>

#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a Client socket for a specific type of message.
 */
template <typename T>
class Client : public GenericSocket<T>
{
public:
  Client(const std::string& address)
    : GenericSocket<T>(zmq_socket(context_, ZMQ_REQ))
  {
    GenericSocket<T>::connect(address);
  }

  ~Client() { zmq_ctx_term(context_); }
  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  bool request(T& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    return request(buffer, buffer_size, msg);
  }

private:
  bool request(const uint8_t* data, const int data_size, T& msg)
  {
    bool success{false};

    if (GenericSocket<T>::sendMsg(data, data_size, "[SIMPLE Client] - "))
    {
      if (GenericSocket<T>::receiveMsg(msg, "[SIMPLE Client] - "))
      {
        success = true;
      }
    }
    return success;
  }
  static void* context_;
};

template <typename T>
void* Client<T>::context_(zmq_ctx_new());
}  // Namespace simple.
