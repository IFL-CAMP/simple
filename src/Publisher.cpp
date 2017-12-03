#include <iostream>

#include "simple/publisher.h"
#include "simple_msgs/header.h"

std::unique_ptr<zmq::context_t, contextCloser> simple::Publisher::context_(new zmq::context_t(1));

simple::Publisher::Publisher(const std::string& port) : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_PUB))
{
  try
  {
    socket_->bind(port);
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Error - Could not bind to the socket:" << e.what();
  }
}

simple::Publisher::~Publisher()
{
  socket_->close();
  context_->close();
}

void simple::Publisher::publish(const uint8_t* msg, const int size)
{
  // get message topic to allow subscription filter
  //const char* topic = flatbuffers::GetBufferIdentifier(msg);
  //get the topic size
  //int s = strlen(topic);
  //create new array
  //uint8_t* prefixedMsg;
  //copy the topic and the buffer into the new array
  //memcpy(prefixedMsg, topic, s);
  //memcpy(prefixedMsg + s, msg, size);
  //create ZMQ message of size (buffer + prefixed topic)
  //zmq::message_t ZMQ_message(size+s);
	//put the data into the ZMQ message
  //memcpy(ZMQ_message.data(), prefixedMsg, size+s);

	//temporary publishing without topic, for demo
	zmq::message_t ZMQ_message(size);
	memcpy(ZMQ_message.data(), msg, size);

  try
  {
    socket_->send(ZMQ_message);
  }
  catch (zmq::error_t& e)
  {
    std::cerr << "Error - Could not send the message: " << e.what();
  }
}

void simple::Publisher::publish(const simple_msgs::GenericMessage& msg)
{
  uint8_t* buffer = msg.getBufferData();
  int buffer_size = msg.getBufferSize();
  publish(buffer, buffer_size);
}

void simple::Publisher::publish(const flatbuffers::FlatBufferBuilder& msg)
{
  // Get the data from the message.
  uint8_t* buffer = msg.GetBufferPointer();
  int buffer_size = msg.GetSize();
  publish(buffer, buffer_size);
}
