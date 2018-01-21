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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <ctime>

#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "test_utils.hpp"

// Test: connecting a subscriber to a wrong type of publisher.

SCENARIO("Publish a Pose and subscribe to a Point message")
{
  GIVEN("An instance of a subscriber")
  {
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunctionConstPoint);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
    REQUIRE(num_received_point == 0);
  }
}
