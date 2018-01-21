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
#include "simple_msgs/point.h"
#include "simple_msgs/point_stamped.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE STAMPED POINT MESSAGE WRAPPER

SCENARIO("Using a PointStamped Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  int random_int = rand() / 100;
  std::string random_string = std::to_string(double_1);
  simple_msgs::Point random_point(double_1, double_2, double_3);
  simple_msgs::Point empty_point;
  simple_msgs::Header empty_header;
  simple_msgs::Header random_header(random_int, random_string, double_1);
  // Test the constructors.
  GIVEN("A PointStamped created from an empty constructor")
  {
    simple_msgs::PointStamped empty_point_stamped;
    WHEN("It is constructed")
    {
      THEN("It's elements have to match the default")
      {
        REQUIRE(empty_point_stamped.getHeader() == empty_header);
        REQUIRE(empty_point_stamped.getPoint() == empty_point);
      }
    }
  }

  GIVEN("A PointStamped created from a point and a header")
  {
    simple_msgs::PointStamped point_stamped(random_header, random_point);
    WHEN("I check the PointStamped's elements")
    {
      THEN("They all have to be equal to the parameters from the constructor")
      {
        REQUIRE(point_stamped.getPoint() == random_point);
        REQUIRE(point_stamped.getHeader() == random_header);
      }
    }
  }

  GIVEN("A PointStamped created from the serialized data of an existing PointStamped")
  {
    simple_msgs::PointStamped point_stamped(random_header, random_point);
    simple_msgs::PointStamped buffer_point_stamped(point_stamped.getBufferData());
    WHEN("I check the PointStamped's elements")
    {
      THEN("The new PointStamped has to be equal to the other") { REQUIRE(buffer_point_stamped == point_stamped); }
    }
  }

  // Testing copy constructors.
  GIVEN("A PointStamped")
  {
    simple_msgs::PointStamped point_stamped(random_header, random_point);
    WHEN("I copy-construct a new PointStamped")
    {
      const simple_msgs::PointStamped& copy_point_stamped(point_stamped);
      THEN("The new PointStamped is equal to the other") { REQUIRE(copy_point_stamped == point_stamped); }
    }
    WHEN("I move-construct a new PointStamped")
    {
      simple_msgs::PointStamped move_point_stamped(std::move(point_stamped));
      THEN("The new PointStamped's elements are equal to the previous' ones")
      {
        REQUIRE(move_point_stamped.getPoint() == random_point);
        REQUIRE(move_point_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A PointStamped")
  {
    simple_msgs::PointStamped point_stamped(random_header, random_point);
    WHEN("I copy-assign from that PointStamped's buffer")
    {
      simple_msgs::PointStamped copy_assigned_buffer_point_stamped;
      copy_assigned_buffer_point_stamped = point_stamped.getBufferData();
      THEN("The new PointStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_buffer_point_stamped == point_stamped);
      }
    }
    WHEN("I copy-assign from that PointStamped")
    {
      simple_msgs::PointStamped copy_assigned_point_stamped;
      copy_assigned_point_stamped = point_stamped;
      THEN("The new PointStamped has to be same as the original")
      {
        REQUIRE(copy_assigned_point_stamped == point_stamped);
      }
    }
    WHEN("I move-assign from that PointStamped")
    {
      simple_msgs::PointStamped move_assigned_point_stamped;
      move_assigned_point_stamped = std::move(point_stamped);
      THEN("The new PointStamped has to be same as the original")
      {
        REQUIRE(move_assigned_point_stamped.getPoint() == random_point);
        REQUIRE(move_assigned_point_stamped.getHeader() == random_header);
      }
    }
  }

  // Testing setters/getters.
  GIVEN("A PointStamped")
  {
    simple_msgs::PointStamped point_stamped;
    WHEN("I set the point of the point_stamped")
    {
      point_stamped.setPoint(random_point);
      THEN("The position is correct") { REQUIRE(point_stamped.getPoint() == random_point); }
    }
    WHEN("I set the header of the point_stamped")
    {
      point_stamped.setHeader(random_header);
      THEN("The quaternion is correct") { REQUIRE(point_stamped.getHeader() == random_header); }
    }
  }

  // Testing operations.
  GIVEN("Two identical PointStampeds")
  {
    simple_msgs::PointStamped first_point_stamped(random_header, random_point);
    simple_msgs::PointStamped second_point_stamped(random_header, random_point);
    WHEN("I compare these PointStampeds")
    {
      THEN("They have to be equal") { REQUIRE(first_point_stamped == second_point_stamped); }
    }
    WHEN("I change something in the first point_stamped")
    {
      first_point_stamped.getPoint().setX(double_2);
      THEN("They have to be different") { REQUIRE(first_point_stamped != second_point_stamped); }
    }
  }

  // Testing Topic
  GIVEN("A point")
  {
    simple_msgs::PointStamped point_stamped;
    WHEN("I get the message topic")
    {
      std::string topic_name = point_stamped.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "PTST"); }
    }
  }
}
