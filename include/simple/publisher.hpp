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

#ifndef SIMPLE_PUBLISHER_HPP
#define SIMPLE_PUBLISHER_HPP

#include <zmq.h>
#include <memory>
#include <string>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher {
public:
  Publisher() = default;

  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the
   * port.
   * @param port string for the connection port.
   */
  explicit Publisher<T>(const std::string& address) : socket_{ZMQ_PUB} { socket_.bind(address); }

  Publisher(const Publisher& other) = delete;
  Publisher& operator=(const Publisher& other) = delete;

  Publisher(Publisher&& other) = default;
  Publisher& operator=(Publisher&& other) = default;

  ~Publisher() = default;

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   * @return size of the message, in bytes, published. Returns -1 if send fails.
   */
  int publish(const T& msg) { return publish(msg.getBufferData()); }
  /**
   * @brief Publishes the message through the open socket.
   * @param buffer: buffer containing the data to be published.
   * @return size of the message, in bytes, published. Returns -1 if send fails.
   */
  int publish(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer) {
    return socket_.sendMsg(buffer, "[Simple Publisher] - ");
  }

  /**
   * @brief Query the endpoint that this object is bound to. 
   * 
   * Can be used to find the bound port if binding to ephemeral ports.
   * @return the endpoint in form of a ZMQ DSN string, i.e. "tcp://0.0.0.0:8000"
   */
  const std::string& endpoint() { return socket_.endpoint(); }

private:
  GenericSocket<T> socket_{};
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
