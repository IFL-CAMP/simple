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

#include "simple_msgs/header.h"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/rotation_matrix_stamped.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE STAMPED ROTATION MATRIX MESSAGE WRAPPER

SCENARIO("Using a RotationMatrixStamped Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  double double_4 = static_cast<double>(rand()) / RAND_MAX;
  double double_5 = static_cast<double>(rand()) / RAND_MAX;
  double double_6 = static_cast<double>(rand()) / RAND_MAX;
  double double_7 = static_cast<double>(rand()) / RAND_MAX;
  double double_8 = static_cast<double>(rand()) / RAND_MAX;
  double double_9 = static_cast<double>(rand()) / RAND_MAX;
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::RotationMatrix random_rotation_matrix(double_1, double_2, double_3, double_4, double_5, double_6,
                                                     double_7, double_8, double_9);
  simple_msgs::RotationMatrix empty_rotation_matrix;
  simple_msgs::Header empty_header;
  simple_msgs::Header random_header(random_int, random_string, double_1);
  // Test the constructors.
  GIVEN("A RotationMatrixStamped created from an empty constructor")
  {
    simple_msgs::RotationMatrixStamped empty_rotation_matrix_stamped;
    WHEN("It is constructed")
    {
      THEN("It's elements have to match the default")
      {
        REQUIRE(empty_rotation_matrix_stamped.getHeader() == empty_header);
        REQUIRE(empty_rotation_matrix_stamped.getRotationMatrix() == empty_rotation_matrix);
      }
    }
  }

  GIVEN("A RotationMatrixStamped created from a rotation_matrix and a header")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped(random_header, random_rotation_matrix);
    WHEN("I check the RotationMatrixStamped's elements")
    {
      THEN("They all have to be equal to the parameters from the constructor")
      {
        REQUIRE(rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  GIVEN("A RotationMatrixStamped created from the serialized data of an existing RotationMatrixStamped")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped(random_header, random_rotation_matrix);
    simple_msgs::RotationMatrixStamped buffer_rotation_matrix_stamped(rotation_matrix_stamped.getBufferData());
    WHEN("I check the RotationMatrixStamped's elements")
    {
      THEN("The new RotationMatrixStamped has to be equal to the other")
      {
        REQUIRE(buffer_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
  }

  // Testing copy constructors.
  GIVEN("A RotationMatrixStamped")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped(random_header, random_rotation_matrix);
    WHEN("I copy-construct a new RotationMatrixStamped")
    {
      const simple_msgs::RotationMatrixStamped& copy_rotation_matrix_stamped(rotation_matrix_stamped);
      THEN("The new RotationMatrixStamped is equal to the other")
      {
        REQUIRE(copy_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I move-construct a new RotationMatrixStamped")
    {
      simple_msgs::RotationMatrixStamped move_rotation_matrix_stamped(std::move(rotation_matrix_stamped));
      THEN("The new RotationMatrixStamped's elements are equal to the previous' ones")
      {
        REQUIRE(move_rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(move_rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A RotationMatrixStamped")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped(random_header, random_rotation_matrix);
    WHEN("I copy-assign from that RotationMatrixStamped's buffer")
    {
      simple_msgs::RotationMatrixStamped copy_assigned_buffer_rotation_matrix_stamped;
      copy_assigned_buffer_rotation_matrix_stamped = rotation_matrix_stamped.getBufferData();
      THEN("The new RotationMatrixStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_buffer_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I copy-assign from that RotationMatrixStamped")
    {
      simple_msgs::RotationMatrixStamped copy_assigned_rotation_matrix_stamped;
      copy_assigned_rotation_matrix_stamped = rotation_matrix_stamped;
      THEN("The new RotationMatrixStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I move-assign from that RotationMatrixStamped")
    {
      simple_msgs::RotationMatrixStamped move_assigned_rotation_matrix_stamped;
      move_assigned_rotation_matrix_stamped = std::move(rotation_matrix_stamped);
      THEN("The new RotationMatrixStamped has to be same as the original")
      {
        REQUIRE(move_assigned_rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(move_assigned_rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A RotationMatrixStamped")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped;
    WHEN("I set the rotation_matrix of the rotation_matrix_stamped")
    {
      rotation_matrix_stamped.setRotationMatrix(random_rotation_matrix);
      THEN("The position is correct")
      {
        REQUIRE(rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
      }
    }
    WHEN("I set the header of the rotation_matrix_stamped")
    {
      rotation_matrix_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(rotation_matrix_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical RotationMatrixStampeds")
  {
    simple_msgs::RotationMatrixStamped first_rotation_matrix_stamped(random_header, random_rotation_matrix);
    simple_msgs::RotationMatrixStamped second_rotation_matrix_stamped(random_header, random_rotation_matrix);
    WHEN("I compare these RotationMatrixStampeds")
    {
      THEN("They have to be equal") { REQUIRE(first_rotation_matrix_stamped == second_rotation_matrix_stamped); }
    }
    WHEN("I change something in the first rotation_matrix_stamped")
    {
      first_rotation_matrix_stamped.setHeader(empty_header);
      THEN("They have to be different") { REQUIRE(first_rotation_matrix_stamped != second_rotation_matrix_stamped); }
    }
  }

  // Testing Topic
  GIVEN("A point")
  {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped;
    WHEN("I get the message topic")
    {
      std::string topic_name = rotation_matrix_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "RMST"); }
    }
  }
}
