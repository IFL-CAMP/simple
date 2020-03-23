/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include <iostream>
#include <memory>
#include <string>

#include "simple/generic_socket.hpp"

namespace simple {

/**
 * @class Client client.hpp.
 * @brief The Client class creates a ZMQ socket of type zmq_socket_type::Req that can send requests to a Server object
 * using the Protobuf message of type T passed to its request() method.
 * @tparam T The Protobuf message type to use.
 *
 * Implements the logic for a Client in the Client / Server paradigm.
 * A Client can send requests to a Server using messages of type T and receive back an answer.
 */
template <typename T>
class Client {
  static_assert(std::is_base_of<google::protobuf::Message, T>::value,
                "The message type must inherit from google::protobuf::Message");

public:
  Client() = default;

  /**
   * @brief Creates a zmq_socket_type::Req socket and connects it to the given address.
   * @param [in] address - address the client connects to, in the form: \<PROTOCOL\>://\<HOSTNAME\>:\<PORT\>. e.g,
   * tcp://localhost:5555.
   * @param [in] timeout - time, in milliseconds, the client shall wait for a reply. Default 2 seconds.
   * @param [in] linger - time, in milliseconds, unsent messages linger in memory after socket is closed.
   * Default -1 (infinite).
   */
  explicit Client(const std::string& address, int timeout = 2000, int linger = -1)
    : socket_{zmq_socket_type::Req, T::descriptor()->full_name()}
    , address_{address}
    , timeout_{timeout}
    , linger_{linger} {
    initClient();
  }

  // A Client cannot be copied, only moved.
  Client(const Client& other) = delete;
  Client& operator=(const Client& other) = delete;

  /**
   * @brief Move constructor.
   */
  Client(Client&& other) noexcept = default;

  /**
   * @brief Move assignment operator.
   */
  Client& operator=(Client&& other) noexcept = default;

  /**
   * @brief Class destructor.
   */
  ~Client() = default;

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param [in,out] message - Protobuf message to send as request and receive as response.
   */
  bool request(T& message) {
    bool success{false};
    // Send the message to the Server and receive back the response.
    if (socket_.sendMessage(message, message.ByteSizeLong(), "[SIMPLE Client] - ")) {
      if (socket_.receiveMessage(message, "[SIMPLE Client] - ")) {
        success = true;
      } else {
        std::cerr << "[SIMPLE Client] - No reply received. Aborting this request." << std::endl;
        // If no message was received back we need to delete the existing socket and create a new one.
        socket_.closeSocket();
        initClient();
      }
    }
    return success;
  }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e., "tcp://0.0.0.0:8000"
   */
  const std::string endpoint() { return socket_.endpoint(); }

private:
  // Initialize the client, setting up the socket and its configuration.
  void initClient() {
    if (!socket_.isSocketValid()) { socket_.initSocket(zmq_socket_type::Req); }
    socket_.setTimeout(timeout_);
    socket_.setLinger(linger_);
    socket_.connect(address_);
  }

  GenericSocket socket_{};   //! The internal socket.
  std::string address_{""};  //! The address the Client is connected to.
  int timeout_{30000};       //! Milliseconds the Client should wait for a reply from a Server.
  int linger_{-1};           //! Milliseconds the messages linger in memory after the socket is closed.
};
}  // Namespace simple.

#endif  // SIMPLE_CLIENT_HPP
