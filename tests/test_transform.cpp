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
#include "simple_msgs/transform.h"

using namespace simple_tests;

// TEST FOR USING THE TRANSFORM MESSAGE WRAPPER

SCENARIO("Using a Transform Message") {
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

  // Testing constructors.
  GIVEN("An empty transform") {
    simple_msgs::Transform transform{};
    WHEN("It is constructed") {
      THEN("Its elements have to be zero") {
        REQUIRE(transform.getRotation() == simple_msgs::RotationMatrix{});
        REQUIRE(transform.getTranslation() == simple_msgs::Point{});
      }
    }
  }

  GIVEN("A Transform created from a Point and a RotationMatrix message") {
    simple_msgs::Transform transform{point, matrix};
    WHEN("We check the Transform elements") {
      THEN("They have to be correct") {
        REQUIRE(transform.getRotation() == matrix);
        REQUIRE(transform.getTranslation() == point);
      }
    }
  }

  GIVEN("A Transform created moving a Point and a RotationMatrix message") {
    simple_msgs::Transform transform{std::move(point), std::move(matrix)};
    WHEN("We check the Transform elements") {
      THEN("They have to be correct") {
        REQUIRE(transform.getRotation() == matrix);
        REQUIRE(transform.getTranslation() == point);
      }
    }
  }

  GIVEN("A Rotation Matrix created from an array of 16 doubles") {
    std::array<double, 16> array{{double_1, double_2, double_3, x, double_4, double_5, double_6, y, double_7, double_8,
                                  double_9, z, 0, 0, 0, 1}};
    simple_msgs::Transform transform{array};
    WHEN("We check the Transform elements") {
      THEN("They have to be correct") {
        REQUIRE(transform.getRotation() == matrix);
        REQUIRE(transform.getTranslation() == point);
      }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A Transform message") {
    simple_msgs::Transform transform{point, matrix};
    WHEN("I construct a new Transform from the serialized data of the existing Transform") {
      simple_msgs::Transform copy_transform{transform.getBufferData()->data()};
      THEN("The new Transform has to be equal to the other") { REQUIRE(copy_transform == transform); }
    }
    WHEN("I copy-construct a new Transform") {
      simple_msgs::Transform copy_transform{transform};
      THEN("The new Transform is equal to the other") { REQUIRE(copy_transform == transform); }
    }
    WHEN("I move-construct a new Transform") {
      simple_msgs::Transform copy_transform{std::move(transform)};
      THEN("The new Transform is equal to the previous one") {
        REQUIRE(transform.getRotation() == matrix);
        REQUIRE(transform.getTranslation() == point);
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A Transform") {
    simple_msgs::Transform transform{point, matrix};
    WHEN("I copy-assign from that Transform's buffer") {
      simple_msgs::Transform copy_assigned_buffer_transform{};
      auto data_ptr = std::make_shared<void*>(transform.getBufferData()->data());
      copy_assigned_buffer_transform = data_ptr;
      THEN("The new Transform has to be same as the original") { REQUIRE(copy_assigned_buffer_transform == transform); }
    }
    WHEN("I copy-assign from that Transform") {
      simple_msgs::Transform copy_assigned_transform{};
      copy_assigned_transform = transform;
      THEN("The new Transform has to be same as the original") { REQUIRE(copy_assigned_transform == transform); }
    }
    WHEN("I move-assign from that Transform") {
      simple_msgs::Transform move_assigned_transform{};
      move_assigned_transform = std::move(transform);
      THEN("The new Transform has to be same as the original") {
        REQUIRE(move_assigned_transform.getTranslation() == point);
        REQUIRE(move_assigned_transform.getRotation() == matrix);
      }
    }
    WHEN("I move-assign from an array") {
      simple_msgs::Transform move_assigned_transform{};
      std::array<double, 16> array{{double_1, double_2, double_3, x, double_4, double_5, double_6, y, double_7,
                                    double_8, double_9, z, 0, 0, 0, 1}};
      move_assigned_transform = std::move(array);
      THEN("The new Rotation Matrix has to be same as the array") {
        REQUIRE(move_assigned_transform.getTranslation() == point);
        REQUIRE(move_assigned_transform.getRotation() == matrix);
      }
    }
  }

  // Testing comparison.
  GIVEN("Two identical Transform") {
    simple_msgs::Transform transform_1{point, matrix};
    simple_msgs::Transform transform_2{point, matrix};
    WHEN("I compare these Transforms") {
      THEN("They have to be equal") { REQUIRE(transform_1 == transform_2); }
    }
    WHEN("I change something in the first matrix") {
      transform_1.setTranslation({0, 0, 1});
      THEN("They have to different") { REQUIRE(transform_1 != transform_2); }
    }
  }

  // Testing getters-setters.
  GIVEN("A Transform") {
    simple_msgs::Transform transform{};
    WHEN("I set its translational component") {
      transform.setTranslation(point);
      THEN("The Transform translation is now the correct one") { REQUIRE(transform.getTranslation() == point); }
    }
    WHEN("I set its rotational component") {
      transform.setRotation(matrix);
      THEN("The Transform rotation is now the correct one") { REQUIRE(transform.getRotation() == matrix); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A Transform") {
    simple_msgs::Transform transform{point, matrix};
    WHEN("I get the message topic") {
      std::string topic_name = transform.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "TRAN"); }
    }
    WHEN("I print the Transform") {
      transform.setTranslation(point);
      transform.setRotation(matrix);
      std::ostringstream out;
      out << transform;
      THEN("The output is correct") {
        std::string correct = "Transform \n \t" + std::to_string(double_1) + " " + std::to_string(double_2) + " " +
                              std::to_string(double_3) + " " + std::to_string(x) + "\n \t" + std::to_string(double_4) +
                              " " + std::to_string(double_5) + " " + std::to_string(double_6) + " " +
                              std::to_string(y) + "\n \t" + std::to_string(double_7) + " " + std::to_string(double_8) +
                              " " + std::to_string(double_9) + " " + std::to_string(z) + "\n" + "0 0 0 1 \n";
        REQUIRE(out.str() == correct);
      }
    }
  }
}
