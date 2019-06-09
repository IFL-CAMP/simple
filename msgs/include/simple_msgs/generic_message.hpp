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

#include <memory>

// Forward declarations.
namespace flatbuffers {
class DetachedBuffer;
}

namespace simple {
class GenericSocket;
}  // namespace simple

namespace simple_msgs {
/**
 * @class GenericMessage generic_message.hpp.
 * @brief Base class for simple_msgs wrappers around Flatbuffers messages.
 */
class GenericMessage {
protected:
  GenericMessage();
  virtual ~GenericMessage() = default;

  // The simple::GenericSocket class is a friend of this base class, so that it has access to the protected methods.
  friend class simple::GenericSocket;
  /**
   * @brief Internal assignment operator from a shared void*.
   *
   * Every message has to be constructable from a void*, since that is what we get from ZMQ.
   * This contructor is not available to users, it is only used internally.
   *
   * @param [in] rhs - the pointer from which construct the message.
   * @return
   */
  virtual GenericMessage& operator=(std::shared_ptr<void*> rhs) = 0;

  /**
   * @brief Returns the built buffer. Each simple_msgs type implements its version of it.
   */
  virtual std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const = 0;
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_GENERIC_MESSAGE_H
