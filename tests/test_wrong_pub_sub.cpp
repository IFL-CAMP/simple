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

// TEST FOR THE CONNECTING A SUBSCRIBER TO A WRONG TYPE OF PUBLISHER

// create static point for comparing with data sent
simple_msgs::Point received_point;
bool running_point = false;
int num_receives_point = 0;

simple_msgs::Point createRandomPoint()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  double y = static_cast<double>(rand()) / RAND_MAX;
  double z = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Point(x, y, z);
}

simple_msgs::Quaternion createRandomQuaternion()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  double y = static_cast<double>(rand()) / RAND_MAX;
  double z = static_cast<double>(rand()) / RAND_MAX;
  double w = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Quaternion(x, y, z, w);
}

simple_msgs::Pose createRandomPose()
{
  simple_msgs::Point p = createRandomPoint();
  simple_msgs::Quaternion q = createRandomQuaternion();
  return simple_msgs::Pose(p, q);
}

// define callback function
void callbackFunctionPoint(const simple_msgs::Point& p)
{
  received_point = p;
  num_receives_point++;
  if (!running_point)
  {
    running_point = true;
  }
}


SCENARIO("Publish a Pose and subscribe to a Point message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunctionPoint);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 1000; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

      }
    }
    REQUIRE(num_receives_point == 0);
  }
}
