/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <thread>

#include "simple/client.hpp"
#include "simple_msgs/pose.h"

int main() {
  const int N_RUN{25};
  const int SLEEP_TIME{1000};  //!  Milliseconds.

  // Create a pose message
  simple_msgs::Pose pose({5.0, 6.0, 7.0}, {1.0, 2.0, 3.0, 4.0});
  // Create a Client that will send request to a Server on "localhost" and on port "5555".
  simple::Client<simple_msgs::Pose> client{"tcp://localhost:5555"};

  // Send a request every SLEEP_TIME milliseconds for N_RUN times.
  // The request is a Pose message, the reply is the modified Pose message.
  // The message is modified by the Server that listens on localhost:5555 accordingly to its callback function.
  for (auto i = 0; i < N_RUN; ++i) {
    std::cout << "Sending: \n" << pose << std::endl;
    if (client.request(pose)) {
      std::cout << "Receiving: \n" << pose << std::endl;
    } else {
      std::cerr << "Request to the server failed." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  std::cout << "Requesting ended." << std::endl;
  return 0;
}
