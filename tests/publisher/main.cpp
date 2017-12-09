// Test how the publisher works

#include <iostream>
#include "simple/publisher.h"
#include <signal.h>
#include <string>
#include <thread>
#include "simple_msgs/pose.h"

int main(int argc, char* argv[])
{
  // create a message, with wrapper
  simple_msgs::Point p(5.0, 6.0, 7.0);

  //  // create a publisher
  simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
  std::cout << "Publish these cordinates: x=5.0, y=6.0, z=7.0" << std::endl;

  for (auto i = 0; i < 10; ++i)
  {
    try
    {
      pub.publish(p);
      std::cout << "A Point message #" << i << " has been published. " << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    catch (zmq::error_t& e)
    {
      std::cout << "Something went wrong with the publisher: " << e.what() << std::endl;
    }
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
