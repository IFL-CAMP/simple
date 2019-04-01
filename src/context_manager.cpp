/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "simple/context_manager.hpp"
#include <zmq.hpp>

//! Static member are here initialized.
std::mutex simple::ContextManager::context_mutex_{};
std::shared_ptr<zmq::context_t> simple::ContextManager::context_{nullptr};

namespace simple {

zmq::context_t* ContextManager::instance() {
  std::lock_guard<std::mutex> lock{context_mutex_};
  // Create a new ZMQ context or return the existing one.
  if (context_ == nullptr) { context_ = std::make_shared<zmq::context_t>(); }
  return context_.get();
}

void ContextManager::destroy() {
  std::lock_guard<std::mutex> lock{context_mutex_};
  context_ = nullptr;
}

}  // namespace simple
