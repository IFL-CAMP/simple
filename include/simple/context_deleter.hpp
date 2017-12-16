#pragma once

#include <zmq.h>

void contextDeleter(void* ctx)
{
  zmq_ctx_term(ctx);
}
