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

#ifndef SIMPLE_SUBSCRIBER_HPP
#define SIMPLE_SUBSCRIBER_HPP

#include <zmq.h>
#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 *@brief Creates a subscriber socket for a specific type of message.
 */

template <typename T>
class Subscriber {
public:
  Subscriber() = default;

  /**
   * @brief Creates a ZMQ_SUB socket and connects it to the given address.
   * The given callback runs on a dedicated thread.s
   * @param Address to subscriber to, in the form: tcp://HOSTNAME:PORT. e.g
   * tcp://localhost:5555.
   * @param User defined callback function for incoming messages.
   * @param Time the subscriber will block the thread waiting for a message. In
   * milliseconds.
   */
  Subscriber<T>(const std::string& address, const std::function<void(const T&)>& callback, int timeout = 1000)
    : socket_{new GenericSocket<T>(ZMQ_SUB)}, callback_{callback} {
    socket_->filter();
    socket_->setTimeout(timeout);
    socket_->connect(address);
    initSubscriber();
  }

  Subscriber(const Subscriber&) = delete;
  Subscriber& operator=(const Subscriber&) = delete;

  Subscriber(Subscriber&& other) : socket_{std::move(other.socket_)}, callback_{std::move(other.callback_)} {
    other.stop();
    initSubscriber();
  }

  Subscriber& operator=(Subscriber&& other) {
    stop();
    if (other.isValid()) {
      other.stop();
      socket_ = std::move(other.socket_);
      callback_ = std::move(other.callback_);
      initSubscriber();
    }
    return *this;
  }

  ~Subscriber<T>() { stop(); }

  /**
   * @brief Stop the subscriber loop. No further messages will be received.
   */
  inline void stop() {
    if (isValid()) {
      alive_->store(false);
      if (subscriber_thread_.joinable()) { subscriber_thread_.join(); }
    }
  }

  /**
   * @brief Query the endpoint that this object is bound to. 
   * 
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000"
   */
  const std::string& endpoint() { return socket_.endpoint(); }

private:
  inline bool isValid() const { return alive_ == nullptr ? false : alive_->load(); }

  void initSubscriber() {
    alive_ = std::make_shared<std::atomic<bool>>(true);

    // Start the callback thread if not yet done.
    if (!subscriber_thread_.joinable() && socket_ != nullptr) {
	  subscriber_thread_ = std::thread(&Subscriber::subscribe, this, alive_, socket_);
    }
  }

  /**
   * @brief Waits for a message to be published to the connected port,
   * Calls the user callback with an instance of T obtained by a publisher.
   */
  void subscribe(std::shared_ptr<std::atomic<bool>> alive, std::shared_ptr<GenericSocket<T>> socket) {
    while (alive->load()) {
      T msg;
      if (socket->receiveMsg(msg, "[SIMPLE Subscriber] - ") != -1) {
        if (alive->load()) { callback_(msg); }
      }
    }
  }

  std::shared_ptr<std::atomic<bool>> alive_{nullptr};
  std::shared_ptr<GenericSocket<T>> socket_{nullptr};
  std::function<void(const T&)> callback_{};
  std::thread subscriber_thread_{};
};
}  // Namespace simple.

#endif  // SIMPLE_SUBSCRIBER_HPP
