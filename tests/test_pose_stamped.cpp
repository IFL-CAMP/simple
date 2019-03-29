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
#include "simple_msgs/pose_stamped.hpp"

using namespace simple_tests;

// TEST FOR USING THE STAMPED POSE MESSAGE WRAPPER

SCENARIO("Using a PoseStamped Message") {
  double double_1 = double_dist(generator);
  double double_2 = double_dist(generator);
  double double_3 = double_dist(generator);
  double double_4 = double_dist(generator);
  double double_5 = double_dist(generator);
  double double_6 = double_dist(generator);
  double double_7 = double_dist(generator);
  long long time = rand();
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::Point random_point{double_1, double_2, double_3};
  simple_msgs::Quaternion random_quaternion{double_4, double_5, double_6, double_7};
  simple_msgs::Pose random_pose{random_point, random_quaternion};
  simple_msgs::Pose empty_pose{};
  simple_msgs::Header empty_header{};
  simple_msgs::Header random_header{random_int, random_string, time};

  // Test the constructors.
  GIVEN("A PoseStamped created from an empty constructor") {
    simple_msgs::PoseStamped empty_pose_stamped{};
    WHEN("It is constructed") {
      THEN("Its elements have to match the default") {
        REQUIRE(empty_pose_stamped.getHeader() == empty_header);
        REQUIRE(empty_pose_stamped.getPose() == empty_pose);
      }
    }
  }

  GIVEN("A PoseStamped created from a pose and a header") {
    simple_msgs::PoseStamped pose_stamped{random_header, random_pose};
    WHEN("I check the PoseStamped elements") {
      THEN("They all have to be equal to the parameters from the constructor") {
        REQUIRE(pose_stamped.getPose() == random_pose);
        REQUIRE(pose_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing copy constructors.
  GIVEN("A PoseStamped") {
    simple_msgs::PoseStamped pose_stamped{random_header, random_pose};
    WHEN("Another PoseStamped is created from the serialized data of the original one") {
      simple_msgs::PoseStamped buffer_pose_stamped{pose_stamped.getBufferData()->data()};
      THEN("The new PoseStamped has to be equal to the other") { REQUIRE(buffer_pose_stamped == pose_stamped); }
    }
    WHEN("I copy-construct a new PoseStamped") {
      simple_msgs::PoseStamped copy_pose_stamped{pose_stamped};
      THEN("The new PoseStamped is equal to the other") { REQUIRE(copy_pose_stamped == pose_stamped); }
    }
    WHEN("I move-construct a new PoseStamped") {
      simple_msgs::PoseStamped move_pose_stamped{std::move(pose_stamped)};
      THEN("The new PoseStamped's elements are equal to the previous' ones") {
        REQUIRE(move_pose_stamped.getPose() == random_pose);
        REQUIRE(move_pose_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A PoseStamped") {
    simple_msgs::PoseStamped pose_stamped{random_header, random_pose};
    WHEN("I copy-assign from that PoseStamped's buffer") {
      simple_msgs::PoseStamped copy_assigned_buffer_pose_stamped{};
      auto data_ptr = std::make_shared<void*>(pose_stamped.getBufferData()->data());
      copy_assigned_buffer_pose_stamped = data_ptr;
      THEN("The new PoseStamped has to be same as the original") {
        REQUIRE(copy_assigned_buffer_pose_stamped == pose_stamped);
      }
    }
    WHEN("I copy-assign from that PoseStamped") {
      simple_msgs::PoseStamped copy_assigned_pose_stamped{};
      copy_assigned_pose_stamped = pose_stamped;
      THEN("The new PoseStamped has to be same as the original") {
        REQUIRE(copy_assigned_pose_stamped == pose_stamped);
      }
    }
    WHEN("I move-assign from that PoseStamped") {
      simple_msgs::PoseStamped move_assigned_pose_stamped{};
      move_assigned_pose_stamped = std::move(pose_stamped);
      THEN("The new PoseStamped has to be same as the original") {
        REQUIRE(move_assigned_pose_stamped.getPose() == random_pose);
        REQUIRE(move_assigned_pose_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A PoseStamped") {
    simple_msgs::PoseStamped pose_stamped{};
    WHEN("I set the pose of the pose_stamped") {
      pose_stamped.setPose(random_pose);
      THEN("The position is correct") { REQUIRE(pose_stamped.getPose() == random_pose); }
    }
    WHEN("I set the header of the pose_stamped") {
      pose_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(pose_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical PoseStampeds") {
    simple_msgs::PoseStamped first_pose_stamped{random_header, random_pose};
    simple_msgs::PoseStamped second_pose_stamped{random_header, random_pose};
    WHEN("I compare these PoseStampeds") {
      THEN("They have to be equal") { REQUIRE(first_pose_stamped == second_pose_stamped); }
    }
    WHEN("I change something in the first pose_stamped") {
      first_pose_stamped.getPose().getPosition().setX(double_7);
      THEN("They have to be different") { REQUIRE(first_pose_stamped != second_pose_stamped); }
    }
  }

  // Testing message topic and stream operators.
  GIVEN("A point") {
    simple_msgs::PoseStamped pose_stamped{};
    WHEN("I get the message topic") {
      std::string topic_name = pose_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POST"); }
    }
    WHEN("I print the PoseStamped") {
      std::ostringstream out;
      out << pose_stamped;
      THEN("The output is correct") {
        REQUIRE(out.str() ==
                "Header\n \tseq_n: " + std::to_string(pose_stamped.getHeader().getSequenceNumber()) +
                    "\n \tframe_id: " + pose_stamped.getHeader().getFrameID() +
                    "\n \ttimestamp: " + std::to_string(pose_stamped.getHeader().getTimestamp()) +
                    "\nPose \n \tPoint \n \tx: " + std::to_string(pose_stamped.getPose().getPosition().getX()) +
                    "\n \ty: " + std::to_string(pose_stamped.getPose().getPosition().getY()) +
                    "\n \tz: " + std::to_string(pose_stamped.getPose().getPosition().getZ()) +
                    "\nQuaternion \n \tx: " + std::to_string(pose_stamped.getPose().getQuaternion().getX()) +
                    "\n \ty: " + std::to_string(pose_stamped.getPose().getQuaternion().getY()) +
                    "\n \tz: " + std::to_string(pose_stamped.getPose().getQuaternion().getZ()) +
                    "\n \tw: " + std::to_string(pose_stamped.getPose().getQuaternion().getW()) + "\n");
      }
    }
  }
}
