// Test how the client works

#include <iostream>
#include <thread>
#include "simple/client.hpp"
#include <string>
#include "simple_msgs/point.h"
#include "simple_msgs/bool.h"

int main(int argc, char* argv[])
{
  // create a message, with wrapper
  simple_msgs::Point p(5.0, 6.0, 7.0);

  simple_msgs::Bool b;

  // create a client
  simple::Client<simple_msgs::Point> client("tcp://localhost:5555");
  std::cout << "Send: x = 5.0, y = 6.0, z = 7.0. Expect: x = 6.0, y = 7.0, z = 8.0" << std::endl;

  //try copy-constructing a new client
  simple::Client<simple_msgs::Point> client2(client);

  for (auto i = 0; i < 10; ++i)
  {
    std::cout << "Requesting point #" << i << "." << std::endl;
    if (client2.request(p))
    {
      std::cout << b << std::endl;
    }
    else
    {
      std::cerr << "Request failed." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "Requesting ended" << std::endl;
  return 0;
}
