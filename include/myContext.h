#pragma once

//#define ZMQ_STATIC
#include <zmq_utils.h>
#include <memory>
#include <zmq.hpp>

namespace simple {

class myContext {
 public:
  std::shared_ptr<zmq::context_t> context;
  myContext() { context = std::make_shared<zmq::context_t>(1); }
  ~myContext() { context->close(); }
};

}  // namespace simple
