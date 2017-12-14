#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/quaternion.h"
#include <time.h>
#include <stdlib.h>


// TEST FOR USING THE QUATERNION MESSAGE WRAPPER

SCENARIO("Using a Quaternion Message")
{
	GIVEN("A quaternion created from 4 doubles")
	{
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = static_cast<double>(rand()) / RAND_MAX;
		double d3 = static_cast<double>(rand()) / RAND_MAX;
		double d4 = static_cast<double>(rand()) / RAND_MAX;
		simple_msgs::Quaternion q(d1, d2, d3, d4);
		WHEN("We check the quaternion's elements")
		{
			THEN("They all have to be equal to the doubles from the constructor")
			{
				REQUIRE(q.getX() == d1);
				REQUIRE(q.getY() == d2);
				REQUIRE(q.getZ() == d3);
				REQUIRE(q.getW() == d4);
			}
		}
	}
	GIVEN("A quaternion created from an array of doubles")
	{
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = static_cast<double>(rand()) / RAND_MAX;
		double d3 = static_cast<double>(rand()) / RAND_MAX;
		double d4 = static_cast<double>(rand()) / RAND_MAX;
		std::array<double, 4> a{ d1, d2, d3, d4 };
		simple_msgs::Quaternion p(a);
		WHEN("We check the quaternion's elements")
		{
			THEN("They all have to be equal to the doubles")
			{
				REQUIRE(p.getX() == d1);
				REQUIRE(p.getY() == d2);
				REQUIRE(p.getZ() == d3);
				REQUIRE(p.getW() == d4);
			}
		}
	}
	GIVEN("A quaternion created from moving an array of doubles")
	{
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = static_cast<double>(rand()) / RAND_MAX;
		double d3 = static_cast<double>(rand()) / RAND_MAX;
		double d4 = static_cast<double>(rand()) / RAND_MAX;
		std::array<double, 4> a{ d1, d2, d3, d4 };
		simple_msgs::Quaternion p(std::move(a));
		WHEN("We check the quaternion's elements")
		{
			THEN("They all have to be equal to the doubles")
			{
				REQUIRE(p.getX() == d1);
				REQUIRE(p.getY() == d2);
				REQUIRE(p.getZ() == d3);
				REQUIRE(p.getW() == d4);
			}
		}
		WHEN("I construct a new quaternion from the serialized data of the existing quaternion")
		{
			simple_msgs::Quaternion p2(p.getBufferData());
			THEN("The new quaternion has to be equal to the other") { REQUIRE(p2 == p); }
		}
		WHEN("I copy-construct a new quaternion")
		{
			simple_msgs::Quaternion p3(p);
			THEN("The new quaternion is equal to the other") { REQUIRE(p3 == p); }
		}
		WHEN("I move-construct a new quaternion")
		{
			simple_msgs::Quaternion p4(std::move(p));
			THEN("The new quaternion's coordinates are equal to the previous' ones")
			{
				REQUIRE(p4.getX() == d1);
				REQUIRE(p4.getY() == d2);
				REQUIRE(p4.getZ() == d3);
				REQUIRE(p4.getW() == d4);
			}
		}
	}
	GIVEN("An instance of a quaternion.")
	{
		// start a quaternion
		simple_msgs::Quaternion q;

		WHEN("We use the empty constructor")
		{
			THEN("The quaternion has to be zero")
			{
				REQUIRE(q.getX() == 0);
				REQUIRE(q.getY() == 0);
				REQUIRE(q.getZ() == 0);
			}
		}

		WHEN("I set the X coordinate of the quaternion")
		{
			double x = static_cast<double>(rand()) / RAND_MAX;
			q.setX(x);
			THEN("The data quaternion has the correct coordinate") { REQUIRE(q.getX() == x); }
		}
		WHEN("I set the Y coordinate of the quaternion")
		{
			double y = static_cast<double>(rand()) / RAND_MAX;
			q.setY(y);
			THEN("The data quaternion has the correct coordinate") { REQUIRE(q.getY() == y); }
		}
		WHEN("I set the Z coordinate of the quaternion")
		{
			double z = static_cast<double>(rand()) / RAND_MAX;
			q.setZ(z);
			THEN("The data quaternion has the correct coordinate") { REQUIRE(q.getZ() == z); }
		}
		WHEN("I set the W coordinate of the quaternion")
		{
			double w = static_cast<double>(rand()) / RAND_MAX;
			q.setW(w);
			THEN("The data quaternion has the correct coordinate") { REQUIRE(q.getW() == w); }
		}
	}
	GIVEN("Two identical quaternions")
	{
		double x = static_cast<double>(rand()) / RAND_MAX;
		double y = static_cast<double>(rand()) / RAND_MAX;
		double z = static_cast<double>(rand()) / RAND_MAX;
		double w = static_cast<double>(rand()) / RAND_MAX;
		simple_msgs::Quaternion q3(x, y, z, w);
		simple_msgs::Quaternion q4(x, y, z, w);
		WHEN("I compare these quaternions") {
			THEN("They have to be equal") {
				REQUIRE(q3 == q4);
			}
		}
	}
	GIVEN("A quaternion")
	{
		double x = static_cast<double>(rand()) / RAND_MAX;
		double y = static_cast<double>(rand()) / RAND_MAX;
		double z = static_cast<double>(rand()) / RAND_MAX;
		double w = static_cast<double>(rand()) / RAND_MAX;
		simple_msgs::Quaternion q6(x, y, z, w);
		WHEN("I copy-assign from that quaternion's buffer")
		{
			simple_msgs::Quaternion q7;
			q7 = q6.getBufferData();
			THEN("The new quaternion has to be same as the original") { REQUIRE(q7 == q6); }
		}
		WHEN("I copy-assign from that quaternion")
		{
			simple_msgs::Quaternion q8;
			q8 = q6;
			THEN("The new quaternion has to be same as the original") { REQUIRE(q8 == q6); }
		}
		WHEN("I move-assign from that point")
		{
			simple_msgs::Quaternion q9;
			q9 = std::move(q6);
			THEN("The new quaternion has to be same as the original") {
				REQUIRE(q9.getX() == x);
				REQUIRE(q9.getY() == y);
				REQUIRE(q9.getZ() == z);
				REQUIRE(q9.getW() == w);
			}
		}
	}
	GIVEN("A quaternion") {
		simple_msgs::Quaternion q;
		WHEN("I copy-assign an array to that quaternion") {
			double x = static_cast<double>(rand()) / RAND_MAX;
			double y = static_cast<double>(rand()) / RAND_MAX;
			double z = static_cast<double>(rand()) / RAND_MAX;
			double w = static_cast<double>(rand()) / RAND_MAX;
			std::array<double, 4> a{ x, y, z, w };
			q = a;
			THEN("The quaternion's elements are equal to the array") {
				REQUIRE(q.getX() == x);
				REQUIRE(q.getY() == y);
				REQUIRE(q.getZ() == z);
				REQUIRE(q.getW() == w);
			}
		}
		WHEN("I move-assign from an array to that quaternion") {
			double x = static_cast<double>(rand()) / RAND_MAX;
			double y = static_cast<double>(rand()) / RAND_MAX;
			double z = static_cast<double>(rand()) / RAND_MAX;
			double w = static_cast<double>(rand()) / RAND_MAX;
			std::array<double, 4> a{ x, y, z, w };
			q = std::move(a);
			THEN("The quaternion's elements are the same as the array") {
				REQUIRE(q.getX() == x);
				REQUIRE(q.getY() == y);
				REQUIRE(q.getZ() == z);
				REQUIRE(q.getW() == w);
			}
		}
	}
	GIVEN("Two different quaternions") {
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = d1 + 1.0;
		simple_msgs::Quaternion q1(d1,d1,d1,d1);
		simple_msgs::Quaternion q2(d2,d2,d2,d2);
		WHEN("I compare these quaternions") {
			THEN("They have to be different") {
				REQUIRE(q1 != q2);
			}
		}
	}
	GIVEN("A quaternion") {
		double x = static_cast<double>(rand()) / RAND_MAX;
		double y = static_cast<double>(rand()) / RAND_MAX;
		double z = static_cast<double>(rand()) / RAND_MAX;
		double w = static_cast<double>(rand()) / RAND_MAX;
		simple_msgs::Quaternion q(x, y, z, w);
		WHEN("I get the buffer of that quaternion and construct a new quaternion") {
			uint8_t* buf = q.getBufferData();
			simple_msgs::Quaternion q2(buf);
			THEN("The new quaternion is equal to the original") {
				REQUIRE(q == q2);
			}
		}
		WHEN("I get the array from the quaternion") {
			std::array<double, 4> a = q.toVector();
			THEN("The array elements are correct") {
				REQUIRE(a[0] == x);
				REQUIRE(a[1] == y);
				REQUIRE(a[2] == z);
				REQUIRE(a[3] == w);
			}
		}
	}
}