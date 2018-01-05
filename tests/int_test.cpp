#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/int.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE INT MESSAGE WRAPPER

SCENARIO("Using a Int Message")
{
  GIVEN("A Int created from an empty constructor")
  {
    simple_msgs::Int int_;
    WHEN("We check the Int's value")
    {
      THEN("It has to be zero") { REQUIRE(int_.get() == 0); }
    }
  }
  GIVEN("A Int created from a int")
  {
    int d1 = static_cast<int>(rand()) / RAND_MAX;
    simple_msgs::Int int_(d1);
    WHEN("We check the Int's value")
    {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(int_.get() == d1); }
    }
    WHEN("I construct a new Int from the serialized data of the existing Int")
    {
      simple_msgs::Int int2(int_.getBufferData());
      THEN("The new Int has to be equal to the other") { REQUIRE(int2 == int_); }
    }
    WHEN("I copy-construct a new Int")
    {
      simple_msgs::Int int3(int_);
      THEN("The new Int is equal to the other") { REQUIRE(int3 == int_); }
    }
    WHEN("I move-construct a new Int")
    {
      simple_msgs::Int int4(std::move(int_));
      THEN("The new Int's value is equal to the previous' one") { REQUIRE(int4.get() == int_.get()); }
    }
  }

  GIVEN("An instance of a Int.")
  {
    // start a Int
    simple_msgs::Int d;

    WHEN("I set the value of the Int")
    {
      int x = static_cast<int>(rand()) / RAND_MAX;
      d.set(x);
      THEN("The Int's value is correct") { REQUIRE(d.get() == x); }
    }
  }
  GIVEN("Two identical Ints")
  {
    int x = static_cast<int>(rand()) / RAND_MAX;
    simple_msgs::Int int_1(x);
    simple_msgs::Int int_2(x);
    WHEN("I compare these Ints")
    {
      THEN("They have to be equal") { REQUIRE(int_1 == int_2); }
    }
  }
  GIVEN("A Int")
  {
    int x = static_cast<int>(rand()) / RAND_MAX;
    simple_msgs::Int int_(x);
    WHEN("I copy-assign from that Int's buffer")
    {
      simple_msgs::Int d2;
      d2 = int_.getBufferData();
      THEN("The new Int has to be same as the original") { REQUIRE(d2 == int_); }
    }
    WHEN("I copy-assign from that Int")
    {
      simple_msgs::Int d3;
      d3 = int_;
      THEN("The new Int has to be same as the original") { REQUIRE(d3 == int_); }
    }
    WHEN("I move-assign from that Int")
    {
      simple_msgs::Int d4;
      d4 = std::move(int_);
      THEN("The new Int has to be same as the original") { REQUIRE(d4.get() == x); }
    }
  }
  GIVEN("Two different Int")
  {
    int d1 = static_cast<int>(rand()) / RAND_MAX;
    int d2 = d1 + 1;
    simple_msgs::Int int1(d1);
    simple_msgs::Int int2(d2);
    WHEN("I compare these Ints")
    {
      THEN("They have to be different") { REQUIRE(int1 != int2); }
    }
  }
  GIVEN("A Int")
  {
    int x = static_cast<int>(rand()) / RAND_MAX;
    simple_msgs::Int int_(x);
    WHEN("I get the buffer of that Int and construct a new Int")
    {
      uint8_t* buf = int_.getBufferData();
      simple_msgs::Int d2(buf);
      THEN("The new Int is equal to the original") { REQUIRE(int_ == d2); }
    }
  }
  GIVEN("A Int")
  {
    int x = static_cast<int>(rand()) / RAND_MAX;
    simple_msgs::Int int_(x);
    WHEN("I increase the value of the int_")
    {
      int_++;
      THEN("The new value is correct") { REQUIRE(int_.get() == x + 1); }
    }
	WHEN("I decrease the value of the int_")
	{
		int_--;
		THEN("The new value is correct") { REQUIRE(int_.get() == x-1); }
	}
	WHEN("I add to the value of the int_")
	{
		int_+=2;
		THEN("The new value is correct") { REQUIRE(int_.get() == x + 2); }
	}
	WHEN("I subtract from the value of the int_")
	{
		int_-=5;
		THEN("The new value is correct") { REQUIRE(int_.get() == x - 5); }
	}
	WHEN("I add a int to the int_")
	{
		simple_msgs::Int int2 = int_+6;
		THEN("The new value is correct") { REQUIRE(int2.get() == x + 6); }
	}
	WHEN("I subtract a int of the int_")
	{
		simple_msgs::Int int3 = int_ - 6;
		THEN("The new value is correct") { REQUIRE(int3.get() == x - 6); }
	}
	WHEN("I multiply to the value of the int_")
	{
		int_ *= 2;
		THEN("The new value is correct") { REQUIRE(int_.get() == x * 2); }
	}
	WHEN("I divide from the value of the int_")
	{
		int_ /= 5;
		THEN("The new value is correct") { REQUIRE(int_.get() == x / 5); }
	}
	WHEN("I multiply a int to the int_")
	{
		simple_msgs::Int int2 = int_ * 6;
		THEN("The new value is correct") { REQUIRE(int2.get() == x * 6); }
	}
	WHEN("I divide the int by a int_")
	{
		simple_msgs::Int int3 = int_ / 6;
		THEN("The new value is correct") { REQUIRE(int3.get() == x / 6); }
	}
  }
}
