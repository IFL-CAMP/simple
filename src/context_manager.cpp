#include <simple/context_manager.hpp>

std::unique_ptr<simple::ContextManager::ZMQContext> simple::ContextManager::context_{nullptr};
std::atomic<void*> simple::ContextManager::ZMQContext::internal_context_{nullptr};
