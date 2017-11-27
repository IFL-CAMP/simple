#include <iostream>

#include "publisher.h"

simple::Publisher::Publisher(const std::string& port, zmq::context_t& context_)
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

simple::Publisher::~Publisher()
{
  socket_->close();
}

void simple::Publisher::publish(const uint8_t* msg, const int size)
{
  // TODO: add message topic to allow subscription filter
  const char* topic = flatbuffers::GetBufferIdentifier(msg);

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
