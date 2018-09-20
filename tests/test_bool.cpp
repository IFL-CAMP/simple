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
#include "simple_msgs/bool.h"

// TEST FOR USING THE BOOL MESSAGE WRAPPER

SCENARIO("Using a Bool Message") {
  // Testing constructors.
  GIVEN("A Bool created from an empty constructor") {
    simple_msgs::Bool empty_bool{};
    WHEN("We check its value") {
      THEN("It has to be false") { REQUIRE(empty_bool.get() == false); }
    }
  }

  GIVEN("A Bool created from a bool") {
    simple_msgs::Bool single_bool(true);
    WHEN("We check its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_bool.get() == true); }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(true);
    WHEN("I construct a new Bool from the serialized data of the existing "
         "Bool") {
      simple_msgs::Bool copy_buffer_bool(single_bool.getBufferData()->data());
      THEN("The new Bool has to be equal to the other") { REQUIRE(copy_buffer_bool == single_bool); }
    }
    WHEN("I copy-construct a new Bool") {
      const simple_msgs::Bool copy_bool(single_bool);
      THEN("The new Bool is equal to the other one") { REQUIRE(copy_bool == single_bool); }
    }
    WHEN("I move-construct a new Bool") {
      simple_msgs::Bool moved_bool(std::move(single_bool));
      THEN("The new Bool contains the value that was contained in the original one") {
        REQUIRE(moved_bool.get() == true);
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(false);
    WHEN("I copy-assign using the Bool's buffer data") {
      simple_msgs::Bool copy_assigned_buffer_bool{};
      auto data_ptr = std::make_shared<void*>(single_bool.getBufferData()->data());
      copy_assigned_buffer_bool = data_ptr;
      THEN("The new Bool is equal to the other one") { REQUIRE(copy_assigned_buffer_bool == single_bool); }
    }
    WHEN("I copy-assign from that Bool") {
      simple_msgs::Bool copy_assigned_bool{};
      copy_assigned_bool = single_bool;
      THEN("The new Bool is equal to the other one") { REQUIRE(copy_assigned_bool == single_bool); }
    }
    WHEN("I move-assign from that Bool") {
      simple_msgs::Bool move_assigned_bool{};
      move_assigned_bool = std::move(single_bool);
      THEN("The new Bool contains the value that was contained in the original one") {
        REQUIRE(move_assigned_bool.get() == false);
      }
    }
  }

  // Testing getter-setter
  GIVEN("A Bool") {
    simple_msgs::Bool bool_msg{};
    WHEN("I set its value") {
      bool_msg.set(true);
      THEN("The Bool contains the correct value") { REQUIRE(bool_msg.get() == true); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Bools") {
    simple_msgs::Bool boolean_1{true};
    simple_msgs::Bool boolean_2{true};
    WHEN("I compare them") {
      THEN("They are equal") { REQUIRE(boolean_1 == boolean_2); }
    }
    WHEN("I change the value of one of them") {
      boolean_1.set(false);
      THEN("They are different") { REQUIRE(boolean_1 != boolean_2); }
    }
    WHEN("I compare one to the negative value of the other") {
      THEN("They are different") { REQUIRE((!boolean_1) != boolean_2); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Bool") {
    simple_msgs::Bool boolean{true};
    WHEN("I get the message topic") {
      std::string topic_name = boolean.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "BOOL"); }
    }
    WHEN("I print the Bool") {
      std::ostringstream out;
      out << boolean;
      THEN("The ostream is correct") { REQUIRE(out.str() == "1"); }
    }
  }
}
