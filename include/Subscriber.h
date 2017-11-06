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
class Subscriber
{
public:
  ///@brief Class constructor: opens a socket of type ZMQ_SUB and connects it to the port. Context shall be provided
  Subscriber(std::string port, zmq::context_t& context);
  ~Subscriber();
  ///@brief Returns a message from the data received through the socket. The message type depends on the template type
  /// of the class instance. Filter for the subscriber socket shall be taken from the message type
  ///@return Protobuf-type message, matching the instance type
  std::unique_ptr<T> subscribe();
  void readMsg(const SIMPLE::TRANSFORM& msg, SIMPLE::HEADER& header, double& px, double& py, double& pz, double& r11,
               double& r12, double& r13, double& r21, double& r22, double& r23, double& r31, double& r32, double& r33);
  void readMsg(const SIMPLE::POSITION& msg, SIMPLE::HEADER& header, double& px, double& py, double& pz, double& e1,
               double& e2, double& e3, double& e4);
  void readMsg(const SIMPLE::STATUS& msg, SIMPLE::HEADER& header, int& code, int& subcode, std::string& errorName,
               std::string& errorMsg);
  void readMsg(const SIMPLE::CAPABILITY& msg, SIMPLE::HEADER& header, std::vector<std::string>& msgNames);
  void readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, bool& data);
  void readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, int& data);
  void readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, float& data);
  void readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, double& data);
  void readMsg(const SIMPLE::GENERIC& msg, SIMPLE::HEADER& header, std::string& data);

private:
  ///@brief set the socket option to match the message type according to the class instance
  ///@param msg Reference to the message instance
  void filterSubscription(const T& msg);
  ///@brief Receive a message from the connected publisher. The subscription filter is already set on the instance
  /// socket
  std::unique_ptr<zmq::socket_t> socket;
  MSGreader msgReader;
};
}  // namespace simple

template <typename T>
void simple::Subscriber<T>::filterSubscription(const T& msg)
{
  std::string topic = msg.GetTypeName();

  socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
}

template <typename T>
std::unique_ptr<T> simple::Subscriber<T>::subscribe()
{
  ///

  zmq::message_t ZMQmessage;

  std::unique_ptr<T> BASEmsg = std::make_unique<T>();
  // filter the type of messages this subscriber will receive. Filter type depends on the message type
  filterSubscription(*BASEmsg);

  try
  {
    socket->recv(&ZMQmessage);  // receive messages that fit the filter of the socket
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Could not receive message: " << e.what();
  }

  std::string strMessage(static_cast<char*>(ZMQmessage.data()),
                         ZMQmessage.size());  // copy data from ZMQ message into string

  // remove the topic string in front of the message
  strMessage.erase(0, BASEmsg->GetTypeName().length());

  BASEmsg->ParseFromString(strMessage);  // copy data from string to protobuf message

  return BASEmsg;
}

template <typename T>
simple::Subscriber<T>::Subscriber(std::string port, zmq::context_t& context)
{
  socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);

  try
  {
    socket->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not connect to socket:" << e.what();
  }
}

template <typename T>
simple::Subscriber<T>::~Subscriber()
{
  // close the socket and destroy the context
  socket->close();
}
