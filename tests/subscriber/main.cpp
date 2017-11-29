#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/subscriber.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"

// define a dummy callback function
void dummyFun(simple_msgs::Header h)
{
  std::cout << "Callback!! Reading header: " << h.getFrameID() << std::endl;
}
std::function<void(simple_msgs::Header)> callback = &dummyFun;

int main(int argc, char* argv[])
{
  // create the subscriber: this should start a new thread that get the messages
  simple::Subscriber<simple_msgs::HeaderFbs> sub("tcp://localhost:5555", callback);

  // wait for 15 seconds
  std::this_thread::sleep_for(std::chrono::seconds(15));

  std::cout << "Leaving main scope" << std::endl;
}
