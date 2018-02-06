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

#ifndef SIMPLE_CLIENT_HPP
#define SIMPLE_CLIENT_HPP

#include "simple/generic_socket.hpp"
#include <memory>
#include <string>
#include <zmq.h>

namespace simple
{
/**
 * @brief Creates a Client socket for a specific type of message.
 */
template <typename T>
class Client : public GenericSocket<T>
{
public:
  /**
   * @brief Default constructor for a socket of type Client.
   * @param address where the client connects to, in the form: tcp://HOSTNAME:PORT. e.g tcp://localhost:5555.
   * @param timeout Time, in msec, the client shall wait for a reply. Default 30 seconds.
   */
  explicit Client(const std::string& address, int timeout = 30000)
    : GenericSocket<T>(ZMQ_REQ)
  {
    GenericSocket<T>::setTimeout(timeout);
    GenericSocket<T>::connect(address);
  }

  /**
   * @brief Copy constructor for Client.
   * Opens a new socket of the same type, connected to the same address, with the same timeout.
   */
  Client(const Client& other)
    : GenericSocket<T>(ZMQ_REQ)
  {
    GenericSocket<T>::setTimeout(other.timeout_);
    GenericSocket<T>::connect(other.address_);
  }

  Client& operator=(const Client& other)
  {
    GenericSocket<T>::renewSocket(ZMQ_REQ);
    GenericSocket<T>::setTimeout(other.timeout_);
    GenericSocket<T>::connect(other.address_);
  }

  ~Client() = default;
  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  bool request(T& msg) { return request(msg.getBufferData(), msg); }
private:
  bool request(const std::shared_ptr<flatbuffers::DetachedBuffer>& buffer, T& msg)
  {
    bool success{false};

    if (GenericSocket<T>::sendMsg(buffer, "[SIMPLE Client] - "))
    {
      zmq_msg_t message{};
      if (GenericSocket<T>::receiveMsg(msg, message, "[SIMPLE Client] - "))
      {
        success = true;
        zmq_msg_close(&message);
      }
      else
      {
        std::cerr << "[SIMPLE Client] - No reply received. Aborting this request." << std::endl;
        // Delete the existing socket and create a new one.
        zmq_close(GenericSocket<T>::socket_);
        GenericSocket<T>::renewSocket(ZMQ_REQ);
        GenericSocket<T>::setTimeout(GenericSocket<T>::timeout_);
        GenericSocket<T>::connect(GenericSocket<T>::address_);
      }
    }
    return success;
  }
};
}  // Namespace simple.

#endif  // SIMPLE_CLIENT_HPP
