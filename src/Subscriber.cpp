#include <iostream>

#include "subscriber.h"

simple::Subscriber::Subscriber(const std::string& port)
{
  socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_PUB);
  try
  {
    socket_->bind(port);
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Error - Could not bind to the socket:" << e.what();
  }
}

simple::Subscriber::~Subscriber()
{
  socket_->close();
}
void simple::Subscriber::subscribe(simple_msgs::GenericMessage& msgRef)
{

	zmq::message_t ZMQmessage;

	try
	{
		socket_->recv(&ZMQmessage);  // receive messages that fit the filter of the socket
	}
	catch (zmq::error_t& e)
	{
		std::cerr << "Could not receive message: " << e.what();
	}

	// put the received data into the buffer
	msgRef = flatbuffers::GetRoot(ZMQmessage.data());
}
void simple::Subscriber::filterSubscription(){
	//No topic for now
	socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
}
