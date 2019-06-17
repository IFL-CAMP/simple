/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include "simple_msgs/string.hpp"

// TEST FOR USING THE STRING MESSAGE WRAPPER

SCENARIO("Using a String Message") {
  std::string string_1{"abcd"};
  std::string string_2{"efg"};
  const char* char_1{"hijk"};

  // Testing constructors.
  GIVEN("A String created from an empty constructor") {
    simple_msgs::String empty_string{};
    WHEN("We check the String's value") {
      THEN("It has to be empty") { REQUIRE(empty_string.empty()); }
    }
  }

  GIVEN("A String created from a string") {
    simple_msgs::String single_string{string_1};
    WHEN("We check the String's value") {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(single_string.get() == string_1); }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A String") {
    simple_msgs::String single_string{string_1};
    WHEN("I copy-construct a new String") {
      simple_msgs::String copy_string{single_string};
      THEN("The new String is equal to the other") { REQUIRE(copy_string == single_string); }
    }
    WHEN("I move-construct a new String") {
      simple_msgs::String moved_string{std::move(single_string)};
      THEN("The new String's value is equal to the previous' one") { REQUIRE(moved_string.get() == string_1); }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A String") {
    simple_msgs::String single_string{string_1};
    WHEN("I copy-assign from that String") {
      simple_msgs::String copy_assigned_string{};
      copy_assigned_string = single_string;
      THEN("The new String has to be same as the original") { REQUIRE(copy_assigned_string == single_string); }
    }
    WHEN("I move-assign from that String") {
      simple_msgs::String move_assigned_string{};
      move_assigned_string = std::move(single_string);
      THEN("The new String has to be same as the original") { REQUIRE(move_assigned_string.get() == string_1); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a String.") {
    simple_msgs::String string_msg{};
    WHEN("I set the value of the String") {
      string_msg.set(string_2);
      THEN("The String's value is correct") { REQUIRE(string_msg.get() == string_2); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Strings") {
    simple_msgs::String single_string_1{string_1};
    simple_msgs::String single_string_2{string_1};
    WHEN("I compare these Strings") {
      THEN("They have to be equal") { REQUIRE(single_string_1 == single_string_2); }
    }
    WHEN("I change the first String") {
      single_string_1.set(string_2);
      THEN("They have to be different") { REQUIRE(single_string_1 != single_string_2); }
    }
  }

  GIVEN("Two Strings") {
    simple_msgs::String single_string_1{string_1};
    simple_msgs::String single_string_2{string_2};
    WHEN("I append one String to the other") {
      single_string_1 += single_string_2;
      THEN("The new value is correct") { REQUIRE(single_string_1.get() == string_1 + string_2); }
    }
    WHEN("I add both Strings to a new string") {
      simple_msgs::String added_string = single_string_1 + single_string_2;
      THEN("The new value is correct") { REQUIRE(added_string.get() == string_1 + string_2); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A String") {
    simple_msgs::String string{string_1};

    WHEN("I get the message topic") {
      std::string topic_name = simple_msgs::String::getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "STRG"); }
    }
    WHEN("I print the String") {
      std::ostringstream out;
      out << string;
      THEN("The output is correct") { REQUIRE(out.str() == string_1); }
    }
  }
}
