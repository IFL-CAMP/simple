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

#ifndef SIMPLE_GENERIC_SOCKET_HPP
#define SIMPLE_GENERIC_SOCKET_HPP

#include <flatbuffers/flatbuffers.h>
#include <zmq.h>
#include <atomic>
#include <iostream>
#include <memory>
#include <string>

#include "context_manager.hpp"

namespace simple {

// Forward declarations.
template <typename T>
class Subscriber;
template <typename T>
class Publisher;
template <typename T>
class Client;
template <typename T>
class Server;

template <typename T>
class GenericSocket {
public:
  virtual ~GenericSocket() { closeSocket(); }

  // A GenericSocket cannot be copied or moved.
  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

  GenericSocket(GenericSocket&& other) {
    socket_.store(other.socket_);
    other.socket_ = nullptr;
  }

  GenericSocket& operator=(GenericSocket&& other) {
    if (other.isValid()) {
      socket_.store(other.socket_);
      other.socket_ = nullptr;
    }
    return *this;
  }

  friend class Publisher<T>;
  friend class Subscriber<T>;
  friend class Client<T>;
  friend class Server<T>;

protected:
  // Class ctors are protected.
  // A GenericSocket cannot be directly initialized but only used within a specialized subclass.
  GenericSocket() = default;
  explicit GenericSocket(int type) { initSocket(type); }

  /**
   * @brief Binds the ZMQ Socket to the given address.
   * @param[in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void bind(const std::string& address) {
    auto success = zmq_bind(socket_, address.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the address/port: " + address +
                               ". ZMQ Error: " + std::string(zmq_strerror(zmq_errno())));
    }
  }

  /**
   * @brief Connect the ZMQ Socket to the given address.
   * @param[in] address - in the form \<PROTOCOL\>://\<IP_ADDRESS\>:\<PORT\>, e.g. tcp://127.0.0.1:5555.
   * @throws std::runtime_error.
   */
  void connect(const std::string& address) {
    auto success = zmq_connect(socket_, address.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot connect to the address/port: " + address +
                               ". ZMQ Error: " + std::string(zmq_strerror(zmq_errno())));
    }
  }

  /**
   * @brief Sends buffer data over the ZMQ Socket.
   * @param[in] buffer - a pointer to the buffer data constructed using Flatbuffers.
   * @param[in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return the number of bytes sent over the ZMQ Socket. -1 for failure.
   */
  int sendMsg(std::shared_ptr<flatbuffers::DetachedBuffer> buffer,
              const std::string& custom_error = "[SIMPLE Error] - ") {
    // Send the topic first and add the rest of the message after it.

    zmq_msg_t topic_message{};
    auto topic_ptr = const_cast<void*>(static_cast<const void*>(topic_.c_str()));
    zmq_msg_init_data(&topic_message, topic_ptr, topic_.size(), nullptr, nullptr);

    // Create a shared_ptr to the given buffer data, this allows to avoid disposing the data to be sent before the
    // actual transmission is termianted. The zmq_msg_send() methos will return as soon as the message has been queued
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
    zmq_msg_t message{};
    zmq_msg_init_data(&message, buffer->data(), buffer->size(), free_function, buffer_pointer);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_msg_send(&topic_message, socket_, ZMQ_SNDMORE);
    auto message_sent = zmq_msg_send(&message, socket_, ZMQ_DONTWAIT);

    if (topic_sent == -1 || message_sent == -1) {
      // If send is not successful, we need to manually close the messages.
      zmq_msg_close(&message);
      zmq_msg_close(&topic_message);
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << zmq_strerror(zmq_errno()) << std::endl;
    }
    // Else, the message was successfully send, we don't need to call zmq_msg_close manually.
    // We return the number of bytes sent.
    return message_sent;
  }

  /**
   * @brief Receive a message of type T from the ZMQ Socket.
   * @param[in,out] msg The message of type T to populate with the data incoming from the ZMQ Socket.
   * @param[in] custom_error - a string to prefix to the error messages printed in failure cases.
   * @return
   */
  int receiveMsg(T& msg, const std::string& custom_error = "") {
    // Local variables to check if data after the topic message is available and its size.
    int data_past_topic{0};
    auto data_past_topic_size{sizeof(data_past_topic)};

    // Local ZMQ message, it is used to collect the data received from the ZMQ socket.
    // A custom deleter is provided, such that when the message has to be disposed zmq_msg_close is called before
    // deleting the pointer. This is the pointer handling the lifetime of the received data.
    std::shared_ptr<zmq_msg_t> local_message(new zmq_msg_t{}, [](zmq_msg_t* disposable_msg) {
      zmq_msg_close(disposable_msg);
      delete disposable_msg;
    });
    // ZMQ Messages have to be initialized.
    zmq_msg_init(local_message.get());

    // Receive the first bytes, this should match the topic message and can be used to check if the right topic (the
    // right message type) has been received. i.e. the received topic message should match the one of the template
    // argument of this socket (stored in the topic_ member variable).
    int bytes_received = zmq_msg_recv(local_message.get(), socket_, 0);

    // Check that some data has been received and that it matches the right message topic.
    if (bytes_received == -1) { return bytes_received; }

    std::string received_message_type = static_cast<char*>(zmq_msg_data(local_message.get()));

    if (strncmp(received_message_type.c_str(), topic_.c_str(), strlen(topic_.c_str())) != 0) {
      std::cerr << custom_error << "Received message type " << received_message_type << " while expecting " << topic_
                << "." << std::endl;
      return -1;
    }

    // If all is good, check that there is more data after the topic.
    zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);

    if (data_past_topic == 0 || data_past_topic_size == 0) {
      std::cerr << custom_error << "No data inside message." << std::endl;
      return -1;
    }

    // Receive the real message.
    bytes_received = zmq_msg_recv(local_message.get(), socket_, 0);

    // Check if any data has been received.
    if (bytes_received == -1 || zmq_msg_size(local_message.get()) == 0) {
      std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << zmq_strerror(zmq_errno())
                << std::endl;
      return -1;
    }

    // At this point we are sure that the message habe been correctly received, it has the right topic type and it is
    // not empty.

    // Set the T msg to the data that has been just received.
    // A shared_ptr to the internal data of the ZMQ message that shares the same reference counter as local_message is
    // passed. This allows to keep local_message alive until the T msg is alive, thus keeping the pointer inside the T
    // msg valid.
    void* data_ptr = zmq_msg_data(local_message.get());  //< Internal data of the ZMQ message.
    // Shared pointer to the inten data that shares ref counter with local_message. Since it is passed by value to the
    // operator= of T, the ref counter is increased and local_message will stay alive until T needs the data.
    msg = std::shared_ptr<void*>{local_message, &data_ptr};

    // Return the number of bytes received.
    return bytes_received;
  }

  inline void filter() { zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_.c_str(), topic_.size()); }

  inline void setTimeout(int timeout) { zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout)); }

  inline void setLinger(int linger) { zmq_setsockopt(socket_, ZMQ_LINGER, &linger, sizeof(linger)); }

  inline void initSocket(int type) {
    if (socket_ == nullptr) { socket_ = zmq_socket(ContextManager::instance(), type); }
  }

  inline void closeSocket() {
    if (socket_ != nullptr) {
      zmq_close(socket_);
      socket_ = nullptr;
    }
  }

  inline bool isValid() { return static_cast<bool>(socket_ != nullptr); }

private:
  std::string topic_{T::getTopic()};
  std::atomic<void*> socket_{nullptr};
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
