#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>
#include "simple\contextCloser.h"

namespace simple
{
/**
 * @brief Creates a reply socket.
 */
template <typename T>
class Server
{
public:
  /**
   * @brief Creates a reply socket. Opens a socket of type REP and connects it to the port. The user defined callback
   * function is responsible for taking the received request and filling it with the reply data
   */
  Server(const std::string& port, const std::function<void(const T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_REP)), callback_(callback)
  {
    try
    {
      socket_->bind(port);
    }
    catch (zmq::error_t& e)
    {
      std::cout << "could not bind socket:" << e.what();
    }

    // start thread of the server: wait for requests on a dedicated thread
    t_ = std::thread(&Server::reply, this);
  }

  ~Server()
  {
    // stop the reply loop
    alive_ = false;

    // join thread
    t_.join();

    socket_->close();
  }

  void reply()
  {
    while (alive_)
    {
      // wait for next request.
      zmq::message_t recvREQ;

      try
      {
        socket->recv(&recvREQ);
      }
      catch (zmq::error_t& e)
      {
        std::cout << "Could not receive message: " << e.what();
      }

      // put the received message into the wrapper
	  auto convertMsg = static_cast<uint8_t*>(recvREQ.data());
      T wrappedReq(convertMsg);//TODO error safe in case of wrong type

      // use the callback to fetch the requested data
	  callback_(wrappedReq);

	  //reply with the message
	  reply(wrappedReq);
	  //get the buffer from the wrapper
	  // put the data into the ZMQ message
	  
    }
  }

  void reply(const T& msg){
	  uint8_t* buffer = msg.getBufferData();
	  int buffer_size = msg.getBufferSize();
	  reply(buffer, buffer_size);
  }

  void reply(const uint8_t* msg, const int size){
	  zmq::message_t rep;

	  memcpy(rep.data(), msg, size);

	  try
	  {
		  socket_->send(rep);
	  }
	  catch (zmq::error_t& e)
	  {
		  std::cerr << "Error - Could not send the message: " << e.what();
	  }
  }

private:
  std::thread t_;
  bool alive_{ true };
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  
  std::function<void(const T&)> callback_;
};
template <typename T>
std::unique_ptr<zmq::context_t, contextCloser> Server<T>::context_(new zmq::context_t(1));

}  // namespace simple
