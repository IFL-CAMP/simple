// Test how the publisher works

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include "simple/publisher.hpp"
#include <signal.h>
#include <string>
#include <thread>

#include "simple_msgs/pose.h"
#include "simple_msgs/image.h"
#include "simple_msgs/header.h"



int main()
{
  
	simple_msgs::Image<uint8_t> img;

  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");

  for (int i = 0; i < 10; i++)
  {
    pub.publish(img);
    std::cout << "Message #" << i << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
