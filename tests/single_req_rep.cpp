#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/server.hpp"
#include "simple_msgs/point.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/bool.h"
#include "simple_msgs/string.h"
#include "simple_msgs/numeric_type.hpp"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/header.h"
#include "simple/client.hpp"
#include "simple_msgs/double.h"
#include "simple_msgs/float.h"
#include "simple_msgs/int.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR A CLIENT AND SERVER OF ALL DATA TYPES

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
void callbackFunctionPoint(simple_msgs::Point& p)
{
  // adds one's to the received point
  simple_msgs::Point point(1, 1, 1);
  p += point;
}

// define callback function
void callbackFunctionHeader(simple_msgs::Header& h)
{
  // fill the header fields
  h.setFrameID("ID");
  h.setSequenceNumber(1);
  h.setTimestamp(1.0);
}

// define callback function
void callbackFunctionPose(simple_msgs::Pose& p)
{
  // add one's to the pose
  p.getPosition() += 1.0;
  p.getQuaternion().setW(p.getQuaternion().getW() + 1);
  p.getQuaternion().setX(p.getQuaternion().getX() + 1);
  p.getQuaternion().setY(p.getQuaternion().getY() + 1);
  p.getQuaternion().setZ(p.getQuaternion().getZ() + 1);
}

// define callback function
void callbackFunctionQuaternion(simple_msgs::Quaternion& q)
{
  // add one's to the quaternion
  q.setW(q.getW() + 1);
  q.setX(q.getX() + 1);
  q.setY(q.getY() + 1);
  q.setZ(q.getZ() + 1);
}

// define callback function
void callbackFunctionString(simple_msgs::String& s)
{
  // replace string by standard reply
  s.set("REPLY");
  
}

// define callback function
void callbackFunctionRotationMatrix(simple_msgs::RotationMatrix& r)
{
  // set rotation matrix to zero
  r.setColumn(0, {0, 0, 0});
  r.setColumn(1, {0, 0, 0});
  r.setColumn(2, {0, 0, 0});
}

// define callback function
void callbackFunctionBool(simple_msgs::Bool& b)
{
  // set bool to true
  b.set(true);
}

// define callback function
void callbackFunctionInt(simple_msgs::NumericType<int>& i)
{
  // add 1 to the int
  i += 1;
}

// define callback function
void callbackFunctionDouble(simple_msgs::NumericType<double>& d)
{
  // add 1 to the double
  d += 1.0;
}

// define callback function
void callbackFunctionFloat(simple_msgs::NumericType<float>& f)
{
  // add 1 to the float
  f += 1.0f;
}

SCENARIO("Client-Server to a Point message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Server<simple_msgs::Point> server("tcp://*:5555", callbackFunctionPoint);
    simple::Client<simple_msgs::Point> client("tcp://localhost:5555");

    WHEN("The client sends a request")
    {
      auto p = createRandomPoint();
      auto sentPoint = p;
      client.request(p);
      simple_msgs::Point unitPoint(1, 1, 1);

      THEN("The data received is the equal to the sent point plus one's") { REQUIRE(p == sentPoint + unitPoint); }
    }
  }
}

SCENARIO("Client-Server to a Pose message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Server<simple_msgs::Pose> server("tcp://*:5556", callbackFunctionPose);
	simple::Client<simple_msgs::Pose> client("tcp://localhost:5556");

    WHEN("The client sends a request")
    {
      auto p = createRandomPose();
      auto sentPose = p;
      client.request(p);
      simple_msgs::Point unitPoint(1, 1, 1);
      THEN("The data received is the equal to the sent pose plus one's")
      {
        REQUIRE(p.getPosition() == sentPose.getPosition() + unitPoint);
        REQUIRE(p.getQuaternion().getW() == sentPose.getQuaternion().getW() + 1);
        REQUIRE(p.getQuaternion().getX() == sentPose.getQuaternion().getX() + 1);
        REQUIRE(p.getQuaternion().getY() == sentPose.getQuaternion().getY() + 1);
        REQUIRE(p.getQuaternion().getZ() == sentPose.getQuaternion().getZ() + 1);
      }
    }
  }
}

