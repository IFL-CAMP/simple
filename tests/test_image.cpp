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

#include "simple_msgs/image.h"
#include "test_utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

// TEST FOR USING THE IMAGE MESSAGE WRAPPER

SCENARIO("Using an Image Message")
{
  simple_msgs::Pose empty_pose;
  simple_msgs::Header empty_header;
  double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
  auto random_int_1 = static_cast<int>(rand() / 100);
  auto random_int_2 = static_cast<int>(rand() / 100);
  auto random_int_3 = static_cast<int>(rand() / 100);
  std::string random_string = std::to_string(random_double_1);
  simple_msgs::Header random_header = createRandomHeader();
  simple_msgs::Pose random_pose = createRandomPose();
  // Testing constructor.
  GIVEN("A uint8_t Image created from an empty constructor")
  {
    simple_msgs::Image<uint8_t> empty_image;
    WHEN("I check the image's fields")
    {
      THEN("They have to match the default parameters")
      {
        REQUIRE(empty_image.getResolution()[0] == 0.0);
        REQUIRE(empty_image.getResolution()[1] == 0.0);
        REQUIRE(empty_image.getResolution()[2] == 0.0);
        REQUIRE(empty_image.getNumChannels() == 1);
        REQUIRE(empty_image.getImageDimensions()[0] == 0);
        REQUIRE(empty_image.getImageDimensions()[1] == 0);
        REQUIRE(empty_image.getImageDimensions()[2] == 0);
        REQUIRE(empty_image.getImageSize() == 0);
        REQUIRE(empty_image.getImageOrigin() == empty_pose);
        REQUIRE(empty_image.getHeader() == empty_header);
        REQUIRE(empty_image.getImageEncoding().empty());
        // REQUIRE(empty_image.getImageData() == nullptr);
      }
    }
    WHEN("I build another empty image from the buffer of the empty image")
    {
      simple_msgs::Image<uint8_t> copy_empty_image(empty_image.getBufferData());
      THEN("All image fields have to match the default parameters") { REQUIRE(copy_empty_image == empty_image); }
    }
  }

  // Testing copy-constructors.
  GIVEN("An image")
  {
    simple_msgs::Image<uint8_t> random_image;
    random_image.setImageResolution(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    WHEN("I copy construct a new image from the existing image's buffer")
    {
      simple_msgs::Image<uint8_t> buffer_copy_image(random_image.getBufferData());
      THEN("The two images are the same") { REQUIRE(buffer_copy_image == random_image); }
    }
    WHEN("I copy-construct a new image")
    {
      const simple_msgs::Image<uint8_t>& copy_image(random_image);
      THEN("Both images are the same") { REQUIRE(copy_image == random_image); }
    }
    WHEN("I move-construct a new image")
    {
      simple_msgs::Image<uint8_t> move_image(std::move(random_image));
      THEN("The new image's fields are the same as the original")
      {
        REQUIRE(move_image.getResolution()[0] == random_double_1);
        REQUIRE(move_image.getResolution()[1] == random_double_2);
        REQUIRE(move_image.getResolution()[2] == random_double_3);
        REQUIRE(move_image.getNumChannels() == 1);
        REQUIRE(move_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_image.getImageSize() == 0);
        REQUIRE(move_image.getImageOrigin() == random_pose);
        REQUIRE(move_image.getHeader() == random_header);
        REQUIRE(move_image.getImageEncoding() == random_string);
        // REQUIRE(move_image.getImageData() == nullptr);
      }
    }
  }

  // Testing copy-assignment.
  GIVEN("An image")
  {
    simple_msgs::Image<uint8_t> random_image;
    random_image.setImageResolution(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    WHEN("I copy assign a new image from the existing image's buffer")
    {
      simple_msgs::Image<uint8_t> buffer_assigned_image;
      buffer_assigned_image = random_image.getBufferData();
      THEN("The two images are the same") { REQUIRE(buffer_assigned_image == random_image); }
    }
    WHEN("I copy-assign a new image")
    {
      simple_msgs::Image<uint8_t> copy_assigned_image;
      copy_assigned_image = random_image;
      THEN("Both images are the same") { REQUIRE(copy_assigned_image == random_image); }
    }
    WHEN("I move-assign a new image")
    {
      simple_msgs::Image<uint8_t> move_assigned_image;
      move_assigned_image = std::move(random_image);
      THEN("The new image's fields are the same as the original")
      {
        REQUIRE(move_assigned_image.getResolution()[0] == random_double_1);
        REQUIRE(move_assigned_image.getResolution()[1] == random_double_2);
        REQUIRE(move_assigned_image.getResolution()[2] == random_double_3);
        REQUIRE(move_assigned_image.getNumChannels() == 1);
        REQUIRE(move_assigned_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_assigned_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_assigned_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_assigned_image.getImageSize() == 0);
        REQUIRE(move_assigned_image.getImageOrigin() == random_pose);
        REQUIRE(move_assigned_image.getHeader() == random_header);
        REQUIRE(move_assigned_image.getImageEncoding() == random_string);
        // REQUIRE(move_assigned_image.getImageData() == nullptr);
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical images")
  {
    simple_msgs::Image<uint8_t> random_image1;
    random_image1.setImageResolution(random_double_1, random_double_2, random_double_3);
    random_image1.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image1.setImageEncoding(random_string);
    random_image1.setHeader(random_header);
    random_image1.setOrigin(random_pose);
    simple_msgs::Image<uint8_t> random_image2;
    random_image2.setImageResolution(random_double_1, random_double_2, random_double_3);
    random_image2.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image2.setImageEncoding(random_string);
    random_image2.setHeader(random_header);
    random_image2.setOrigin(random_pose);
    WHEN("I compare them")
    {
      THEN("They have to be the same") { REQUIRE(random_image1 == random_image2); }
    }
    WHEN("I change something in the first image")
    {
      random_image1.setImageResolution(random_double_2, random_double_3, random_double_1);
      THEN("They have to be different") { REQUIRE(random_image1 != random_image2); }
    }
  }

  // Testing getters and setters.
  GIVEN("A uint8_t Image created from an empty constructor")
  {
    simple_msgs::Image<uint8_t> empty_image;
    WHEN("I set the image resolution")
    {
      empty_image.setImageResolution(random_double_1, random_double_2, random_double_3);
      THEN("The resolution is correct")
      {
        REQUIRE(empty_image.getResolution()[0] == random_double_1);
        REQUIRE(empty_image.getResolution()[1] == random_double_2);
        REQUIRE(empty_image.getResolution()[2] == random_double_3);
      }
    }
    WHEN("I set the image encoding")
    {
      empty_image.setImageEncoding(random_string);
      THEN("The encoding is correct") { REQUIRE(empty_image.getImageEncoding() == random_string); }
    }
    WHEN("I set the image dimensions")
    {
      empty_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
      THEN("The dimensions are correct")
      {
        REQUIRE(empty_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(empty_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(empty_image.getImageDimensions()[2] == random_int_3);
      }
    }
    WHEN("I set the image header")
    {
      empty_image.setHeader(random_header);
      THEN("The header is correct") { REQUIRE(empty_image.getHeader() == random_header); }
    }
    WHEN("I set the image origin")
    {
      empty_image.setOrigin(random_pose);
      THEN("The origin is correct") { REQUIRE(empty_image.getImageOrigin() == random_pose); }
    }
    WHEN("I get the message topic")
    {
      std::string topic_name = empty_image.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "IMAG"); }
    }
  }
}
