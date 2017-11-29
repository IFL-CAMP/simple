#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include "simple/subscriber.h"
#include "simple_msgs/generic_message.h"
#include "simple_msgs/header.h"

// std::function<int(simple_msgs::Header)> callback = dummyFun;

void dummy(const simple_msgs::Header& h)
{
  std::cout << "Test" << std::endl;
  std::cout << h.getSequenceNumber() << std::endl;
  std::cout << h.getFrameID() << std::endl;
  std::cout << h.getTimestamp() << std::endl;
}

int main(int argc, char* argv[])
{
  simple::Subscriber<simple_msgs::Header> sub("tcp://localhost:5555", dummy);

  // wait for 15 seconds
  std::this_thread::sleep_for(std::chrono::seconds(15));

  std::cout << "Leaving main scope" << std::endl;
}
