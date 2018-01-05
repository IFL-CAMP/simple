#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/float.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE FLOAT MESSAGE WRAPPER

SCENARIO("Using a Float Message")
{
  GIVEN("A Float created from an empty constructor")
  {
    simple_msgs::Float float_;
    WHEN("We check the Float's value")
    {
      THEN("It has to be zero") { REQUIRE(float_.get() == 0); }
    }
  }
  GIVEN("A Float created from a float")
  {
    float d1 = static_cast<float>(rand()) / RAND_MAX;
    simple_msgs::Float float_(d1);
    WHEN("We check the Float's value")
    {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(float_.get() == d1); }
    }
    WHEN("I construct a new Float from the serialized data of the existing Float")
    {
      simple_msgs::Float float2(float_.getBufferData());
      THEN("The new Float has to be equal to the other") { REQUIRE(float2 == float_); }
    }
    WHEN("I copy-construct a new Float")
    {
      simple_msgs::Float float3(float_);
      THEN("The new Float is equal to the other") { REQUIRE(float3 == float_); }
    }
    WHEN("I move-construct a new Float")
    {
      simple_msgs::Float float4(std::move(float_));
      THEN("The new Float's value is equal to the previous' one") { REQUIRE(float4.get() == float_.get()); }
    }
  }

  GIVEN("An instance of a Float.")
  {
    // start a Float
    simple_msgs::Float d;

    WHEN("I set the value of the Float")
    {
      float x = static_cast<float>(rand()) / RAND_MAX;
      d.set(x);
      THEN("The Float's value is correct") { REQUIRE(d.get() == x); }
    }
  }
  GIVEN("Two identical Floats")
  {
    float x = static_cast<float>(rand()) / RAND_MAX;
    simple_msgs::Float float_1(x);
    simple_msgs::Float float_2(x);
    WHEN("I compare these Floats")
    {
      THEN("They have to be equal") { REQUIRE(float_1 == float_2); }
    }
  }
  GIVEN("A Float")
  {
    float x = static_cast<float>(rand()) / RAND_MAX;
    simple_msgs::Float float_(x);
    WHEN("I copy-assign from that Float's buffer")
    {
      simple_msgs::Float d2;
      d2 = float_.getBufferData();
      THEN("The new Float has to be same as the original") { REQUIRE(d2 == float_); }
    }
    WHEN("I copy-assign from that Float")
    {
      simple_msgs::Float d3;
      d3 = float_;
      THEN("The new Float has to be same as the original") { REQUIRE(d3 == float_); }
    }
    WHEN("I move-assign from that Float")
    {
      simple_msgs::Float d4;
      d4 = std::move(float_);
      THEN("The new Float has to be same as the original") { REQUIRE(d4.get() == x); }
    }
  }
  GIVEN("Two different Float")
  {
    float d1 = static_cast<float>(rand()) / RAND_MAX;
    float d2 = d1 + 1.0f;
    simple_msgs::Float float1(d1);
    simple_msgs::Float float2(d2);
    WHEN("I compare these Floats")
    {
      THEN("They have to be different") { REQUIRE(float1 != float2); }
    }
  }
  GIVEN("A Float")
  {
    float x = static_cast<float>(rand()) / RAND_MAX;
    simple_msgs::Float float_(x);
    WHEN("I get the buffer of that Float and construct a new Float")
    {
      uint8_t* buf = float_.getBufferData();
      simple_msgs::Float d2(buf);
      THEN("The new Float is equal to the original") { REQUIRE(float_ == d2); }
    }
  }
  GIVEN("A Float")
  {
    float x = static_cast<float>(rand()) / RAND_MAX;
    simple_msgs::Float float_(x);
    WHEN("I increase the value of the float_")
    {
      float_++;
      THEN("The new value is correct") { REQUIRE(float_.get() == x + 1.0f); }
    }
	WHEN("I decrease the value of the float_")
	{
		float_--;
		THEN("The new value is correct") { REQUIRE(float_.get() == x-1.0f); }
	}
	WHEN("I add to the value of the float_")
	{
		float_+=2.0f;
		THEN("The new value is correct") { REQUIRE(float_.get() == x + 2.0f); }
	}
	WHEN("I subtract from the value of the float_")
	{
		float_-=5.0f;
		THEN("The new value is correct") { REQUIRE(float_.get() == x - 5.0f); }
	}
	WHEN("I add a float to the float_")
	{
		simple_msgs::Float float2 = float_+6.0f;
		THEN("The new value is correct") { REQUIRE(float2.get() == x + 6.0f); }
	}
	WHEN("I subtract a float of the float_")
	{
		simple_msgs::Float float3 = float_ - 6.0f;
		THEN("The new value is correct") { REQUIRE(float3.get() == x - 6.0f); }
	}
	WHEN("I multiply to the value of the float_")
	{
		float_ *= 2.0f;
		THEN("The new value is correct") { REQUIRE(float_.get() == x * 2.0f); }
	}
	WHEN("I divide from the value of the float_")
	{
		float_ /= 5.0f;
		THEN("The new value is correct") { REQUIRE(float_.get() == x / 5.0f); }
	}
	WHEN("I multiply a float to the float_")
	{
		simple_msgs::Float float2 = float_ * 6.0f;
		THEN("The new value is correct") { REQUIRE(float2.get() == x * 6.0f); }
	}
	WHEN("I divide the float by a float_")
	{
		simple_msgs::Float float3 = float_ / 6.0f;
		THEN("The new value is correct") { REQUIRE(float3.get() == x / 6.0f); }
	}
  }
}
