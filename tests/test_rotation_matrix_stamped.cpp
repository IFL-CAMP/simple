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
#include "simple_msgs/rotation_matrix_stamped.h"

using namespace simple_tests;

// TEST FOR USING THE STAMPED ROTATION MATRIX MESSAGE WRAPPER

SCENARIO("Using a RotationMatrixStamped Message") {
  double double_1 = double_dist(generator);
  double double_2 = double_dist(generator);
  double double_3 = double_dist(generator);
  double double_4 = double_dist(generator);
  double double_5 = double_dist(generator);
  double double_6 = double_dist(generator);
  double double_7 = double_dist(generator);
  double double_8 = double_dist(generator);
  double double_9 = double_dist(generator);
  long long time = rand();
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::RotationMatrix random_rotation_matrix{double_1, double_2, double_3, double_4, double_5,
                                                     double_6, double_7, double_8, double_9};
  simple_msgs::RotationMatrix empty_rotation_matrix{};
  simple_msgs::Header empty_header{};
  simple_msgs::Header random_header{random_int, random_string, time};

  // Test the constructors.
  GIVEN("A RotationMatrixStamped created from an empty constructor") {
    simple_msgs::RotationMatrixStamped empty_rotation_matrix_stamped{};
    WHEN("It is constructed") {
      THEN("Its elements have to match the default") {
        REQUIRE(empty_rotation_matrix_stamped.getHeader() == empty_header);
        REQUIRE(empty_rotation_matrix_stamped.getRotationMatrix() == empty_rotation_matrix);
      }
    }
  }

  GIVEN("A RotationMatrixStamped created from a rotation_matrix and a header") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{random_header, random_rotation_matrix};
    WHEN("I check the RotationMatrixStamped's elements") {
      THEN("They all have to be equal to the parameters from the constructor") {
        REQUIRE(rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  GIVEN("A RotationMatrixStamped created from the serialized data of an existing RotationMatrixStamped") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{random_header, random_rotation_matrix};
    simple_msgs::RotationMatrixStamped buffer_rotation_matrix_stamped{rotation_matrix_stamped.getBufferData()->data()};
    WHEN("I check the RotationMatrixStamped's elements") {
      THEN("The new RotationMatrixStamped has to be equal to the other") {
        REQUIRE(buffer_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
  }

  // Testing copy constructors.
  GIVEN("A RotationMatrixStamped") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{random_header, random_rotation_matrix};
    WHEN("I copy-construct a new RotationMatrixStamped") {
      simple_msgs::RotationMatrixStamped copy_rotation_matrix_stamped{rotation_matrix_stamped};
      THEN("The new RotationMatrixStamped is equal to the other") {
        REQUIRE(copy_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I move-construct a new RotationMatrixStamped") {
      simple_msgs::RotationMatrixStamped move_rotation_matrix_stamped{std::move(rotation_matrix_stamped)};
      THEN("The new RotationMatrixStamped's elements are equal to the previous' ones") {
        REQUIRE(move_rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(move_rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A RotationMatrixStamped") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{random_header, random_rotation_matrix};
    WHEN("I copy-assign from that RotationMatrixStamped's buffer") {
      simple_msgs::RotationMatrixStamped copy_assigned_buffer_rotation_matrix_stamped{};
      auto data_ptr = std::make_shared<void*>(rotation_matrix_stamped.getBufferData()->data());
      copy_assigned_buffer_rotation_matrix_stamped = data_ptr;
      THEN("The new RotationMatrixStamped has to be same as the original") {
        REQUIRE(copy_assigned_buffer_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I copy-assign from that RotationMatrixStamped") {
      simple_msgs::RotationMatrixStamped copy_assigned_rotation_matrix_stamped{};
      copy_assigned_rotation_matrix_stamped = rotation_matrix_stamped;
      THEN("The new RotationMatrixStamped has to be same as the original") {
        REQUIRE(copy_assigned_rotation_matrix_stamped == rotation_matrix_stamped);
      }
    }
    WHEN("I move-assign from that RotationMatrixStamped") {
      simple_msgs::RotationMatrixStamped move_assigned_rotation_matrix_stamped{};
      move_assigned_rotation_matrix_stamped = std::move(rotation_matrix_stamped);
      THEN("The new RotationMatrixStamped has to be same as the original") {
        REQUIRE(move_assigned_rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
        REQUIRE(move_assigned_rotation_matrix_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A RotationMatrixStamped") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{};
    WHEN("I set the rotation_matrix of the rotation_matrix_stamped") {
      rotation_matrix_stamped.setRotationMatrix(random_rotation_matrix);
      THEN("The position is correct") {
        REQUIRE(rotation_matrix_stamped.getRotationMatrix() == random_rotation_matrix);
      }
    }
    WHEN("I set the header of the rotation_matrix_stamped") {
      rotation_matrix_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(rotation_matrix_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical RotationMatrixStampeds") {
    simple_msgs::RotationMatrixStamped first_rotation_matrix_stamped{random_header, random_rotation_matrix};
    simple_msgs::RotationMatrixStamped second_rotation_matrix_stamped{random_header, random_rotation_matrix};
    WHEN("I compare these RotationMatrixStampeds") {
      THEN("They have to be equal") { REQUIRE(first_rotation_matrix_stamped == second_rotation_matrix_stamped); }
    }
    WHEN("I change something in the first rotation_matrix_stamped") {
      first_rotation_matrix_stamped.setHeader(empty_header);
      THEN("They have to be different") { REQUIRE(first_rotation_matrix_stamped != second_rotation_matrix_stamped); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Rotation Matrix Stamped") {
    simple_msgs::RotationMatrixStamped rotation_matrix_stamped{random_header, random_rotation_matrix};
    WHEN("I get the message topic") {
      std::string topic_name = rotation_matrix_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "RMST"); }
    }
    WHEN("I print the RotatioMatrixStamped") {
      std::ostringstream out;
      out << rotation_matrix_stamped;
      THEN("The output is correct") {
        std::string correct =
            "Header\n \tseq_n: " + std::to_string(rotation_matrix_stamped.getHeader().getSequenceNumber()) +
            "\n \tframe_id: " + rotation_matrix_stamped.getHeader().getFrameID() + "\n \t" +
            "timestamp: " + std::to_string(rotation_matrix_stamped.getHeader().getTimestamp()) + "\n" +
            "RotationMatrix \n \t" + std::to_string(double_1) + " " + std::to_string(double_2) + " " +
            std::to_string(double_3) + "\n \t" + std::to_string(double_4) + " " + std::to_string(double_5) + " " +
            std::to_string(double_6) + "\n \t" + std::to_string(double_7) + " " + std::to_string(double_8) + " " +
            std::to_string(double_9) + "\n";
        REQUIRE(out.str() == correct);
      }
    }
  }
}
