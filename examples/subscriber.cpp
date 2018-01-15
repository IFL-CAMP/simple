/**
* S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
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

#include "simple/subscriber.hpp"
#include "simple_msgs/pose_stamped.h"

void example_callback(const simple_msgs::PoseStamped& p)
{
  std::cout << p << std::endl;  //< Just print the content of the message.
}

int main()
{
  const int SLEEP_TIME = 60000;  //< Milliseconds.

  std::cout << "Creating a subscriber." << std::endl;
  simple::Subscriber<simple_msgs::PoseStamped> subscriber("tcp://localhost:5555", example_callback);

  // Run this thread for 60 seconds
  std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
