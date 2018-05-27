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

#define ZMQ_BUILD_DRAFT_API 1

#include <zmq.h>
#include <memory>
#include <string>
#include "simple/generic_socket.hpp"

namespace simple {
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher : public GenericSocket<T> {
public:
  Publisher() = default;
  ~Publisher() = default;

  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the
   * port.
   * @param port string for the connection port.
   */
  explicit Publisher<T>(const std::string& address, const std::string& group_name)
    : GenericSocket<T>(ZMQ_RADIO, address, group_name) {
    GenericSocket<T>::connect();
  }

  Publisher(const Publisher& other) : Publisher<T>(other.address_, other.group_name_) {}

  /**
   * @brief Copy assignment for a Publisher socket.
   */
  Publisher& operator=(const Publisher& other) {
    GenericSocket<T>::renewSocket(ZMQ_RADIO, other.address_, other.group_name_);
    GenericSocket<T>::connect();
    return *this;
  }

  /**
   * @brief Allows the user to change the group name used to publish messages.
   * Listening Subscribers will have to use the same group name to receive the messages.
   * @param group_name: the group name of accepted messages. Max length is 16 characters.
   */
  void changeGroup(const std::string& group_name) {
    if (group_name.length() <= GenericSocket<T>::MAX_GROUP_NAME_CHARS) {
      GenericSocket<T>::group_name_ = group_name;
    } else {
      std::cerr << "[SIMPLE Publisher] - Given group name is too long. Max length of a group name is "
                << std::to_string(GenericSocket<T>::MAX_GROUP_NAME_CHARS) << " characters." << std::endl;
    }
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   * @return size of the message, in bytes, published. Returns -1 if send fails.
   */
  int publish(const T& msg) { return GenericSocket<T>::send(msg.getBufferData()); }
};
}  // Namespace simple.

#endif  // SIMPLE_PUBLISHER_HPP
