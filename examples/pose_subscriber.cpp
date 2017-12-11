#include <iostream>

#include "simple/subscriber.hpp"
#include "simple_msgs/pose.h"

void example_callback(const simple_msgs::Pose& p)
{
  std::cout << p << std::endl;
}

int main()
{
  std::cout << "Creating a subscriber." << std::endl;
  simple::Subscriber<simple_msgs::Pose> subscriber("tcp://localhost:5555", example_callback);

  // Run this thread for 60 seconds
  std::this_thread::sleep_for(std::chrono::seconds(60));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
