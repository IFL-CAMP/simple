#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>
#include "MSGreader.h"

namespace simple
{
template <typename T>
class Client
{
public:
  Client(std::string port, zmq::context_t& context);
  ~Client();
  std::unique_ptr<T> request();

private:
  MSGreader msgReader;
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple
template <typename T>
simple::Client<T>::Client(std::string port, zmq::context_t& context)
{
  /// Class constructor: opens the socket, of type ZMQ_REQ
  /// Binds socket to port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_REQ);

  try
  {
    socket->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not bind socket:" << e.what();
  }
}
template <typename T>
simple::Client<T>::~Client()
{
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
std::unique_ptr<T> simple::Client<T>::request()
{
  // send a request to the server, containing the type of message desired and wait for a reply

  std::unique_ptr<T> SIMPLEreply = std::make_unique<T>();

  std::string msgType = SIMPLEreply->GetTypeName();

  zmq::message_t ZMQmsg(msgType.size());

  memcpy(ZMQmsg.data(), msgType.c_str(), msgType.size());

  try
  {
    socket->send(ZMQmsg);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not send request: " << e.what();
  }

  // get reply
  zmq::message_t MSGreply;

  try
  {
    socket->recv(&MSGreply);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not receive message: " << e.what();
  }

  std::string strMessage(static_cast<char*>(MSGreply.data()),
                         MSGreply.size());  // copy data from ZMQ message into string

  SIMPLEreply->ParseFromString(strMessage);  // copy data from string to protobuf message

  return SIMPLEreply;
}
