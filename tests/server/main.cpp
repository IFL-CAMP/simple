#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/server.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"

// std::function<int(simple_msgs::Header)> callback = dummyFun;

void dummy(simple_msgs::Point& p)//get the point and add (1,1,1) to it
{
  std::cout << "Received point: " << std::endl;
  std::vector<double> vec = p.getPoint();
  vec.at(0) = vec.at(0) + 1.0;
  vec.at(1) = vec.at(1) + 1.0;
  vec.at(2) = vec.at(2) + 1.0;
  std::cout << "new X: " << vec.at(0) << std::endl;
  std::cout << "new Y: " << vec.at(1) << std::endl;
  std::cout << "new Z: " << vec.at(2) << std::endl;
  p.setPoint(vec);
}

int main(int argc, char* argv[])
{
	std::cout << "Creating server for Point" << std::endl;
  simple::Server<simple_msgs::Point> sub("tcp://localhost:5555", dummy);
  //simple::Subscriber<simple_msgs::Header> sub2("tcp://localhost:5555", dummy2);

  // wait for 25 seconds
  std::this_thread::sleep_for(std::chrono::seconds(25));

  std::cout << "Leaving main scope" << std::endl;
}
