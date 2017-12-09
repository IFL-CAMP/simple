#pragma once

#include "zmq.hpp"

namespace simple
{
struct contextDeleter
{
  void operator()(void* ctx)
  {
    zmq_ctx_term(ctx);
    zmq_close(ctx);
  }
};
}  // Namespace simple.
