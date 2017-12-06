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

  // create a publisher
  simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
  std::cout << "Publish these cordinates: x=5.0, y=6.0, z=7.0" << std::endl;
  int num = 10;
  while (num > 0)
  {
    try
    {  // send the message continously
      pub.publish(p);
      std::cout << "Point Message published: " << num << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    catch (zmq::error_t& e)
    {
      std::cout << "Something went wrong with the publishing..." << std::endl;
    }
    num--;
  }
  std::cout << "Publishing ended" << std::endl;
  return 0;
}
