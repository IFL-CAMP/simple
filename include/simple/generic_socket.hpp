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
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include "context_manager.hpp"

namespace simple {
template <typename T>
class GenericSocket {
public:
  virtual ~GenericSocket() { zmq_close(socket_); }

  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

protected:
  GenericSocket() = default;

  explicit GenericSocket(int type) { socket_ = zmq_socket(ContextManager::instance(), type); }

  void bind(const std::string& address) {
    address_ = address;
    auto success = zmq_bind(socket_, address.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given "
                               "address/port. ZMQ Error: " +
                               std::string(zmq_strerror(zmq_errno())));
    }
  }

  void connect(const std::string& address) {
    address_ = address;
    auto success = zmq_connect(socket_, address.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot connect to the given "
                               "address/port. ZMQ Error: " +
                               std::string(zmq_strerror(zmq_errno())));
    }
  }

  int sendMsg(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer,
              const std::string& custom_error = "[SIMPLE Error] - ") {
    // Send the topic first and add the rest of the message after it.

    zmq_msg_t topic{};
    auto topic_ptr = const_cast<void*>(static_cast<const void*>(topic_.c_str()));
    zmq_msg_init_data(&topic, topic_ptr, topic_.size(), nullptr, nullptr);

    auto buffer_pointer = new std::shared_ptr<flatbuffers::DetachedBuffer>{buffer};

    auto free_function = [](void* /*unused*/, void* hint) {
      if (hint != nullptr) {
        auto b = static_cast<std::shared_ptr<flatbuffers::DetachedBuffer>*>(hint);
        delete b;
      }
    };

    zmq_msg_t message{};
    zmq_msg_init_data(&message, buffer->data(), buffer->size(), free_function, buffer_pointer);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_msg_send(&topic, socket_, ZMQ_SNDMORE);
    auto message_sent = zmq_msg_send(&message, socket_, ZMQ_DONTWAIT);

    if (topic_sent == -1 || message_sent == -1) {
      // If send is not successful, close the message.
      zmq_msg_close(&message);
      zmq_msg_close(&topic);
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << zmq_strerror(zmq_errno()) << std::endl;
    }
    return message_sent;
  }

  std::shared_ptr<void*> getMessageData(std::shared_ptr<zmq_msg_t> msg) {
    auto data_ptr = zmq_msg_data(msg.get());
    return {msg, &data_ptr};
  }

  int receiveMsg(T& msg, const std::string& custom_error = "") {
    int data_past_topic{0};
    auto data_past_topic_size{sizeof(data_past_topic)};

    std::shared_ptr<zmq_msg_t> local_message(new zmq_msg_t{},
                                             [](zmq_msg_t* disposable_msg) { zmq_msg_close(disposable_msg); });
    zmq_msg_init(local_message.get());

    int bytes_received = zmq_msg_recv(local_message.get(), socket_, 0);

    if (bytes_received == -1) { return bytes_received; }

    if (std::string{static_cast<char*>(zmq_msg_data(local_message.get()))} == topic_) {
      std::cerr << custom_error << "Received the wrong message type." << std::endl;
      return -1;
    }

    zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);

    if (data_past_topic == 0 || data_past_topic_size == 0) {
      std::cerr << custom_error << "No data inside message." << std::endl;
      return -1;
    }

    bytes_received = zmq_msg_recv(local_message.get(), socket_, 0);

    if (bytes_received == -1 || zmq_msg_size(local_message.get()) == 0) {
      std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << zmq_strerror(zmq_errno())
                << std::endl;
      return -1;
    }

    void* data_ptr = zmq_msg_data(local_message.get());
    msg = {local_message, &data_ptr};

    return bytes_received;
  }

  void filter() { zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_.c_str(), topic_.size()); }

  void setTimeout(int timeout) {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    timeout_ = timeout;
  }

  void setLinger(int linger) {
    zmq_setsockopt(socket_, ZMQ_LINGER, &linger, sizeof(linger));
    linger_ = linger;
  }

  void renewSocket(int type) { socket_ = zmq_socket(ContextManager::instance(), type); }

  void* socket_{nullptr};
  std::string topic_{T::getTopic()}, address_{""};
  int timeout_{0}, linger_{30000};
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
