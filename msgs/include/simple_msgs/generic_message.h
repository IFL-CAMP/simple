/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_MSGS_GENERIC_MESSAGE_H
#define SIMPLE_MSGS_GENERIC_MESSAGE_H

#include <flatbuffers/flatbuffers.h>
#include <iostream>
#include <memory>
#include <mutex>

namespace simple_msgs {
// Custom make_unique method since supporting C++11
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * @brief GenericMessageBase: base class for SIMPLE wrappers around flatbuffers messages.
 */
class GenericMessage {
public:
  GenericMessage() = default;
  virtual ~GenericMessage() = default;

  virtual std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const = 0;

protected:
  mutable std::mutex mutex_{};
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_GENERIC_MESSAGE_H
