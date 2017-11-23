#pragma once

#define ZMQ_STATIC

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
/*#include "simple_msgs/simple.pb.h"

namespace simple {

class Proxy {
  /// Proxy class.
 public:
  Proxy(std::string backport, std::string frontport, zmq::context_t& context);
  ~Proxy();

 private:
  std::unique_ptr<zmq::socket_t> frontend;
  std::unique_ptr<zmq::socket_t> backend;
};

}  // namespace simple*/
