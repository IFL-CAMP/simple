/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "simple_msgs/bool.h"

// TEST FOR USING THE BOOL MESSAGE WRAPPER

SCENARIO("Using a Bool Message") {
  bool true_boolean{true}, false_boolean{false};
  // Testing constructors.
  GIVEN("A Bool created from an empty constructor") {
    simple_msgs::Bool empty_bool{};
    WHEN("We check its value") {
      THEN("It has to be false") { REQUIRE(empty_bool.get() == false_boolean); }
    }
  }

  GIVEN("A Bool created from a bool") {
    simple_msgs::Bool single_bool(true_boolean);
    WHEN("We check its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_bool.get() == true_boolean); }
    }
  }

  // Testing copy-constructors.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(true_boolean);
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
        REQUIRE(moved_bool.get() == true_boolean);
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(false_boolean);
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
        REQUIRE(move_assigned_bool.get() == false_boolean);
      }
    }
  }

  // Testing getter-setter
  GIVEN("A Bool") {
    // start a Bool
    simple_msgs::Bool bool_msg{};
    WHEN("I set its value") {
      bool_msg.set(true_boolean);
      THEN("The Bool contains the correct value") { REQUIRE(bool_msg.get() == true_boolean); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Bools") {
    simple_msgs::Bool single_true_boolean{true_boolean};
    simple_msgs::Bool single_false_boolean{true_boolean};
    WHEN("I compare them") {
      THEN("They are equal") { REQUIRE(single_true_boolean == single_false_boolean); }
    }
    WHEN("I change the value of one of them") {
      single_true_boolean.set(false_boolean);
      THEN("They are different") { REQUIRE(single_true_boolean != single_false_boolean); }
    }
    WHEN("I compare one to the negative value of the other") {
      THEN("They are different") { REQUIRE((!single_true_boolean) != single_false_boolean); }
    }
  }

  GIVEN("A Bool") {
    simple_msgs::Bool single_bool{true_boolean};
    WHEN("I get the message topic") {
      std::string topic_name = single_bool.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "BOOL"); }
    }
    WHEN("I print the Bool") {
      std::ostringstream out;
      out << single_bool;
      THEN("The ostream is correct") { REQUIRE(out.str() == "1"); }
    }
  }
}
