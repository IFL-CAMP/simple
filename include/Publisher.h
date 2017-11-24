#pragma once

#define ZMQ_STATIC

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>
#include "flatbuffers/flatbuffers.h"
#include <iostream>


namespace simple
{
///@brief Creates a publisher socket for a specific type of message
class Publisher
{
public:
  ///
  ///@brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port
  ///@param port string for the connection port. Ex: "tcp://*:5556"
  ///@param context reference to the existing context
  ///
  Publisher(std::string port, zmq::context_t& context);
  ///@brief Class destructor. Closes the existing socket
  ~Publisher();
  ///@brief publishes the message through the open socket
  ///@param msg Protobuf-type message to be published
  void publish(const flatbuffers::FlatBufferBuilder& msg);

private:
  std::unique_ptr<zmq::socket_t> socket;
};

}  // namespace simple


