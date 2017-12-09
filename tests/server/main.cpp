#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/server.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"

void dummy(simple_msgs::Point& p)  // get the point and add (1,1,1) to it
{
  std::cout << "Received point. " << std::endl;
  p += 1.0;
}

int main(int argc, char* argv[])
{
  std::cout << "Creating server for Point messages" << std::endl;
  simple::Server<simple_msgs::Point> server("tcp://*:5555", dummy);

  // wait for 25 seconds
  std::this_thread::sleep_for(std::chrono::seconds(25));

  std::cout << "Leaving main scope" << std::endl;
}
