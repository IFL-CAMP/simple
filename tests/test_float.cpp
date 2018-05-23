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
#include "simple_msgs/float.hpp"

// TEST FOR USING THE Float MESSAGE WRAPPER

SCENARIO("Using a Float Message") {
  float float_1 = static_cast<float>(rand()) / RAND_MAX;
  float float_2 = static_cast<float>(rand()) / RAND_MAX;
  // Testing constructors.
  GIVEN("A Float created from an empty constructor") {
    simple_msgs::Float empty_Float{};
    WHEN("We check its value") {
      THEN("It has to be zero") { REQUIRE(empty_Float.get() == 0); }
    }
  }

  GIVEN("A Float created from a Float") {
    simple_msgs::Float single_Float{float_1};
    WHEN("We check the its value") {
      THEN("It has to be equal to the given parameter") { REQUIRE(single_Float.get() == float_1); }
    }
  }

  // Testing copy-constructors.
  GIVEN("A Float") {
    simple_msgs::Float single_Float{float_1};
    WHEN("I construct a new Float from the serialized data of the existing Float") {
      simple_msgs::Float copy_buffer_Float(single_Float.getBufferData()->data());
      THEN("The new Float is equal to the original") { REQUIRE(copy_buffer_Float == single_Float); }
    }
    WHEN("I copy-construct a new Float") {
      const simple_msgs::Float& copy_Float{single_Float};
      THEN("The new Float is equal to the original") { REQUIRE(copy_Float == single_Float); }
    }
    WHEN("I move-construct a new Float") {
      simple_msgs::Float moved_Float{std::move(single_Float)};
      THEN("The new Float contains the value that was contained in the orignal one") {
        REQUIRE(moved_Float.get() == float_1);
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Float") {
    simple_msgs::Float single_float{float_1};
    WHEN("I copy-assign from that Float's buffer") {
      simple_msgs::Float copy_assigned_buffer_float{};
      auto data_ptr = std::make_shared<void*>(single_float.getBufferData()->data());
      copy_assigned_buffer_float = data_ptr;
      THEN("The new Float is equal to the original") { REQUIRE(copy_assigned_buffer_float == single_float); }
    }
    WHEN("I copy-assign from that Float") {
      simple_msgs::Float copy_assigned_float{};
      copy_assigned_float = single_float;
      THEN("The new Float is equal to the original") { REQUIRE(copy_assigned_float == single_float); }
    }
    WHEN("I move-assign from that Float") {
      simple_msgs::Float move_assigned_float{};
      move_assigned_float = std::move(single_float);
      THEN("The new Float is equal to the original") { REQUIRE(move_assigned_float.get() == float_1); }
    }
  }

  // Testing getter-setter
  GIVEN("An instance of a Float.") {
    // start a Float
    simple_msgs::Float d{};
    WHEN("I set the value of the Float") {
      float x = static_cast<float>(rand()) / RAND_MAX;
      d.set(x);
      THEN("Its value is correct") { REQUIRE(d.get() == x); }
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

  GIVEN("A Float") {
    simple_msgs::Float single_Float(float_1);
    WHEN("I increase its value (operator++)") {
      single_Float++;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 + 1.0f); }
    }
    WHEN("I decrease its value (operator--)") {
      single_Float--;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 - 1.0f); }
    }
    WHEN("I add a value to it (operator+)") {
      single_Float += 2.0;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 + 2.0f); }
    }
    WHEN("I subtract a value to it (operator-)") {
      single_Float -= 5.0;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 - 5.0f); }
    }
    WHEN("I multiply its value (operator*)") {
      single_Float *= 2.0;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 * 2.0f); }
    }
    WHEN("I divide its value (operator/)") {
      single_Float /= 5.0;
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 / 5.0f); }
    }
    WHEN("I add another Float to it") {
      single_Float = single_Float + simple_msgs::Float{6.0};
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 + 6.0f); }
    }
    WHEN("I subtract another Float to it") {
      single_Float = single_Float - simple_msgs::Float{6.0};
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 - 6.0f); }
    }
    WHEN("I multiply another Float to it") {
      single_Float = single_Float * simple_msgs::Float{6.0};
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 * 6.0f); }
    }
    WHEN("I divide another Float to it") {
      single_Float = single_Float / simple_msgs::Float{6.0};
      THEN("The new value is correct") { REQUIRE(single_Float.get() == float_1 / 6.0f); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = single_Float.getTopic();
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
