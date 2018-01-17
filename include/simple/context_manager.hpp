#pragma once

#include <zmq.h>
#include <memory>

void deleter(void* ctx)
{
  zmq_ctx_term(ctx);
}

class ContextManager
{
public:
  ContextManager()
    : context_(zmq_ctx_new(), deleter)
  {
  }
  //~ContextManager() { zmq_ctx_term(context_); }
  void* instance() const { return context_.get(); }

private:
  std::shared_ptr<void> context_;
};
