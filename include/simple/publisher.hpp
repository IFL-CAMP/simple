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
  bool publish(const T& msg) { return publish(msg.getBufferData()); }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000"
   */
  const std::string& endpoint() { return socket_.endpoint(); }

private:
  /**
   * @brief Publishes the given message through the open socket.
   * @param [in] buffer - Flatbuffers buffer containing the data to be published.
   * @return size of the published message, in bytes. Returns -1 if send fails.
   */
  bool publish(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer) const {
    return socket_.sendMsg(buffer, "[Simple Publisher] - ");
  }

private:
  GenericSocket<T> socket_{};  //! The internal socket.
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
