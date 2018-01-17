#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "test_utils.hpp"
#include "simple/publisher.hpp"
#include "simple_msgs/pose.h"
#include "simple/subscriber.hpp"

// TEST ONE SUBSCRIBER CONNECTING AFTER SOME MESSAGES WERE PUBLISHED

SCENARIO("Have a publisher and a subscriber that connects late.")
{
  GIVEN("An instance of a publisher publishing data.")
  {
    // start a publisher
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");

    for (int i = 0; i < 20; ++i)
    {
      auto p = createRandomPose();
      pub.publish(p);
    }

    WHEN("The subscriber is connected.")
    {
      // start subscriber
      simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5555", callbackFunctionConstPose);
      // wait so the subscribers get every message
      std::this_thread::sleep_for(std::chrono::seconds(5));

      THEN("The subscriber will receive messages sent from now on")
      {
        for (int i = 0; i < 20; ++i)
        {
          auto p = createRandomPose();
          pub.publish(p);
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          REQUIRE(received_pose == p);
        }
        REQUIRE(num_receives_pose == 20);
      }
    }
  }
}
