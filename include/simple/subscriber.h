#pragma once

#include <functional>
#include <memory>
#include <zmq.hpp>
#include <string>
#include <thread>

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
   * should be a wrapper message type
   * @param port string for the connection port.
   * @param context reference to the existing context.
   */
  Subscriber<T>(const std::string& port, const std::function<void(const T&)>& callback)
    : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_SUB)), callback_(callback)
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
    t_ = std::thread(&Subscriber::subscribe, this);
  }

  ~Subscriber<T>()
  {
    socket_->close();
    context_->close();

    // stop the subscription loop
    alive_ = false;

    // join thread
    t_.join();
  }

  /**
   * @brief publishes the message through the open socket.
   */
  void subscribe()
  {
    // while the subscriber is still alive
    while (alive_)
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

      auto bah = static_cast<uint8_t*>(ZMQmsg.data());

      T wrappedData(bah);

      callback_(wrappedData);
    }
  }

private:
  void filter()
  {
    // No topic for now
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }

  std::thread t_;
  bool alive_{ true };
  static std::unique_ptr<zmq::context_t> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
  std::function<void(const T&)> callback_;
};

template <typename T>
std::unique_ptr<zmq::context_t> Subscriber<T>::context_ = std::make_unique<zmq::context_t>(1);

}  // namespace simple
