#include <simple/context_manager.hpp>
//< Static member are here initialized.
std::mutex simple::ContextManager::context_creation_mutex_{};
std::shared_ptr<simple::ContextManager::ZMQContext> simple::ContextManager::context_{nullptr};
