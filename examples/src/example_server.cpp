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
#include "simple_msgs/pose.hpp"

// Callback function for a Server object.
// Whenever the Server receives a request (a Pose message), it is elaborated by this function.
// In this trivial case, all elements of the Pose translation part will be increased by 1.
void example_callback(simple_msgs::Pose& p) {
  std::cout << "Received a point. Adding 1 to its elements." << std::endl;
  p.getPosition() += 1.0;
}

int main() {
  const int SLEEP_TIME{60000};  //! Milliseconds.

  // A Server listening on port 5555 for requests from any IP address.
  std::cout << "Creating a server." << std::endl;
  simple::Server<simple_msgs::Pose> server{"tcp://*:5555", example_callback};

  // Wait for 60 seconds. The Service callback is called asynchronously.
  std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

  std::cout << "Closing the server." << std::endl;
}
