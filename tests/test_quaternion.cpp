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
#include "simple_msgs/quaternion.h"

// TEST FOR USING THE QUATERNION MESSAGE WRAPPER

SCENARIO("Using a Quaternion Message") {
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  double double_4 = static_cast<double>(rand()) / RAND_MAX;
  std::array<double, 4> double_array{{double_1, double_2, double_3, double_4}};
  // Testing constructors.
  GIVEN("An empty Quaternion") {
    simple_msgs::Quaternion empty_quaternion{};
    WHEN("It is constructed") {
      THEN("The first 3 elements have to be zero and the last one") {
        REQUIRE(empty_quaternion.getX() == 0.0);
        REQUIRE(empty_quaternion.getY() == 0.0);
        REQUIRE(empty_quaternion.getZ() == 0.0);
        REQUIRE(empty_quaternion.getW() == 1.0);
      }
    }
  }

  GIVEN("A Quaternion created from 4 doubles") {
    simple_msgs::Quaternion q{double_1, double_2, double_3, double_4};
    WHEN("We check the Quaternion elements") {
      THEN("They all have to be equal to the doubles from the constructor") {
        REQUIRE(q.getX() == double_1);
        REQUIRE(q.getY() == double_2);
        REQUIRE(q.getZ() == double_3);
        REQUIRE(q.getW() == double_4);
      }
    }
  }

  GIVEN("A Quaternion created from an array of doubles") {
    simple_msgs::Quaternion array_quaternion{double_array};
    WHEN("We check the Quaternion elements") {
      THEN("They all have to be equal to the doubles") {
        REQUIRE(array_quaternion.getX() == double_1);
        REQUIRE(array_quaternion.getY() == double_2);
        REQUIRE(array_quaternion.getZ() == double_3);
        REQUIRE(array_quaternion.getW() == double_4);
      }
    }
  }

  GIVEN("A Quaternion created from moving an array of doubles") {
    simple_msgs::Quaternion moved_array_quaternion{std::move(double_array)};
    WHEN("We check the Quaternion elements") {
      THEN("They all have to be equal to the doubles") {
        REQUIRE(moved_array_quaternion.getX() == double_1);
        REQUIRE(moved_array_quaternion.getY() == double_2);
        REQUIRE(moved_array_quaternion.getZ() == double_3);
        REQUIRE(moved_array_quaternion.getW() == double_4);
      }
    }
  }

  // Testing Copy-constructors.
  GIVEN("A Quaternion") {
    simple_msgs::Quaternion quaternion{double_array};
    WHEN("I construct a new Quaternion from the serialized data of the existing Quaternion") {
      simple_msgs::Quaternion buffer_quaternion(quaternion.getBufferData()->data());
      THEN("The new Quaternion has to be equal to the other") { REQUIRE(buffer_quaternion == quaternion); }
    }
    WHEN("I copy-construct a new Quaternion") {
      const simple_msgs::Quaternion& copy_quaternion{quaternion};
      THEN("The new Quaternion is equal to the other") { REQUIRE(copy_quaternion == quaternion); }
    }
    WHEN("I move-construct a new Quaternion") {
      simple_msgs::Quaternion moved_quaternion{std::move(quaternion)};
      THEN("The new Quaternion coordinates are equal to the previous' ones") {
        REQUIRE(moved_quaternion.getX() == double_1);
        REQUIRE(moved_quaternion.getY() == double_2);
        REQUIRE(moved_quaternion.getZ() == double_3);
        REQUIRE(moved_quaternion.getW() == double_4);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A Quaternion") {
    simple_msgs::Quaternion quaternion{double_array};
    WHEN("I copy-assign from that Quaternion's buffer") {
      simple_msgs::Quaternion copy_assigned_buffer_quaternion{};
      auto data_ptr = std::make_shared<void*>(quaternion.getBufferData()->data());
      copy_assigned_buffer_quaternion = data_ptr;
      THEN("The new Quaternion has to be same as the original") {
        REQUIRE(copy_assigned_buffer_quaternion == quaternion);
      }
    }
    WHEN("I copy-assign from that Quaternion") {
      simple_msgs::Quaternion copy_assigned_quaternion{};
      copy_assigned_quaternion = quaternion;
      THEN("The new Quaternion has to be same as the original") { REQUIRE(copy_assigned_quaternion == quaternion); }
    }
    WHEN("I move-assign from that Quaternion") {
      simple_msgs::Quaternion move_assigned_quaternion{};
      move_assigned_quaternion = std::move(quaternion);
      THEN("The new Quaternion has to be same as the original") {
        REQUIRE(move_assigned_quaternion.getX() == double_1);
        REQUIRE(move_assigned_quaternion.getY() == double_2);
        REQUIRE(move_assigned_quaternion.getZ() == double_3);
        REQUIRE(move_assigned_quaternion.getW() == double_4);
      }
    }
    WHEN("I copy-assign an array to that Quaternion") {
      simple_msgs::Quaternion copy_assined_array_quaternion{};
      copy_assined_array_quaternion = double_array;
      THEN("The Quaternion elements are equal to the array") {
        REQUIRE(copy_assined_array_quaternion.getX() == double_1);
        REQUIRE(copy_assined_array_quaternion.getY() == double_2);
        REQUIRE(copy_assined_array_quaternion.getZ() == double_3);
        REQUIRE(copy_assined_array_quaternion.getW() == double_4);
      }
    }
    WHEN("I move-assign from an array to that Quaternion") {
      simple_msgs::Quaternion move_assined_array_quaternion{};
      move_assined_array_quaternion = std::move(double_array);
      THEN("The Quaternion elements are the same as the array") {
        REQUIRE(move_assined_array_quaternion.getX() == double_1);
        REQUIRE(move_assined_array_quaternion.getY() == double_2);
        REQUIRE(move_assined_array_quaternion.getZ() == double_3);
        REQUIRE(move_assined_array_quaternion.getW() == double_4);
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Quaternions") {
    simple_msgs::Quaternion quaternion_1{double_array};
    simple_msgs::Quaternion quaternion_2{double_array};
    WHEN("I compare them") {
      THEN("They have to be equal") { REQUIRE(quaternion_1 == quaternion_2); }
    }
    WHEN("I change one of the Quaternion") {
      quaternion_1.setX(double_1 + double_2);
      THEN("They have to be different") { REQUIRE(quaternion_1 != quaternion_2); }
    }
    WHEN("I get the array from the quaternion") {
      std::array<double, 4> vector = quaternion_1.toVector();
      THEN("The array elements are correct") {
        REQUIRE(vector[0] == double_1);
        REQUIRE(vector[1] == double_2);
        REQUIRE(vector[2] == double_3);
        REQUIRE(vector[3] == double_4);
      }
    }
    WHEN("I get the message topic") {
      std::string topic_name = quaternion_1.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "QUAT"); }
    }
    WHEN("I print the Quaternion") {
      std::ostringstream out;
      out << quaternion_1;
      THEN("The output is correct") {
        REQUIRE(out.str() == "Quaternion \n \tx: " + std::to_string(quaternion_1.getX()) +
                                 "\n \ty: " + std::to_string(quaternion_1.getY()) +
                                 "\n \tz: " + std::to_string(quaternion_1.getZ()) +
                                 "\n \tw: " + std::to_string(quaternion_1.getW()) + "\n");
      }
    }
  }

  // Testing coordinates setters/getters.
  GIVEN("An instance of a quaternion.") {
    // start a quaternion
    simple_msgs::Quaternion quaternion{};

    WHEN("I set the X coordinate of the quaternion") {
      quaternion.setX(double_1);
      THEN("The data quaternion has the correct coordinate") { REQUIRE(quaternion.getX() == double_1); }
    }
    WHEN("I set the Y coordinate of the quaternion") {
      quaternion.setY(double_2);
      THEN("The data quaternion has the correct coordinate") { REQUIRE(quaternion.getY() == double_2); }
    }
    WHEN("I set the Z coordinate of the quaternion") {
      quaternion.setZ(double_3);
      THEN("The data quaternion has the correct coordinate") { REQUIRE(quaternion.getZ() == double_3); }
    }
    WHEN("I set the W coordinate of the quaternion") {
      quaternion.setW(double_4);
      THEN("The data quaternion has the correct coordinate") { REQUIRE(quaternion.getW() == double_4); }
    }
  }
}
