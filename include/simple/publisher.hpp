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
#include <memory>
#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher : public GenericSocket<T>
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Publisher<T>(const std::string& address)
    : GenericSocket<T>(ZMQ_PUB)
  {
    GenericSocket<T>::bind(address);
  }

  Publisher(const Publisher& other)
    : GenericSocket<T>(ZMQ_PUB))
  {
    GenericSocket<T>::bind(other.address_);
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void publish(const T& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer = msg.getBuilderPointer();
    publish(buffer, buffer_size, builder_pointer);
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be publish.
   */
  void publish(uint8_t* msg, const int msg_size, std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer)
  {
    GenericSocket<T>::sendMsg(msg, msg_size, builder_pointer, "[Simple Publisher] - ");
  }
};
}  // Namespace simple.
