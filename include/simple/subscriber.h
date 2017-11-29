#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "simple_msgs/generic_message.h"
#include "header_generated.h"
#include <thread>
#include "flatbuffers\reflection.h"

namespace simple
{
template <typename T>
class Message
{
  template <typename T>
  friend class Subscriber;

public:
  const T& operator*()
  {
    return *m_data;
  }

private:
  std::unique_ptr<zmq::message_t> m_zmqMessage;
  const T* m_data;
};

/**
 * @brief Creates a subscriber socket.
 */
template <typename T>
class Subscriber
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_SUB socket and connects it to the port. The type of the subscriber should be a wrapper message type
   * @param port string for the connection port.
   * @param context reference to the existing context.
   */
  Subscriber<T>(const std::string& port, std::function<void(simple_msgs::GenericMessage)> callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_SUB))
  {
    try
    {
      socket_->connect(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }
    // set the topic for the messages to be received
    filter();

    // start thread of subscription
    t_(subscribe, callback);
  }

  ~Subscriber<T>()
  {
    socket_->close();
    context_->close();

    // stop the subscription loop
    stop_ = true;

    // join thread
    t_.join();
  }

  /**
   * @brief publishes the message through the open socket.
   */
  void subscribe(std::function<void(simple_msgs::GenericMessage)> callback)
  {
    // while the subscriber is still alive
    while (!stop_)
    {
      // start a ZMQ message to receive the data
      zmq::message_t ZMQmsg;
      try
      {
        socket_->recv(&ZMQmsg);  // receive messages that fit the filter of the socket
      }
      catch (zmq::error_t& e)
      {
        std::cerr << "Could not receive message: " << e.what();
      }

      // TODO
      // return the received data as buffer

      auto data = flatbuffers::GetAnyRoot(ZMQmsg->data())->GetBufferPointer();  // data is a pointer to the table offset
      // wrap the received data into the correct wrapper type
	  T wrappedData(data);

      // call the callback function with the message wrapper
	  callback(wrappedData);
    }
  }

private:
  void filter()
  {
    // No topic for now
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }
  std::thread t_;
  bool stop_{ false };
  static std::unique_ptr<zmq::context_t> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

template <typename T>
std::unique_ptr<zmq::context_t> Subscriber<T>::context_ = std::make_unique<zmq::context_t>(1);

}  // namespace simple
