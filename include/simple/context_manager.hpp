/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_CONTEXT_MANAGER_HPP
#define SIMPLE_CONTEXT_MANAGER_HPP

#include <zmq.h>
#include <memory>
#include <mutex>

#include "simple_export.h"

namespace simple {
/**
 * @class ContextManager context_manager.hpp.
 * @brief The ContextManager handles a singleton ZMQ Context that is shared between GenericSocket objects.
 *
 * Any GenericSocket object, created within one unit, works on a single instance of a ZMQ
 * Context, since it is not recommended to create more than one context in that case.
 * A ContextManager cannot be instantiated, a ContextManager object can only access the internal instance of the ZMQ
 * Context.
 */
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
    std::lock_guard<std::mutex> lock{context_mutex_};
    // Create a new ZMQ context or return the existing one.
    if (context_ == nullptr) { context_ = std::make_shared<ZMQContext>(); }
    return context_->getContext();
  }

  /**
   * @brief Destroys the current instance of the ZMQ context.
   *
   * It is sometimes required to control the lifetime of the zmq context object explicitly, most notably
   * when using simple as (or from a) dynamic library. In such a case, the context needs to be destroyed
   * _before_ the DLL gets unloaded, but _after_ any other SIMPLE objects are destroyed.
   * \attention {Under normal circumstances, using this function is not necessary as the context normally will
   *       be destroyed at static object destruction. Use this only when necessary and be aware that
   *       early destruction can result in application crashes or hangs.}
   */
  static void destroy() {
    std::lock_guard<std::mutex> lock{context_mutex_};
    context_ = nullptr;
  }

private:
  /**
   * @brief The ZMQContext class handles the lifetime of a ZMQ context instance.
   *
   * The ZMQ context is instatiated only on construction and automatically terminated when the objects lifetime is over.
   */
  class ZMQContext {
  public:
    // Instantiate a new ZMQ context.
    ZMQContext() { internal_context_ = zmq_ctx_new(); }

    ZMQContext(const ZMQContext&) = delete;
    ZMQContext& operator=(const ZMQContext&) = delete;
    ZMQContext(ZMQContext&&) = delete;
    ZMQContext& operator=(ZMQContext&&) = delete;

    // Terminate the ZMQ context instance.
    ~ZMQContext() {
      if (internal_context_ != nullptr) { zmq_ctx_term(internal_context_); }
    }

    // Returns the ZMQ context instance.
    void* getContext() { return internal_context_; }

  private:
    void* internal_context_{nullptr};  //! The actual ZMQ Context.
  };

  static SIMPLE_EXPORT std::mutex context_mutex_;
  static SIMPLE_EXPORT std::shared_ptr<ZMQContext>
      context_;  //! This allows to automatically dispose (and therefore, terminate) the
                 // ZMQ context handled by the ZMQContext class.
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
