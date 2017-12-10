#include <iostream>
#include <string>
#include "simple/subscriber.hpp"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"

void dummy_callback(const simple_msgs::Point& p)
{
  std::cout << p << std::endl;
}

int main(int argc, char* argv[])
{
  std::cout << "Creating a subscriber for Point messages." << std::endl;
  simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", dummy_callback);

  // wait for 5 seconds
  std::this_thread::sleep_for(std::chrono::seconds(20));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
