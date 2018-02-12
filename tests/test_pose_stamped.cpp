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

#include "simple_msgs/header.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/pose_stamped.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE STAMPED POSE MESSAGE WRAPPER

SCENARIO("Using a PoseStamped Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  double double_4 = static_cast<double>(rand()) / RAND_MAX;
  double double_5 = static_cast<double>(rand()) / RAND_MAX;
  double double_6 = static_cast<double>(rand()) / RAND_MAX;
  double double_7 = static_cast<double>(rand()) / RAND_MAX;
  long time = static_cast<long>(rand());
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::Point random_point(double_1, double_2, double_3);
  simple_msgs::Quaternion random_quaternion(double_4, double_5, double_6, double_7);
  simple_msgs::Pose random_pose(random_point, random_quaternion);
  simple_msgs::Pose empty_pose;
  simple_msgs::Header empty_header;
  simple_msgs::Header random_header(random_int, random_string, time);
  // Test the constructors.
  GIVEN("A PoseStamped created from an empty constructor")
  {
    simple_msgs::PoseStamped empty_pose_stamped;
    WHEN("It is constructed")
    {
      THEN("It's elements have to match the default")
      {
        REQUIRE(empty_pose_stamped.getHeader() == empty_header);
        REQUIRE(empty_pose_stamped.getPose() == empty_pose);
      }
    }
  }

  GIVEN("A PoseStamped created from a pose and a header")
  {
    simple_msgs::PoseStamped pose_stamped(random_header, random_pose);
    WHEN("I check the PoseStamped's elements")
    {
      THEN("They all have to be equal to the parameters from the constructor")
      {
        REQUIRE(pose_stamped.getPose() == random_pose);
        REQUIRE(pose_stamped.getHeader() == random_header);
      }
    }
  }

  GIVEN("A PoseStamped created from the serialized data of an existing PoseStamped")
  {
    simple_msgs::PoseStamped pose_stamped(random_header, random_pose);
    simple_msgs::PoseStamped buffer_pose_stamped(pose_stamped.getBufferData()->data());
    WHEN("I check the PoseStamped's elements")
    {
      THEN("The new PoseStamped has to be equal to the other") { REQUIRE(buffer_pose_stamped == pose_stamped); }
    }
  }

  // Testing copy constructors.
  GIVEN("A PoseStamped")
  {
    simple_msgs::PoseStamped pose_stamped(random_header, random_pose);
    WHEN("I copy-construct a new PoseStamped")
    {
      const simple_msgs::PoseStamped& copy_pose_stamped(pose_stamped);
      THEN("The new PoseStamped is equal to the other") { REQUIRE(copy_pose_stamped == pose_stamped); }
    }
    WHEN("I move-construct a new PoseStamped")
    {
      simple_msgs::PoseStamped move_pose_stamped(std::move(pose_stamped));
      THEN("The new PoseStamped's elements are equal to the previous' ones")
      {
        REQUIRE(move_pose_stamped.getPose() == random_pose);
        REQUIRE(move_pose_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A PoseStamped")
  {
    simple_msgs::PoseStamped pose_stamped(random_header, random_pose);
    WHEN("I copy-assign from that PoseStamped's buffer")
    {
      simple_msgs::PoseStamped copy_assigned_buffer_pose_stamped;
      copy_assigned_buffer_pose_stamped = pose_stamped.getBufferData()->data();
      THEN("The new PoseStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_buffer_pose_stamped == pose_stamped);
      }
    }
    WHEN("I copy-assign from that PoseStamped")
    {
      simple_msgs::PoseStamped copy_assigned_pose_stamped;
      copy_assigned_pose_stamped = pose_stamped;
      THEN("The new PoseStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_pose_stamped == pose_stamped);
      }
    }
    WHEN("I move-assign from that PoseStamped")
    {
      simple_msgs::PoseStamped move_assigned_pose_stamped;
      move_assigned_pose_stamped = std::move(pose_stamped);
      THEN("The new PoseStamped has to be same as the original")
      {
        REQUIRE(move_assigned_pose_stamped.getPose() == random_pose);
        REQUIRE(move_assigned_pose_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A PoseStamped")
  {
    simple_msgs::PoseStamped pose_stamped;
    WHEN("I set the pose of the pose_stamped")
    {
      pose_stamped.setPose(random_pose);
      THEN("The position is correct") { REQUIRE(pose_stamped.getPose() == random_pose); }
    }
    WHEN("I set the header of the pose_stamped")
    {
      pose_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(pose_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical PoseStampeds")
  {
    simple_msgs::PoseStamped first_pose_stamped(random_header, random_pose);
    simple_msgs::PoseStamped second_pose_stamped(random_header, random_pose);
    WHEN("I compare these PoseStampeds")
    {
      THEN("They have to be equal") { REQUIRE(first_pose_stamped == second_pose_stamped); }
    }
    WHEN("I change something in the first pose_stamped")
    {
      first_pose_stamped.getPose().getPosition().setX(double_7);
      THEN("They have to be different") { REQUIRE(first_pose_stamped != second_pose_stamped); }
    }
  }

  // Testing Topic
  GIVEN("A point")
  {
    simple_msgs::PoseStamped pose_stamped;
    WHEN("I get the message topic")
    {
      std::string topic_name = pose_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POST"); }
    }
    WHEN("I print the PoseStamped")
    {
      std::ostringstream out;
      out << pose_stamped;
      THEN("The output is correct")
      {
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
