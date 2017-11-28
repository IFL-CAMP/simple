#pragma once

#include <memory>
#include <zmq.hpp>
#include <string>
#include "generic_message.h"
#include "header_generated.h"

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
   * @brief Class constructor. Creates a ZMQ_SUB socket and connects it to the port.
   * @param port string for the connection port.
   * @param context reference to the existing context.
   */
  Subscriber<T>(const std::string& port) : socket_(std::make_unique<zmq::socket_t>(*context_, ZMQ_SUB))
  {
    try
    {
      socket_->connect(port);
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Error - Could not bind to the socket:" << e.what();
    }

    filter();
  }

  ~Subscriber<T>()
  {
    socket_->close();
    context_->close();
  }

  /**
   * @brief publishes the message through the open socket.
   * @return TODO
   */
  const std::shared_ptr<Message<T> > subscribe()
  {
    std::shared_ptr<Message<T> > msg = std::make_shared<Message<T> >();
    msg->m_zmqMessage = std::make_unique<zmq::message_t>();
    try
    {
      socket_->recv(msg->m_zmqMessage.get());  // receive messages that fit the filter of the socket
    }
    catch (zmq::error_t& e)
    {
      std::cerr << "Could not receive message: " << e.what();
    }

    // test the received data for correct type
    auto buf = simple_msgs::GetHeaderFbs(msg->m_zmqMessage->data());
    auto seq = buf->sequence_number();
    auto framr = buf->frame_id();
    auto time = buf->timestamp();

    // return the received data as buffer
    auto data = flatbuffers::GetRoot<T>(msg->m_zmqMessage->data());
    msg->m_data = data;
    auto seq2 = data->sequence_number();
    auto framr2 = data->frame_id();
    auto time2 = data->timestamp();

    // verify the consistency of the data
    flatbuffers::Verifier Ver((uint8_t*)(msg->m_zmqMessage->data()), msg->m_zmqMessage->size());

    auto check = Ver.VerifyTable(data);

    return msg;
  }

private:
  void filter()
  {
    // No topic for now
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }

  static std::unique_ptr<zmq::context_t> context_;
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

template <typename T>
std::unique_ptr<zmq::context_t> Subscriber<T>::context_ = std::make_unique<zmq::context_t>(1);

}  // namespace simple
