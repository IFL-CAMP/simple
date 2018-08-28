/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy Langsch - fernanda.langsch@tum.de
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

#ifndef SIMPLE_MSGS_GENERIC_MESSAGE_H
#define SIMPLE_MSGS_GENERIC_MESSAGE_H

#include <flatbuffers/flatbuffers.h>
#include <mutex>

namespace simple_msgs {
/**
 * @class GenericMessage generic_message.h.
 * @brief Base class for simple_msgs wrappers around Flatbuffers messages.
 */
class GenericMessage {
public:
  GenericMessage() = default;
  virtual ~GenericMessage() = default;

  /**
   * @brief Returns the built buffer. Each simple_msgs type implements its version of it.
   */
  virtual std::shared_ptr<flatbuffers::DetachedBuffer> getBufferData() const = 0;

protected:
  mutable std::mutex mutex_{};  ///< Mutex to implement thread-safe message classes.
};
}  // Namespace simple_msgs.

#endif  // SIMPLE_MSGS_GENERIC_MESSAGE_H
