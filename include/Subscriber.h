#pragma once

#define ZMQ_STATIC  // TODO: is this necessary now?

#include <memory>
#include <zmq.hpp>
#include <string>
#include "generic_message.h"

namespace simple
{
/**
 * @brief Creates a subscriber socket.
 */
class Subscriber
{
public:
  /**
   * @brief Class constructor. Creates a ZMQ_SUB socket and connects it to the port.
   * @param port string for the connection port.
   * @param context reference to the existing context.
   */
	Subscriber(const std::string& port, zmq::context_t& context);

  ~Subscriber();

  /**
   * @brief publishes the message through the open socket.
   * @return TODO
   */
  void subscribe(simple_msgs::GenericMessage&);

private:
	void filterSubscription();
  std::unique_ptr<zmq::socket_t> socket_;  //<
};

}  // namespace simple
