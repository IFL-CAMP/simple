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

#ifndef SIMPLE_SERVER_HPP
#define SIMPLE_SERVER_HPP

#include "simple/generic_socket.hpp"
#include <memory>
#include <string>
#include <thread>
#include <zmq.h>

namespace simple
{
/**
 * @brief Creates a reply socket for a specific type of message.
 */
template <typename T>
class Server : public GenericSocket<T>
{
public:
  Server() = default;
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it to the port. The user defined callback
   * function is responsible for taking the received request and filling it with the reply data
   * @param address where the server binds to, in the form: tcp://HOSTNAME:PORT. e.g tcp://localhost:5555.
   * @param callback function that deals with the received request in the desired manner.
   * @param timeout Time, in msec, the server shall wait for a request before restarting the awaitRequest loop. Default
   * 100 miliseconds.
   * @param linger Time, in msec, unsent messages linger in memory after socket is closed. Default -1 (infinite).
   */
  Server(const std::string& address, const std::function<void(T&)>& callback, int timeout = 100, int linger = -1)
    : GenericSocket<T>(ZMQ_REP)
    , callback_(callback)
  {
    initServer(address, timeout, linger);
  }

  Server(const Server& other)
    : GenericSocket<T>(ZMQ_REP)
    , callback_(other.callback_)
  {
    initServer(other.address_, other.timeout_, other.linger_);
  }

  Server& operator=(const Server& other)
  {
    GenericSocket<T>::renewSocket(ZMQ_REP);
	callback_ = other.callback_;
    initServer(other.address_, other.timeout_, other.linger_);
    return *this;
  }

  ~Server()
  {
    alive_ = false;         //< Stop the request/reply loop.
    server_thread_.join();  //< Wait for the server thead.
  }

private:
  void initServer(const std::string& address, int timeout, int linger)
  {
    GenericSocket<T>::bind(address);
    GenericSocket<T>::filter();
    GenericSocket<T>::setTimeout(timeout);
    GenericSocket<T>::setLinger(linger);

    // Start the thread of the server: wait for requests on the dedicated thread.
    server_thread_ = std::thread(&Server::awaitRequest, this);
  }

  /**
   * @brief Keep waiting for a request to arrive. Process the request with the callback function and reply.
   */
  void awaitRequest()
  {
    while (alive_)
    {
      T msg;
      if (GenericSocket<T>::receiveMsg(msg, "[SIMPLE Server] - "))
      {
        callback_(msg);
        reply(msg);
      }
    }
  }

  /**
   * @brief Sends the message back to the client who requested it.
   * @param msg The message to be sent.
   */
  void reply(const T& msg) { GenericSocket<T>::sendMsg(msg.getBufferData(), "[SIMPLE Server] - "); }
  std::thread server_thread_;
  bool alive_{true};
  std::function<void(T&)> callback_;
};
}  // Namespace simple.

#endif  // SIMPLE_SERVER_HPP
