#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>
#include "simple/contextCloser.h"

namespace simple
{
/**
 * @brief Creates a subscriber socket.
 */
template <typename T>
class Subscriber
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_SUB socket and connects it to the port. The type of the subscriber
   * should be a wrapper message type. Opens a dedicated thread for receiving messages.
   * @param port string for the connection port.
   * @param callback user defined callback function to be called for every data received through the socket.
   */
  Subscriber<T>(const std::string& port, const std::function<void(const T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_SUB))
    , callback_(callback)
  {
    // set socket timeout
    int timeOut = 5000;  // miliseconds
    socket_->setsockopt(ZMQ_RCVTIMEO, &timeOut, sizeof(timeOut));
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
    t_ = std::thread(&Subscriber::subscribe, this);
  }

  ~Subscriber<T>()
  {
    // stop the subscription loop
    alive_ = false;

    // join thread
    t_.join();

    socket_->close();
  }

  /**
   * @brief Continuously waits for a message to be published in the connected port, while the subscriber is alive.
   * Creates an instance of a wrapper with the received data matching the instance type and calls the callback function
   * supplied by the user in the creation of the subscriber with the received data as parameter.
   */
  void subscribe()
  {
    // while the subscriber is still alive
    while (alive_)
    {
      // start a ZMQ message to receive the data
      zmq::message_t ZMQmsg;
      bool success = false;
      try
      {
        success = socket_->recv(&ZMQmsg);  // receive messages that fit the filter of the socket
      }
      catch (zmq::error_t& e)
      {
        std::cerr << "Could not receive message: " << e.what();
      }
      // if receive was successful
      if (success)
      {
        // get the buffer data ignoring the first few bytes (the topic prefix)
        const char* topic = T::getTopic();
        int s = strlen(topic);

        auto convertMsg = static_cast<uint8_t*>(ZMQmsg.data());

        T wrappedData(convertMsg + s);

        callback_(wrappedData);
      }
    }
  }

private:
  /**
   * @brief Set the socket option to match the identifier of the data type. Only messages prefixed with this identifier
   * will be received.
   */
  void filter()
  {
    // get topic from the wrapper
    const char* topic = T::getTopic();
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }

  std::thread t_;
  bool alive_{true};
  static std::unique_ptr<zmq::context_t, contextCloser> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
  std::function<void(const T&)> callback_;
};

template <typename T>
std::unique_ptr<zmq::context_t, contextCloser> Subscriber<T>::context_(new zmq::context_t(1));

}  // namespace simple
