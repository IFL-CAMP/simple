#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "simple_msgs/generic_message.h"
#include "simple\contextCloser.h"
#include <thread>

namespace simple
{
/**
 * @brief Creates a client socket for a specific type of message.
 */
template <typename T>
class Client
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_REQ socket and connects it to the port. The user defined callback function
   * is responsible for using the received reply the way it wants
   * @param port string for the connection port.
   * @param callback user defined callback function to be called for every reply received to a request.
   */
  Client<T>(const std::string& port, const std::function<void(const T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_REQ)), callback_(callback)
  {
	  try
	  {
		  socket_->connect(port);
	  }
	  catch (zmq::error_t& e)
	  {
		  std::cout << "could not bind socket:" << e.what();
	  }

  }

  ~Client<T>(){
	  socket_->close();
  }

  /**
   * @brief TODO
   * @param msg TODO
   */
  void request(const flatbuffers::FlatBufferBuilder& msg){
	  // Get the data from the message.
	  uint8_t* buffer = msg.GetBufferPointer();
	  int buffer_size = msg.GetSize();
	  request(buffer, buffer_size);
  }

  void request(const simple_msgs::GenericMessage<T>& msg){
	  uint8_t* buffer = msg.getBufferData();
	  int buffer_size = msg.getBufferSize();
	  request(buffer, buffer_size);
  }

  void request(const uint8_t* msg, const int size){
	  zmq::message_t ZMQ_message(size);
	  // put the data into the ZMQ message
	  memcpy(ZMQ_message.data(), msg, size);

	  try
	  {
		  socket_->send(ZMQ_message);
	  }
	  catch (zmq::error_t& e)
	  {
		  std::cerr << "Error - Could not send the message: " << e.what();
	  }
	  //wait for reply
	  zmq::message_t MSGreply;

	  try
	  {
		  socket_->recv(&MSGreply);
	  }
	  catch (zmq::error_t& e)
	  {
		  std::cout << "Could not receive message: " << e.what();
	  }

	  auto convertMsg = static_cast<uint8_t*>(MSGreply.data());

	  T wrappedData(convertMsg);

	  callback_(wrappedData);
  }

private:
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;
  std::function<void(const T&)> callback_;
};
template <typename T>
std::unique_ptr<zmq::context_t, contextCloser> Client<T>::context_(new zmq::context_t(1));
}  // namespace simple
