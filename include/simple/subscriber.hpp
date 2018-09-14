/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
/**
 * @class Subscriber subscriber.hpp.
 * @brief The Subscriber class creates a ZMQ Socket of type ZMQ_SUB that receives messages of type T from a simple
 * Publisher.
 * @tparam T The simple_msgs type to be received.
 *
 * Implements the logic for a Subscriber in the Publisher / Subscriber paradigm. A Subscriber receives messages of type
 * T from a simple Publisher. The received messages will be passed to the callback function which is providede to the
 * Subscriber upon construction.
 */
template <typename T>
class Subscriber {
public:
  Subscriber() = default;
  /**
   * @brief Creates a ZMQ_PUB socket and binds it to the given address.
   *
   * Subscribers can subscribe to a Publisher connecting to its address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   */
  /**
   * @brief Creates a ZMQ_SUB socket and connects it to the given address, a Publisher is expected to be workin on that
   * address. The given callback function  runs on a dedicated thread.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @param [in] callback - user defined callback function for incoming messages.
   * @param [in] timeout - Time the subscriber will block the thread waiting for a message. In
   * milliseconds.
   */
  explicit Subscriber<T>(const std::string& address, const std::function<void(const T&)>& callback, int timeout = 1000)
    : socket_{new GenericSocket<T>(ZMQ_SUB)}, callback_{callback} {
    socket_->filter();  //! Filter the type of message that can be received, only the type T is accepted.
    socket_->setTimeout(timeout);
    socket_->connect(address);
    initSubscriber();
  }

  // A Subscriber cannot be copied, only moved.
  Subscriber(const Subscriber&) = delete;
  Subscriber& operator=(const Subscriber&) = delete;

  /**
   * @brief Move constructor.
   */
  Subscriber(Subscriber&& other) : socket_{std::move(other.socket_)}, callback_{std::move(other.callback_)} {
    other.stop();  //! The moved Subscriber has to be stopped.
    initSubscriber();
  }

  /**
   * @brief Move assignment operator.
   */
  Subscriber& operator=(Subscriber&& other) {
    stop();                 //! Stop the current Subscriber object.
    if (other.isValid()) {  //! Move the Subscriber only if it's a valid one, e.g. if it was not default constructed.
      other.stop();         //! The moved Subscriber has to be stopped.
      socket_ = std::move(other.socket_);
      callback_ = std::move(other.callback_);
      initSubscriber();
    }
    return *this;
  }

  ~Subscriber<T>() { stop(); }

  /**
   * @brief Stop the subscriber loop. No further messages will be received.
   *
   * A Subscriber cannot be restarted, but a new one (with the same parameters) can be constructed.
   */
  inline void stop() {
    if (isValid()) {
      alive_.store(false);
      if (subscriber_thread_.joinable()) { subscriber_thread_.join(); }
    }
  }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000"
   */
  const std::string& endpoint() { return socket_->endpoint(); }

private:
  /**
   * @brief Checks if the Subscriber is properly initialied and its internal thread is running.
   */
  inline bool isValid() const { return alive_.load(); }

  /**
   * @brief Initializes the Subscriber thread.
   */
  void initSubscriber() {
    alive_.store(true);

    // Start the callback thread if not yet done.
    if (!subscriber_thread_.joinable() && socket_ != nullptr) {
      subscriber_thread_ = std::thread(&Subscriber::subscribe, this);
    }
  }

  /**
   * @brief Waits for a message to be published to the connected port.
   * Calls the user callback with an instance of T obtained by a simple Publisher.
   */
  void subscribe() {
    while (alive_.load()) {  //! Run this in a loop until the Subscriber is stopped.
      T msg;
      if (socket_->receiveMsg(msg, "[SIMPLE Subscriber] - ") != -1) {
        if (alive_.load()) { callback_(msg); }
      }
    }
  }

  std::atomic<bool> alive_{false};                     //! Flag keeping track of the internal thread's state.
  std::unique_ptr<GenericSocket<T>> socket_{nullptr};  //! The internal socket.
  std::function<void(const T&)> callback_{};           //! The callback function called at each message arrival.
  std::thread subscriber_thread_{};  //! The internal Subscriber thread on which the given callback runs.
};
}  // Namespace simple.

#endif  // SIMPLE_SUBSCRIBER_HPP
