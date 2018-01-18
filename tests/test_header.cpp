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

#include <iostream>
#include "simple_msgs/header.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE HEADER MESSAGE WRAPPER

SCENARIO("Using a Header Message")
{
  int int_1 = static_cast<int>(rand()) / RAND_MAX;
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  std::string string_1 = std::to_string(double_1);
  // Testing constructors.
  GIVEN("A Header created from an empty constructor")
  {
    simple_msgs::Header empty_header;
    WHEN("We check the Header's fields")
    {
      THEN("They have to be the default ones")
      {
        REQUIRE(empty_header.getSequenceNumber() == 0);
        REQUIRE(empty_header.getFrameID() == "");
        REQUIRE(empty_header.getTimestamp() == 0.0);
      }
    }
  }

  GIVEN("A Header created from a int, a string and a double")
  {
    simple_msgs::Header normal_header(int_1, string_1, double_1);
    WHEN("We check the Header's fields")
    {
      THEN("It has to be equal to the params from the constructor")
      {
        REQUIRE(normal_header.getSequenceNumber() == int_1);
        REQUIRE(normal_header.getFrameID() == string_1);
        REQUIRE(normal_header.getTimestamp() == double_1);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("A header")
  {
    simple_msgs::Header normal_header(int_1, string_1, double_1);
    WHEN("I construct a new Header from the serialized data of the existing Header")
    {
      simple_msgs::Header buffer_copy_header(normal_header.getBufferData());
      THEN("The new Header has to be equal to the other")
      {
        REQUIRE(buffer_copy_header.getSequenceNumber() == int_1);
        REQUIRE(buffer_copy_header.getFrameID() == string_1);
        REQUIRE(buffer_copy_header.getTimestamp() == double_1);
      }
    }
    WHEN("I copy-construct a new Header")
    {
      simple_msgs::Header copy_header(normal_header);
      THEN("The new Header is equal to the other")
      {
        REQUIRE(copy_header.getSequenceNumber() == int_1);
        REQUIRE(copy_header.getFrameID() == string_1);
        REQUIRE(copy_header.getTimestamp() == double_1);
      }
    }
    WHEN("I move-construct a new Header")
    {
      simple_msgs::Header moved_header(std::move(normal_header));
      THEN("The new Header's fields are equal to the previous' one")
      {
        REQUIRE(moved_header.getSequenceNumber() == int_1);
        REQUIRE(moved_header.getFrameID() == string_1);
        REQUIRE(moved_header.getTimestamp() == double_1);
      }
    }
  }

  // Testing copy-assignments.
  GIVEN("A Header")
  {
    simple_msgs::Header normal_header(int_1, string_1, double_1);
    WHEN("I copy-assign from that Header's buffer")
    {
      simple_msgs::Header copy_assigned_buffer_header;
      copy_assigned_buffer_header = normal_header.getBufferData();
      THEN("The new Header has to be same as the original") { REQUIRE(copy_assigned_buffer_header == normal_header); }
    }
    WHEN("I copy-assign from that Header")
    {
      simple_msgs::Header copy_assigned_header;
      copy_assigned_header = normal_header;
      THEN("The new Header has to be same as the original") { REQUIRE(copy_assigned_header == normal_header); }
    }
    WHEN("I move-assign from that Header")
    {
      simple_msgs::Header move_assigned_header;
      move_assigned_header = std::move(normal_header);
      THEN("The new Header has to be same as the original")
      {
        REQUIRE(move_assigned_header.getSequenceNumber() == int_1);
        REQUIRE(move_assigned_header.getFrameID() == string_1);
        REQUIRE(move_assigned_header.getTimestamp() == double_1);
      }
    }
  }

  // Testing getters and setters.
  GIVEN("An empty Header.")
  {
    simple_msgs::Header empty_header;

    WHEN("I set the time stamp of the Header")
    {
      empty_header.setTimestamp(double_1);
      THEN("The Header's time stamp is correct") { REQUIRE(empty_header.getTimestamp() == double_1); }
    }
    WHEN("I set the frame id of the Header")
    {
      empty_header.setFrameID(string_1);
      THEN("The Header's frame id is correct") { REQUIRE(empty_header.getFrameID() == string_1); }
    }
    WHEN("I set the sequence number of the Header")
    {
      empty_header.setSequenceNumber(int_1);
      THEN("The Header's sequence number is correct") { REQUIRE(empty_header.getSequenceNumber() == int_1); }
    }
  }

  // Testing operations.
  GIVEN("Two identical Headers")
  {
    simple_msgs::Header header1(int_1, string_1, double_1);
    simple_msgs::Header header2(int_1, string_1, double_1);
    WHEN("I compare these Headers")
    {
      THEN("They have to be equal") { REQUIRE(header1 == header2); }
    }
    WHEN("I change something in the first header")
    {
      header1.setTimestamp(double_2);
      THEN("They have to be different") { REQUIRE(header1 != header2); }
    }
    WHEN("I get the message topic")
    {
      std::string topic_name = header1.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "HEAD"); }
    }
  }
}
