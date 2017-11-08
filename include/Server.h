#pragma once

#include <string>
#include "SIMPLE.pb.h"
#include <zmq.hpp>
#include <zmq_utils.h>
#include <memory>
#include <vector>
#include "MSGcreator.h"

namespace simple
{
template <typename T>
class Server
{
  ///@brief Server class.
public:
  ///@brief Class constructor, opens a socket of type ZMQ_REP and connects it to the given port
  ///@param port String containing the port to be connected to
  ///@param context ZMQ context used for creating the socket
  Server(std::string port, zmq::context_t& context);
  ///@brief Class destructor, closes the socket
  ~Server();
  ///@brief Replies with the content provided
  ///@param msg Protobuf message containing the data to be replied
  void reply(const T& msg);
  ///@brief Receives a request from a client through the socket and checks if the type requested matches the server
  /// instance type
  ///@param msg
  ///@return Returns True if the request can be answered, False otherwise
  bool receivedRequest(T& msg);

private:
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple
template <typename T>
simple::Server<T>::Server(std::string port, zmq::context_t& context)
{
  /// Class constructor: opens a socket of type REP and connects it to the port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_REP);

  try
  {
    socket->bind(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not bind socket:" << e.what();
  }
}
template <typename T>
simple::Server<T>::~Server()
{
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
bool simple::Server<T>::receivedRequest(T& msg)
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

  std::string strMessage(static_cast<char*>(recvREQ.data()),
                         recvREQ.size());          // copy data from ZMQ message into string
  bool success = msg.ParseFromString(strMessage);  // get data from string into protobuf message

  return success;
}
template <typename T>
void simple::Server<T>::reply(const T& msg)
{
  // Reply with the data

  // serialize data to string
  std::string strReply;
  msg.SerializeToString(&strReply);
  // create ZMQ message for reply
  zmq::message_t ZMQreply(strReply.size());
  // copy data to ZMQ message
  memcpy(ZMQreply.data(), strReply.c_str(), strReply.size());
  // send the reply through the socket
  try
  {
    socket->send(ZMQreply);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not send message: " << e.what();
  }
}
