#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/quaternion_stamped.h"
#include "simple_msgs/header.h"
#include "simple_msgs/quaternion.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE STAMPED QUATERNION MESSAGE WRAPPER

SCENARIO("Using a QuaternionStamped Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  double double_4 = static_cast<double>(rand()) / RAND_MAX;
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::Quaternion random_quaternion(double_1, double_2, double_3, double_4);
  simple_msgs::Quaternion empty_quaternion;
  simple_msgs::Header empty_header;
  simple_msgs::Header random_header(random_int, random_string, double_1);
  // Test the constructors.
  GIVEN("A QuaternionStamped created from an empty constructor")
  {
    simple_msgs::QuaternionStamped empty_quaternion_stamped;
    WHEN("It is constructed")
    {
      THEN("It's elements have to match the default")
      {
        REQUIRE(empty_quaternion_stamped.getHeader() == empty_header);
        REQUIRE(empty_quaternion_stamped.getQuaternion() == empty_quaternion);
      }
    }
  }

  GIVEN("A QuaternionStamped created from a quaternion and a header")
  {
    simple_msgs::QuaternionStamped quaternion_stamped(random_header, random_quaternion);
    WHEN("I check the QuaternionStamped's elements")
    {
      THEN("They all have to be equal to the parameters from the constructor")
      {
        REQUIRE(quaternion_stamped.getQuaternion() == random_quaternion);
        REQUIRE(quaternion_stamped.getHeader() == random_header);
      }
    }
  }

  GIVEN("A QuaternionStamped created from the serialized data of an existing QuaternionStamped")
  {
    simple_msgs::QuaternionStamped quaternion_stamped(random_header, random_quaternion);
    simple_msgs::QuaternionStamped buffer_quaternion_stamped(quaternion_stamped.getBufferData());
    WHEN("I check the QuaternionStamped's elements")
    {
      THEN("The new QuaternionStamped has to be equal to the other") { REQUIRE(buffer_quaternion_stamped == quaternion_stamped); }
    }
  }

  // Testing copy constructors.
  GIVEN("A QuaternionStamped")
  {
    simple_msgs::QuaternionStamped quaternion_stamped(random_header, random_quaternion);
    WHEN("I copy-construct a new QuaternionStamped")
    {
      simple_msgs::QuaternionStamped copy_quaternion_stamped(quaternion_stamped);
      THEN("The new QuaternionStamped is equal to the other") { REQUIRE(copy_quaternion_stamped == quaternion_stamped); }
    }
    WHEN("I move-construct a new QuaternionStamped")
    {
      simple_msgs::QuaternionStamped move_quaternion_stamped(std::move(quaternion_stamped));
      THEN("The new QuaternionStamped's elements are equal to the previous' ones")
      {
        REQUIRE(move_quaternion_stamped.getQuaternion() == random_quaternion);
        REQUIRE(move_quaternion_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A QuaternionStamped")
  {
    simple_msgs::QuaternionStamped quaternion_stamped(random_header, random_quaternion);
    WHEN("I copy-assign from that QuaternionStamped's buffer")
    {
      simple_msgs::QuaternionStamped copy_assigned_buffer_quaternion_stamped;
      copy_assigned_buffer_quaternion_stamped = quaternion_stamped.getBufferData();
      THEN("The new QuaternionStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_buffer_quaternion_stamped == quaternion_stamped);
      }
    }
    WHEN("I copy-assign from that QuaternionStamped")
    {
      simple_msgs::QuaternionStamped copy_assigned_quaternion_stamped;
      copy_assigned_quaternion_stamped = quaternion_stamped;
      THEN("The new QuaternionStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_quaternion_stamped == quaternion_stamped);
      }
    }
    WHEN("I move-assign from that QuaternionStamped")
    {
      simple_msgs::QuaternionStamped move_assigned_quaternion_stamped;
      move_assigned_quaternion_stamped = std::move(quaternion_stamped);
      THEN("The new QuaternionStamped has to be same as the original")
      {
        REQUIRE(move_assigned_quaternion_stamped.getQuaternion() == random_quaternion);
        REQUIRE(move_assigned_quaternion_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A QuaternionStamped")
  {
	  simple_msgs::QuaternionStamped quaternion_stamped;
    WHEN("I set the quaternion of the quaternion_stamped")
    {
      quaternion_stamped.setQuaternion(random_quaternion);
      THEN("The position is correct") { REQUIRE(quaternion_stamped.getQuaternion() == random_quaternion); }
    }
    WHEN("I set the header of the quaternion_stamped")
    {
      quaternion_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(quaternion_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical QuaternionStampeds")
  {
    simple_msgs::QuaternionStamped first_quaternion_stamped(random_header, random_quaternion);
    simple_msgs::QuaternionStamped second_quaternion_stamped(random_header, random_quaternion);
    WHEN("I compare these QuaternionStampeds")
    {
      THEN("They have to be equal") { REQUIRE(first_quaternion_stamped == second_quaternion_stamped); }
    }
    WHEN("I change something in the first quaternion_stamped")
    {
      first_quaternion_stamped.getQuaternion().setX(double_4);
      THEN("They have to be different") { REQUIRE(first_quaternion_stamped != second_quaternion_stamped); }
    }
  }

  // Testing Topic
  GIVEN("A point")
  {
    simple_msgs::QuaternionStamped quaternion_stamped;
    WHEN("I get the message topic")
    {
      std::string topic_name = quaternion_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "QTST"); }
    }
  }
}
