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

#include "random_generators.hpp"
#include "simple_msgs/float.hpp"

using namespace simple_tests;

// TEST FOR USING THE Float MESSAGE WRAPPER

SCENARIO("Using a Float Message") {
  float float_1 = static_cast<float>(double_dist(generator));
  float float_2 = static_cast<float>(double_dist(generator));

  // Testing constructors.
  GIVEN("A Float created from an empty constructor") {
    simple_msgs::Float empty_Float{};
    WHEN("We check its value") {
      THEN("It has to be zero") { REQUIRE(empty_Float.get() == Approx(0)); }
    }
  }

  GIVEN("A Float created from a Float") {
    simple_msgs::Float single_Float{float_1};
    WHEN("We check the its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_Float.get() == Approx(float_1)); }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A Float") {
    simple_msgs::Float single_Float{float_1};
    WHEN("I copy-construct a new Float") {
      const simple_msgs::Float& copy_Float{single_Float};
      THEN("The new Float is equal to the original") { REQUIRE(copy_Float == single_Float); }
    }
    WHEN("I move-construct a new Float") {
      simple_msgs::Float moved_Float{std::move(single_Float)};
      THEN("The new Float contains the value that was contained in the orignal one") {
        REQUIRE(moved_Float.get() == Approx(float_1));
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A Float") {
    simple_msgs::Float single_float{float_1};
    WHEN("I copy-assign from that Float") {
      simple_msgs::Float copy_assigned_float{};
      copy_assigned_float = single_float;
      THEN("The new Float is equal to the original") { REQUIRE(copy_assigned_float == single_float); }
    }
    WHEN("I move-assign from that Float") {
      simple_msgs::Float move_assigned_float{};
      move_assigned_float = std::move(single_float);
      THEN("The new Float is equal to the original") { REQUIRE(move_assigned_float.get() == Approx(float_1)); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a Float.") {
    // start a Float
    simple_msgs::Float d{};
    WHEN("I set the value of the Float") {
      float x = static_cast<float>(rand()) / RAND_MAX;
      d.set(x);
      THEN("Its value is correct") { REQUIRE(d.get() == Approx(x)); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Floats") {
    simple_msgs::Float single_float_1{float_1};
    simple_msgs::Float single_float_2{float_1};
    WHEN("I compare these Floats") {
      THEN("They are equal") { REQUIRE(single_float_1 == single_float_2); }
    }
    WHEN("I change the first Float") {
      single_float_1.set(float_2);
      THEN("They are different") { REQUIRE(single_float_1 != single_float_2); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Float") {
    simple_msgs::Float single_Float(float_1);
    WHEN("I get the message topic") {
      std::string topic_name = simple_msgs::Float::getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "FLOT"); }
    }
    WHEN("I print the Float") {
      std::ostringstream out_msg, out_check;
      out_msg << single_Float;
      out_check << float_1;
      THEN("The output is correct") { REQUIRE(out_msg.str() == out_check.str()); }
    }
  }
}
