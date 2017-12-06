#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>

#include "simple_msgs/generic_message.h"
#include "simple/context_deleter.h"

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
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_REQ))
    , callback_(callback)
  {
    try
    {
      socket_->connect(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }
  }

  ~Client<T>() { socket_->close(); }

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: Flatbuffer-type message to be sent as request.
   */
  void request(const flatbuffers::FlatBufferBuilder& msg)
  {
    uint8_t* buffer = msg.GetBufferPointer();  //< Get the data from the message.
    int buffer_size = msg.GetSize();
    request(buffer, buffer_size);
  }

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void request(const simple_msgs::GenericMessage& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
    request(buffer, buffer_size);
  }

  /**
   * @brief Sends the request to a server and waits for an answer.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be published.
   */
  void request(const uint8_t* msg, int size)
  {
    zmq::message_t ZMQ_message(size);
    memcpy(ZMQ_message.data(), msg, size);  //< Data into the ZMQ message.

    try
    {
      socket_->send(ZMQ_message);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not send the request to the server: " << e.what();
    }
    zmq::message_t reply;  //< Wait for a reply.

    try
    {
      socket_->recv(&reply);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not receive a response from the server: " << e.what();
    }

    T reply_data(static_cast<uint8_t*>(reply.data()));  //< Build a T object from the server reply.
    callback_(reply_data);
  }

private:
  static std::unique_ptr<zmq::context_t, contextDeleter> context_;
  std::unique_ptr<zmq::socket_t> socket_;
  std::function<void(const T&)> callback_;
};

template <typename T>
std::unique_ptr<zmq::context_t, contextDeleter> Client<T>::context_(new zmq::context_t(1));
}  // Namespace simple.
