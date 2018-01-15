#pragma once

#include <zmq.h>
#include <string>
#include <thread>
#include <memory>
#include "context_deleter.hpp"
#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a reply socket for a specific type of message.
 */
template <typename T>
class Server : public GenericSocket<T>
{
public:
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it to the port. The user defined callback
   * function is responsible for taking the received request and filling it with the reply data
   */
  Server(const std::string& address, const std::function<void(T&)>& callback, int timeout = 100)
    : GenericSocket<T>(zmq_socket(context_.get(), ZMQ_REP))
    , callback_(callback)
  {
    GenericSocket<T>::bind(address);
    GenericSocket<T>::filter();
    GenericSocket<T>::setTimeout(timeout);

    // Start the thread of the server: wait for requests on the dedicated thread.
    server_thread_ = std::thread(&Server::awaitRequest, this);
  }

  Server(const Server& other)
    : GenericSocket<T>(zmq_socket(context_.get(), ZMQ_REP))
    , callback_(other.callback_)
  {
    GenericSocket<T>::bind(other.address_);
    GenericSocket<T>::filter();
    GenericSocket<T>::setTimeout(other.timeout_);

    // Start the thread of the server: wait for requests on the dedicated thread.
    server_thread_ = std::thread(&Server::awaitRequest, this);
  }

  ~Server()
  {
    alive_ = false;         //< Stop the request/reply loop.
    server_thread_.join();  //< Wait for the server thead.
  }

private:
  /**
   * @brief TODO
   */
  void awaitRequest()
  {
    while (alive_)
    {
      T msg;
      if (GenericSocket<T>::receiveMsg(msg, "[SIMPLE Server] - "))
      {
        callback_(msg);
        reply(msg);
      }
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
	std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer = msg.getBuilderPointer();
    GenericSocket<T>::sendMsg(buffer, buffer_size, builder_pointer, "[SIMPLE Server] - ");
  }

  std::thread server_thread_;
  bool alive_{true};
  std::function<void(T&)> callback_;
  static std::shared_ptr<void> context_;
};

template <typename T>
std::shared_ptr<void> Server<T>::context_(zmq_ctx_new(), contextDeleter);
}  // Namespace simple.
