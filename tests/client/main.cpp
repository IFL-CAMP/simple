// Test how the client works

#include <iostream>
#include "simple/client.h"
#include <string>
#include "simple_msgs/point.h"

int main(int argc, char* argv[])
{
  // create a message, with wrapper
  simple_msgs::Point p(5.0, 6.0, 7.0);

  // create a client
  simple::Client<simple_msgs::Point> client("tcp://localhost:5555");
  std::cout << "Send: x = 5.0, y = 6.0, z = 7.0. Expect: x = 6.0, y = 7.0, z = 8.0" << std::endl;

  for (auto i = 0; i < 10; ++i)
  {
    std::cout << "Requesting point #" << i << "." << std::endl;
    auto response = client.request(p);
    std::cout << response << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "Requesting ended" << std::endl;
  return 0;
}
