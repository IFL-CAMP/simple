// Test how the client works

#include <iostream>
#include "simple/client.h"
#include <signal.h>
#include <string>
#include "simple_msgs/header.h"
#include <thread>
#include "simple_msgs\image.h"
#include "simple_msgs\pose.h"

// handle interruptions
static int s_interrupted = 0;
static void s_signal_handler(int signal_value)
{
  s_interrupted = 1;
}

static void s_catch_signals()
{
  signal(SIGINT, s_signal_handler);
  signal(SIGTERM, s_signal_handler);
}

void dummy(const simple_msgs::Point& p)//get the point and read the values inside
{
	std::cout << "Received point: " << std::endl;
	std::vector<double> vec = p.getPoint();
	std::cout << "new X: " << vec.at(0) << std::endl;
	std::cout << "new Y: " << vec.at(1) << std::endl;
	std::cout << "new Z: " << vec.at(2) << std::endl;
}

int main(int argc, char* argv[])
{
  

  // create a message, with wrapper
  simple_msgs::Point p(5.0, 6.0, 7.0);
  simple_msgs::Header(1, "ID", 1.2);

  // create a client
  simple::Client<simple_msgs::Point> client("tcp://localhost:5555",dummy);
  std::cout << "Request these cordinates: x=5.0, y=6.0, z=7.0" << std::endl;
  // s_catch_signals();
  int num = 10;
  while (num > 0)
  {
    try
    {  // send the request num times
      client.request(p);
      std::cout << "Point requested: " << num << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    catch (zmq::error_t& e)
    {
      std::cout << "Something went wrong with the request..." << std::endl;
    }
    num--;
  }
  std::cout << "Requesting ended" << std::endl;
  return 0;
}
