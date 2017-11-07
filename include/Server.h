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
  /// Server class.
public:
  Server(std::string port, zmq::context_t& context);
  ~Server();
  ///@brief Receives a request from a client through the socket and replies, if the requested type matches the server
  ///instance type
  void reply();

private:
  std::unique_ptr<zmq::socket_t> socket;
  std::unique_ptr<T> getData();
};

}  // namespace simple
template <typename T>
simple::Server<T>::Server(std::string port, zmq::context_t& context)
{
  /// Class constructor: opens a socket of type REP and connects it to the port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_REP);

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
simple::Server<T>::~Server()
{
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
void simple::Server<T>::reply()
{
  // wait for next request. Fetch the required data and reply
  zmq::message_t recvREQ;

  try
  {
    socket->recv(&recvREQ);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not receive message: " << e.what();
  }

  // create reply message
  T msgReply;

  // check requested type matches the reply type
  std::string replyTopic = msgReply.GetTypeName();

  std::string strMessage(static_cast<char*>(recvREQ.data()),
                         recvREQ.size());  // copy data from ZMQ message into string

  if (strMessage == replyTopic)
  {
    // get the data
    std::unique_ptr<T> data = getData();
    // serialize data to string
    std::string strReply;
    data->SerializeoString(&strReply);
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
  else
  {
    // this Server can't provide a reply that matches the requested type
    std::cout << "Error! This server only provides messages of type " << replyTopic << "\n";
  }

}
