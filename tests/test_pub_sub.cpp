#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <time.h>

#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "simple_msgs/bool.h"
#include "simple_msgs/int.h"
#include "simple_msgs/float.h"
#include "simple_msgs/double.h"
#include "simple_msgs/string.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"
#include "simple_msgs/point_stamped.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/quaternion_stamped.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/pose_stamped.h"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/rotation_matrix_stamped.h"
#include "test_utils.hpp"

// Test: Publish and Subscribe to all data types.

SCENARIO("Publish and subscribe to a Bool message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Bool> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Bool> sub("tcp://localhost:5555", callbackFunctionConstBool);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomBool();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_bool)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_bool); }
        }
      }
    }
    REQUIRE(num_received_bool == 10);
  }
}

SCENARIO("Publish and subscribe to a Int message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Int> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Int> sub("tcp://localhost:5555", callbackFunctionConstInt);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomInt();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_int)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_int); }
        }
      }
    }
    REQUIRE(num_received_int == 10);
  }
}

SCENARIO("Publish and subscribe to a Float message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Float> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Float> sub("tcp://localhost:5555", callbackFunctionConstFloat);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomFloat();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_float)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_float); }
        }
      }
    }
    REQUIRE(num_received_float == 10);
  }
}

SCENARIO("Publish and subscribe to a Double message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Double> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Double> sub("tcp://localhost:5555", callbackFunctionConstDouble);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomDouble();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_double)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_double); }
        }
      }
    }
    REQUIRE(num_received_double == 10);
  }
}

SCENARIO("Publish and subscribe to a String message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::String> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::String> sub("tcp://localhost:5555", callbackFunctionConstString);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomString();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_string)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_string); }
        }
      }
    }
    REQUIRE(num_received_string == 10);
  }
}

SCENARIO("Publish and subscribe to a Header message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Header> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Header> sub("tcp://localhost:5555", callbackFunctionConstHeader);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomHeader();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_header)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_header); }
        }
      }
    }
    REQUIRE(num_received_header == 10);
  }
}

SCENARIO("Publish and subscribe to a Point message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunctionConstPoint);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPoint();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point); }
        }
      }
    }
    REQUIRE(num_received_point == 10);
  }
}

SCENARIO("Publish and subscribe to a Quaternion message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Quaternion> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Quaternion> sub("tcp://localhost:5555", callbackFunctionConstQuaternion);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomQuaternion();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_quaternion); }
        }
      }
    }
    REQUIRE(num_received_quaternion == 10);
  }
}

SCENARIO("Publish and subscribe to a Pose message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5555", callbackFunctionConstPose);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_pose); }
        }
      }
    }
    REQUIRE(num_received_pose == 10);
  }
}

SCENARIO("Publish and subscribe to a Rotation Matrix message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::RotationMatrix> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::RotationMatrix> sub("tcp://localhost:5555", callbackFunctionConstRotationMatrix);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomRotationMatrix();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_rotation_matrix); }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix == 10);
  }
}

SCENARIO("Publish and subscribe to a PointStamped message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::PointStamped> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::PointStamped> sub("tcp://localhost:5555", callbackFunctionConstPointStamped);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPointStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point_stamped)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point_stamped); }
        }
      }
    }
    REQUIRE(num_received_point_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a QuaternionStamped message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::QuaternionStamped> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::QuaternionStamped> sub("tcp://localhost:5555",
                                                           callbackFunctionConstQuaternionStamped);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomQuaternionStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion_stamped)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_quaternion_stamped); }
        }
      }
    }
    REQUIRE(num_received_quaternion_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a PoseStamped message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::PoseStamped> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::PoseStamped> sub("tcp://localhost:5555", callbackFunctionConstPoseStamped);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPoseStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose_stamped)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_pose_stamped); }
        }
      }
    }
    REQUIRE(num_received_pose_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a RotationMatrixStamped message.")
{
  GIVEN("An instance of a subscriber.")
  {
    simple::Publisher<simple_msgs::RotationMatrixStamped> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::RotationMatrixStamped> sub("tcp://localhost:5555",
                                                               callbackFunctionConstRotationMatrixStamped);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomRotationMatrixStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix_stamped)
        {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_rotation_matrix_stamped); }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix_stamped == 10);
  }
}
