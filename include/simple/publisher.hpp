#pragma once

#include <zmq.h>
#include <string>
#include <memory>
#include "context_deleter.hpp"
#include "simple/generic_socket.hpp"

namespace simple
{
/**
 * @brief Creates a publisher socket for a specific type of message.
 */
template <typename T>
class Publisher : public GenericSocket<T>
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_PUB socket and binds it to the port.
   * @param port string for the connection port.
   */
  Publisher<T>(const std::string& address)
    : GenericSocket<T>(zmq_socket(context_.get(), ZMQ_PUB))
  {
    GenericSocket<T>::bind(address);
  }

  ~Publisher<T>() {}
  /**
   * @brief Publishes the message through the open socket.
   * @param msg: SIMPLE class wrapper for Flatbuffer messages.
   */
  void publish(const T& msg)
  {
    uint8_t* buffer = msg.getBufferData();
    int buffer_size = msg.getBufferSize();
	std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer = msg.getBuilderPointer();
    publish(buffer, buffer_size, builder_pointer);
  }

  /**
   * @brief Publishes the message through the open socket.
   * @param msg: buffer containing the data to be published.
   * @param size: size of the buffer to be publish.
   */
  void publish(uint8_t* msg, const int msg_size, std::shared_ptr<flatbuffers::FlatBufferBuilder>* builder_pointer) { GenericSocket<T>::sendMsg(msg, msg_size, builder_pointer, "[Simple Publisher] - "); }
private:
  static std::shared_ptr<void> context_;
};

template <typename T>
std::shared_ptr<void> Publisher<T>::context_(zmq_ctx_new(), contextDeleter);
}  // Namespace simple.
