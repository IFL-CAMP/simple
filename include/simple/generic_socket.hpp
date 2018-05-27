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

#define ZMQ_BUILD_DRAFT_API 1

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
  virtual ~GenericSocket() { zmq_close(socket_); }

  GenericSocket(const GenericSocket&) = delete;
  GenericSocket& operator=(const GenericSocket&) = delete;

protected:
  GenericSocket() = default;

  explicit GenericSocket(int socket_type, const std::string& address, const std::string& group_name = "")
    : address_(address), group_name_(group_name) {
    if (group_name.length() > MAX_GROUP_NAME_CHARS) {
      // If the given group name is too long the created socket would be unusable, throw an exeption here.
      throw std::invalid_argument("[SIMPLE Error] - Given group name is too long. Max length of a group name is " +
                                  std::to_string(GenericSocket<T>::MAX_GROUP_NAME_CHARS) + " characters.");
    }
    socket_ = zmq_socket(ContextManager::instance(), socket_type);
  }

  void bind() {
    auto success = zmq_bind(socket_, address_.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given "
                               "address/port. ZMQ Error: " +
                               std::string(zmq_strerror(zmq_errno())));
    }
  }

  void connect() {
    auto success = zmq_connect(socket_, address_.c_str());
    if (success != 0) {
      throw std::runtime_error("[SIMPLE Error] - Cannot connect to the given "
                               "address/port. ZMQ Error: " +
                               std::string(zmq_strerror(zmq_errno())));
    }
  }

  int send(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer,
           const std::string& custom_error = "[SIMPLE Error] - ") {
    auto buffer_pointer = new std::shared_ptr<flatbuffers::DetachedBuffer>{buffer};

    auto free_function = [](void* /*unused*/, void* hint) {
      if (hint != nullptr) {
        auto b = static_cast<std::shared_ptr<flatbuffers::DetachedBuffer>*>(hint);
        delete b;
      }
    };

    zmq_msg_t message{};
    zmq_msg_init_data(&message, buffer->data(), buffer->size(), free_function, buffer_pointer);

    // group_name is only set for Publishers and Subscribers, if it was set, add it to the message.
    if (group_name_ != "") { zmq_msg_set_group(&message, group_name_.c_str()); }

    // routing_id is only set for Clients and Servers, it is was set add it to the message.
    if (routing_id_ != 0) { zmq_msg_set_routing_id(&message, routing_id_); }

    // Send the topic first and add the rest of the message after it.
    auto sent_bytes = zmq_msg_send(&message, socket_, 0);

    if (sent_bytes == -1) {
      // If send is not successful, close the message.
      zmq_msg_close(&message);
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " << zmq_strerror(zmq_errno()) << std::endl;
    }
    return sent_bytes;
  }

  int receive(T& msg, const std::string& custom_error = "") {
    std::shared_ptr<zmq_msg_t> local_message(new zmq_msg_t{}, [](zmq_msg_t* disposable_msg) {
      zmq_msg_close(disposable_msg);
      delete disposable_msg;
    });

    zmq_msg_init(local_message.get());

    auto received_bytes = zmq_msg_recv(local_message.get(), socket_, 0);

    // If a message was received, move it to the member message. This keeps the message alive until the socket is.
    if (received_bytes != -1 && zmq_msg_size(local_message.get()) != 0) {
      // Attempt to get the message routing_id, this exists only if
      // the message was sent by a Client socket.
      // For other socket, the return value will be always 0;
      routing_id_ = zmq_msg_routing_id(local_message.get());
      void* data_ptr = zmq_msg_data(local_message.get());
      msg = std::shared_ptr<void*>{local_message, &data_ptr};
    } else if (zmq_errno() != EAGAIN)  //< ZMQ_DISH sockets generate an error is no data is available, this avoid
                                       // printing the error also in that case.
    {
      // If receive failed, close the local message.
      zmq_msg_close(local_message.get());
      std::cerr << custom_error << "Failed to receive a message. ZMQ Error: " << zmq_strerror(zmq_errno()) << std::endl;
    } else {
      zmq_msg_close(local_message.get());
    }

    return received_bytes;
  }

  void setTimeout(int timeout) {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    timeout_ = timeout;
  }

  void setLinger(int linger) {
    zmq_setsockopt(socket_, ZMQ_LINGER, &linger, sizeof(linger));
    linger_ = linger;
  }

  void renewSocket(int type, const std::string& address, const std::string& group_name = "") {
    socket_ = zmq_socket(ContextManager::instance(), type);
    address_ = address;
    group_name_ = group_name;
  }

  void* socket_{nullptr};
  std::string topic_{T::getTopic()}, address_{""}, group_name_{""};
  int timeout_{0}, linger_{30000}, routing_id_{0};
  constexpr static size_t MAX_GROUP_NAME_CHARS{16};
};
}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_HPP
