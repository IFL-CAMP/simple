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

#include "simple/client.hpp"
#include "simple_msgs/pose.pb.h"

void printPose(const simple_msgs::Pose& pose) {
  std::cout << "(" << pose.point().x() << ", " << pose.point().y() << ", " << pose.point().z() << ") "
            << "(" << pose.quaternion().x() << ", " << pose.quaternion().y() << ", " << pose.quaternion().z() << ", "
            << pose.quaternion().w() << ")" << std::endl;
}

int main() {
  constexpr int kNumberRuns{25};
  constexpr int kSleepTimeMilliseconds{1000};  //!  Milliseconds.
  const std::string kAddress{"tcp://localhost:5555"};

  // Create a pose message
  simple_msgs::Pose pose;
  pose.mutable_point()->set_x(5.0);
  pose.mutable_point()->set_y(6.0);
  pose.mutable_point()->set_z(7.0);
  pose.mutable_quaternion()->set_x(1.0);
  pose.mutable_quaternion()->set_y(2.0);
  pose.mutable_quaternion()->set_z(3.0);
  pose.mutable_quaternion()->set_w(4.0);

  // Create a Client that will send request to a Server on "localhost" and on port "5555".
  simple::Client<simple_msgs::Pose> client{kAddress};

  // Send a request every kSleepTime milliseconds for kNumberRuns times.
  // The request is a Pose message, the reply is the modified Pose message.
  // The message is modified by the Server that listens on localhost:5555 accordingly to its callback function.
  for (auto i = 0; i < kNumberRuns; ++i) {
    std::cout << "Sending Pose: \n";
    printPose(pose);
    if (client.request(pose)) {
      std::cout << "Receiving Pose: \n";
      printPose(pose);
    } else {
      std::cerr << "Request to the server failed." << std::endl;
    }
    std::cout << "##########################################" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(kSleepTimeMilliseconds));
  }

  std::cout << "Quitting..." << std::endl;
  return 0;
}
