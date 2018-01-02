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

// TEST FOR THE PUBLISHING AND SUBSCRIPTION OF ALL DATA TYPES

// create static header for comparing with data sent
simple_msgs::Header received_header;
bool running_header = false;
int num_receives_header = 0;

// create static point for comparing with data sent
simple_msgs::Point received_point;
bool running_point = false;
int num_receives_point = 0;

// create static pose for comparing with data sent
simple_msgs::Pose received_pose;
bool running_pose = false;
int num_receives_pose = 0;

// create static quaternion for comparing with data sent
simple_msgs::Quaternion received_quaternion;
bool running_quaternion = false;
int num_receives_quaternion = 0;

// create static rotation matrix for comparing with data sent
simple_msgs::RotationMatrix received_rotation_matrix;
bool running_rotation_matrix = false;
int num_receives_rotation_matrix = 0;

// create static string for comparing with data sent
simple_msgs::String received_string;
bool running_string = false;
int num_receives_string = 0;

// create static bool for comparing with data sent
simple_msgs::Bool received_bool;
bool running_bool = false;
int num_receives_bool = 0;

// create static int for comparing with data sent
simple_msgs::NumericType<int> received_int;
bool running_int = false;
int num_receives_int = 0;

// create static double for comparing with data sent
simple_msgs::NumericType<double> received_double;
bool running_double = false;
int num_receives_double = 0;

// create static float for comparing with data sent
simple_msgs::NumericType<float> received_float;
bool running_float = false;
int num_receives_float = 0;

simple_msgs::Header createRandomHeader()
{
  int x = rand() % 100;
  std::string y("Header string:");
  y.append(std::to_string(rand() % 100));
  double z = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Header(x, y, z);
}

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

simple_msgs::RotationMatrix createRandomRotationMatrix()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  double y = static_cast<double>(rand()) / RAND_MAX;
  double z = static_cast<double>(rand()) / RAND_MAX;
  double w = static_cast<double>(rand()) / RAND_MAX;
  double a = static_cast<double>(rand()) / RAND_MAX;
  double b = static_cast<double>(rand()) / RAND_MAX;
  double c = static_cast<double>(rand()) / RAND_MAX;
  double d = static_cast<double>(rand()) / RAND_MAX;
  double e = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::RotationMatrix(x, y, z, w, a, b, c, d, e);
}

simple_msgs::Pose createRandomPose()
{
  simple_msgs::Point p = createRandomPoint();
  simple_msgs::Quaternion q = createRandomQuaternion();
  return simple_msgs::Pose(p, q);
}

simple_msgs::NumericType<int> createRandomInt()
{
  int x = rand() % 100;
  return simple_msgs::NumericType<int>(x);
}

simple_msgs::NumericType<double> createRandomDouble()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::NumericType<double>(x);
}

simple_msgs::NumericType<float> createRandomFloat()
{
  float x = static_cast<float>(rand()) / RAND_MAX;
  return simple_msgs::NumericType<float>(x);
}

simple_msgs::Bool createRandomBool()
{
  bool x = rand() % 1;
  return simple_msgs::Bool(x);
}

simple_msgs::String createRandomString()
{
  std::string s("Random string:");
  s.append(std::to_string(rand() % 100));
  return simple_msgs::String(s);
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

// define callback function
void callbackFunctionHeader(const simple_msgs::Header& h)
{
  received_header = h;
  num_receives_header++;
  if (!running_header)
  {
    running_header = true;
  }
}

// define callback function
void callbackFunctionPose(const simple_msgs::Pose& p)
{
  received_pose = p;
  num_receives_pose++;
  if (!running_pose)
  {
    running_pose = true;
  }
}

// define callback function
void callbackFunctionQuaternion(const simple_msgs::Quaternion& q)
{
  received_quaternion = q;
  num_receives_quaternion++;
  if (!running_quaternion)
  {
    running_quaternion = true;
  }
}

// define callback function
void callbackFunctionString(const simple_msgs::String& s)
{
  received_string = s;
  num_receives_string++;
  if (!running_string)
  {
    running_string = true;
  }
}

// define callback function
void callbackFunctionRotationMatrix(const simple_msgs::RotationMatrix& r)
{
  received_rotation_matrix = r;
  num_receives_rotation_matrix++;
  if (!running_rotation_matrix)
  {
    running_rotation_matrix = true;
  }
}

// define callback function
void callbackFunctionBool(const simple_msgs::Bool& b)
{
  received_bool = b;
  num_receives_bool++;
  if (!running_bool)
  {
    running_bool = true;
  }
}

// define callback function
void callbackFunctionInt(const simple_msgs::NumericType<int>& i)
{
  received_int = i;
  num_receives_int++;
  if (!running_int)
  {
    running_int = true;
  }
}

// define callback function
void callbackFunctionDouble(const simple_msgs::NumericType<double>& d)
{
  received_double = d;
  num_receives_double++;
  if (!running_double)
  {
    running_double = true;
  }
}

// define callback function
void callbackFunctionFloat(const simple_msgs::NumericType<float>& f)
{
  received_float = f;
  num_receives_float++;
  if (!running_float)
  {
    running_float = true;
  }
}

SCENARIO("Publish and subscribe to a Point message.")
{
  GIVEN("An instance of a subscriber.")
  {
    // start a subscriber
    simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5555", callbackFunctionPoint);

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
    simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5555", callbackFunctionPose);

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
    simple::Subscriber<simple_msgs::Quaternion> sub("tcp://localhost:5555", callbackFunctionQuaternion);

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
    simple::Subscriber<simple_msgs::RotationMatrix> sub("tcp://localhost:5555", callbackFunctionRotationMatrix);

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
    simple::Subscriber<simple_msgs::String> sub("tcp://localhost:5555", callbackFunctionString);

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
    simple::Subscriber<simple_msgs::Bool> sub("tcp://localhost:5555", callbackFunctionBool);

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
    simple::Subscriber<simple_msgs::NumericType<double>> sub("tcp://localhost:5555", callbackFunctionDouble);

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
    simple::Subscriber<simple_msgs::NumericType<int>> sub("tcp://localhost:5555", callbackFunctionInt);

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
    simple::Subscriber<simple_msgs::NumericType<float>> sub("tcp://localhost:5555", callbackFunctionFloat);

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
    simple::Subscriber<simple_msgs::Header> sub("tcp://localhost:5555", callbackFunctionHeader);

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

