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
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE POINT MESSAGE WRAPPER

SCENARIO("Using a Point Message")
{
  GIVEN("An instance of a point.")
  {
    // start a point
	  simple_msgs::Point p;

    WHEN("I set the X coordinate of the point")
    {
		double x = static_cast<double>(rand()) / RAND_MAX;
      p.setX(x);
      THEN("The data point has the correct coordinate") { REQUIRE(p.getX() == x); }
    }
	WHEN("I set the Y coordinate of the point")
	{
		double y = static_cast<double>(rand()) / RAND_MAX;
		p.setY(y);
		THEN("The data point has the correct coordinate") { REQUIRE(p.getY() == y); }
	}
	WHEN("I set the Z coordinate of the point")
	{
		double z = static_cast<double>(rand()) / RAND_MAX;
		p.setZ(z);
		THEN("The data point has the correct coordinate") { REQUIRE(p.getZ() == z); }
	}
	
    
  }
  GIVEN("Two identical points") {
	  simple_msgs::Point p1(1, 1, 1);
	  simple_msgs::Point p2(1,1,1);
	  WHEN("I sum these two points") {
		  simple_msgs::Point p3 = p1 + p2;
		  THEN("The sum has to be correct") {
			  REQUIRE(p3.getX() == (p1.getX() + p2.getX()));
			  REQUIRE(p3.getY() == (p1.getY() + p2.getY()));
			  REQUIRE(p3.getZ() == (p1.getZ() + p2.getZ()));
		  }
	  }
  }
  GIVEN("A point") {
	  simple_msgs::Point p1(1, 1, 1);
	  WHEN("I copy-construct from that point") {
		  simple_msgs::Point p2 = p1;
		  THEN("The new point has to be same as the original") {
			  REQUIRE(p1==p2);
		  }
	  }
	  WHEN("I copy-assign from that point") {
		  simple_msgs::Point p2;
		  p2= p1;
		  THEN("The new point has to be same as the original") {
			  REQUIRE(p1 == p2);
		  }
	  }
  }

}
