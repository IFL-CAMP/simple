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

// TEST FOR THE PUBLISHING AND SUBSCRIPTION OF ALL DATA TYPES


SCENARIO("Publish and subscribe to a Point message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunctionConstPoint);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPoint();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_point)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point); }
        }
      }
    }
    REQUIRE(num_receives_point == 10);
  }
}

SCENARIO("Publish and subscribe to a Pose message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5555", callbackFunctionConstPose);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_pose)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_pose); }
        }
      }
    }
    REQUIRE(num_receives_pose == 10);
  }
}

SCENARIO("Publish and subscribe to a Quaternion message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Quaternion> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Quaternion> sub("tcp://localhost:5555", callbackFunctionConstQuaternion);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomQuaternion();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_quaternion)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_quaternion); }
        }
      }
    }
    REQUIRE(num_receives_quaternion == 10);
  }
}

SCENARIO("Publish and subscribe to a Rotation Matrix message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::RotationMatrix> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::RotationMatrix> sub("tcp://localhost:5555", callbackFunctionConstRotationMatrix);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomRotationMatrix();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_rotation_matrix)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_rotation_matrix); }
        }
      }
    }
    REQUIRE(num_receives_rotation_matrix == 10);
  }
}

SCENARIO("Publish and subscribe to a String message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::String> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::String> sub("tcp://localhost:5555", callbackFunctionConstString);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomString();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_string)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_string); }
        }
      }
    }
    REQUIRE(num_receives_string == 10);
  }
}

SCENARIO("Publish and subscribe to a Bool message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Bool> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Bool> sub("tcp://localhost:5555", callbackFunctionConstBool);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomBool();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_bool)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_bool); }
        }
      }
    }
    REQUIRE(num_receives_bool == 10);
  }
}
SCENARIO("Publish and subscribe to a Double message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::NumericType<double>> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::NumericType<double>> sub("tcp://localhost:5555", callbackFunctionConstDouble);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomDouble();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_double)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_double); }
        }
      }
    }
    REQUIRE(num_receives_double == 10);
  }
}

SCENARIO("Publish and subscribe to a Int message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::NumericType<int>> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::NumericType<int>> sub("tcp://localhost:5555", callbackFunctionConstInt);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomInt();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_int)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_int); }
        }
      }
    }
    REQUIRE(num_receives_int == 10);
  }
}

SCENARIO("Publish and subscribe to a Float message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::NumericType<float>> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::NumericType<float>> sub("tcp://localhost:5555", callbackFunctionConstFloat);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomFloat();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_float)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_float); }
        }
      }
    }
    REQUIRE(num_receives_float == 10);
  }
}

SCENARIO("Publish and subscribe to a Header message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Header> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Header> sub("tcp://localhost:5555", callbackFunctionConstHeader);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomHeader();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running_header)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_header); }
        }
      }
    }
    REQUIRE(num_receives_header == 10);
  }
}

