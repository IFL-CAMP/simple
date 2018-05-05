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
#include <string>
#include "context_manager.hpp"

namespace simple {
template <typename T>
class GenericSocket {
public:
  virtual ~GenericSocket() {
    zmq_msg_close(&recv_message_);
    zmq_close(socket_);
  }

  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

protected:
  GenericSocket() = default;

  explicit GenericSocket(int type) {
    socket_ = zmq_socket(ContextManager::instance(), type);
    zmq_msg_init(&recv_message_);
  }

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

  // Keep a copy of the buffer alive until the message is sent.
  static void freeBuffer(void* /*unused*/, void* hint) {
    if (hint != nullptr) {
      auto b = static_cast<std::shared_ptr<flatbuffers::DetachedBuffer>*>(hint);
      delete b;
    }
  }

  int sendMsg(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer,
              const std::string& custom_error = "[SIMPLE Error] - ") {
    // Send the topic first and add the rest of the message after it.
    zmq_msg_t topic = {};
    zmq_msg_init_data(&topic, const_cast<void*>(static_cast<const void*>(topic_)), topic_size_, nullptr, nullptr);

    zmq_msg_t message = {};
    auto buffer_pointer = new std::shared_ptr<flatbuffers::DetachedBuffer>{buffer};
    zmq_msg_init_data(&message, buffer->data(), buffer->size(), freeBuffer, buffer_pointer);

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

  int receiveMsg(T& msg, const std::string& custom_error = "") {
    int data_past_topic{0};
    auto data_past_topic_size{sizeof(data_past_topic)};

    // Start local zmq_message.
    zmq_msg_t local_message{};

    zmq_msg_init(&local_message);

    int message_received = zmq_msg_recv(&local_message, socket_, 0);

    if (message_received != -1) {
      if (strncmp(static_cast<char*>(zmq_msg_data(&local_message)), topic_, topic_size_) == 0) {
        zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
        if (data_past_topic != 0) {
          message_received = zmq_msg_recv(&local_message, socket_, 0);
          if (message_received != -1 && zmq_msg_size(&local_message) != 0) {
            // If message was received, move it to member message.
            zmq_msg_move(&recv_message_, &local_message);
            msg = static_cast<uint8_t*>(zmq_msg_data(&recv_message_));  //< Build a T object from the server reply.
          } else {
            // If receive failed, close the local message.
            zmq_msg_close(&local_message);
            std::cerr << custom_error << "Failed to receive the message. ZMQ Error: " << zmq_strerror(zmq_errno())
                      << std::endl;
          }
        } else {
          // If no data past topic, close the local message.
          zmq_msg_close(&local_message);
          std::cerr << custom_error << "No data inside message." << std::endl;
        }
      } else {
        // If receive failed, close the local message.
        zmq_msg_close(&local_message);
        std::cerr << custom_error << "Received the wrong message type." << std::endl;
      }
    } else {
      // If receive failed, close the local message.
      zmq_msg_close(&local_message);
    }
    return message_received;
  }

  void filter() { zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_, topic_size_); }
  void setTimeout(int timeout) {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    timeout_ = timeout;
  }

  void setLinger(int linger) {
    zmq_setsockopt(socket_, ZMQ_LINGER, &linger, sizeof(linger));
    linger_ = linger;
  }

  void renewSocket(int type) {
    socket_ = zmq_socket(ContextManager::instance(), type);
    zmq_msg_init(&recv_message_);
  }

  void* socket_{nullptr};
  const char* topic_{T::getTopic()};
  const size_t topic_size_{strlen(topic_)};
  std::string address_{""};
  int timeout_{0};
  int linger_{30000};
  zmq_msg_t recv_message_{};
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
