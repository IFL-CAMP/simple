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

#include <iostream>
#include <memory>
#include <string>

#include "simple/generic_socket.hpp"

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace simple {
/**
 * @class Publisher publisher.hpp.
 * @brief The Publisher class creates a ZMQ Socket of type zmq_socket_type::Pub that can publish Protobuf messages of
 * type T passed to its publish() method.
 * @tparam T The Protobuf message type to publish.
 *
 * Implements the logic for a Publisher in the Publisher / Subscriber paradigm. A Publisher can publish messages of
 * types T that can be received by any number of Subscribers.
 */
template <typename T>
class Publisher {
  static_assert(std::is_base_of<google::protobuf::Message, T>::value,
                "The message type must inherit from google::protobuf::Message");

public:
  Publisher() = default;

  /**
   * @brief Creates a zmq_socket_type::Pub socket and binds it to the given address.
   *
   * Subscribers can subscribe to a Publisher connecting to its address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   */
  explicit Publisher<T>(const std::string& address) : socket_{zmq_socket_type::Pub, T::descriptor()->full_name()} {
    socket_.bind(address);
  }

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

  /**
   * @brief Class destructor.
   */
  ~Publisher() = default;

  /**
   * @brief Publishes the given message of type T through the open socket.
   * @param [in] message - Protobuf message to publish.
   * @return success or failure.
   */
  bool publish(const T& message) {
    if (!message.IsInitialized()) {
      std::cerr << "[Simple Publisher] - Attempting to publish an uninitialized " << socket_.topic_
                << " message. All required fields "
                   "have to be set."
                << std::endl;
      return false;
    }
    return socket_.sendMessage(message, message.ByteSizeLong(), "[Simple Publisher] - ");
  }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e., "tcp://0.0.0.0:8000"
   */
  const std::string endpoint() { return socket_.endpoint(); }

private:
  GenericSocket socket_{};  //! The internal socket.
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
