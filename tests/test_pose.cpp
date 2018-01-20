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

#include "simple_msgs/point.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/quaternion.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE POSE MESSAGE WRAPPER

SCENARIO("Using a Pose Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  double double_4 = static_cast<double>(rand()) / RAND_MAX;
  double double_5 = static_cast<double>(rand()) / RAND_MAX;
  double double_6 = static_cast<double>(rand()) / RAND_MAX;
  double double_7 = static_cast<double>(rand()) / RAND_MAX;
  simple_msgs::Point point_(double_1, double_2, double_3);
  simple_msgs::Quaternion quaternion_(double_4, double_5, double_6, double_7);
  // Test the constructors.
  GIVEN("A Pose created from an empty constructor")
  {
    simple_msgs::Pose empty_pose;
    WHEN("It is constructed")
    {
      THEN("It's elements have to be zero")
      {
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

  GIVEN("A Pose created from a point and a quaternion")
  {
    simple_msgs::Pose pose(point_, quaternion_);
    WHEN("I check the Pose's elements")
    {
      THEN("They all have to be equal to the parameters from the constructor")
      {
        REQUIRE(pose.getPosition() == point_);
        REQUIRE(pose.getQuaternion() == quaternion_);
      }
    }
  }

  GIVEN("A Pose created from the serialized data of an existing Pose")
  {
    simple_msgs::Pose pose(point_, quaternion_);
    simple_msgs::Pose buffer_pose(pose.getBufferData());
    WHEN("I check the Pose's elements")
    {
      THEN("The new Pose has to be equal to the other") { REQUIRE(buffer_pose == pose); }
    }
  }

  // Testing copy constructors.
  GIVEN("A Pose")
  {
    simple_msgs::Pose pose(point_, quaternion_);
    WHEN("I copy-construct a new Pose")
    {
      simple_msgs::Pose copy_pose(pose);
      THEN("The new Pose is equal to the other") { REQUIRE(copy_pose == pose); }
    }
    WHEN("I move-construct a new Pose")
    {
      simple_msgs::Pose move_pose(std::move(pose));
      THEN("The new Pose's elements are equal to the previous' ones")
      {
        REQUIRE(move_pose.getPosition() == point_);
        REQUIRE(move_pose.getQuaternion() == quaternion_);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A Pose")
  {
    simple_msgs::Pose pose(point_, quaternion_);
    WHEN("I copy-assign from that Pose's buffer")
    {
      simple_msgs::Pose copy_assigned_buffer_pose;
      copy_assigned_buffer_pose = pose.getBufferData();
      THEN("The new Pose has to be same as the original") { REQUIRE(copy_assigned_buffer_pose == pose); }
    }
    WHEN("I copy-assign from that Pose")
    {
      simple_msgs::Pose copy_assigned_pose;
      copy_assigned_pose = pose;
      THEN("The new Pose has to be same as the original") { REQUIRE(copy_assigned_pose == pose); }
    }
    WHEN("I move-assign from that Pose")
    {
      simple_msgs::Pose move_assigned_pose;
      move_assigned_pose = std::move(pose);
      THEN("The new Pose has to be same as the original")
      {
        REQUIRE(move_assigned_pose.getPosition() == point_);
        REQUIRE(move_assigned_pose.getQuaternion() == quaternion_);
      }
    }
  }

  // Testing coordinates setters/getters.
  GIVEN("An instance of a Pose.")
  {
    simple_msgs::Pose pose(point_, quaternion_);
    WHEN("I set the X coordinate of the Pose's position")
    {
      pose.getPosition().setX(double_7);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(pose.getPosition().getX() == double_7); }
    }
    WHEN("I set the Y coordinate of the Pose's position")
    {
      pose.getPosition().setY(double_6);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(pose.getPosition().getY() == double_6); }
    }
    WHEN("I set the Z coordinate of the Pose's position")
    {
      pose.getPosition().setZ(double_5);
      THEN("The data Pose's position has the correct coordinate") { REQUIRE(pose.getPosition().getZ() == double_5); }
    }
    WHEN("I set the X coordinate of the Pose's quaternion")
    {
      pose.getQuaternion().setX(double_1);
      THEN("The data Pose's quaternion has the correct coordinate")
      {
        REQUIRE(pose.getQuaternion().getX() == double_1);
      }
    }
    WHEN("I set the Y coordinate of the Pose's quaternion")
    {
      pose.getQuaternion().setY(double_2);
      THEN("The data Pose's quaternion has the correct coordinate")
      {
        REQUIRE(pose.getQuaternion().getY() == double_2);
      }
    }
    WHEN("I set the Z coordinate of the Pose's quaternion")
    {
      pose.getQuaternion().setZ(double_3);
      THEN("The data Pose's quaternion has the correct coordinate")
      {
        REQUIRE(pose.getQuaternion().getZ() == double_3);
      }
    }
    WHEN("I set the W coordinate of the Pose's quaternion")
    {
      pose.getQuaternion().setW(double_4);
      THEN("The data Pose's quaternion has the correct coordinate")
      {
        REQUIRE(pose.getQuaternion().getW() == double_4);
      }
    }
  }

  GIVEN("A Pose")
  {
    simple_msgs::Pose pose;
    WHEN("I set the position of the pose")
    {
      pose.setPosition(point_);
      THEN("The position is correct") { REQUIRE(pose.getPosition() == point_); }
    }
    WHEN("I set the quaternion of the pose")
    {
      pose.setQuaternion(quaternion_);
      THEN("The quaternion is correct") { REQUIRE(pose.getQuaternion() == quaternion_); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Poses")
  {
    simple_msgs::Pose first_pose(point_, quaternion_);
    simple_msgs::Pose second_pose(point_, quaternion_);
    WHEN("I compare these Poses")
    {
      THEN("They have to be equal") { REQUIRE(first_pose == second_pose); }
    }
    WHEN("I change something in the first pose")
    {
      first_pose.getPosition().setX(double_7);
      THEN("They have to be different") { REQUIRE(first_pose != second_pose); }
    }
  }

  // Testing Topic
  GIVEN("A point")
  {
    simple_msgs::Pose pose;
    WHEN("I get the message topic")
    {
      std::string topic_name = pose.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POSE"); }
    }
  }
}
