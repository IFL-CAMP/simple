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

#include "simple/subscriber.hpp"
#include "simple_msgs/point_stamped.pb.h"

int main() {
  constexpr int kSleepTime{60};  //! Seconds.
  const std::string kAddress{"tcp://localhost:5555"};

  // A Subscriber listening on port 5555 to messages coming from the IP address "localhost"
  std::cout << "Creating a subscriber." << std::endl;

  // clang-format off
  simple::Subscriber<simple_msgs::PointStamped> subscriber{kAddress,[](const simple_msgs::PointStamped& p) {
    std::cout << "[" << p.header().timestamp() << "] " << "Received point"
    " (" << p.point().x() << ", " << p.point().y() << ", " << p.point().z() << ")" << std::endl;
  }};
  // clang-format on

  // Run this thread for 60 seconds. The subscriber callback is called asynchronously.
  std::this_thread::sleep_for(std::chrono::seconds(kSleepTime));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
