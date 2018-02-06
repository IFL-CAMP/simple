/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
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

#ifndef SIMPLE_PUBLISHER_HPP
#define SIMPLE_PUBLISHER_HPP

#include "simple/generic_socket.hpp"
#include <memory>
#include <string>
#include <zmq.h>
#include <iostream>

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
  explicit Publisher<T>(const std::string& address)
    : GenericSocket<T>(ZMQ_PUB)
  {
    GenericSocket<T>::bind(address);
  }

  Publisher(const Publisher& other)
    : GenericSocket<T>(ZMQ_PUB)
  {
    GenericSocket<T>::bind(other.address_);
  }

  Publisher& operator=(const Publisher& other)
  {
    GenericSocket<T>::renewSocket(ZMQ_PUB);
    GenericSocket<T>::bind(other.address_);
  }

  ~Publisher() = default;

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void publish(const T& msg, int n = 0)
  {
    auto buffer = msg.getBufferData();
    publish(buffer);
    std::cout << "Publishing #" << n << std::endl;
    //publish(buffer.data(), buffer.size());
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be publish.
   */
  void publish(std::shared_ptr<flatbuffers::DetachedBuffer> b)
  {
    GenericSocket<T>::sendMsg(b, "[Simple Publisher] - ");
    //GenericSocket<T>::sendMsg(msg, msg_size, "[Simple Publisher] - ");
  }
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
