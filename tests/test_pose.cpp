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
#include "simple_msgs/pose.hpp"

using namespace simple_tests;

// TEST FOR USING THE POSE MESSAGE WRAPPER

SCENARIO("Using a Pose Message") {
  double double_1 = double_dist(generator);
  double double_2 = double_dist(generator);
  double double_3 = double_dist(generator);
  double double_4 = double_dist(generator);
  double double_5 = double_dist(generator);
  double double_6 = double_dist(generator);
  double double_7 = double_dist(generator);
  simple_msgs::Point point{double_1, double_2, double_3};
  simple_msgs::Quaternion quaternion{double_4, double_5, double_6, double_7};

  // Test the constructors.
  GIVEN("A Pose created from an empty constructor") {
    simple_msgs::Pose empty_pose{};
    WHEN("It is constructed") {
      THEN("Its elements have to be zero") {
        REQUIRE(empty_pose.getPosition().getX() == 0.0);
        REQUIRE(empty_pose.getPosition().getY() == 0.0);
        REQUIRE(empty_pose.getPosition().getZ() == 0.0);
        REQUIRE(empty_pose.getQuaternion().getX() == 0.0);
        REQUIRE(empty_pose.getQuaternion().getY() == 0.0);
        REQUIRE(empty_pose.getQuaternion().getZ() == 0.0);
        REQUIRE(empty_pose.getQuaternion().getW() == 1.0);
      }
    }
  }

  GIVEN("A Pose created from a point and a quaternion") {
    simple_msgs::Pose pose{point, quaternion};
    WHEN("I check its elements") {
      THEN("They all have to be equal to the parameters from the constructor") {
        REQUIRE(pose.getPosition() == point);
        REQUIRE(pose.getQuaternion() == quaternion);
      }
    }
  }

  // Testing copy constructors.
  GIVEN("A Pose") {
    simple_msgs::Pose pose{point, quaternion};
    WHEN("Another Pose is created from the its serialized data") {
      simple_msgs::Pose buffer_pose{pose.getBufferData()->data()};
      THEN("The new Pose has to be equal to the other") { REQUIRE(buffer_pose == pose); }
    }
    WHEN("I copy-construct a new Pose") {
      const simple_msgs::Pose copy_pose{pose};
      THEN("The new Pose is equal to the other") { REQUIRE(copy_pose == pose); }
    }
    WHEN("I move-construct a new Pose") {
      simple_msgs::Pose move_pose{std::move(pose)};
      THEN("The new Pose elements are equal to the previous' ones") {
        REQUIRE(move_pose.getPosition() == point);
        REQUIRE(move_pose.getQuaternion() == quaternion);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A Pose") {
    simple_msgs::Pose pose{point, quaternion};
    WHEN("I copy-assign from that Pose's buffer") {
      simple_msgs::Pose copy_assigned_buffer_pose{};
      auto data_ptr = std::make_shared<void*>(pose.getBufferData()->data());
      copy_assigned_buffer_pose = data_ptr;
      THEN("The new Pose has to be same as the original") { REQUIRE(copy_assigned_buffer_pose == pose); }
    }
    WHEN("I copy-assign from that Pose") {
      simple_msgs::Pose copy_assigned_pose{};
      copy_assigned_pose = pose;
      THEN("The new Pose has to be same as the original") { REQUIRE(copy_assigned_pose == pose); }
    }
    WHEN("I move-assign from that Pose") {
      simple_msgs::Pose move_assigned_pose{};
      move_assigned_pose = std::move(pose);
      THEN("The new Pose has to be same as the original") {
        REQUIRE(move_assigned_pose.getPosition() == point);
        REQUIRE(move_assigned_pose.getQuaternion() == quaternion);
      }
    }
  }

  // Testing coordinates setters/getters.
  GIVEN("An instance of a Pose.") {
    simple_msgs::Pose pose{point, quaternion};
    WHEN("I set the X coordinate of the Pose's position") {
      pose.getPosition().setX(double_7);
      THEN("The data Pose's position has the correct coordinate") {
        REQUIRE(pose.getPosition().getX() == Approx(double_7));
      }
    }
    WHEN("I set the Y coordinate of the Pose's position") {
      pose.getPosition().setY(double_6);
      THEN("The data Pose's position has the correct coordinate") {
        REQUIRE(pose.getPosition().getY() == Approx(double_6));
      }
    }
    WHEN("I set the Z coordinate of the Pose's position") {
      pose.getPosition().setZ(double_5);
      THEN("The data Pose's position has the correct coordinate") {
        REQUIRE(pose.getPosition().getZ() == Approx(double_5));
      }
    }
    WHEN("I set the X coordinate of the Pose's quaternion") {
      pose.getQuaternion().setX(double_1);
      THEN("The data Pose's quaternion has the correct coordinate") {
        REQUIRE(pose.getQuaternion().getX() == Approx(double_1));
      }
    }
    WHEN("I set the Y coordinate of the Pose's quaternion") {
      pose.getQuaternion().setY(double_2);
      THEN("The data Pose's quaternion has the correct coordinate") {
        REQUIRE(pose.getQuaternion().getY() == Approx(double_2));
      }
    }
    WHEN("I set the Z coordinate of the Pose's quaternion") {
      pose.getQuaternion().setZ(double_3);
      THEN("The data Pose's quaternion has the correct coordinate") {
        REQUIRE(pose.getQuaternion().getZ() == Approx(double_3));
      }
    }
    WHEN("I set the W coordinate of the Pose's quaternion") {
      pose.getQuaternion().setW(double_4);
      THEN("The data Pose's quaternion has the correct coordinate") {
        REQUIRE(pose.getQuaternion().getW() == Approx(double_4));
      }
    }
  }

  GIVEN("A Pose") {
    simple_msgs::Pose pose{};
    WHEN("I set the position of the pose") {
      pose.setPosition(point);
      THEN("The position is correct") { REQUIRE(pose.getPosition() == point); }
    }
    WHEN("I set the quaternion of the pose") {
      pose.setQuaternion(quaternion);
      THEN("The quaternion is correct") { REQUIRE(pose.getQuaternion() == quaternion); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Poses") {
    simple_msgs::Pose first_pose{point, quaternion};
    simple_msgs::Pose second_pose{point, quaternion};
    WHEN("I compare these Poses") {
      THEN("They have to be equal") { REQUIRE(first_pose == second_pose); }
    }
    WHEN("I change something in the first pose") {
      first_pose.getPosition().setX(double_7);
      THEN("They have to be different") { REQUIRE(first_pose != second_pose); }
    }
  }

  // Testing message topic and stream operator.
  GIVEN("A point") {
    simple_msgs::Pose pose{};
    WHEN("I get the message topic") {
      std::string topic_name = pose.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POSE"); }
    }
    WHEN("I print the Pose") {
      std::ostringstream out;
      out << pose;
      THEN("The output is correct") {
        REQUIRE(out.str() == "Pose \n \tPoint \n \tx: " + std::to_string(pose.getPosition().getX()) +
                                 "\n \ty: " + std::to_string(pose.getPosition().getY()) +
                                 "\n \tz: " + std::to_string(pose.getPosition().getZ()) +
                                 "\nQuaternion \n \tx: " + std::to_string(pose.getQuaternion().getX()) +
                                 "\n \ty: " + std::to_string(pose.getQuaternion().getY()) +
                                 "\n \tz: " + std::to_string(pose.getQuaternion().getZ()) +
                                 "\n \tw: " + std::to_string(pose.getQuaternion().getW()) + "\n");
      }
    }
  }
}
