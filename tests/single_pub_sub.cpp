#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/publisher.h"
#include "simple_msgs/point.h"
#include "simple/subscriber.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR THE PUBLISHING AND SUBSCRIPTION OF A POINT

// create static point for comparing with data sent
simple_msgs::Point received_point(0.0, 0.0, 0.0);
bool running = false;

simple_msgs::Point createRandomPoint()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  double y = static_cast<double>(rand()) / RAND_MAX;
  double z = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Point(x, y, z);
}

// define callback function
void callbackFunction(const simple_msgs::Point& p)
{
  received_point = p;
  if (!running)
  {
    running = true;
  }
}

SCENARIO("Publish and subscribe to a Point message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunction);
    WHEN("A publisher publishes data")
    {
      simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
      std::this_thread::sleep_for(std::chrono::seconds(2));

      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPoint();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (running)
        {
          THEN("The data received is the same as the one sent")
          {
            REQUIRE(p == received_point);
          }
        }
      }
    }
  }
}
