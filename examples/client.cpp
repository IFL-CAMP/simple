#include <iostream>
#include <thread>

#include "simple/client.hpp"
#include "simple_msgs/pose.h"

int main()
{
  const int N_RUN = 25;
  const int SLEEP_TIME = 1000;  //<  Milliseconds.

  simple_msgs::Pose p(simple_msgs::Point(5.0, 6.0, 7.0),simple_msgs::Quaternion(1.0,2.0,3.0,4.0));  //< Create a point message.

  simple::Client<simple_msgs::Pose> client("tcp://localhost:5555");

  for (auto i = 0; i < N_RUN; ++i)
  {
    std::cout << "Sending: \n" << p << std::endl;
    if (client.request(p))
    {
      std::cout << "Receiving: \n" << p << std::endl;
    }
    else
    {
      std::cerr << "Request to the server failed." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  std::cout << "Requesting ended." << std::endl;
  return 0;
}
