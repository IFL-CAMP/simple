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
  ContextManager() = delete;
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;
  ~ContextManager() = default;

  static void* instance() {
    if (context_ == nullptr) {
      context_ = std::unique_ptr<ZMQContext>(new ZMQContext);
    }
    return context_->getContext();
  }

private:
  class ZMQContext {
  public:
    ZMQContext() {
      if (context_ == nullptr) {  // If we are the first ones to get context instance.
        // Make a new context and atomically swap it with the member context.
        void* tmp = nullptr;
        auto new_context = zmq_ctx_new();
        context_.compare_exchange_strong(tmp, new_context);

        if (context_ != new_context) {
          // We were late, someone else already created the context, we throw away the one we just created.
          zmq_ctx_term(new_context);
        }
      }
    }

    ~ZMQContext() {
      void* context_to_delete = context_;
      context_.compare_exchange_strong(context_to_delete, nullptr);
      zmq_ctx_term(context_to_delete);
    }

    void* getContext() { return context_;}

  private:
    static std::atomic<void*> context_;
  };

  static std::unique_ptr<ZMQContext> context_;
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
