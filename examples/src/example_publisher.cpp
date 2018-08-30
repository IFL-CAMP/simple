/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <thread>

#include "simple/publisher.hpp"
#include "simple_msgs/pose_stamped.h"

// Helper function to get the current time as a double.
long long getTimeNow() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
}

int main() {
  const int N_RUN{3000};
  const int SLEEP_TIME{1};  //!  Milliseconds.
  int sequence_num{1};

  auto beginning = getTimeNow();  //! Time at the start of this program.

  simple_msgs::Header my_header{sequence_num, "root", 0};  //! Build a Header.
  simple_msgs::Point my_point{1.0, 2.0, 3.0};              //! Build a Point.
  simple_msgs::Quaternion my_quaternion{};                 //! Identity Quaternion.

  // Create a PoseStamped message from the Header, Point and Quaternion.
  simple_msgs::PoseStamped my_pose_stamped{my_header, {my_point, my_quaternion}};
  // Create a Publisher, it will send messages to any Subscriber listening on port 5555 from any IP address.
  simple::Publisher<simple_msgs::PoseStamped> publisher{"tcp://*:5555"};

  std::cout << "Starting publishing " << N_RUN << " messages." << std::endl;

  // Publishing message for N_RUN times every SLEEP_TIME milliseconds.
  for (; sequence_num <= N_RUN; ++sequence_num) {
    publisher.publish(my_pose_stamped);  // Publish the current PoseStamped message.

    // Modify the pose at each iteration.
    my_point += 1.0;  //! The translation part of the Pose is modified.
    my_pose_stamped.getPose().setPosition(my_point);
    my_pose_stamped.getHeader().setSequenceNumber(sequence_num + 1);
    my_pose_stamped.getHeader().setTimestamp(getTimeNow() - beginning);

    std::cout << "Time: " << my_pose_stamped.getHeader().getTimestamp() << std::endl;
    std::cout << "Message #" << (my_pose_stamped.getHeader().getSequenceNumber()) << " has been published."
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
