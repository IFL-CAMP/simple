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
  Subscriber(std::string port, zmq::context_t& context);
  ~Subscriber();
  ///@brief Returns a message from the data received through the socket. The message type depends on the template type of the class instance
  ///@return Protobuf-type message, matching the instance type
  std::unique_ptr<T> subscribe();

private:
	///@brief set the socket option to match the message type according to the class instance
  void filterSubscription();
  ///@brief Receive a message from the connected publisher. The subscription filter is already set on the instance socket
  std::unique_ptr<T> retrieveBaseMsg();
  std::unique_ptr<zmq::socket_t> socket;
};
}  // namespace simple

template <typename T>
void simple::Subscriber<T>::filterSubscription() {
	
  socket->setsockopt(ZMQ_SUBSCRIBE, T::GetTypeName().c_str(), T::GetTypeName().length());
}

template <typename T>
std::unique_ptr<T> simple::Subscriber<T>::retrieveBaseMsg(){
	///

	zmq::message_t ZMQmessage;

	try{
		socket->recv(&ZMQmessage);//receive messages that fit the filter of the socket
	}
	catch (zmq::error_t& e){
		std::cout << "Could not receive message: " << e.what();
	}

	std::unique_ptr<T> BASEmsg = std::make_unique<T>();

	std::string strMessage(static_cast<char*>(ZMQmessage.data()), ZMQmessage.size());//copy data from ZMQ message into string

	BASEmsg->ParseFromString(strMessage);//copy data from string to protobuf message

	return BASEmsg;

}

/*void simple::Subscriber<SIMPLE::CAPABILITY>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives CAPABILITY messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3TYP";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}

void simple::Subscriber<SIMPLE::IMAGE>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives IMAGE messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3IMG";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}

void simple::Subscriber<SIMPLE::TRANSFORM>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives TRANSFORM messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3TRF";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}

void simple::Subscriber<SIMPLE::GENERIC>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives GENERIC messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3GEN";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}

void simple::Subscriber<SIMPLE::POSITION>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives POSITION messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3POS";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}

void simple::Subscriber<SIMPLE::STATUS>::filterSubscription()
{
  /// Sets up a filter so the subscription only receives STATUS messages

  // the message type consists of 3 letters: the last 3 on filter have to match the message type
  std::string filterStr = "\n\x3STT";

  socket->setsockopt(ZMQ_SUBSCRIBE, filterStr.c_str(), filterStr.length());
}*/

template <typename T>
simple::Subscriber<T>::Subscriber(std::string port, zmq::context_t& context)
{
  /// Constructor: opens a socket of type ZMQ_SUB and connects it to the port. Context shall be provided
  /// Sets up a filter for the subscription depending on the template type of the instance

  socket = std::make_unique<zmq::socket_t>(context, ZMQ_SUB);

  try
  {
    socket->connect(port);
  }
  catch (zmq::error_t& e)
  {
    std::cout << "could not connect to socket:" << e.what();
  }

  // filter the type of messages this subscriber will receive. Filter type depends on the templated instance
  filterSubscription();
}

template <typename T>
simple::Subscriber<T>::~Subscriber()
{
  // close the socket and destroy the context
  socket->close();
}


