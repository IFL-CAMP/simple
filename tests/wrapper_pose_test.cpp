#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/pose.h"
#include "simple_msgs/point.h"
#include "simple_msgs/quaternion.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE POSE MESSAGE WRAPPER

SCENARIO("Using a Pose Message")
{
  GIVEN("A Pose created from a point and a quaternion")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = static_cast<double>(rand()) / RAND_MAX;
    double d3 = static_cast<double>(rand()) / RAND_MAX;
    double d4 = static_cast<double>(rand()) / RAND_MAX;
    double d5 = static_cast<double>(rand()) / RAND_MAX;
    double d6 = static_cast<double>(rand()) / RAND_MAX;
    double d7 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point p(d1, d2, d3);
    simple_msgs::Quaternion q(d4, d5, d6, d7);
    simple_msgs::Pose pose(p, q);
    WHEN("We check the Pose's elements")
    {
      THEN("They all have to be equal to the params from the constructor")
      {
        REQUIRE(pose.getPosition() == p);
        REQUIRE(pose.getQuaternion() == q);
      }
    }
    WHEN("I construct a new Pose from the serialized data of the existing Pose")
    {
      simple_msgs::Pose p2(pose.getBufferData());
      THEN("The new Pose has to be equal to the other") { REQUIRE(p2 == pose); }
    }
    WHEN("I copy-construct a new Pose")
    {
      simple_msgs::Pose p3(pose);
      THEN("The new Pose is equal to the other") { REQUIRE(p3 == pose); }
    }
    WHEN("I move-construct a new Pose")
    {
      simple_msgs::Pose p4(std::move(pose));
      THEN("The new Pose's elements are equal to the previous' ones")
      {
        REQUIRE(p4.getPosition() == p);
        REQUIRE(p4.getQuaternion() == q);
      }
    }
  }
  GIVEN("A Pose created from an empty constructor")
  {
    simple_msgs::Point point;
    simple_msgs::Quaternion quat;
    simple_msgs::Pose pose;
    WHEN("We check the Pose's elements")
    {
      THEN("They both have to be empty")
      {
        REQUIRE(pose.getPosition() == point);
        REQUIRE(pose.getQuaternion() == quat);
      }
    }
  }
  GIVEN("An instance of a Pose.")
  {
    // start a Pose
    simple_msgs::Pose p;

    WHEN("I set the X coordinate of the Pose's position")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      p.getPosition().setX(x);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(p.getPosition().getX() == x); }
    }
    WHEN("I set the Y coordinate of the Pose's position")
    {
      double y = static_cast<double>(rand()) / RAND_MAX;
      p.getPosition().setY(y);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(p.getPosition().getY() == y); }
    }
    WHEN("I set the Z coordinate of the Pose's position")
    {
      double z = static_cast<double>(rand()) / RAND_MAX;
      p.getPosition().setZ(z);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(p.getPosition().getZ() == z); }
    }
    WHEN("I set the X coordinate of the Pose's quaternion")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      p.getQuaternion().setX(x);
      THEN("The data Pose's quaternion has the correct coordinate") { REQUIRE(p.getQuaternion().getX() == x); }
    }
    WHEN("I set the Y coordinate of the Pose's quaternion")
    {
      double y = static_cast<double>(rand()) / RAND_MAX;
      p.getQuaternion().setY(y);
      THEN("The data Pose's quaternion has the correct coordinate") { REQUIRE(p.getQuaternion().getY() == y); }
    }
    WHEN("I set the Z coordinate of the Pose's quaternion")
    {
      double z = static_cast<double>(rand()) / RAND_MAX;
      p.getQuaternion().setZ(z);
      THEN("The data Pose's quaternion has the correct coordinate") { REQUIRE(p.getQuaternion().getZ() == z); }
    }
    WHEN("I set the W coordinate of the Pose's quaternion")
    {
      double w = static_cast<double>(rand()) / RAND_MAX;
      p.getQuaternion().setW(w);
      THEN("The data Pose's quaternion has the correct coordinate") { REQUIRE(p.getQuaternion().getW() == w); }
    }
  }
  GIVEN("Two identical Poses")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;
    double z = static_cast<double>(rand()) / RAND_MAX;
    double w = static_cast<double>(rand()) / RAND_MAX;
    double x1 = static_cast<double>(rand()) / RAND_MAX;
    double y1 = static_cast<double>(rand()) / RAND_MAX;
    double z1 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point point1(x1, y1, z1);
    simple_msgs::Quaternion quat1(x, y, z, w);
    simple_msgs::Point point2 = point1;
    simple_msgs::Quaternion quat2 = quat1;
    simple_msgs::Pose pose1(point1, quat1);
    simple_msgs::Pose pose2(point2, quat2);
    WHEN("I compare these Poses")
    {
      THEN("They have to be equal") { REQUIRE(pose1 == pose2); }
    }
  }
  GIVEN("A Pose")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;
    double z = static_cast<double>(rand()) / RAND_MAX;
    double w = static_cast<double>(rand()) / RAND_MAX;
    double x1 = static_cast<double>(rand()) / RAND_MAX;
    double y1 = static_cast<double>(rand()) / RAND_MAX;
    double z1 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point point1(x1, y1, z1);
    simple_msgs::Quaternion quat1(x, y, z, w);
    simple_msgs::Pose pose(point1, quat1);
    WHEN("I copy-assign from that Pose's buffer")
    {
      simple_msgs::Pose p2;
      p2 = pose.getBufferData();
      THEN("The new Pose has to be same as the original") { REQUIRE(p2 == pose); }
    }
    WHEN("I copy-assign from that Pose")
    {
      simple_msgs::Pose p3;
      p3 = pose;
      THEN("The new Pose has to be same as the original") { REQUIRE(p3 == pose); }
    }
    WHEN("I move-assign from that Pose")
    {
      simple_msgs::Pose p4;
      p4 = std::move(pose);
      THEN("The new Pose has to be same as the original")
      {
        REQUIRE(p4.getPosition().getX() == x1);
        REQUIRE(p4.getPosition().getY() == y1);
        REQUIRE(p4.getPosition().getZ() == z1);
        REQUIRE(p4.getQuaternion().getX() == x);
        REQUIRE(p4.getQuaternion().getY() == y);
        REQUIRE(p4.getQuaternion().getZ() == z);
        REQUIRE(p4.getQuaternion().getW() == w);
      }
    }
  }
  GIVEN("Two different Pose")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = d1 + 1.0;
    simple_msgs::Pose p1(simple_msgs::Point(d1, d1, d1), simple_msgs::Quaternion(d1, d1, d1, d1));
    simple_msgs::Pose p2(simple_msgs::Point(d2, d2, d2), simple_msgs::Quaternion(d2, d2, d2, d2));
    WHEN("I compare these Poses")
    {
      THEN("They have to be different") { REQUIRE(p1 != p2); }
    }
  }
  GIVEN("A Pose")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;
    double z = static_cast<double>(rand()) / RAND_MAX;
    double w = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Pose p(simple_msgs::Point(x, y, z), simple_msgs::Quaternion(x,y,z,w));
    WHEN("I get the buffer of that Pose and construct a new Pose")
    {
      uint8_t* buf = p.getBufferData();
      simple_msgs::Pose p2(buf);
      THEN("The new Pose is equal to the original") { REQUIRE(p == p2); }
    }
  }
  GIVEN("A Pose")
  {
	  simple_msgs::Pose pose;
	  WHEN("I set the position of the pose") {
		  double x = static_cast<double>(rand()) / RAND_MAX;
		  double y = static_cast<double>(rand()) / RAND_MAX;
		  double z = static_cast<double>(rand()) / RAND_MAX;
		  pose.setPosition(simple_msgs::Point(x, y, z));
		  THEN("The position is correct") {
			  REQUIRE(pose.getPosition() == simple_msgs::Point(x, y, z));
		  }
	  }
	  WHEN("I set the quaternion of the pose") {
		  double x = static_cast<double>(rand()) / RAND_MAX;
		  double y = static_cast<double>(rand()) / RAND_MAX;
		  double z = static_cast<double>(rand()) / RAND_MAX;
		  double w = static_cast<double>(rand()) / RAND_MAX;
		  pose.setQuaternion(simple_msgs::Quaternion(x, y, z, w));
		  THEN("The quaternion is correct") {
			  REQUIRE(pose.getQuaternion() == simple_msgs::Quaternion(x, y, z, w));
		  }
	  }
  }
}
