#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>

#include "simple/context_deleter.h"

namespace simple
{
/**
 * @brief Creates a subscriber socket for a specific type of message.
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
    int timeout = 100;  //< Socket timeout in miliseconds.
    socket_->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    try
    {
      socket_->connect(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }
    filter();  //< Filter the messages to be received accordingly to their identification tag.

    // Start the thread for the subscription.
    subscriber_thread_ = std::thread(&Subscriber::subscribe, this);
  }

  ~Subscriber<T>()
  {
    alive_ = false;  //< Stop the subscription loop.
    subscriber_thread_.join();
    socket_->close();
  }

private:
  /**
   * @brief Continuously waits for a message to be published in the connected port, while the subscriber is alive.
   * Creates an instance of a wrapper with the received data matching the instance type and calls the callback function
   * supplied by the user in the creation of the subscriber with the received data as parameter.
   */
  void subscribe()
  {
    while (alive_)
    {
      zmq::message_t message;
      bool success = false;
      try
      {
        success = socket_->recv(&message);
      }
      catch (zmq::error_t& e)
      {
        std::cerr << "Error: Could not receive the message: " << e.what();
      }
      if (success)  //< If a message was successfully received.
      {
        // get the buffer data ignoring the first few bytes (the topic prefix)
        const char* topic = T::getTopic();
        int s = strlen(topic);
        auto message_data = static_cast<uint8_t*>(message.data());
        T wrapped_data(message_data + s);  //< TODO: ??
        callback_(wrapped_data);
      }
    }
  }

  /**
   * @brief Set the socket option to match the identifier of the data type. Only messages prefixed with this identifier
   * will be received.
   */
  void filter()
  {
    const char* topic = T::getTopic();          //< Get the message identificator.
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);  //< TODO? topic is not used?
  }

  std::thread subscriber_thread_;
  bool alive_{true};
  static std::unique_ptr<zmq::context_t, contextDeleter> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
  std::function<void(const T&)> callback_;
};

template <typename T>
std::unique_ptr<zmq::context_t, contextDeleter> Subscriber<T>::context_(new zmq::context_t(1));

}  // Namespace simple.
