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

namespace simple {
/**
 * @class ContextManager context_manager.hpp.
 * @brief The ContextManager handles a singleton ZMQ Context that is shared between Sockets objects.
 *
 * Any Socket object (Publisher/Subscriber/Client/Server), created within one unit, works on a single instance of a ZMQ
 * Context, since it is not recommended to create more than one context in that case.
 * A ContextManager cannot be instantiated, a Socket object can only access the internal instance of the ZMQ Context.
 */
class ContextManager {
public:
  ContextManager() = delete;
  ContextManager(const ContextManager&) = delete;
  ContextManager& operator=(const ContextManager&) = delete;

  /**
   * @brief ContextManager destructor
   * The internal ZMQ Context is terminated and set to nullptr.
   */
  ~ContextManager() {
    zmq_ctx_term(context_);
    context_ = nullptr;
  }

  /**
   * @brief Returns a raw pointer to the unique instance of the internal ZMQ Context.
   */
  static void* instance() {
    // If the ZMQ Context instance is has not been yet created, a new one is instantiated.
    if (context_ == nullptr) { context_ = zmq_ctx_new(); }
    // Else, the current instance is returned
    return context_;
  }

private:
  static void* context_;
};
}  // Namespace simple.

void* simple::ContextManager::context_ = nullptr;  //< The static ZMQ context is initialized to nullptr.

#endif  // SIMPLE_CONTEXT_MANAGER_HPP
