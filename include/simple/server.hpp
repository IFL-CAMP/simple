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

#ifndef SIMPLE_SERVER_HPP
#define SIMPLE_SERVER_HPP

#include <zmq.h>
#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @brief Creates a reply socket for a specific type of message.
 */
template <typename T>
class Server {
public:
  Server() = default;
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it
   * to the port. The user defined callback function is responsible for taking
   * the received request and filling it with the reply data
   * @param address where the server binds to, in the form: tcp://HOSTNAME:PORT.
   * e.g tcp://localhost:5555.
   * @param callback function that deals with the received request in the
   * desired manner.
   * @param timeout Time, in msec, the server shall wait for a request before
   * restarting the awaitRequest loop. Default 100 miliseconds.
   * @param linger Time, in msec, unsent messages linger in memory after socket
   * is closed. Default -1 (infinite).
   */
  Server(std::string address, const std::function<void(T&)>& callback, int timeout = 1000, int linger = -1)
    : callback_{callback}, address_{std::move(address)}, timeout_{timeout}, linger_{linger} {
    initServer(address_, timeout_, linger_);
  }

  Server(const Server& other) = delete;
  Server& operator=(const Server& other) = delete;

  Server(Server&& other)
    : callback_{std::move(other.callback_)}
    , address_{std::move(other.address_)}
    , timeout_{other.timeout_}
    , linger_{other.linger_} {
    other.stop();
    initServer(address_, timeout_, linger_);
  }

  Server& operator=(Server&& other) {
    stop();
    if (other.isValid()) {
      other.stop();
      callback_ = std::move(other.callback_);
      address_ = std::move(other.address_);
      timeout_ = other.timeout_;
      linger_ = other.linger_;
      initServer(address_, timeout_, linger_);
    }
    return *this;
  }

  ~Server() { stop(); }

  inline void stop() {
    if (isValid()) {
      *alive_ = false;
      if (server_thread_.joinable()) { server_thread_.detach(); }
    }
  }

private:
  inline bool isValid() const { return alive_ == nullptr ? false : alive_->load(); }

  void initServer(const std::string& address, int timeout, int linger) {
    if (!address.empty()) {
      alive_ = std::make_shared<std::atomic<bool>>(true);

      auto socket = std::unique_ptr<GenericSocket<T>>(new GenericSocket<T>{ZMQ_REP});
      socket->bind(address);
      socket->filter();
      socket->setTimeout(timeout);
      socket->setLinger(linger);

      // Start the thread of the server if not yet done: wait for requests on the
      // dedicated thread.
      if (!server_thread_.joinable()) {
        server_thread_ = std::thread(&Server::awaitRequest, this, alive_, std::move(socket));
      }
    }
  }

  /**
   * @brief Keep waiting for a request to arrive. Process the request with the
   * callback function and reply.
   */
  void awaitRequest(std::shared_ptr<std::atomic<bool>> alive, std::unique_ptr<GenericSocket<T>> socket) {
    while (*alive) {
      T msg;
      if (socket->receiveMsg(msg, "[SIMPLE Server] - ") != -1) {
        if (*alive) { callback_(msg); }
        if (*alive) { reply(socket.get(), msg); }
      }
    }
  }

  /**
   * @brief Sends the message back to the client who requested it.
   * @param msg The message to be sent.
   */
  void reply(GenericSocket<T>* socket, const T& msg) { socket->sendMsg(msg.getBufferData(), "[SIMPLE Server] - "); }

  std::shared_ptr<std::atomic<bool>> alive_{nullptr};
  std::function<void(T&)> callback_;
  std::thread server_thread_{};
  std::string address_{""};
  int timeout_{1000};
  int linger_{-1};
};
}  // Namespace simple.

#endif  // SIMPLE_SERVER_HPP
