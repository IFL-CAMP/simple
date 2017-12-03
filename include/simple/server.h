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
 * @brief Creates a subscriber socket.
 */
class Server
{
public:
  Server(const std::string& port)
  {
  }

  ~Server();

  void reply();

private:
  std::thread t_;
  bool alive_{ true };
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

std::unique_ptr<zmq::context_t, contextCloser> Server::context_(new zmq::context_t(1));

}  // namespace simple
