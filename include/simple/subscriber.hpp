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

#define ZMQ_BUILD_DRAFT_API 1

#include <zmq.h>
#include <memory>
#include <string>
#include <thread>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 *@brief Creates a subscriber socket for a specific type of message.
 */

template <typename T>
class Subscriber : public GenericSocket<T> {
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
  Subscriber<T>(const std::string& address, const std::string& group_name,
                const std::function<void(const T&)>& callback, int timeout = 100)
    : GenericSocket<T>(ZMQ_DISH, address, group_name), callback_(callback) {
    initSubscriber(timeout);
  }

  Subscriber(const Subscriber& other) : Subscriber(other.address_, other.group_name_, other.callback_) {}

  Subscriber& operator=(const Subscriber& other) {
    GenericSocket<T>::renewSocket(ZMQ_DISH, other.address_, other.group_name_);
    callback_ = other.callback_;
    initSubscriber(other.timeout_);
    return *this;
  }

  ~Subscriber() {
    alive_ = false;  //< Stop the subscription loop.
    if (subscriber_thread_.joinable()) {
      subscriber_thread_.join();  //< Wait for the subscriber thread.
    }
    // Stop filtering the incoming messages using the given group_name.
    zmq_leave(GenericSocket<T>::socket_, GenericSocket<T>::group_name_.c_str());
  }

  /**
   * @brief Allows the user to change the message group name the Subscriber uses to filter incoming messages.
   * Accepted messages have to be sent by a Publisher using the given group name.
   * @param group_name: the group name of accepted messages. Max length is 16 characters.
   */
  void changeGroup(const std::string& group_name) {
    if (group_name.length() <= GenericSocket<T>::MAX_GROUP_NAME_CHARS) {
      GenericSocket<T>::group_name_ = group_name;
      alive_ = false;
      zmq_leave(GenericSocket<T>::socket_, GenericSocket<T>::group_name_.c_str());
      zmq_join(GenericSocket<T>::socket_, GenericSocket<T>::group_name_.c_str());
      alive_ = true;
    } else {
      std::cerr << "[SIMPLE Subscriber] - Given group name is too long. Max length of a group name is "
                << std::to_string(GenericSocket<T>::MAX_GROUP_NAME_CHARS) << " characters." << std::endl;
    }
  }

private:
  /**
   * @brief Initiaze the Subscriber with the given parameters.
   */
  void initSubscriber(int timeout) {
    GenericSocket<T>::bind();
    // Filter the incoming messages by the given group name.
    zmq_join(GenericSocket<T>::socket_, GenericSocket<T>::group_name_.c_str());
    GenericSocket<T>::setTimeout(timeout);  //< Set how many milliseconds the subscriber will wait for messages.

    // Start the callback thread if not yet done.
    // The Subscriber waits for messages of the specific thread, such that they are eleborated asynchronously by the
    // callback function.
    if (!subscriber_thread_.joinable()) { subscriber_thread_ = std::thread(&Subscriber::subscribe, this); }
  }

  /**
   * @brief Waits for a message to be published to the connected port,
   * Calls the user callback with an instance of T obtained by a publisher.
   */
  void subscribe() {
    while (alive_) {
      T msg;
      if (GenericSocket<T>::receive(msg, "[SIMPLE Subscriber] - ") != -1) { callback_(msg); }
    }
  }

  std::thread subscriber_thread_;
  bool alive_{true};
  std::function<void(const T&)> callback_;
};
}  // Namespace simple.

#endif  // SIMPLE_SUBSCRIBER_HPP
