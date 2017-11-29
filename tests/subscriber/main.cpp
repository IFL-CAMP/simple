#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/subscriber.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"

// define a dummy callback function
int dummyFun(simple_msgs::Header h)
{
  // std::cout << "Callback!! Reading header: " << h.getFrameID() << std::endl;
  return 1;
}

// std::function<int(simple_msgs::Header)> callback = dummyFun;

int my_func(int a, int b)
{
  std::cout << a + b << std::endl;
  return a + b;
}

std::function<int(int, int)> f(my_func);

int main(int argc, char* argv[])
{
  simple::Subscriber<simple_msgs::HeaderFbs> sub2("tcp://localhost:5555", my_func);

  // create the subscriber: this should start a new thread that get the messages
  // simple::Subscriber<simple_msgs::HeaderFbs> sub("tcp://localhost:5555", my_func);

  // wait for 15 seconds
  std::this_thread::sleep_for(std::chrono::seconds(15));

  std::cout << "Leaving main scope" << std::endl;
}
