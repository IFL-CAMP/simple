#include "Publisher.h"

void simple::Publisher::publish(const flatbuffers::FlatBufferBuilder& msg)
{
	/// Sends the message by the open socket of the publisher. Any type of message
	/// is supported (T is a flatbufferBuilder)

	//get the data from the message
	uint8_t* buf = msg.GetBufferPointer();

	// add message topic to allow subscription filter TODO

	const char* topic = flatbuffers::GetBufferIdentifier(buf);


	int buffersize = msg.GetSize();

	zmq::message_t ZMQmsg(buffersize);

	memcpy(ZMQmsg.data(), buf, buffersize);

	try
	{
		socket->send(ZMQmsg);
	}
	catch (zmq::error_t& e)
	{
		std::cerr << "Could not send message: " << e.what();
	}
}
simple::Publisher::Publisher(std::string port, zmq::context_t& context)
{
	/// Class constructor: opens a socket of type ZMQ_PUB and binds it to port

	socket = std::make_unique<zmq::socket_t>(context, ZMQ_PUB);
	try
	{
		socket->bind(port);
	}
	catch (zmq::error_t& e)
	{
		std::cerr << "could not bind to socket:" << e.what();
	}
}
simple::Publisher::~Publisher()
{
	/// Class destructor: Closes the socket
	socket->close();
}