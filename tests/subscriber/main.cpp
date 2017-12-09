#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/subscriber.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"

void dummy_callback(const simple_msgs::Point& p)
{
  std::cout << "Received point: " << std::endl;
  auto vec = p.toVector();
  std::cout << "X: " << vec.at(0) << std::endl;
  std::cout << "Y: " << vec.at(1) << std::endl;
  std::cout << "Z: " << vec.at(2) << std::endl;
}

int main(int argc, char* argv[])
{
  std::cout << "Creating a subscriber for Point messages." << std::endl;
  simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", dummy_callback);

  // wait for 5 seconds
  std::this_thread::sleep_for(std::chrono::seconds(10));

  std::cout << "Leaving main scope" << std::endl;
}
