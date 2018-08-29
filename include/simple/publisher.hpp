/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_PUBLISHER_HPP
#define SIMPLE_PUBLISHER_HPP

#include <zmq.h>
#include <memory>
#include <string>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher {
public:
  Publisher() = default;

  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the
   * port.
   * @param port string for the connection port.
   */
  explicit Publisher<T>(const std::string& address) : socket_{ZMQ_PUB} { socket_.bind(address); }

  Publisher(const Publisher& other) = delete;
  Publisher& operator=(const Publisher& other) = delete;

  Publisher(Publisher&& other) = default;
  Publisher& operator=(Publisher&& other) = default;

  ~Publisher() = default;

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   * @return size of the message, in bytes, published. Returns -1 if send fails.
   */
  int publish(const T& msg) { return publish(msg.getBufferData()); }
  /**
   * @brief Publishes the message through the open socket.
   * @param buffer: buffer containing the data to be published.
   * @return size of the message, in bytes, published. Returns -1 if send fails.
   */
  int publish(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer) {
    return socket_.sendMsg(buffer, "[Simple Publisher] - ");
  }

private:
  GenericSocket<T> socket_{};
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
