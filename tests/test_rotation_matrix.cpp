/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
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

#include "simple_msgs/rotation_matrix.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE ROTATION MATRIX MESSAGE WRAPPER

SCENARIO("Using a Rotation Matrix Message")
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
  std::array<double, 9> doubles_array{
      {double_1, double_2, double_3, double_4, double_5, double_6, double_7, double_8, double_9}};
  std::array<double, 3> column0_array{{double_1, double_4, double_7}};
  std::array<double, 3> column1_array{{double_2, double_5, double_8}};
  std::array<double, 3> column2_array{{double_3, double_6, double_9}};
  std::array<double, 3> row0_array{{double_1, double_2, double_3}};
  std::array<double, 3> row1_array{{double_4, double_5, double_6}};
  std::array<double, 3> row2_array{{double_7, double_8, double_9}};
  // Testing constructors.
  GIVEN("An empty rotation matrix")
  {
    simple_msgs::RotationMatrix rotation_matrix;

    WHEN("It is constructed")
    {
      THEN("The Rotation Matrix elements have to be zero")
      {
        REQUIRE(rotation_matrix.getRow(0)[0] == 0);
        REQUIRE(rotation_matrix.getRow(0)[1] == 0);
        REQUIRE(rotation_matrix.getRow(0)[2] == 0);
        REQUIRE(rotation_matrix.getRow(1)[0] == 0);
        REQUIRE(rotation_matrix.getRow(1)[1] == 0);
        REQUIRE(rotation_matrix.getRow(1)[2] == 0);
        REQUIRE(rotation_matrix.getRow(2)[0] == 0);
        REQUIRE(rotation_matrix.getRow(2)[1] == 0);
        REQUIRE(rotation_matrix.getRow(2)[2] == 0);
      }
    }
  }

  GIVEN("A Rotation Matrix created from 1 double")
  {
    simple_msgs::RotationMatrix double_rotation_matrix(double_1);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the double from the constructor")
      {
        REQUIRE(double_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(double_rotation_matrix.getRow(0)[1] == double_1);
        REQUIRE(double_rotation_matrix.getRow(0)[2] == double_1);
        REQUIRE(double_rotation_matrix.getRow(1)[0] == double_1);
        REQUIRE(double_rotation_matrix.getRow(1)[1] == double_1);
        REQUIRE(double_rotation_matrix.getRow(1)[2] == double_1);
        REQUIRE(double_rotation_matrix.getRow(2)[0] == double_1);
        REQUIRE(double_rotation_matrix.getRow(2)[1] == double_1);
        REQUIRE(double_rotation_matrix.getRow(2)[2] == double_1);
      }
    }
  }

  GIVEN("A Rotation Matrix created from 9 doubles")
  {
    simple_msgs::RotationMatrix doubles_rotation_matrix(double_1, double_2, double_3, double_4, double_5, double_6,
                                                        double_7, double_8, double_9);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles from the constructor")
      {
        REQUIRE(doubles_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(doubles_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(doubles_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(doubles_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(doubles_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(doubles_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(doubles_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(doubles_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(doubles_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  GIVEN("A Rotation Matrix created from an array of doubles")
  {
    simple_msgs::RotationMatrix array_rotation_matrix(doubles_array);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles")
      {
        REQUIRE(array_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(array_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(array_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(array_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(array_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(array_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(array_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(array_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(array_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  GIVEN("A Rotation Matrix created from moving an array of doubles")
  {
    simple_msgs::RotationMatrix moved_array_rotation_matrix(doubles_array);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles")
      {
        REQUIRE(moved_array_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(moved_array_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(moved_array_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(moved_array_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(moved_array_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(moved_array_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(moved_array_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(moved_array_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(moved_array_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("A rotation Matrix")
  {
    simple_msgs::RotationMatrix array_rotation_matrix(doubles_array);
    WHEN("I construct a new Rotation Matrix from the serialized data of the existing Rotation Matrix")
    {
      simple_msgs::RotationMatrix copy_buffer_rotation_matrix(array_rotation_matrix.getBufferData()->data());
      THEN("The new Rotation Matrix has to be equal to the other")
      {
        REQUIRE(copy_buffer_rotation_matrix == array_rotation_matrix);
      }
    }
    WHEN("I copy-construct a new Rotation Matrix")
    {
      const simple_msgs::RotationMatrix& copy_rotation_matrix(array_rotation_matrix);
      THEN("The new Rotation Matrix is equal to the other") { REQUIRE(copy_rotation_matrix == array_rotation_matrix); }
    }
    WHEN("I move-construct a new Rotation Matrix")
    {
      simple_msgs::RotationMatrix moved_rotation_matrix(std::move(array_rotation_matrix));
      THEN("The new Rotation Matrix's coordinates are equal to the previous' ones")
      {
        REQUIRE(moved_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(moved_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(moved_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(moved_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(moved_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(moved_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(moved_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(moved_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(moved_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Rotation Matrix")
  {
    simple_msgs::RotationMatrix array_rotation_matrix(doubles_array);
    WHEN("I copy-assign from that Rotation Matrix's buffer")
    {
      simple_msgs::RotationMatrix copy_assigned_buffer_rotation_matrix;
      copy_assigned_buffer_rotation_matrix = array_rotation_matrix.getBufferData()->data();
      THEN("The new Rotation Matrix has to be same as the original")
      {
        REQUIRE(copy_assigned_buffer_rotation_matrix == array_rotation_matrix);
      }
    }
    WHEN("I copy-assign from that Rotation Matrix")
    {
      simple_msgs::RotationMatrix copy_assigned_rotation_matrix;
      copy_assigned_rotation_matrix = array_rotation_matrix;
      THEN("The new Rotation Matrix has to be same as the original")
      {
        REQUIRE(copy_assigned_rotation_matrix == array_rotation_matrix);
      }
    }
    WHEN("I move-assign from that Rotation Matrix")
    {
      simple_msgs::RotationMatrix move_assigned_rotation_matrix;
      move_assigned_rotation_matrix = std::move(array_rotation_matrix);
      THEN("The new Rotation Matrix has to be same as the original")
      {
        REQUIRE(move_assigned_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(move_assigned_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(move_assigned_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(move_assigned_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(move_assigned_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(move_assigned_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(move_assigned_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(move_assigned_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(move_assigned_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  GIVEN("A Rotation Matrix")
  {
    simple_msgs::RotationMatrix empty_rotation_matrix;
    WHEN("I copy-assign an array to that Rotation Matrix")
    {
      empty_rotation_matrix = doubles_array;
      THEN("The Rotation Matrix's elements are equal to the array")
      {
        REQUIRE(empty_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(empty_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(empty_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(empty_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(empty_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(empty_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(empty_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(empty_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(empty_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
    WHEN("I move-assign from an array to that Rotation Matrix")
    {
      empty_rotation_matrix = doubles_array;
      THEN("The quaternion's elements are the same as the array")
      {
        REQUIRE(empty_rotation_matrix.getRow(0)[0] == double_1);
        REQUIRE(empty_rotation_matrix.getRow(0)[1] == double_2);
        REQUIRE(empty_rotation_matrix.getRow(0)[2] == double_3);
        REQUIRE(empty_rotation_matrix.getRow(1)[0] == double_4);
        REQUIRE(empty_rotation_matrix.getRow(1)[1] == double_5);
        REQUIRE(empty_rotation_matrix.getRow(1)[2] == double_6);
        REQUIRE(empty_rotation_matrix.getRow(2)[0] == double_7);
        REQUIRE(empty_rotation_matrix.getRow(2)[1] == double_8);
        REQUIRE(empty_rotation_matrix.getRow(2)[2] == double_9);
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Rotation Matrixes")
  {
    simple_msgs::RotationMatrix rotation_matrix_1(doubles_array);
    simple_msgs::RotationMatrix rotation_matrix_2(doubles_array);
    WHEN("I compare these Rotation Matrixes")
    {
      THEN("They have to be equal") { REQUIRE(rotation_matrix_1 == rotation_matrix_2); }
    }
    WHEN("I change something in the first matrix")
    {
      rotation_matrix_1.setColumn(0, column2_array);
      THEN("They have to different") { REQUIRE(rotation_matrix_1 != rotation_matrix_2); }
    }
  }

  // Testing elements getters-setters.
  GIVEN("An empty rotation matrix")
  {
    simple_msgs::RotationMatrix empty_rotation_matrix;
    WHEN("I set the 1st row of the Rotation Matrix")
    {
      empty_rotation_matrix.setRow(0, row0_array);
      THEN("The Rotation Matrix row has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getRow(0) == row0_array);
      }
    }
    WHEN("I set the 1st column of the Rotation Matrix")
    {
      empty_rotation_matrix.setColumn(0, column0_array);
      THEN("The Rotation Matrix column has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getColumn(0) == column0_array);
      }
    }
    WHEN("I set the 2nd row of the Rotation Matrix")
    {
      empty_rotation_matrix.setRow(1, row1_array);
      THEN("The Rotation Matrix row has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getRow(1) == row1_array);
      }
    }
    WHEN("I set the 2nd column of the Rotation Matrix")
    {
      empty_rotation_matrix.setColumn(1, column1_array);
      THEN("The Rotation Matrix column has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getColumn(1) == column1_array);
      }
    }
    WHEN("I set the 3rd row of the Rotation Matrix")
    {
      empty_rotation_matrix.setRow(2, row2_array);
      THEN("The Rotation Matrix row has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getRow(2) == row2_array);
      }
    }
    WHEN("I set the 3rd column of the Rotation Matrix")
    {
      empty_rotation_matrix.setColumn(2, column2_array);
      THEN("The Rotation Matrix column has the correct elements")
      {
        REQUIRE(empty_rotation_matrix.getColumn(2) == column2_array);
      }
    }
  }

  GIVEN("A Rotation Matrix")
  {
    simple_msgs::RotationMatrix rotation_matrix(doubles_array);
    WHEN("I get the array from the Rotation Matrix")
    {
      std::array<double, 9> array = rotation_matrix.toVector();
      THEN("The array elements are correct")
      {
        REQUIRE(array[0] == double_1);
        REQUIRE(array[1] == double_2);
        REQUIRE(array[2] == double_3);
        REQUIRE(array[3] == double_4);
        REQUIRE(array[4] == double_5);
        REQUIRE(array[5] == double_6);
        REQUIRE(array[6] == double_7);
        REQUIRE(array[7] == double_8);
        REQUIRE(array[8] == double_9);
      }
    }
    WHEN("I get the transpose from the Rotation Matrix")
    {
      simple_msgs::RotationMatrix transpose = rotation_matrix.getTranspose();
      THEN("The transpose elements are correct")
      {
        REQUIRE(transpose.getRow(0)[0] == double_1);
        REQUIRE(transpose.getRow(0)[1] == double_4);
        REQUIRE(transpose.getRow(0)[2] == double_7);
        REQUIRE(transpose.getRow(1)[0] == double_2);
        REQUIRE(transpose.getRow(1)[1] == double_5);
        REQUIRE(transpose.getRow(1)[2] == double_8);
        REQUIRE(transpose.getRow(2)[0] == double_3);
        REQUIRE(transpose.getRow(2)[1] == double_6);
        REQUIRE(transpose.getRow(2)[2] == double_9);
      }
    }
    WHEN("I get the message topic")
    {
      std::string topic_name = rotation_matrix.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "RMAT"); }
    }
  }
}
