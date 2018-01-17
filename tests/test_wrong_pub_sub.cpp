#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <time.h>

#include "simple/publisher.hpp"
#include "simple_msgs/point.h"
#include "simple_msgs/pose.h"
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
