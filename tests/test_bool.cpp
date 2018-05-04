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
  bool bool_1 = true;
  bool bool_2 = false;
  // Testing constructors.
  GIVEN("A Bool created from an empty constructor") {
    simple_msgs::Bool empty_bool;
    WHEN("We check the Bool's value") {
      THEN("It has to be false") { REQUIRE(empty_bool.get() == false); }
    }
  }

  GIVEN("A Bool created from a bool") {
    simple_msgs::Bool single_bool(bool_1);
    WHEN("We check the Bool's value") {
      THEN("It has to be equal to the params from the constructor") { REQUIRE(single_bool.get() == bool_1); }
    }
  }

  // Testing copy-constructors.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(bool_1);
    WHEN("I construct a new Bool from the serialized data of the existing "
         "Bool") {
      simple_msgs::Bool copy_buffer_bool(single_bool.getBufferData()->data());
      THEN("The new Bool has to be equal to the other") { REQUIRE(copy_buffer_bool == single_bool); }
    }
    WHEN("I copy-construct a new Bool") {
      const simple_msgs::Bool copy_bool(single_bool);
      THEN("The new Bool is equal to the other") { REQUIRE(copy_bool == single_bool); }
    }
    WHEN("I move-construct a new Bool") {
      simple_msgs::Bool moved_bool(std::move(single_bool));
      THEN("The new Bool's value is equal to the previous' one") { REQUIRE(moved_bool.get() == bool_1); }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(bool_2);
    WHEN("I copy-assign from that Bool's buffer") {
      simple_msgs::Bool copy_assigned_buffer_bool;
      copy_assigned_buffer_bool = single_bool.getBufferData()->data();
      THEN("The new Bool has to be same as the original") { REQUIRE(copy_assigned_buffer_bool == single_bool); }
    }
    WHEN("I copy-assign from that Bool") {
      simple_msgs::Bool copy_assigned_bool;
      copy_assigned_bool = single_bool;
      THEN("The new Bool has to be same as the original") { REQUIRE(copy_assigned_bool == single_bool); }
    }
    WHEN("I move-assign from that Bool") {
      simple_msgs::Bool move_assigned_bool;
      move_assigned_bool = std::move(single_bool);
      THEN("The new Bool has to be same as the original") { REQUIRE(move_assigned_bool.get() == bool_2); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a Bool.") {
    // start a Bool
    simple_msgs::Bool bool_msg;

    WHEN("I set the value of the Bool") {
      bool_msg.set(true);
      THEN("The Bool's value is correct") { REQUIRE(bool_msg.get() == true); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Bools") {
    simple_msgs::Bool single_bool_1(bool_1);
    simple_msgs::Bool single_bool_2(bool_1);
    WHEN("I compare these Bools") {
      THEN("They have to be equal") { REQUIRE(single_bool_1 == single_bool_2); }
    }
    WHEN("I change the first Bool") {
      single_bool_1.set(bool_2);
      THEN("They have to be different") { REQUIRE(single_bool_1 != single_bool_2); }
    }
    WHEN("I take the negative of one of the bools") {
      THEN("They have to be different") { REQUIRE((!single_bool_1) != single_bool_2); }
    }
  }

  GIVEN("A Bool") {
    simple_msgs::Bool single_bool(bool_1);

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
