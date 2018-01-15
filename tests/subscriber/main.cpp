#include <iostream>
#include <string>

#include "simple/subscriber.hpp"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"
#include "simple_msgs/image.h"

void dummy_callback(const simple_msgs::Image<uint8_t>& i)
{
  auto h = i.getHeader();
  std::cout << h << std::endl;
  auto p = i.getImageOrigin();
  std::cout << p << std::endl;
  auto img = i.getImageData();
  auto size = i.getImageSize();
  auto dimensions = i.getImageDimensions();

}

int main(int argc, char* argv[])
{
  std::cout << "Creating a subscriber for Image messages." << std::endl;
  simple::Subscriber<simple_msgs::Image<uint8_t>> sub("tcp://localhost:5555", dummy_callback);

  // wait for 5 seconds
  std::this_thread::sleep_for(std::chrono::seconds(20));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
