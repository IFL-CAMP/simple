#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/publisher.hpp"
#include "simple_msgs/point.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/bool.h"
#include "simple_msgs/string.h"
#include "simple_msgs/numeric_type.hpp"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/header.h"
#include "simple/subscriber.hpp"
#include "simple_msgs/double.h"
#include "simple_msgs/float.h"
#include "simple_msgs/int.h"
#include <time.h>
#include <stdlib.h>
#include "test_utils.hpp"

// TEST ONE PUBLISHER WITH ONE SUBSCRIBER PUBLISHING FOR LONG

int numPublishes = 100000;

SCENARIO("Publish a pose for a long time.")
{
  GIVEN("An instance of a publisher and 1 subscriber.")
  {
    // Start a publisher
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    // Start 20 subscribers
    simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5555", callbackFunctionConstPose);
    // Wait so the subscribers get every message
    std::this_thread::sleep_for(std::chrono::seconds(1));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < numPublishes; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        REQUIRE(received_pose == p);
      }
      REQUIRE(num_receives_pose == numPublishes);
    }
  }
}
