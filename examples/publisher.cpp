#include <iostream>
#include <thread>

#include "simple/publisher.hpp"
#include "simple_msgs/pose_stamped.h"

using namespace std::chrono;

double getTimeNow()
{
  auto now = system_clock::now();
  return time_point_cast<milliseconds>(now).time_since_epoch().count();
}

int main()
{
  const int N_RUN = 30000;
  const int SLEEP_TIME = 1;  //<  Milliseconds.
  int sequence_num = 1;

  auto beginning = getTimeNow();

  simple_msgs::Header my_header(sequence_num, "root", 0);
  simple_msgs::Point my_point(1.0, 2.0, 3.0);
  simple_msgs::Quaternion my_quaternion;  //< Identity quaternion.

  simple_msgs::PoseStamped my_pose_stamped(my_header, simple_msgs::Pose(my_point, my_quaternion));  //< 1
  simple::Publisher<simple_msgs::PoseStamped> publisher("tcp://*:5555");                            //< 2

  std::cout << "Starting publishing " << N_RUN << " messages." << std::endl;

  for (; sequence_num <= N_RUN; ++sequence_num)
  {
    publisher.publish(my_pose_stamped);  //< 3

    // Modify the pose at each iteration.
    my_point += 1.0;
    my_pose_stamped.getPose().setPosition(my_point);
    my_pose_stamped.getHeader().setSequenceNumber(sequence_num + 1);
    my_pose_stamped.getHeader().setTimestamp(getTimeNow() - beginning);

    std::cout << "Message timestamp#" << (my_pose_stamped.getHeader().getTimestamp()) << " has been published."
              << std::endl;
    std::this_thread::sleep_for(milliseconds(SLEEP_TIME));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