SCENARIO("Client-Server to a Quaternion message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::Quaternion> client("tcp://localhost:5555");
    simple::Server<simple_msgs::Quaternion> server("tcp://*:5555", callbackFunctionQuaternion);

    WHEN("The client sends a request")
    {
      auto q = createRandomQuaternion();
      auto sentQ = q;
      client.request(q);
      THEN("The data received is the equal to the sent pose plus one's")
      {
        REQUIRE(q.getW() == sentQ.getW() + 1);
        REQUIRE(q.getX() == sentQ.getX() + 1);
        REQUIRE(q.getY() == sentQ.getY() + 1);
        REQUIRE(q.getZ() == sentQ.getZ() + 1);
      }
    }
  }
}

SCENARIO("Client-Server to a Rotation Matrix message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server.
    simple::Client<simple_msgs::RotationMatrix> client("tcp://localhost:5555");
    simple::Server<simple_msgs::RotationMatrix> server("tcp://*:5555", callbackFunctionRotationMatrix);

    WHEN("The client sends a request")
    {
      auto r = createRandomRotationMatrix();
      client.request(r);
      THEN("The data received is the the identity matrix") { REQUIRE(r == simple_msgs::RotationMatrix()); }
    }
  }
}

SCENARIO("Client-Server to a String message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::String> client("tcp://localhost:5555");
    simple::Server<simple_msgs::String> server("tcp://*:5555", callbackFunctionString);

    WHEN("The client sends a request")
    {
      auto p = createRandomString();
      client.request(p);

      THEN("The string received back is the default answer") { REQUIRE(p == "REPLY"); }
    }
  }
}

SCENARIO("Client-Server to a Bool message.")
{
  GIVEN("An instance of a server.")
  {
    // Start a server
    simple::Client<simple_msgs::Bool> client("tcp://localhost:5555");
    simple::Server<simple_msgs::Bool> server("tcp://*:5555", callbackFunctionBool);

    WHEN("The client sends a request")
    {
      auto p = createRandomBool();
      client.request(p);

      THEN("The data received is true") { REQUIRE(p == true); }
    }
  }
}
SCENARIO("Client-Server to a Double message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::NumericType<double>> client("tcp://localhost:5555");
    simple::Server<simple_msgs::NumericType<double>> server("tcp://*:5555", callbackFunctionDouble);

    WHEN("The client sends a request")
    {
      auto p = createRandomDouble();
      auto sentDouble = p;
      client.request(p);
      THEN("The data received is the same as the one sent plus 1") { REQUIRE(p == sentDouble + 1.0); }
    }
  }
}

SCENARIO("Client-Server to a Int message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::NumericType<int>> client("tcp://localhost:5555");
    simple::Server<simple_msgs::NumericType<int>> server("tcp://*:5555", callbackFunctionInt);

    WHEN("The client sends a request")
    {
      auto i = createRandomInt();
      auto sentInt = i;
      client.request(i);
      THEN("The data received is the same as the one sent plus 1") { REQUIRE(i == sentInt + 1); }
    }
  }
}

SCENARIO("Client-Server to a Float message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::NumericType<float>> client("tcp://localhost:5555");
    simple::Server<simple_msgs::NumericType<float>> server("tcp://*:5555", callbackFunctionFloat);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("The client sends a request")
    {
      auto p = createRandomFloat();
      auto sentFloat = p;
      client.request(p);

      THEN("The data received is the same as the one sent plus 1") { REQUIRE(p == sentFloat + 1.0f); }
    }
  }
}

SCENARIO("Client-Server to a Header message.")
{
  GIVEN("An instance of a server.")
  {
    // start a server
    simple::Client<simple_msgs::Header> client("tcp://localhost:5555");
    simple::Server<simple_msgs::Header> server("tcp://*:5555", callbackFunctionHeader);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("The client sends a request")
    {
      auto p = createRandomHeader();
      client.request(p);
      THEN("The data received is filled wth the standard values")
      {
        REQUIRE(p.getFrameID() == "ID");
        REQUIRE(p.getSequenceNumber() == 1);
        REQUIRE(p.getTimestamp() == 1.0);
      }
    }
  }
}
