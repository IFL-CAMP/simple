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
#include "simple_msgs/transform_stamped.hpp"

using namespace simple_tests;

// TESTS FOR USING THE TRANSFORM STAMPED MESSAGE WRAPPER

SCENARIO("Using a TransformStamped Message") {
  double double_1 = double_dist(generator);
  double double_2 = double_dist(generator);
  double double_3 = double_dist(generator);
  double double_4 = double_dist(generator);
  double double_5 = double_dist(generator);
  double double_6 = double_dist(generator);
  double double_7 = double_dist(generator);
  double double_8 = double_dist(generator);
  double double_9 = double_dist(generator);
  double x = double_dist(generator);
  double y = double_dist(generator);
  double z = double_dist(generator);
  std::array<double, 9> rotation{
      {double_1, double_2, double_3, double_4, double_5, double_6, double_7, double_8, double_9}};
  std::array<double, 3> translation{{x, y, z}};
  simple_msgs::Point point{translation};
  simple_msgs::RotationMatrix matrix{rotation};
  long long time = rand();
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);

  simple_msgs::Transform random_transform{point, matrix};
  simple_msgs::Transform empty_rotation_matrix{};
  simple_msgs::Header empty_header{};
  simple_msgs::Header random_header{random_int, random_string, time};

  // Test the constructors.
  GIVEN("A TransformStamped created from an empty constructor") {
    simple_msgs::TransformStamped empty_transform_stamped{};
    WHEN("It is constructed") {
      THEN("Its elements have to match the default") {
        REQUIRE(empty_transform_stamped.getHeader() == empty_header);
        REQUIRE(empty_transform_stamped.getTransform() == empty_rotation_matrix);
      }
    }
  }

  GIVEN("A TransformStamped created from a rotation_matrix and a header") {
    simple_msgs::TransformStamped transform_stamped{random_header, random_transform};
    WHEN("I check the TransformStamped's elements") {
      THEN("They all have to be equal to the parameters from the constructor") {
        REQUIRE(transform_stamped.getTransform() == random_transform);
        REQUIRE(transform_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A TransformStamped") {
    simple_msgs::TransformStamped transform_stamped{random_header, random_transform};
    WHEN("I copy-construct a new TransformStamped") {
      simple_msgs::TransformStamped copy_transform_stamped{transform_stamped};
      THEN("The new TransformStamped is equal to the other") { REQUIRE(copy_transform_stamped == transform_stamped); }
    }
    WHEN("I move-construct a new TransformStamped") {
      simple_msgs::TransformStamped move_transform_stamped{std::move(transform_stamped)};
      THEN("The new TransformStamped's elements are equal to the previous' ones") {
        REQUIRE(move_transform_stamped.getTransform() == random_transform);
        REQUIRE(move_transform_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A TransformStamped") {
    simple_msgs::TransformStamped transform_stamped{random_header, random_transform};
    WHEN("I copy-assign from that TransformStamped") {
      simple_msgs::TransformStamped copy_assigned_transform_stamped{};
      copy_assigned_transform_stamped = transform_stamped;
      THEN("The new TransformStamped has to be same as the original") {
        REQUIRE(copy_assigned_transform_stamped == transform_stamped);
      }
    }
    WHEN("I move-assign from that TransformStamped") {
      simple_msgs::TransformStamped move_assigned_transform_stamped{};
      move_assigned_transform_stamped = std::move(transform_stamped);
      THEN("The new TransformStamped has to be same as the original") {
        REQUIRE(move_assigned_transform_stamped.getTransform() == random_transform);
        REQUIRE(move_assigned_transform_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A TransformStamped") {
    simple_msgs::TransformStamped transform_stamped{};
    WHEN("I set the rotation_matrix of the transform_stamped") {
      transform_stamped.setTransform(random_transform);
      THEN("The position is correct") { REQUIRE(transform_stamped.getTransform() == random_transform); }
    }
    WHEN("I set the header of the transform_stamped") {
      transform_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(transform_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical TransformStampeds") {
    simple_msgs::TransformStamped first_transform_stamped{random_header, random_transform};
    simple_msgs::TransformStamped second_transform_stamped{random_header, random_transform};
    WHEN("I compare these TransformStampeds") {
      THEN("They have to be equal") { REQUIRE(first_transform_stamped == second_transform_stamped); }
    }
    WHEN("I change something in the first transform_stamped") {
      first_transform_stamped.setHeader(empty_header);
      THEN("They have to be different") { REQUIRE(first_transform_stamped != second_transform_stamped); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Rotation Matrix Stamped") {
    simple_msgs::TransformStamped transform_stamped{random_header, random_transform};
    WHEN("I get the message topic") {
      std::string topic_name = simple_msgs::TransformStamped::getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "TRST"); }
    }
    WHEN("I print the RotatioMatrixStamped") {
      std::ostringstream out;
      out << transform_stamped;
      THEN("The output is correct") {
        std::string correct = "Header\n \tseq_n: " + std::to_string(transform_stamped.getHeader().getSequenceNumber()) +
                              "\n \tframe_id: " + transform_stamped.getHeader().getFrameID() + "\n \t" +
                              "timestamp: " + std::to_string(transform_stamped.getHeader().getTimestamp()) + "\n" +
                              "Transform \n \t" + std::to_string(double_1) + " " + std::to_string(double_2) + " " +
                              std::to_string(double_3) + " " + std::to_string(x) + "\n \t" + std::to_string(double_4) +
                              " " + std::to_string(double_5) + " " + std::to_string(double_6) + " " +
                              std::to_string(y) + "\n \t" + std::to_string(double_7) + " " + std::to_string(double_8) +
                              " " + std::to_string(double_9) + " " + std::to_string(z) + "\n" + "0 0 0 1 \n";
        REQUIRE(out.str() == correct);
      }
    }
  }
}
