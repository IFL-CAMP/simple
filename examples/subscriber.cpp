#include <iostream>

#include "simple/subscriber.hpp"
#include "simple_msgs/pose_stamped.h"

void example_callback(const simple_msgs::PoseStamped& p)
{
  std::cout << p << std::endl;  //< Just print the content of the message.
}

int main()
{
  const int SLEEP_TIME = 60000;  //< Milliseconds.

  std::cout << "Creating a subscriber." << std::endl;
  simple::Subscriber<simple_msgs::PoseStamped> subscriber("tcp://localhost:5555", example_callback);

  // Run this thread for 60 seconds
  std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
