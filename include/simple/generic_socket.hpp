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

#include <memory>
#include <mutex>
#include <string>

#include "context_manager.hpp"

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace zmq {
class socket_t;
}  // namespace zmq

namespace simple {

/**
 * @brief The zmq::socket_type are redefined locally to avoid including the zmq.hpp header in simple headers.
 */
enum class zmq_socket_type : int { Pub = 1, Sub = 2, Req = 3, Rep = 4 };

/**
 * @class GenericSocket generic_socket.hpp.
 * @brief The GenericSocket class implements the logic to transmit Protobuf data over ZMQ sockets.
 * It is a thread-safe class.
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
  GenericSocket();

  /**
   * @brief Constructs a socket with the given ZMQ socket type.
   * @param [in] type - the ZMQ type.
   * @param [in] topic - the message topic, received messages can be filtered using this value.
   *
   * Accepted types are:
   * zmq_socket_type::Pub - for a Publisher.
   * zmq_socket_type::Sub - for a Subscriber.
   * zmq_socket_type::Req - for a Client.
   * zmq_socket_type::Sub - for a Server.
   */
  explicit GenericSocket(const zmq_socket_type& type, const std::string& topic);

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
   * @param [in] message - a Protobuf messages.
   * @param [in] message_size - size of the message to send.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return success or failure in sending the message over ZMQ.
   */
  bool sendMessage(const ::google::protobuf::Message& message, const size_t message_size,
                   const std::string& custom_error = "[SIMPLE Error] - ") const;

  /**
   * @brief Receives a Protobuf message from the ZMQ Socket.
   * @param [in,out] msg - Protobuf message to populate with the data incoming from the ZMQ Socket.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return success or failure in receiving the message over ZMQ.
   */
  bool receiveMessage(google::protobuf::Message& msg, const std::string& custom_error = "");

  /**
   * @brief Set the ZMQ socket to accept only messages with the correct topic name.
   * The topic name is set to the one provided during construction.
   */
  void filter();

  /**
   * @brief Set the timeout of the ZMQ socket.
   * @param [in] timeout - in milliseconds.
   *
   * The timeout is used by Subscriber, Server and Client sockets as the maximum allowed time to wait for an incoming
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
   * zmq_socket_type::Pub - for a Publisher.
   * zmq_socket_type::Sub - for a Subscriber.
   * zmq_socket_type::Req - for a Client.
   * zmq_socket_type::Rep - for a Server.
   */
  void initSocket(const zmq_socket_type& type);

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
   * @return The endpoint in form of a ZMQ DSN string, i.e., "tcp://0.0.0.0:8000".
   */
  inline const std::string& endpoint() { return endpoint_; }

private:
  void setEndpoint();

  mutable std::mutex mutex_{};                      //! Mutex for thread-safety.
  std::string topic_{""};                           //! The message topic, internally defined for each SIMPLE message.
  std::unique_ptr<zmq::socket_t> socket_{nullptr};  //! The internal ZMQ socket.
  std::string endpoint_{""};                        //! Stores the used endpoint for connection.
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
