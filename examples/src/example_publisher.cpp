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
#include "simple_msgs/point_stamped.pb.h"

// Helper function to get the current time in nanoseconds as a double.
long long getTimeNow() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
}

int main() {
  constexpr int kNumberRuns{3000};
  constexpr int kSleepTimeMilliseconds{100};  //!  Milliseconds.
  const std::string kAddress{"tcp://*:5555"};

  simple_msgs::PointStamped my_point;
  my_point.mutable_point()->set_x(1);
  my_point.mutable_point()->set_y(2);
  my_point.mutable_point()->set_z(3);
  my_point.mutable_header()->set_timestamp(getTimeNow());
  my_point.mutable_header()->set_id(1);
  my_point.mutable_header()->set_frame("");

  // Create a Publisher, it will send messages to any Subscriber listening on port 5555 from any IP address.
  simple::Publisher<simple_msgs::PointStamped> publisher{kAddress};

  std::cout << "Starting publishing " << kNumberRuns << " messages." << std::endl;

  // Publishing message for kNumberRuns times every kSleepTime milliseconds.
  for (size_t i = 1; i <= kNumberRuns; ++i) {
    // Publish the current PoseStamped message. Quit on failure.
    if (!publisher.publish(my_point)) {
      std::cerr << "Error while publishing a message! " << std::endl;
      break;
    }

    // Modify the pose at each iteration.
    my_point.mutable_point()->set_x(my_point.point().x() + 1);
    my_point.mutable_point()->set_y(my_point.point().y() + 1);
    my_point.mutable_point()->set_z(my_point.point().z() + 1);
    my_point.mutable_header()->set_timestamp(getTimeNow());
    my_point.mutable_header()->set_id(i);

    std::cout << "Message #" << i << " has been published." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(kSleepTimeMilliseconds));
  }

  std::cout << "Quitting..." << std::endl;
  return 0;
}
