#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "simple_msgs/pose.h"
#include <time.h>

#include "test_utils.hpp"

// TEST ONE PUBLISHER DISCONNECT AND RECONNECT

SCENARIO("Publish a pose and have many subscribers.")
{
  GIVEN("An instance of a publisher and 1 subscriber.")
  {
    // Start a publisher
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    // Start a subscriber
    simple::Subscriber<simple_msgs::Pose> sub01("tcp://localhost:5555", callbackFunctionConstPose);
    // Wait so that the subscriber get every message
    std::this_thread::sleep_for(std::chrono::seconds(5));

    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 20; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        REQUIRE(received_pose == p);
      }
      REQUIRE(num_receives_pose == 20);

      WHEN("The publisher is disconnected and then reconnected.")
      {
        pub.~Publisher();
        std::this_thread::sleep_for(std::chrono::seconds(5));
        simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        THEN("The subscriber will go back to receiving messages")
        {
          for (int i = 0; i < 20; ++i)
          {
            auto p = createRandomPose();
            pub.publish(p);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            REQUIRE(received_pose == p);
          }
          REQUIRE(num_receives_pose == 40);
        }
      }
    }
  }
}
