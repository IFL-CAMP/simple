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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "simple_msgs/double.hpp"

// TEST FOR USING THE DOUBLE MESSAGE WRAPPER

SCENARIO("Using a Double Message") {
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  // Testing constructors.
  GIVEN("A Double created from an empty constructor") {
    simple_msgs::Double empty_double{};
    WHEN("We check its value") {
      THEN("It has to be zero") { REQUIRE(empty_double.get() == 0); }
    }
  }

  GIVEN("A Double created from a double") {
    simple_msgs::Double single_double{double_1};
    WHEN("We check the its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_double.get() == double_1); }
    }
  }

  // Testing copy-constructors.
  GIVEN("A Double") {
    simple_msgs::Double single_double{double_1};
    WHEN("I construct a new Double from the serialized data of the existing Double") {
      simple_msgs::Double copy_buffer_double(single_double.getBufferData()->data());
      THEN("The new Double is equal to the original") { REQUIRE(copy_buffer_double == single_double); }
    }
    WHEN("I copy-construct a new Double") {
      const simple_msgs::Double& copy_double{single_double};
      THEN("The new Double is equal to the original") { REQUIRE(copy_double == single_double); }
    }
    WHEN("I move-construct a new Double") {
      simple_msgs::Double moved_double{std::move(single_double)};
      THEN("The new Double contains the value that was contained in the orignal one") {
        REQUIRE(moved_double.get() == double_1);
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Double") {
    simple_msgs::Double single_double{double_1};
    WHEN("I copy-assign from that Double's buffer") {
      simple_msgs::Double copy_assigned_buffer_double{};
      auto data_ptr = std::make_shared<void*>(single_double.getBufferData()->data());
      copy_assigned_buffer_double = data_ptr;
      THEN("The new Double is equal to the original") { REQUIRE(copy_assigned_buffer_double == single_double); }
    }
    WHEN("I copy-assign from that Double") {
      simple_msgs::Double copy_assigned_double{};
      copy_assigned_double = single_double;
      THEN("The new Double is equal to the original") { REQUIRE(copy_assigned_double == single_double); }
    }
    WHEN("I move-assign from that Double") {
      simple_msgs::Double move_assigned_double{};
      move_assigned_double = std::move(single_double);
      THEN("The new Double is equal to the original") { REQUIRE(move_assigned_double.get() == double_1); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a Double.") {
    // start a Double
    simple_msgs::Double d{};
    WHEN("I set the value of the Double") {
      double x = static_cast<double>(rand()) / RAND_MAX;
      d.set(x);
      THEN("Its value is correct") { REQUIRE(d.get() == x); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Doubles") {
    simple_msgs::Double single_double_1{double_1};
    simple_msgs::Double single_double_2{double_1};
    WHEN("I compare these Doubles") {
      THEN("They are equal") { REQUIRE(single_double_1 == single_double_2); }
    }
    WHEN("I change the first Double") {
      single_double_1.set(double_2);
      THEN("They are different") { REQUIRE(single_double_1 != single_double_2); }
    }
  }

  GIVEN("A Double") {
    simple_msgs::Double single_double(double_1);
    WHEN("I increase its value (operator++)") {
      single_double++;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 + 1.0); }
    }
    WHEN("I decrease its value (operator--)") {
      single_double--;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 - 1.0); }
    }
    WHEN("I add a value to it (operator+)") {
      single_double += 2.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 + 2.0); }
    }
    WHEN("I subtract a value to it (operator-)") {
      single_double -= 5.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 - 5.0); }
    }
    WHEN("I multiply its value (operator*)") {
      single_double *= 2.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 * 2.0); }
    }
    WHEN("I divide its value (operator/)") {
      single_double /= 5.0;
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 / 5.0); }
    }
    WHEN("I add another Double to it") {
      single_double = single_double + simple_msgs::Double{6.0};
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 + 6.0); }
    }
    WHEN("I subtract another Double to it") {
      single_double = single_double - simple_msgs::Double{6.0};
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 - 6.0); }
    }
    WHEN("I multiply another Double to it") {
      single_double = single_double * simple_msgs::Double{6.0};
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 * 6.0); }
    }
    WHEN("I divide another Double to it") {
      single_double = single_double / simple_msgs::Double{6.0};
      THEN("The new value is correct") { REQUIRE(single_double.get() == double_1 / 6.0); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = single_double.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "DOBL"); }
    }
    WHEN("I print the double") {
      std::ostringstream out_msg, out_check;
      out_msg << single_double;
      out_check << double_1;
      THEN("The output is correct") { REQUIRE(out_msg.str() == out_check.str()); }
    }
  }
}
