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
  ContextManager(ContextManager&&) = delete;
  ContextManager& operator=(ContextManager&&) = delete;
  ~ContextManager() = default;

  /**
   * @brief Returns the static instance of the ZMQ context.
   *
   * During the first call, a new ZMQContext object is instantiated.
   * That instantiation performs thread-safe operations to create/dispose the underlying ZMQ context object.
   */
  static void* instance() {
    if (context_ == nullptr) { context_ = std::unique_ptr<ZMQContext>(new ZMQContext); }
    return context_->getContext();
  }

private:
  /**
   * @brief The ZMQContext class handles the lifetime of a static ZMQ context instance with thread safe operations.
   *
   * The ZMQ context is instatiated only once and automatically terminated when the objects lifetime is over.
   */
  class ZMQContext {
  public:
    /**
     * @brief Atomically instantiate a new ZMQ context if none was yet created.
     */
    ZMQContext() {
      if (context_ == nullptr) {  // If we are the first ones to ask for the context instance.
        // Make a new context and atomically swap it with the member context.
        void* tmp = nullptr;
        auto new_context = zmq_ctx_new();
        internal_context_.compare_exchange_strong(tmp, new_context);

        if (internal_context_ != new_context) {
          // Someone else already created the context, we throw away the one we just created.
          zmq_ctx_term(new_context);
        }
      }
    }

    ZMQContext(const ZMQContext&) = delete;
    ZMQContext& operator=(const ZMQContext&) = delete;
    ZMQContext(ZMQContext&&) = delete;
    ZMQContext& operator=(ZMQContext&&) = delete;

    /// Atomically terminate the static ZMQ context instance.
    ~ZMQContext() {
      // First we swap the static member context to nullptr with a local variable.
      void* context_to_delete = internal_context_;
      internal_context_.compare_exchange_strong(context_to_delete, nullptr);
      // Then we terminate the local copy of the context.
      zmq_ctx_term(context_to_delete);
    }

    /**
     * @brief Returns the ZMQ context instance.
     */
    void* getContext() { return internal_context_; }

  private:
    static std::atomic<void*> internal_context_;  //< Atomic static ZMQ Context.
  };

  static std::unique_ptr<ZMQContext> context_;  //< This allows to automatically dispose (and therefore, terminate) the
                                                // static ZMQ context handled by the ZMQContext class.
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
