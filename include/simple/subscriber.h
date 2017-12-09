#pragma once

#include <functional>
#include <memory>
#include <zmq.h>
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
   * @brief Creates a ZMQ_SUB socket and connects it to the given address.
   * The given callback runs on a dedicated thread.s
   * @param Address to subscriber to, in the form: tcp://HOSTNAME:PORT. e.g tcp://localhost:5555.
   * @param User defined callback function for incoming messages.
   * @param Time the subscriber will block the thread waiting for a message. In milliseconds.
   */
  Subscriber<T>(const std::string& address, const std::function<void(const T&)>& callback, int timeout = 100)
    : socket_(zmq_socket(context_, ZMQ_SUB))
    , callback_(callback)
  {
    zmq_setsockopt(socket_, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));

    auto success = zmq_connect(socket_, address.c_str());
    if (success != 0)
    {
      throw std::runtime_error("Cannot bind to the given address/port. ZMQ Error: " + zmq_errno());
    }

    filter();  //< Filter the messages to be received accordingly to their identification tag.

    // Start the callback thread.
    subscriber_thread_ = std::thread(&Subscriber::subscribe, this);
  }

  ~Subscriber<T>()
  {
    alive_ = false;             //< Stop the subscription loop.
    subscriber_thread_.join();  //< Wait for the subscriber thead.
    zmq_close(socket_);         //< Close the socket.
    zmq_ctx_term(context_);     //< Terminate the context.
  }

private:
  /**
   * @brief Waits for a message to be published to the connected port,
   * Calls the user callback with an instance of T obtained by a publisher.
   */
  void subscribe()
  {
    while (alive_)
    {
      int data_past_topic;
      auto data_past_topic_size = sizeof(data_past_topic);

      // Messages are sent in two parts. The first one containing the message identifier.
      // The two parts are considered a single message by ZMQ, only the first part is evaluated for filtering.
      // Once the first part is received, we check if there is more to receive. That is the message content.

      do
      {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket_, 0);
        zmq_getsockopt(socket_, ZMQ_RCVMORE, &data_past_topic, &data_past_topic_size);
        if (data_past_topic)
        {
          auto success = zmq_msg_recv(&message, socket_, 0);
          if (success != -1)
          {
            T data(static_cast<uint8_t*>(zmq_msg_data(&message)));
            callback_(data);
          }
        }
        zmq_msg_close(&message);
      } while (data_past_topic);
    }
  }

  /**
   * @brief Filter the incoming messages according to the message identifier.s
   */
  void filter()
  {
    // Get the message identificator.
    const char* topic = T::getTopic();
    // Subscribe only to messages with the right identificator.
    zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, topic, strlen(topic));
  }

  std::thread subscriber_thread_;
  bool alive_{true};
  static void* context_;
  void* socket_;
  std::function<void(const T&)> callback_;
};

template <typename T>
void* Subscriber<T>::context_(zmq_ctx_new());

}  // Namespace simple.
