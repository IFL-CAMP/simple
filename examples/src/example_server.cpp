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

#include "simple/server.hpp"
#include "simple_msgs/pose.pb.h"

// Callback function for a Server object.
// Whenever the Server receives a request (a Pose message), it is elaborated by this function.
// In this trivial case, all elements of the Pose translation part will be increased by 1.
void exampleCallback(simple_msgs::Pose& p) {
  std::cout << "Received a pose. Adding 1 to all its elements." << std::endl;
  p.mutable_point()->set_x(p.point().x() + 1);
  p.mutable_point()->set_y(p.point().y() + 1);
  p.mutable_point()->set_z(p.point().z() + 1);
  p.mutable_quaternion()->set_x(p.quaternion().x() + 1);
  p.mutable_quaternion()->set_y(p.quaternion().y() + 1);
  p.mutable_quaternion()->set_z(p.quaternion().z() + 1);
  p.mutable_quaternion()->set_w(p.quaternion().w() + 1);
}

int main() {
  constexpr int kSleepTime{60000};             //! Milliseconds.
  const std::string kAddress{"tcp://*:5555"};  //! Listening on port 5555 for requests from any IP address.

  std::cout << "Creating a server." << std::endl;
  simple::Server<simple_msgs::Pose> server{kAddress, exampleCallback};

  // Sleep for kSleepTime. The Service callback is called asynchronously.
  std::this_thread::sleep_for(std::chrono::milliseconds(kSleepTime));

  std::cout << "Quitting..." << std::endl;
}
