/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_SERVER_HPP
#define SIMPLE_SERVER_HPP

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @class Server server.hpp.
 * @brief The Server class creates a ZMQ socket of type ZMQ_REP that accept requests from Client(s) and sends them back
 * a reply according to the computation performed in the provided callback function.
 * @tparam T The simple_msgs type to use.
 *
 * Implements the logic for a Server in the Client / Server paradigm.
 */
template <typename T>
class Server {
public:
  Server() = default;

  /**
   * @brief Creates a ZMQ_REP socket and connects it to the given address.
   * The user defined callback function is responsible for taking
   * the received request and filling it with the reply data.
   * @param [in] address - address the server binds to, in the form: \<PROTOCOL\>://\<HOSTNAME\>:\<PORT\>. e.g
   * tcp://localhost:5555.
   * @param [in] callback - user defined callback function for incoming requests.
   * @param [in] timeout - Time the server will block the thread waiting for a message. In
   * milliseconds.
   * @param [in] linger - Time the unsent messages linger in memory after the socket
   * is closed. In milliseconds. Default is -1 (infinite).
   */
  explicit Server(const std::string& address, const std::function<void(T&)>& callback, int timeout = 1000,
                  int linger = -1)
    : socket_{new GenericSocket(zmq_socket_type::rep, T::getTopic())}, callback_{callback} {
    socket_->setTimeout(timeout);
    socket_->setLinger(linger);
    socket_->bind(address);
    initServer();
  }

  // A Server cannot be copied, only moved
  Server(const Server& other) = delete;
  Server& operator=(const Server& other) = delete;

  /**
   * @brief Move constructor.
   */
  Server(Server&& other) : socket_{std::move(other.socket_)}, callback_{std::move(other.callback_)} {
    other.stop();  //! The moved Server has to be stopped.
    initServer();
  }

  /**
   * @brief Move assignment operator.
   */
  Server& operator=(Server&& other) {
    stop();                 //! Stop the current Server object.
    if (other.isValid()) {  //! Move the Server only if it's a valid one, e.g. if it was not default constructed.
      other.stop();         //! The moved Server has to be stopped.
      socket_ = std::move(other.socket_);
      callback_ = std::move(other.callback_);
      initServer();
    }
    return *this;
  }

  ~Server() { stop(); }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000"
   */
  const std::string& endpoint() { return socket_->endpoint(); }

private:
  /**
   * @brief Stop the server loop. No further requests will be handled.
   */
  void stop() {
    if (isValid()) {
      alive_->store(false);
      if (server_thread_.joinable()) { server_thread_.join(); }
    }
  }

  /**
   * @brief Checks if the Server is properly initialied and its internal thread is running.
   */
  inline bool isValid() const { return alive_ == nullptr ? false : alive_->load(); }

  /**
   * @brief Initializes the server thread.
   */
  void initServer() {
    alive_ = std::make_shared<std::atomic<bool>>(true);

    // Start the thread of the server if not yet done. Wait for requests on the
    // dedicated thread.
    if (!server_thread_.joinable() && socket_ != nullptr) {
      server_thread_ = std::thread(&Server::awaitRequest, this, alive_, socket_);
    }
  }

  /**
   * @brief Keep waiting for a request to arrive. Process the request using the
   * callback function and reply.
   */
  void awaitRequest(std::shared_ptr<std::atomic<bool>> alive, std::shared_ptr<GenericSocket> socket) {
    while (alive->load()) {
      T msg;
      if (socket->receiveMsg(msg, "[SIMPLE Server] - ")) {
        if (alive->load()) { callback_(msg); }
        if (alive->load()) { reply(socket.get(), msg); }
      }
    }
  }

  /**
   * @brief Sends the message back to the client who requested it.
   * @param [in] msg - The message to be sent.
   */
  void reply(GenericSocket* socket, const T& msg) { socket->sendMsg(msg, "[SIMPLE Server] - "); }

  std::shared_ptr<std::atomic<bool>> alive_{nullptr};  //! Flag keeping track of the internal thread's state.
  std::shared_ptr<GenericSocket> socket_{nullptr};     //! The internal socket.
  std::function<void(T&)> callback_;                   //! The callback function called at each message arrival.
  std::thread server_thread_{};                        //! The internal Server thread on which the given callback runs.
};
}  // Namespace simple.

#endif  // SIMPLE_SERVER_HPP
