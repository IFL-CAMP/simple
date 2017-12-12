#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/publisher.hpp"
#include "simple_msgs/point.h"
#include "simple/subscriber.hpp"
#include <time.h>
#include <stdlib.h>

// TEST FOR THE PUBLISHING AND SUBSCRIPTION OF A POINT

// create static point for comparing with data sent
simple_msgs::Point received_point(0.0, 0.0, 0.0);
bool running = false;
int numReceives = 0;

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
  numReceives++;
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
    simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
    auto p = createRandomPoint();
    pub.publish(p);
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunction);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPoint();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point); }
        }
      }
    }
    REQUIRE(numReceives == 10);
  }
}
