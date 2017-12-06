#pragma once

#include "zmq.hpp"

namespace simple
{
struct contextDeleter
{
  void operator()(zmq::context_t* ctx)
  {
    ctx->close();
    delete ctx;
  }
};
}  // Namespace simple.
