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

#include <flatbuffers/flatbuffers.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#include "context_manager.hpp"

namespace simple {

// Forward declarations. Only necessary for friend declarations later on.
template <typename T>
class Subscriber;
template <typename T>
class Publisher;
template <typename T>
class Client;
template <typename T>
class Server;

/**
 * @class GenericSocket generic_socket.hpp.
 * @brief The GenericSocket class implements the logic to transmit Flatbuffers data over ZMQ sockets. It is a
 * thread-safe class.
 * @tparam T The simple_msgs type to handle.
 */
template <typename T>
class GenericSocket {
public:
  virtual ~GenericSocket() { closeSocket(); }  //! The socket is closed.

  // A GenericSocket cannot be copied, only moved.
  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

  /**
   * @brief Move constructor.
   */
  GenericSocket(GenericSocket&& other) noexcept {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    socket_ = std::move(other.socket_);
    other.socket_ = nullptr;
  }

  /**
   * @brief Move assignment operator.
   */
  GenericSocket& operator=(GenericSocket&& other) noexcept {
    std::lock(mutex_, other.mutex_);
    std::lock_guard<std::mutex> lock{mutex_, std::adopt_lock};
    std::lock_guard<std::mutex> other_lock{other.mutex_, std::adopt_lock};
    if (other.isValid()) {
      socket_ = std::move(other.socket_);
      other.socket_ = nullptr;
    }
    return *this;
  }

  // Only friend classes can instantiate a GenericSocket.
  friend class Publisher<T>;
  friend class Subscriber<T>;
  friend class Client<T>;
  friend class Server<T>;

protected:
  // Class ctors are protected. A user cannot instantiate a GenericSocket.
  GenericSocket() = default;

  /**
   * @brief Constructs a socket with the given ZMQ socket type.
   * @param [in] type - the ZMQ type.
   *
   * Accepted types are:
   * ZMQ_PUB - for a Publisher.
   * ZMQ_SUB - for a Subscriber.
   * ZMQ_REQ - for a Client.
   * ZMQ_REP - for a Server.
   */
  explicit GenericSocket(int type) { initSocket(type); }

  /**
   * @brief Binds the ZMQ Socket to the given address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void bind(const std::string& address) {
    std::lock_guard<std::mutex> lock{mutex_};
    try {
      socket_->bind(address);
    } catch (const zmq::error_t& error) {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the address/port: " + address +
                               ". ZMQ Error: " + error.what());
    }

    // Query the bound endpoint from the ZMQ API.
    char last_endpoint[1024];
    size_t size = sizeof(last_endpoint);
    socket_->getsockopt(ZMQ_LAST_ENDPOINT, &last_endpoint, &size);
    endpoint_ = last_endpoint;
  }

  /**
   * @brief Connect the ZMQ Socket to the given address.
   * @param [in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void connect(const std::string& address) {
    std::lock_guard<std::mutex> lock{mutex_};
    try {
      socket_->connect(address);
    } catch (const zmq::error_t& error) {
      throw std::runtime_error("[SIMPLE Error] - Cannot connect to the address/port: " + address +
                               ". ZMQ Error: " + error.what());
    }
  }

  /**
   * @brief Sends buffer data over the ZMQ Socket.
   * @param [in] buffer - a pointer to the buffer data constructed using Flatbuffers.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return the number of bytes sent over the ZMQ Socket. -1 for failure.
   */
  int sendMsg(std::shared_ptr<flatbuffers::DetachedBuffer> buffer,
              const std::string& custom_error = "[SIMPLE Error] - ") const {
    if (socket_ == nullptr) { return -1; }

    std::lock_guard<std::mutex> lock{mutex_};

    // This is ugly, but we need a void* from the const char*.
    auto topic_ptr = const_cast<void*>(static_cast<const void*>(topic_.c_str()));
    zmq::message_t topic_message{topic_ptr, topic_.size()};

    // Create a shared_ptr to the given buffer data, this allows to avoid disposing the data to be sent before the
    // actual transmission is termianted. The zmq_msg_send() method will return as soon as the message has been queued
    // but there are no guaranteed that the data is yet transmitted. This pointer will keep the data alive and it will
    // be disposed when not necessary anymore.
    auto buffer_pointer = new std::shared_ptr<flatbuffers::DetachedBuffer>{buffer};

    // Functor to call when the data transmission is over. This deletes the buffer_pointer object.
    auto free_function = [](void* /*unused*/, void* hint) {
      if (hint != nullptr) {
        auto b = static_cast<std::shared_ptr<flatbuffers::DetachedBuffer>*>(hint);
        delete b;
      }
    };

    // Initialize the message itself using the buffer data.
    // The functor free_function is passed as the function to call when this zmq_msg_t object has to be disposed.
    // This is automatically called when the data transmission is over.
    // buffer_pointer is passed as the pointer to use for the "hint" parameter in the free_function method.
    zmq::message_t message{buffer->data(), buffer->size(), free_function, buffer_pointer};

    // Send the topic first and add the rest of the message after it.
    bool topic_success{false}, message_success{false};
    try {
      topic_success = socket_->send(topic_message, ZMQ_SNDMORE);
      message_success = socket_->send(message, ZMQ_DONTWAIT);
    } catch (const zmq::error_t& error) {
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << error.what() << std::endl;
    }

    if (topic_success == false || message_success == false) {
      std::cerr << custom_error << "Failed to send the message." << std::endl;
      return -1;
    } else {
      return 1;
    }
  }

