#include <iostream>
#include <thread>

#include "simple/publisher.hpp"
#include "simple_msgs/pose.h"

int main()
{
  const int N_RUN = 25;
  const int SLEEP_TIME = 2000;  //<  Milliseconds.

  simple_msgs::Point my_point(1.0, 2.0, 3.0);
  simple_msgs::Quaternion my_quaternion;  //< Identity quaternion.

  simple_msgs::Pose my_pose(my_point, my_quaternion);              //< 1
  simple::Publisher<simple_msgs::Pose> publisher("tcp://*:5555");  //< 2

  std::cout << "Starting publishing " << N_RUN << " messages." << std::endl;

  for (auto i = 0; i < N_RUN; i++)
  {
    publisher.publish(my_pose);  //< 3

    // Modify the pose at each iteration.
    my_point += 1.0;
    my_pose.setPosition(my_point);

    std::cout << "Message #" << (i + 1) << " has been published." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
