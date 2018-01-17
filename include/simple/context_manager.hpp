#pragma once

#include <zmq.h>

class ContextManager
{
public:
  ContextManager()
    : context_(zmq_ctx_new())
  {
  }
  ~ContextManager() { zmq_ctx_term(context_); }
  void* instance() const { return context_; }

private:
  void * context_;
};
