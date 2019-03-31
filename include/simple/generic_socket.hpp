/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_GENERIC_SOCKET_HPP
#define SIMPLE_GENERIC_SOCKET_HPP

#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <simple_msgs/generic_message.hpp>
#include <string>

#include "context_manager.hpp"

namespace flatbuffers {
class DetachedBuffer;
}

namespace simple {

/**
 * @class GenericSocket generic_socket.hpp.
 * @brief The GenericSocket class implements the logic to transmit Flatbuffers data over ZMQ sockets. It is a
 * thread-safe class.
 * @tparam T The simple_msgs type to handle.
 */
class GenericSocket {
public:
  virtual ~GenericSocket();

  // A GenericSocket cannot be copied, only moved.
  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

  /**
   * @brief Move constructor.
   */
  GenericSocket(GenericSocket&& other) noexcept;

  /**
   * @brief Move assignment operator.
   */
  GenericSocket& operator=(GenericSocket&& other) noexcept;

  // Friend declarations. Every Client<T>, Server<T>, Publisher<T> and Subscriber<T> is a friend.
  template <typename T>
  friend class Client;
  template <typename T>
  friend class Server;
  template <typename T>
  friend class Publisher;
  template <typename T>
  friend class Subscriber;

protected:
  // Class ctors are protected. A user cannot instantiate a GenericSocket.
  GenericSocket() = default;

  /**
   * @brief Constructs a socket with the given ZMQ socket type.
   * @param [in] type - the ZMQ type.
   * @param [in] topic - the message topic, it is internally defined for every simple_msgs.
   *
   * Accepted types are:
   * ZMQ_PUB - for a Publisher.
   * ZMQ_SUB - for a Subscriber.
   * ZMQ_REQ - for a Client.
   * ZMQ_REP - for a Server.
   */
  explicit GenericSocket(const zmq::socket_type& type, const std::string& topic);

  /**
   * @brief Binds the ZMQ Socket to the given address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void bind(const std::string& address);

  /**
   * @brief Connect the ZMQ Socket to the given address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void connect(const std::string& address);

  /**
   * @brief Sends buffer data over the ZMQ Socket.
   * @param [in] buffer - a pointer to the buffer data constructed using Flatbuffers.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return the number of bytes sent over the ZMQ Socket. -1 for failure.
   */
  bool sendMsg(std::shared_ptr<flatbuffers::DetachedBuffer> buffer,
               const std::string& custom_error = "[SIMPLE Error] - ") const;

  /**
   * @brief Receive a message of type T from the ZMQ Socket.
   * @param [in,out] msg - The message of type T to populate with the data incoming from the ZMQ Socket.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return the number of bytes received by the ZMQ Socket. -1 for failure.
   */
  bool receiveMsg(simple_msgs::GenericMessage& msg, const std::string& custom_error = "");

  /**
   * @brief Set the ZMQ socket to accept only messages with the correct topic name.
   *
   * The topic name is set to the one privded by the template argument of this socket.
   */
  void filter();

  /**
   * @brief Set the timeout of the ZMQ socket.
   * @param [in] timeout - in milliseconds.
   *
   * The timeout is used by Subscriber, Server and Client  sockets as the maximum allowed time to wait for an incoming
   * message, request or reply (respectively).
   */
  void setTimeout(int timeout);

  /**
   * @brief Set the linger time of the ZMQ socket.
   * @param [in] linger - in milliseconds.
   *
   * After a socket is closed, unsent messages linger in memory to the given amount of time.
   */
  void setLinger(int linger);

  /**
   * @brief Initialize the ZMQ socket given its type.
   * @param [in] type - the ZMQ Socket Type.
   *
   * Accepted types are:
   * zmq::socket_type::pub - for a Publisher.
   * zmq::socket_type::sub - for a Subscriber.
   * zmq::socket_type::req - for a Client.
   * zmq::socket_type::rep - for a Server.
   */
  void initSocket(const zmq::socket_type& type);

  /**
   * @brief Closes the ZMQ socket.
   */
  void closeSocket();

  /**
   * @brief Returns if the ZMQ socket has been initialized (is valid) or not.
   */
  bool isSocketValid();

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return The endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000".
   */
  inline const std::string& endpoint() { return endpoint_; }

private:
  mutable std::mutex mutex_{};                      //! Mutex for thread-safety.
  std::string topic_{""};                           //! The message topic, internally defined for each SIMPLE message.
  std::unique_ptr<zmq::socket_t> socket_{nullptr};  //! The internal ZMQ socket.
  std::string endpoint_{""};                        //! Stores the used endpoint for connection.
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
