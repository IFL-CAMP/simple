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

#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include <zmq.h>
#include <memory>
#include <string>

#include "simple/generic_socket.hpp"

namespace simple {

/**
 * @class Client client.hpp.
 * @brief The Client class creates a ZMQ socket of type ZMQ_REQ that can send requests to a Server object using the
 * message of type T passed to its request() method.
 * @tparam T The simple_msgs type to use.
 *
 * Implements the logic for a Client in the Client / Server paradigm.
 * A Client can send requests to a Server using messages of types T and receive back an answer.
 */
template <typename T>
class Client {
public:
  Client() = default;

  /**
   * @brief Creates a ZMQ_REQ socket and connects it to the given address.
   * @param [in] address - address the client connects to, in the form: \<PROTOCOL\>://\<HOSTNAME\>:\<PORT\>. e.g
   * tcp://localhost:5555.
   * @param [in] timeout - Time, in msec, the client shall wait for a reply. Default 30 seconds.
   * @param [in] linger - Time, in msec, unsent messages linger in memory after socket is closed. Default -1 (infinite).
   */
  explicit Client(const std::string& address, int timeout = 30000, int linger = -1)
    : socket_{ZMQ_REQ}, address_{address}, timeout_{timeout}, linger_{linger} {
    initClient();
  }

  // Copy operations are not available.
  Client(const Client& other) = delete;
  Client& operator=(const Client& other) = delete;

  /**
   * @brief Move constructor.
   */
  Client(Client&& other) = default;

  /**
   * @brief Move assignment operator.
   */
  Client& operator=(Client&& other) = default;

  ~Client() = default;

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param [in,out] msg - simple_msgs class wrapper for Flatbuffer messages..
   */
  bool request(T& msg) { return request(msg.getBufferData(), msg); }

private:
  // Initialize the client, setting up the socket and its configuration.
  void initClient() {
    if (!socket_.isValid()) { socket_.initSocket(ZMQ_REQ); }
    socket_.setTimeout(timeout_);
    socket_.setLinger(linger_);
    socket_.connect(address_);
  }

  bool request(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer, T& msg) {
    bool success{false};

    // Send the message to the Server and receive back the response.
    if (socket_.sendMsg(buffer, "[SIMPLE Client] - ") != -1) {
      if (socket_.receiveMsg(msg, "[SIMPLE Client] - ") != -1) {
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

  GenericSocket<T> socket_{};  //! The internal socket.
  std::string address_{""};    //! The address the Client is connected to.
  int timeout_{30000};         //! Milliseconds the Client should wait for a reply from a Server.
  int linger_{-1};             //! Milliseconds the messages linger in memory after the socket is closed.
};
}  // Namespace simple.

#endif  // SIMPLE_CLIENT_HPP
