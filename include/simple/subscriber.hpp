/**
* S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
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

#pragma once

#include <zmq.h>
#include <string>
#include <thread>
#include <memory>
#include "context_deleter.hpp"
#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a subscriber socket for a specific type of message.
 */
template <typename T>
class Subscriber : public GenericSocket<T>
{
public:
  /**
   * @brief Creates a ZMQ_SUB socket and connects it to the given address.
   * The given callback runs on a dedicated thread.s
   * @param Address to subscriber to, in the form: tcp://HOSTNAME:PORT. e.g tcp://localhost:5555.
   * @param User defined callback function for incoming messages.
   * @param Time the subscriber will block the thread waiting for a message. In milliseconds.
   */
  Subscriber<T>(const std::string& address, const std::function<void(const T&)>& callback, int timeout = 100)
    : GenericSocket<T>(zmq_socket(context_.get(), ZMQ_SUB))
    , callback_(callback)
  {
    GenericSocket<T>::connect(address);
    GenericSocket<T>::filter();
    GenericSocket<T>::setTimeout(timeout);

    // Start the callback thread.
    subscriber_thread_ = std::thread(&Subscriber::subscribe, this);
  }

  Subscriber(const Subscriber& other)
    : GenericSocket<T>(zmq_socket(context_.get(), ZMQ_SUB))
    , callback_(callback)
  {
    GenericSocket<T>::connect(other.address_);
    GenericSocket<T>::filter();
    GenericSocket<T>::setTimeout(other.timeout_);

    // Start the callback thread.
    subscriber_thread_ = std::thread(&Subscriber::subscribe, this);
  }

  ~Subscriber<T>()
  {
    alive_ = false;             //< Stop the subscription loop.
    subscriber_thread_.join();  //< Wait for the subscriber thead.
  }

private:
  /**
   * @brief Waits for a message to be published to the connected port,
   * Calls the user callback with an instance of T obtained by a publisher.
   */
  void subscribe()
  {
    while (alive_)
    {
      T msg;
      if (GenericSocket<T>::receiveMsg(msg, "[SIMPLE Subscriber] - "))
      {
        callback_(msg);
      }
    }
  }

  std::thread subscriber_thread_;
  bool alive_{true};
  std::function<void(const T&)> callback_;
  static std::shared_ptr<void> context_;
};

template <typename T>
std::shared_ptr<void> Subscriber<T>::context_(zmq_ctx_new(), contextDeleter);

}  // Namespace simple.