  /**
   * @brief Receive a message of type T from the ZMQ Socket.
   * @param [in,out] msg - The message of type T to populate with the data incoming from the ZMQ Socket.
   * @param [in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return the number of bytes received by the ZMQ Socket. -1 for failure.
   */
  int receiveMsg(T& msg, const std::string& custom_error = "") {
    std::lock_guard<std::mutex> lock{mutex_};
    bool success{false};

    // Local variables to check if data after the topic message is available and its size.
    int data_past_topic{0};
    auto data_past_topic_size{sizeof(data_past_topic)};

    // Local ZMQ message, it is used to collect the data received from the ZMQ socket.
    // This is the pointer handling the lifetime of the received data.
    std::shared_ptr<zmq::message_t> local_message(std::make_shared<zmq::message_t>());

    // Receive the first bytes, this should match the topic message and can be used to check if the right topic (the
    // right message type) has been received. i.e. the received topic message should match the one of the template
    // argument of this socket (stored in the topic_ member variable).
    try {
      success = socket_->recv(local_message.get());
    } catch (const zmq::error_t& error) {
      std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << error.what() << std::endl;
      return -1;
    }

    // Check that some data has been received.
    if (success == false) { return -1; }
    // Check if the received topic matches the right message topic.
    std::string received_message_type = static_cast<char*>(local_message->data());
    if (strncmp(received_message_type.c_str(), topic_.c_str(), strlen(topic_.c_str())) != 0) {
      std::cerr << custom_error << "Received message type " << received_message_type << " while expecting " << topic_
                << "." << std::endl;
      return -1;
    }

    // If all is good, check that there is more data after the topic.
    socket_->getsockopt(ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);

    if (data_past_topic == 0 || data_past_topic_size == 0) {
      std::cerr << custom_error << "No data inside message." << std::endl;
      return -1;
    }

    // Receive the real message.
    try {
      success = socket_->recv(local_message.get());
    } catch (const zmq::error_t& error) {
      std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << error.what() << std::endl;
      return -1;
    }

    // Check if any data has been received.
    if (success == false || local_message->size() == 0) {
      std::cerr << custom_error << "Failed to receive the message." << std::endl;
      return -1;
    }

    // At this point we are sure that the message habe been correctly received, it has the right topic type and it is
    // not empty.

    // Set the T msg to the data that has been just received.
    void* data_ptr = local_message->data();

    // Shared pointer to the internal data that shares ref counter with local_message. Since it is passed by value to
    // the operator= of T, the ref counter is increased and local_message will stay alive until the object T needs the
    // data.
    msg = std::shared_ptr<void*>{local_message, &data_ptr};

    // Return the number of bytes received.
    if (success == false) {
      return -1;
    } else {
      return 1;
    }
  }

  /**
   * @brief Set the ZMQ socket to accept only messages with the correct topic name.
   *
   * The topic name is set to the one privded by the template argument of this socket.
   */
  void filter() {
    std::lock_guard<std::mutex> lock{mutex_};
    socket_->setsockopt(ZMQ_SUBSCRIBE, topic_.c_str(), topic_.size());
  }

  /**
   * @brief Set the timeout of the ZMQ socket.
   * @param [in] timeout - in milliseconds.
   *
   * The timeout is used by Subscriber, Server and Client  sockets as the maximum allowed time to wait for an incoming
   * message, request or reply (respectively).
   */
  void setTimeout(int timeout) {
    std::lock_guard<std::mutex> lock{mutex_};
    socket_->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
  }

  /**
   * @brief Set the linger time of the ZMQ socket.
   * @param [in] linger - in milliseconds.
   *
   * After a socket is closed, unsent messages linger in memory to the given amount of time.
   */
  void setLinger(int linger) {
    std::lock_guard<std::mutex> lock{mutex_};
    socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
  }

  /**
   * @brief Initialize the ZMQ socket given its type.
   * @param [in] type - the ZMQ type.
   *
   * Accepted types are:
   * ZMQ_PUB - for a Publisher.
   * ZMQ_SUB - for a Subscriber.
   * ZMQ_REQ - for a Client.
   * ZMQ_REP - for a Server.
   */
  void initSocket(int type) {
    std::lock_guard<std::mutex> lock{mutex_};
    if (socket_ == nullptr) {
      socket_ = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(*ContextManager::instance(), type));
    }
  }

  /**
   * @brief Closes the ZMQ socket.
   */
  void closeSocket() {
    std::lock_guard<std::mutex> lock{mutex_};
    if (socket_ != nullptr) {
      socket_->close();
      socket_ = nullptr;
    }
  }

  /**
   * @brief Returns if the ZMQ socket has been initialized (is valid) or not.
   */
  bool isValid() { return static_cast<bool>(socket_ != nullptr); }

  /**
   * @brief Query the endpoint that this object is bound to.
   *
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return The endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000".
   */
  inline const std::string& endpoint() { return endpoint_; }

private:
  mutable std::mutex mutex_{};                      //! Mutex for thread-safety.
  const std::string topic_{T::getTopic()};          //! The message topic, internally defined for each SIMPLE message.
  std::unique_ptr<zmq::socket_t> socket_{nullptr};  //! The internal ZMQ socket.
  std::string endpoint_{};
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
