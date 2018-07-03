/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIMPLE_CONTEXT_MANAGER_HPP
#define SIMPLE_CONTEXT_MANAGER_HPP

#include <zmq.h>
#include <atomic>
#include <iostream>
#include <memory>

namespace simple {
class ContextManager {
public:
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;

  ~ContextManager() = default;

  static void* instance() {
    if (context_ == nullptr) {  // If we are the first ones to get context instance.
      context_ = std::unique_ptr<std::atomic<void*>, decltype(&ContextManager::deleteContext)>(
          new std::atomic<void*>, &ContextManager::deleteContext);
      // Make a new context and atomically swap it with the member context.
      void* tmp = nullptr;
      auto new_context = zmq_ctx_new();
      context_->compare_exchange_strong(tmp, new_context);

      if (context_.get() == new_context) {
        // We were the first one to perform the atomic exchange.
        // We instanciate a ContextManager object, just so that the dctor is called and terminates the context when
        // necessary.
      } else {
        // We were late, someone else already created the context, we throw away the one we just created.
        zmq_ctx_term(new_context);
      }
    }
    return context_.get();
  }

private:
  ContextManager() = default;

  static void deleteContext(std::atomic<void*> context) {
    void* instanceToDelete = context;
    context.compare_exchange_strong(instanceToDelete, nullptr);
    if (instanceToDelete != nullptr) { zmq_ctx_term(instanceToDelete); }
  }

  static std::unique_ptr<std::atomic<void*>, decltype(&ContextManager::deleteContext)> context_;
  //  static std::atomic<void*> context_;
  //  static std::unique_ptr<ContextManager> manager_;
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
