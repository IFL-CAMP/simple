#include <simple/context_manager.hpp>

std::unique_ptr<std::atomic<void*>, decltype(&simple::ContextManager::deleteContext)> simple::ContextManager::context_{
    nullptr};
// std::atomic<void*> simple::ContextManager::context_{nullptr};
// std::unique_ptr<simple::ContextManager> simple::ContextManager::manager_{nullptr};
