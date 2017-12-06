#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>

#include "simple/context_deleter.h"

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
  Server(const std::string& port, const std::function<void(T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_REP))
    , callback_(callback)
  {
    int timeout = 100;  //< Socket timeout in miliseconds.
    socket_->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    try
    {
      socket_->bind(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }

    // Start the thread of the server: wait for requests on the dedicated thread.
    server_thread_ = std::thread(&Server::awaitRequest, this);
  }

  ~Server()
  {
    alive_ = false;  //< Stop the request/reply loop.
    server_thread_.join();
    socket_->close();
  }

private:
  /**
   * @brief TODO
   */
  void awaitRequest()
  {
    while (alive_)
    {
      zmq::message_t request;
      bool success{false};
      try
      {
        success = socket_->recv(&request);  //< Wait for the next request.
      }
      catch (zmq::error_t& e)
      {
        std::cerr << "Error: Could not receive the request: " << e.what();
      }
      if (success)  //< If a request was successfully received.
      {
        // TODO: What if the wrong type of request arrives?
        T request_data(static_cast<uint8_t*>(request.data()));  //< The request is wrappen in a T object.
        callback_(request_data);                                //< Send the requested data to the callback.
        reply(request_data);                                    //< Reply to the request.
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
    reply(buffer, buffer_size);
  }

  /**
   * @brief TODO
   * @param msg
   * @param size
   */
  void reply(const uint8_t* msg, const int size)
  {
    zmq::message_t reply(size);
    memcpy(reply.data(), msg, size);

    try
    {
      socket_->send(reply);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not send the reply to the client: " << e.what();
    }
  }

  std::thread server_thread_;
  bool alive_{true};
  static std::unique_ptr<zmq::context_t, contextDeleter> context_;
  std::unique_ptr<zmq::socket_t> socket_;
  std::function<void(T&)> callback_;
};

template <typename T>
std::unique_ptr<zmq::context_t, contextDeleter> Server<T>::context_(new zmq::context_t(1));
}  // Namespace simple.
