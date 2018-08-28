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

#ifndef SIMPLE_PUBLISHER_HPP
#define SIMPLE_PUBLISHER_HPP

#include <zmq.h>
#include <memory>
#include <string>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @class Publisher publisher.hpp.
 * @brief The Publisher class creates a ZMQ Socket of type ZMQ_PUB that can publish messages of type T passed to its
 * publish() method.
 * @tparam T The simple_msgs type to publish.
 *
 * Implements the logic for a Publisher in the Publisher / Subscriber paradigm. A Publisher can publish messages of
 * types T that can be received by any number of Subscribers.
 */
template <typename T>
class Publisher {
public:
  Publisher() = default;

  /**
   * @brief Creates a ZMQ_PUB socket and binds it to the given address.
   *
   * Subscribers can subscribe to a Publisher connecting to its address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   */
  explicit Publisher<T>(const std::string& address) : socket_{ZMQ_PUB} { socket_.bind(address); }

  // A Publisher cannot be copied, only moved.
  Publisher(const Publisher& other) = delete;
  Publisher& operator=(const Publisher& other) = delete;

  /**
   * @brief Move constructor.
   */
  Publisher(Publisher&& other) = default;

  /**
   * @brief Move assignment operator.
   */
  Publisher& operator=(Publisher&& other) = default;

  ~Publisher() = default;

  /**
   * @brief Publishes the given message of type T through the open socket.
   * @param [in] msg - simple_msgs class wrapper for Flatbuffer messages.
   * @return size of the published message, in bytes. Returns -1 if send fails.
   */
  int publish(const T& msg) { return publish(msg.getBufferData()); }

private:
  /**
   * @brief Publishes the given message through the open socket.
   * @param [in] buffer - Flatbuffers buffer containing the data to be published.
   * @return size of the published message, in bytes. Returns -1 if send fails.
   */
  int publish(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer) {
    return socket_.sendMsg(buffer, "[Simple Publisher] - ");
  }

private:
  GenericSocket<T> socket_{};  //! The internal socket.
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
