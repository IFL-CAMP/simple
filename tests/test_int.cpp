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
#include "simple_msgs/int.hpp"

using namespace simple_tests;

// TEST FOR USING THE Int MESSAGE WRAPPER

SCENARIO("Using a Int Message") {
  int int_1 = int_dist(generator);
  int int_2 = int_dist(generator);

  // Testing constructors.
  GIVEN("A Int created from an empty constructor") {
    simple_msgs::Int empty_Int{};
    WHEN("We check its value") {
      THEN("It has to be zero") { REQUIRE(empty_Int.get() == 0); }
    }
  }

  GIVEN("A Int created from a Int") {
    simple_msgs::Int single_Int{int_1};
    WHEN("We check the its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_Int.get() == int_1); }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A Int") {
    simple_msgs::Int single_Int{int_1};
    WHEN("I construct a new Int from the serialized data of the existing Int") {
      simple_msgs::Int copy_buffer_Int(single_Int.getBufferData()->data());
      THEN("The new Int is equal to the original") { REQUIRE(copy_buffer_Int == single_Int); }
    }
    WHEN("I copy-construct a new Int") {
      const simple_msgs::Int& copy_Int{single_Int};
      THEN("The new Int is equal to the original") { REQUIRE(copy_Int == single_Int); }
    }
    WHEN("I move-construct a new Int") {
      simple_msgs::Int moved_Int{std::move(single_Int)};
      THEN("The new Int contains the value that was contained in the orignal one") {
        REQUIRE(moved_Int.get() == int_1);
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A Int") {
    simple_msgs::Int single_Int{int_1};
    WHEN("I copy-assign from that Int's buffer") {
      simple_msgs::Int copy_assigned_buffer_Int{};
      auto data_ptr = std::make_shared<void*>(single_Int.getBufferData()->data());
      copy_assigned_buffer_Int = data_ptr;
      THEN("The new Int is equal to the original") { REQUIRE(copy_assigned_buffer_Int == single_Int); }
    }
    WHEN("I copy-assign from that Int") {
      simple_msgs::Int copy_assigned_Int{};
      copy_assigned_Int = single_Int;
      THEN("The new Int is equal to the original") { REQUIRE(copy_assigned_Int == single_Int); }
    }
    WHEN("I move-assign from that Int") {
      simple_msgs::Int move_assigned_Int{};
      move_assigned_Int = std::move(single_Int);
      THEN("The new Int is equal to the original") { REQUIRE(move_assigned_Int.get() == int_1); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a Int.") {
    // start a Int
    simple_msgs::Int d{};
    WHEN("I set the value of the Int") {
      int x = static_cast<int>(rand()) / RAND_MAX;
      d.set(x);
      THEN("Its value is correct") { REQUIRE(d.get() == x); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Ints") {
    simple_msgs::Int single_int_1{int_1};
    simple_msgs::Int single_int_2{int_1};
    WHEN("I compare these Ints") {
      THEN("They are equal") { REQUIRE(single_int_1 == single_int_2); }
    }
    WHEN("I change the first Int") {
      single_int_1.set(int_2);
      THEN("They are different") { REQUIRE(single_int_1 != single_int_2); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Int") {
    simple_msgs::Int single_Int(int_1);
    WHEN("I get the message topic") {
      std::string topic_name = single_Int.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "INTF"); }
    }
    WHEN("I print the Int") {
      std::ostringstream out_msg, out_check;
      out_msg << single_Int;
      out_check << int_1;
      THEN("The output is correct") { REQUIRE(out_msg.str() == out_check.str()); }
    }
  }
}
