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

#ifndef SIMPLE_GENERIC_SOCKET_H
#define SIMPLE_GENERIC_SOCKET_H

#pragma once

#include <zmq.h>
#include <string>
#include <string.h>
#include <flatbuffers/flatbuffers.h>
#include "context_manager.hpp"

namespace simple
{
template <typename T>
class GenericSocket
{
public:
  virtual ~GenericSocket() { zmq_close(socket_); }
  GenericSocket(const GenericSocket&) = delete;

protected:
  GenericSocket() = default;
  GenericSocket(int type) { socket_ = zmq_socket(context_.instance(), type); }

  void bind(const std::string& address)
  {
    address_ = address;
    auto success = zmq_bind(socket_, address.c_str());
    if (success != 0)
    {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
  }

  void connect(const std::string& address)
  {
    address_ = address;
    auto success = zmq_connect(socket_, address.c_str());
    if (success != 0)
    {
      throw std::runtime_error("[SIMPLE Error] - Cannot bind to the given address/port. ZMQ Error: " +
                               std::to_string(zmq_errno()));
    }
  }

  static void freeMsg(void*, void* hint)
  {
    if (hint)
    {
      // Keep a copy of the message builder alive until the sending of the message is done.
      delete (static_cast<std::shared_ptr<flatbuffers::FlatBufferBuilder>*>(hint));
    }
  }

  bool sendMsg(uint8_t* msg, int msg_size, std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer,
               const std::string& custom_error = "[SIMPLE Error] - ")
  {
    zmq_msg_t topic;
    zmq_msg_init_data(&topic, const_cast<void*>(static_cast<const void*>(topic_)), topic_size_, freeMsg, NULL);

    zmq_msg_t message;
    zmq_msg_init_data(&message, msg, msg_size, freeMsg, builder_pointer);

    // Send the topic first and add the rest of the message after it.
    auto topic_sent = zmq_sendmsg(socket_, &topic, ZMQ_SNDMORE);
    auto message_sent = zmq_sendmsg(socket_, &message, 0);

    if (topic_sent == -1 || message_sent == -1)
    {
      std::cerr << custom_error << "Failed to send the message. ZMQ Error: " + std::to_string(zmq_errno()) << std::endl;
      return false;
    }
    return true;
  }

  bool receiveMsg(T& msg, const std::string& custom_error = "")
  {
    bool success{false};
    int data_past_topic{0};
    auto data_past_topic_size = sizeof(data_past_topic);

    zmq_msg_t message;
    zmq_msg_init(&message);

    int message_received = zmq_msg_recv(&message, socket_, 0);

    if (message_received != -1)
    {
      if (strncmp(static_cast<char*>(zmq_msg_data(&message)), topic_, topic_size_) == 0)
      {
        zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
        if (data_past_topic)
        {
          message_received = zmq_msg_recv(&message, socket_, 0);
          if (message_received != -1 && zmq_msg_size(&message) != 0)
          {
            msg = static_cast<uint8_t*>(zmq_msg_data(&message));  //< Build a T object from the server reply.
            success = true;
            zmq_msg_close(&message);
          }
          else
          {
            std::cerr << custom_error << "Failed to receive the message. ZMQ Error:" << std::to_string(zmq_errno())
                      << std::endl;
          }
        }
      }
      else
      {
        std::cerr << custom_error << "Received the wrong message type." << std::endl;
      }
    }
    return success;
  }

  void filter() { zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic_, topic_size_); }
  void setTimeout(int timeout)
  {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    timeout_ = timeout;
  }

  void renewSocket(int type) { socket_ = zmq_socket(context_.instance(), type); }

  void* socket_;
  const char* topic_{T::getTopic()};
  const size_t topic_size_{strlen(topic_)};
  std::string address_{""};
  int timeout_{0};
  ContextManager context_;
};

}  // Namespace simple.

#endif  // SIMPLE_GENERIC_SOCKET_H
