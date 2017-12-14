#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/point.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE POINT MESSAGE WRAPPER

SCENARIO("Using a Point Message")
{
  GIVEN("A point created from a double")
  {
    double d = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point p1(d);
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the double")
      {
        REQUIRE(p1.getX() == d);
        REQUIRE(p1.getY() == d);
        REQUIRE(p1.getZ() == d);
      }
    }
  }
  GIVEN("A point created from 3 double")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = static_cast<double>(rand()) / RAND_MAX;
    double d3 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point p9(d1, d2, d3);
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the doubles fromthe constructor")
      {
        REQUIRE(p9.getX() == d1);
        REQUIRE(p9.getY() == d2);
        REQUIRE(p9.getZ() == d3);
      }
    }
  }
  GIVEN("A point created from an array of a double")
  {
    double d4 = static_cast<double>(rand()) / RAND_MAX;
    std::array<double, 3> a{d4,d4,d4};
    simple_msgs::Point p10(a);
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the double")
      {
        REQUIRE(p10.getX() == d4);
        REQUIRE(p10.getY() == d4);
        REQUIRE(p10.getZ() == d4);
      }
    }
  }
  GIVEN("A point created from moving an array of a double")
  {
    double d5 = static_cast<double>(rand()) / RAND_MAX;
    std::array<double, 3> a{d5,d5,d5};
    simple_msgs::Point p11(std::move(a));
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the double")
      {
        REQUIRE(p11.getX() == d5);
        REQUIRE(p11.getY() == d5);
        REQUIRE(p11.getZ() == d5);
      }
    }
    WHEN("I construct a new point from the serialized data of the existing point")
    {
      simple_msgs::Point p12(p11.getBufferData());
      THEN("The new point has to be equal to the other") { REQUIRE(p12 == p11); }
    }
    WHEN("I copy-construct a new point")
    {
      simple_msgs::Point p13(p11);
      THEN("The new point is equal to the other") { REQUIRE(p13 == p11); }
    }
    WHEN("I move-construct a new point")
    {
      simple_msgs::Point p14(std::move(p11));
      THEN("The new point's coordinates are equal to the previous' ones")
      {
        REQUIRE(p14.getX() == d5);
        REQUIRE(p14.getY() == d5);
        REQUIRE(p14.getZ() == d5);
      }
    }
  }
  GIVEN("An instance of a point.")
  {
    // start a point
    simple_msgs::Point p2;

    WHEN("We use the empty constructor")
    {
      THEN("The point has to be zero")
      {
        REQUIRE(p2.getX() == 0);
        REQUIRE(p2.getY() == 0);
        REQUIRE(p2.getZ() == 0);
      }
    }

    WHEN("I set the X coordinate of the point")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      p2.setX(x);
      THEN("The data point has the correct coordinate") { REQUIRE(p2.getX() == x); }
    }
    WHEN("I set the Y coordinate of the point")
    {
      double y = static_cast<double>(rand()) / RAND_MAX;
      p2.setY(y);
      THEN("The data point has the correct coordinate") { REQUIRE(p2.getY() == y); }
    }
    WHEN("I set the Z coordinate of the point")
    {
      double z = static_cast<double>(rand()) / RAND_MAX;
      p2.setZ(z);
      THEN("The data point has the correct coordinate") { REQUIRE(p2.getZ() == z); }
    }
  }
  GIVEN("Two identical points")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    double y = static_cast<double>(rand()) / RAND_MAX;
    double z = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point p3(x, y, z);
    simple_msgs::Point p4(x, y, z);
    WHEN("I sum these two points")
    {
      simple_msgs::Point p5 = p3 + p4;
      THEN("The sum has to be correct")
      {
        REQUIRE(p5.getX() == (p3.getX() + p4.getX()));
        REQUIRE(p5.getY() == (p3.getY() + p4.getY()));
        REQUIRE(p5.getZ() == (p3.getZ() + p4.getZ()));
      }
    }
	WHEN("I compare these points") {
		THEN("They have to be equal") {
			REQUIRE(p3 == p4);
		}
	}
  }
  GIVEN("A point")
  {
	  double d = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Point p6(d);
    WHEN("I copy-assign from that point's buffer")
    {
      simple_msgs::Point p7;
      p7 = p6.getBufferData();
      THEN("The new point has to be same as the original") { REQUIRE(p7 == p6); }
    }
    WHEN("I copy-assign from that point")
    {
      simple_msgs::Point p8;
      p8 = p6;
      THEN("The new point has to be same as the original") { REQUIRE(p8 == p6); }
    }
	WHEN("I move-assign from that point")
	{
		simple_msgs::Point p9;
		p9 = std::move(p6);
		THEN("The new point has to be same as the original") { 
			REQUIRE(p9.getX() == d); 
			REQUIRE(p9.getY() == d);
			REQUIRE(p9.getZ() == d);

		}
	}
  }
  GIVEN("A point") {
	  simple_msgs::Point p;
	  WHEN("I copy-assign an array to that point") {
		  double d1 = static_cast<double>(rand()) / RAND_MAX;
		  std::array<double, 3> a{ d1,d1,d1 };
		  p = a;
		  THEN("The point's coordinates are equal to the array") {
			  REQUIRE(p.getX() == d1);
			  REQUIRE(p.getY() == d1);
			  REQUIRE(p.getZ() == d1);
		  }
	  }
	  WHEN("I move-assign from an array to that point") {
		  double d2 = static_cast<double>(rand()) / RAND_MAX;
		  std::array<double, 3> a2{ d2,d2,d2 };
		  p = std::move(a2);
		  THEN("The point's coordinates are the same as the array") {
			  REQUIRE(p.getX() == d2);
			  REQUIRE(p.getY() == d2);
			  REQUIRE(p.getZ() == d2);
		  }
	  }
  }
  GIVEN("Two different points") {
	  double d1 = static_cast<double>(rand()) / RAND_MAX;
	  double d2 = d1 + 1.0;
	  simple_msgs::Point p1(d1);
	  simple_msgs::Point p2(d2);
	  WHEN("I compare these points") {
		  THEN("They have to be different") {
			  REQUIRE(p1 != p2);
		  }
	  }
	  WHEN("I subtract them") {
		  simple_msgs::Point p3 = p1 - p2;
		  THEN("The difference should be correct") {
			  REQUIRE(p3.getX() == (p1.getX() - p2.getX()));
			  REQUIRE(p3.getY() == (p1.getY() - p2.getY()));
			  REQUIRE(p3.getZ() == (p1.getZ() - p2.getZ()));
		  }
	  }
	  WHEN("I add them into the first") {
		  p1 += p2;
		  THEN("The first point should have increased by the second point") {
			  REQUIRE(p1.getX() == d1 + d2);
			  REQUIRE(p1.getY() == d1 + d2);
			  REQUIRE(p1.getZ() == d1 + d2);
		  }
	  }
	  WHEN("I subtract them into the first") {
		  p1 -= p2;
		  THEN("The first point should have decreased by the second point") {
			  REQUIRE(p1.getX() == d1 - d2);
			  REQUIRE(p1.getY() == d1 - d2);
			  REQUIRE(p1.getZ() == d1 - d2);
		  }
	  }
	  WHEN("I multiply them") {
		  simple_msgs::Point p3 = p1 * p2;
		  THEN("The result is correct") {
			  REQUIRE(p3.getX() == p1.getX()*p2.getX());
			  REQUIRE(p3.getY() == p1.getY()*p2.getY());
			  REQUIRE(p3.getZ() == p1.getZ()*p2.getZ());
		  }
	  }
	  WHEN("I multiply them into the first") {
		  p1 *= p2;
		  THEN("The result is correct") {
			  REQUIRE(p1.getX() == d1*d2);
			  REQUIRE(p1.getY() == d1*d2);
			  REQUIRE(p1.getZ() == d1*d2);
		  }
	  }
	  WHEN("I divide them") {
		  simple_msgs::Point p3 = p1 / p2;
		  THEN("The result is correct") {
			  REQUIRE(p3.getX() == p1.getX()/p2.getX());
			  REQUIRE(p3.getY() == p1.getY()/p2.getY());
			  REQUIRE(p3.getZ() == p1.getZ()/p2.getZ());
		  }
	  }
	  WHEN("I divide them into the first") {
		  p1 /= p2;
		  THEN("The result is correct") {
			  REQUIRE(p1.getX() == d1 / d2);
			  REQUIRE(p1.getY() == d1 / d2);
			  REQUIRE(p1.getZ() == d1 / d2);
		  }
	  }
  }
  GIVEN("A point") {
	  double x = static_cast<double>(rand()) / RAND_MAX;
	  double y = static_cast<double>(rand()) / RAND_MAX;
	  double z = static_cast<double>(rand()) / RAND_MAX;
	  simple_msgs::Point p(x, y, z);
	  WHEN("I get the buffer of that point and construct a new point") {
		  uint8_t* buf = p.getBufferData();
		  simple_msgs::Point p2(buf);
		  THEN("The new point is equal to the original") {
			  REQUIRE(p == p2);
		  }
	  }
	  WHEN("I get the array from the point") {
		  std::array<double, 3> a = p.toVector();
		  THEN("The array elements are correct") {
			  REQUIRE(a[0] == x);
			  REQUIRE(a[1] == y);
			  REQUIRE(a[2] == z);
		  }
	  }
  }
}
