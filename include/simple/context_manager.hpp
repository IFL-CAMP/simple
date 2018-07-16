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
#include <memory>
#include <mutex>

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
    std::lock_guard<std::mutex> lock(context_creation_mutex_);
    // Make a new context and atomically swap it with the member context.
    if (context_ == nullptr) { context_ = std::make_shared<ZMQContext>(); }
    return context_->getContext();
  }

private:
  /**
   * @brief The ZMQContext class handles the lifetime of a static ZMQ context instance.
   *
   * The ZMQ context is instatiated only on construction and automatically terminated when the objects lifetime is over.
   */
  class ZMQContext {
  public:
    /**
     * @brief Atomically instantiate a new ZMQ context if none was yet created.
     */
    ZMQContext() { internal_context_ = zmq_ctx_new(); }

    ZMQContext(const ZMQContext&) = delete;
    ZMQContext& operator=(const ZMQContext&) = delete;
    ZMQContext(ZMQContext&&) = delete;
    ZMQContext& operator=(ZMQContext&&) = delete;

    // Terminate the static ZMQ context instance.
    ~ZMQContext() {
      if (internal_context_ != nullptr) { zmq_ctx_term(internal_context_); }
    }

    /**
     * @brief Returns the ZMQ context instance.
     */
    void* getContext() { return internal_context_; }

  private:
    void* internal_context_{nullptr};  //< Atomic static ZMQ Context.
  };
  static std::mutex context_creation_mutex_;
  static std::shared_ptr<ZMQContext> context_;  //< This allows to automatically dispose (and therefore, terminate) the
                                                // ZMQ context handled by the ZMQContext class.
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
