#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/double.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE DOUBLE MESSAGE WRAPPER

SCENARIO("Using a Double Message")
{
  GIVEN("A Double created from an empty constructor")
  {
    simple_msgs::Double double_;
    WHEN("We check the Double's value")
    {
      THEN("It has to be zero") { REQUIRE(double_.get() == 0); }
    }
  }
  GIVEN("A Double created from a double")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Double double_(d1);
    WHEN("We check the Double's value")
    {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(double_.get() == d1); }
    }
    WHEN("I construct a new Double from the serialized data of the existing Double")
    {
      simple_msgs::Double double2(double_.getBufferData());
      THEN("The new Double has to be equal to the other") { REQUIRE(double2 == double_); }
    }
    WHEN("I copy-construct a new Double")
    {
      simple_msgs::Double double3(double_);
      THEN("The new Double is equal to the other") { REQUIRE(double3 == double_); }
    }
    WHEN("I move-construct a new Double")
    {
      simple_msgs::Double double4(std::move(double_));
      THEN("The new Double's value is equal to the previous' one") { REQUIRE(double4.get() == double_.get()); }
    }
  }

  GIVEN("An instance of a Double.")
  {
    // start a Double
    simple_msgs::Double d;

    WHEN("I set the value of the Double")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      d.set(x);
      THEN("The Double's value is correct") { REQUIRE(d.get() == x); }
    }
  }
  GIVEN("Two identical Doubles")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Double double_1(x);
    simple_msgs::Double double_2(x);
    WHEN("I compare these Doubles")
    {
      THEN("They have to be equal") { REQUIRE(double_1 == double_2); }
    }
  }
  GIVEN("A Double")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Double double_(x);
    WHEN("I copy-assign from that Double's buffer")
    {
      simple_msgs::Double d2;
      d2 = double_.getBufferData();
      THEN("The new Double has to be same as the original") { REQUIRE(d2 == double_); }
    }
    WHEN("I copy-assign from that Double")
    {
      simple_msgs::Double d3;
      d3 = double_;
      THEN("The new Double has to be same as the original") { REQUIRE(d3 == double_); }
    }
    WHEN("I move-assign from that Double")
    {
      simple_msgs::Double d4;
      d4 = std::move(double_);
      THEN("The new Double has to be same as the original") { REQUIRE(d4.get() == x); }
    }
  }
  GIVEN("Two different Double")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = d1 + 1.0;
    simple_msgs::Double double1(d1);
    simple_msgs::Double double2(d2);
    WHEN("I compare these Doubles")
    {
      THEN("They have to be different") { REQUIRE(double1 != double2); }
    }
  }
  GIVEN("A Double")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Double double_(x);
    WHEN("I get the buffer of that Double and construct a new Double")
    {
      uint8_t* buf = double_.getBufferData();
      simple_msgs::Double d2(buf);
      THEN("The new Double is equal to the original") { REQUIRE(double_ == d2); }
    }
  }
  GIVEN("A Double")
  {
    double x = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::Double double_(x);
    WHEN("I increase the value of the double_")
    {
      double_++;
      THEN("The new value is correct") { REQUIRE(double_.get() == x + 1.0); }
    }
	WHEN("I decrease the value of the double_")
	{
		double_--;
		THEN("The new value is correct") { REQUIRE(double_.get() == x-1.0); }
	}
	WHEN("I add to the value of the double_")
	{
		double_+=2.0;
		THEN("The new value is correct") { REQUIRE(double_.get() == x + 2.0); }
	}
	WHEN("I subtract from the value of the double_")
	{
		double_-=5.0;
		THEN("The new value is correct") { REQUIRE(double_.get() == x - 5.0); }
	}
	WHEN("I add a double to the double_")
	{
		simple_msgs::Double double2 = double_+6.0;
		THEN("The new value is correct") { REQUIRE(double2.get() == x + 6.0); }
	}
	WHEN("I subtract a double of the double_")
	{
		simple_msgs::Double double3 = double_ - 6.0;
		THEN("The new value is correct") { REQUIRE(double3.get() == x - 6.0); }
	}
	WHEN("I multiply to the value of the double_")
	{
		double_ *= 2.0;
		THEN("The new value is correct") { REQUIRE(double_.get() == x * 2.0); }
	}
	WHEN("I divide from the value of the double_")
	{
		double_ /= 5.0;
		THEN("The new value is correct") { REQUIRE(double_.get() == x / 5.0); }
	}
	WHEN("I multiply a double to the double_")
	{
		simple_msgs::Double double2 = double_ * 6.0;
		THEN("The new value is correct") { REQUIRE(double2.get() == x * 6.0); }
	}
	WHEN("I divide the double by a double_")
	{
		simple_msgs::Double double3 = double_ / 6.0;
		THEN("The new value is correct") { REQUIRE(double3.get() == x / 6.0); }
	}
  }
}
