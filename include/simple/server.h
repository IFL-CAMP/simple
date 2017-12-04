#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>
#include "simple\contextCloser.h"

namespace simple
{
/**
 * @brief Creates a reply socket.
 */
template <typename T>
class Server
{
public:
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it to the port. The user defined callback
   * function is responsible for taking the received request and filling it with the reply data
   */
  Server(const std::string& port, const std::function<void(const T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_REP)), callback_(callback)
  {
    try
    {
      socket_->bind(port);
    }
    catch (zmq::error_t& e)
    {
      std::cout << "could not bind socket:" << e.what();
    }

	// start thread of the server: wait for requests on a dedicated thread
	t_ = std::thread(&Server::reply, this);

  }

  ~Server(){
	  // stop the reply loop
	  alive_ = false;

	  // join thread
	  t_.join();

	  socket_->close();
  }

  void reply(){

  }

private:
  std::thread t_;
  bool alive_{ true };
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};
template <typename T>
std::unique_ptr<zmq::context_t, contextCloser> Server<T>::context_(new zmq::context_t(1));

}  // namespace simple
