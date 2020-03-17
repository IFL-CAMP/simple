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

#include <memory>
#include <mutex>

#include "simple_export.h"

// Forward declaration.
namespace zmq {
class context_t;
}

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
   * During the first call, a new ZMQ context object is instantiated.
   * That instantiation performs thread-safe operations to create/dispose the underlying ZMQ context object.
   */
  static zmq::context_t* instance();
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
  static void destroy();

private:
  static SIMPLE_EXPORT std::mutex context_mutex_;
  static SIMPLE_EXPORT std::shared_ptr<zmq::context_t> context_;  //! zmq::context_t is automatically disposed.
};
}  // Namespace simple.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
