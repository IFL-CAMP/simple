#pragma once

#include <string>
#include "simple_msgs/simple.pb.h"
#define ZMQ_STATIC
#include <memory>
#include <vector>
#include <zmq.hpp>
#include "MSGreader.h"

namespace simple {
template <typename T>
class Client {
 public:
  Client(std::string port, zmq::context_t& context);
  ~Client();
  ///@brief Sends the request through the open socket and fills the message with
  ///the data received. Methods hangs until
  /// a reply is received
  ///@param req Message containing the data to be processed at the server, with
  ///an empty field to be filled by the reply
  ///@return Returns true if a suitable reply was received, false otherwise
  bool request(T& req);

 private:
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple
template <typename T>
simple::Client<T>::Client(std::string port, zmq::context_t& context) {
  /// Class constructor: opens the socket, of type ZMQ_REQ
  /// Binds socket to port

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_REQ);

  try {
    socket->connect(port);
  } catch (zmq::error_t& e) {
    std::cerr << "could not bind socket:" << e.what();
  }
  // set maximum time out for sending requests: time out = 600ms
  socket->setsockopt(ZMQ_SNDTIMEO, 600);
  // set maximum time out for receiving replies from the server: time out =
  // 4000ms
  socket->setsockopt(ZMQ_RCVTIMEO, 4000);
}
template <typename T>
simple::Client<T>::~Client() {
  /// Class destructor: Closes the socket
  socket->close();
}
template <typename T>
bool simple::Client<T>::request(T& req) {
  // send a request to the server, containing a message of the type desired and
  // wait for a reply

  std::string msgReq;
  req.SerializeToString(&msgReq);

  zmq::message_t ZMQmsg(msgReq.size());

  memcpy(ZMQmsg.data(), msgReq.c_str(), msgReq.size());

  try {
    socket->send(ZMQmsg);
  } catch (zmq::error_t& e) {
    std::cerr << "Could not send request: " << e.what();
    // fail if request not sent
    return false;
  }

  // get reply
  zmq::message_t MSGreply;

  try {
    socket->recv(&MSGreply);
  } catch (zmq::error_t& e) {
    std::cerr << "Could not receive message: " << e.what();
    // fail if reply not received
    return false;
  }

  std::string strMessage(
      static_cast<char*>(MSGreply.data()),
      MSGreply.size());  // copy data from ZMQ message into string

  // check if this fails when received type is wrong
  bool success = req.ParseFromString(
      strMessage);  // copy data from string to protobuf message

  return success;
}
