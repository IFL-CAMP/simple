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
static simple_msgs::Point received_point(0.0, 0.0, 0.0);

// define callback function
void callbackFunction(const simple_msgs::Point& p)
{
  received_point = p;
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
      // randomly generate the data to be sent
      srand(time(NULL));  // start random seed
      double x = rand() / RAND_MAX;
      double y = rand() / RAND_MAX;
      double z = rand() / RAND_MAX;
      simple_msgs::Point p(x, y, z);

      for (int i = 0; i < 10; ++i)
      {
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        THEN("The data received is the same as the one sent")
        {
          REQUIRE(p == received_point);
        }
      }
    }
  }
}
