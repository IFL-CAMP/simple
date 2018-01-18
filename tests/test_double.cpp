/**
 * S.I.M.P.L.E. - Smart Intra-operative Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/double.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE DOUBLE MESSAGE WRAPPER

SCENARIO("Using a Double Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  // Testing constructors.
  GIVEN("A Double created from an empty constructor")
  {
    simple_msgs::Double empty_double;
    WHEN("We check the Double's value")
    {
      THEN("It has to be zero") { REQUIRE(empty_double.get() == 0); }
    }
  }

  GIVEN("A Double created from a double")
  {
    simple_msgs::Double single_double(double_1);
    WHEN("We check the Double's value")
    {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(single_double.get() == double_1); }
    }
  }

  // Testing copy-constructors.
  GIVEN("A Double")
  {
    simple_msgs::Double single_double(double_1);
    WHEN("I construct a new Double from the serialized data of the existing Double")
    {
      simple_msgs::Double copy_buffer_double(single_double.getBufferData());
      THEN("The new Double has to be equal to the other") { REQUIRE(copy_buffer_double == single_double); }
    }
    WHEN("I copy-construct a new Double")
    {
      simple_msgs::Double copy_double(single_double);
      THEN("The new Double is equal to the other") { REQUIRE(copy_double == single_double); }
    }
    WHEN("I move-construct a new Double")
    {
      simple_msgs::Double moved_double(std::move(single_double));
      THEN("The new Double's value is equal to the previous' one")
      {
        REQUIRE(moved_double.get() == single_double.get());
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Double")
  {
    simple_msgs::Double single_double(double_1);
    WHEN("I copy-assign from that Double's buffer")
    {
      simple_msgs::Double copy_assigned_buffer_double;
      copy_assigned_buffer_double = single_double.getBufferData();
      THEN("The new Double has to be same as the original") { REQUIRE(copy_assigned_buffer_double == single_double); }
    }
    WHEN("I copy-assign from that Double")
    {
      simple_msgs::Double copy_assigned_double;
      copy_assigned_double = single_double;
      THEN("The new Double has to be same as the original") { REQUIRE(copy_assigned_double == single_double); }
    }
    WHEN("I move-assign from that Double")
    {
      simple_msgs::Double move_assigned_double;
      move_assigned_double = std::move(single_double);
      THEN("The new Double has to be same as the original") { REQUIRE(move_assigned_double.get() == double_1); }
    }
  }

  // Testing getter-setter
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

  // Testing operations.
  GIVEN("Two identical Doubles")
  {
    simple_msgs::Double single_double_1(double_1);
    simple_msgs::Double single_double_2(double_1);
    WHEN("I compare these Doubles")
    {
      THEN("They have to be equal") { REQUIRE(single_double_1 == single_double_2); }
    }
    WHEN("I change the first Double")
    {
      single_double_1.set(double_2);
      THEN("They have to be different") { REQUIRE(single_double_1 != single_double_2); }
    }
  }

  GIVEN("A Double")
  {
    simple_msgs::Double single_double(double_1);
    WHEN("I increase the value of the double_")
    {
      single_double++;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 + 1.0); }
    }
    WHEN("I decrease the value of the double_")
    {
      single_double--;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 - 1.0); }
    }
    WHEN("I add to the value of the double_")
    {
      single_double += 2.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 + 2.0); }
    }
    WHEN("I subtract from the value of the double_")
    {
      single_double -= 5.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 - 5.0); }
    }
    WHEN("I add a double to the double_")
    {
      simple_msgs::Double added_double = single_double + 6.0;
      THEN("The new value is correct") { REQUIRE(added_double.get() == double_1 + 6.0); }
    }
    WHEN("I subtract a double of the double_")
    {
      simple_msgs::Double subtracted_double = single_double - 6.0;
      THEN("The new value is correct") { REQUIRE(subtracted_double.get() == double_1 - 6.0); }
    }
    WHEN("I multiply to the value of the double_")
    {
      single_double *= 2.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 * 2.0); }
    }
    WHEN("I divide from the value of the double_")
    {
      single_double /= 5.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 / 5.0); }
    }
    WHEN("I multiply a double to the double_")
    {
      simple_msgs::Double multiplied_double = single_double * 6.0;
      THEN("The new value is correct") { REQUIRE(multiplied_double.get() == double_1 * 6.0); }
    }
    WHEN("I divide the double by a double_")
    {
      simple_msgs::Double divided_double = single_double / 6.0;
      THEN("The new value is correct") { REQUIRE(divided_double.get() == double_1 / 6.0); }
    }
    WHEN("I get the message topic")
    {
      std::string topic_name = single_double.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "DOBL"); }
    }
  }
}
