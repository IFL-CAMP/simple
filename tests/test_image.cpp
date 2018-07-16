/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "simple_msgs/image.hpp"
#include "test_utils.hpp"

// TEST FOR USING THE IMAGE MESSAGE WRAPPER

SCENARIO("Using an uint8 Image Message") {
  simple_msgs::Pose empty_pose{};
  simple_msgs::Header empty_header{};
  double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
  auto random_int_1 = rand() / 100;
  auto random_int_2 = rand() / 100;
  auto random_int_3 = rand() / 100;
  std::string random_string = std::to_string(random_double_1);
  simple_msgs::Header random_header = createRandomHeader();
  simple_msgs::Pose random_pose = createRandomPose();
  // uint8_t data
  int image_size{1};
  auto image_data = std::make_shared<uint8_t>(static_cast<uint8_t>(rand() % 256));

  // Testing constructor.
  GIVEN("A uint8_t Image created from an empty constructor") {
    simple_msgs::Image<uint8_t> empty_image{};
    WHEN("I check the Image fields") {
      THEN("They have to match the default parameters") {
        REQUIRE(empty_image.getSpacing()[0] == 0.0);
        REQUIRE(empty_image.getSpacing()[1] == 0.0);
        REQUIRE(empty_image.getSpacing()[2] == 0.0);
        REQUIRE(empty_image.getNumChannels() == 1);
        REQUIRE(empty_image.getImageDimensions()[0] == 0);
        REQUIRE(empty_image.getImageDimensions()[1] == 0);
        REQUIRE(empty_image.getImageDimensions()[2] == 0);
        REQUIRE(empty_image.getImageSize() == 0);
        REQUIRE(empty_image.getImageOrigin() == empty_pose);
        REQUIRE(empty_image.getHeader() == empty_header);
        REQUIRE(empty_image.getImageEncoding().empty());
        REQUIRE(empty_image.getImageData() == nullptr);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("An Image") {
    simple_msgs::Image<uint8_t> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data, image_size);
    WHEN("I copy-construct a new Image") {
      simple_msgs::Image<uint8_t> copy_image{random_image};
      THEN("Both images are the same") { REQUIRE(copy_image == random_image); }
    }
    WHEN("I move-construct a new Image") {
      simple_msgs::Image<uint8_t> move_image{std::move(random_image)};
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_image.getNumChannels() == 1);
        REQUIRE(move_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_image.getImageSize() == image_size);
        REQUIRE(move_image.getImageOrigin() == random_pose);
        REQUIRE(move_image.getHeader() == random_header);
        REQUIRE(move_image.getImageEncoding() == random_string);
        REQUIRE((move_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing copy-assignment.
  GIVEN("An Image") {
    simple_msgs::Image<uint8_t> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data.get(), image_size);
    WHEN("I copy-assign an Image from the original buffer") {
      simple_msgs::Image<uint8_t> copy_buffer_image{};
      auto data_ptr = std::make_shared<void*>(random_image.getBufferData()->data());
      copy_buffer_image = data_ptr;
      THEN("The new Image has to be same as the original") { REQUIRE(copy_buffer_image == random_image); }
    }
    WHEN("I copy-assign a new Image") {
      simple_msgs::Image<uint8_t> copy_assigned_image{};
      copy_assigned_image = random_image;
      THEN("Both images are the same") { REQUIRE(copy_assigned_image == random_image); }
    }
    WHEN("I move-assign a new Image") {
      simple_msgs::Image<uint8_t> move_assigned_image{};
      move_assigned_image = std::move(random_image);
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_assigned_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_assigned_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_assigned_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_assigned_image.getNumChannels() == 1);
        REQUIRE(move_assigned_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_assigned_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_assigned_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_assigned_image.getImageSize() == image_size);
        REQUIRE(move_assigned_image.getImageOrigin() == random_pose);
        REQUIRE(move_assigned_image.getHeader() == random_header);
        REQUIRE(move_assigned_image.getImageEncoding() == random_string);
        REQUIRE((move_assigned_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Images") {
    simple_msgs::Image<uint8_t> random_image1{};
    random_image1.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image1.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image1.setImageEncoding(random_string);
    random_image1.setHeader(random_header);
    random_image1.setOrigin(random_pose);
    simple_msgs::Image<uint8_t> random_image2{};
    random_image2.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image2.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image2.setImageEncoding(random_string);
    random_image2.setHeader(random_header);
    random_image2.setOrigin(random_pose);
    WHEN("I compare them") {
      THEN("They have to be the same") { REQUIRE(random_image1 == random_image2); }
    }
    WHEN("I change something in the first Image") {
      random_image1.setImageSpacing(random_double_2, random_double_3, random_double_1);
      THEN("They have to be different") { REQUIRE(random_image1 != random_image2); }
    }
  }

  // Testing getters and setters.
  GIVEN("A uint8_t Image created from an empty constructor") {
    simple_msgs::Image<uint8_t> empty_image{};
    WHEN("I set the Image resolution") {
      empty_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
      THEN("The resolution is correct") {
        REQUIRE(empty_image.getSpacing()[0] == random_double_1);
        REQUIRE(empty_image.getSpacing()[1] == random_double_2);
        REQUIRE(empty_image.getSpacing()[2] == random_double_3);
      }
    }
    WHEN("I set the Image encoding") {
      empty_image.setImageEncoding(random_string);
      THEN("The encoding is correct") { REQUIRE(empty_image.getImageEncoding() == random_string); }
    }
    WHEN("I set the Image dimensions") {
      empty_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
      THEN("The dimensions are correct") {
        REQUIRE(empty_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(empty_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(empty_image.getImageDimensions()[2] == random_int_3);
      }
    }
    WHEN("I set the Image header") {
      empty_image.setHeader(random_header);
      THEN("The header is correct") { REQUIRE(empty_image.getHeader() == random_header); }
    }
    WHEN("I set the Image origin") {
      empty_image.setOrigin(random_pose);
      THEN("The origin is correct") { REQUIRE(empty_image.getImageOrigin() == random_pose); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = empty_image.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "IMAG"); }
    }
  }
}

SCENARIO("Using an int16_t Image Message") {
  simple_msgs::Pose empty_pose{};
  simple_msgs::Header empty_header{};
  double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
  auto random_int_1 = rand() / 100;
  auto random_int_2 = rand() / 100;
  auto random_int_3 = rand() / 100;
  std::string random_string = std::to_string(random_double_1);
  simple_msgs::Header random_header = createRandomHeader();
  simple_msgs::Pose random_pose = createRandomPose();
  // int16_t data
  int image_size{1};
  auto image_data = std::make_shared<int16_t>(static_cast<int16_t>(rand() % 256));

  // Testing constructor.
  GIVEN("A int16_t Image created from an empty constructor") {
    simple_msgs::Image<int16_t> empty_image{};
    WHEN("I check the Image fields") {
      THEN("They have to match the default parameters") {
        REQUIRE(empty_image.getSpacing()[0] == 0.0);
        REQUIRE(empty_image.getSpacing()[1] == 0.0);
        REQUIRE(empty_image.getSpacing()[2] == 0.0);
        REQUIRE(empty_image.getNumChannels() == 1);
        REQUIRE(empty_image.getImageDimensions()[0] == 0);
        REQUIRE(empty_image.getImageDimensions()[1] == 0);
        REQUIRE(empty_image.getImageDimensions()[2] == 0);
        REQUIRE(empty_image.getImageSize() == 0);
        REQUIRE(empty_image.getImageOrigin() == empty_pose);
        REQUIRE(empty_image.getHeader() == empty_header);
        REQUIRE(empty_image.getImageEncoding().empty());
        REQUIRE(empty_image.getImageData() == nullptr);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("An Image") {
    simple_msgs::Image<int16_t> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data, image_size);
    WHEN("I copy-construct a new Image") {
      simple_msgs::Image<int16_t> copy_image{random_image};
      THEN("Both images are the same") { REQUIRE(copy_image == random_image); }
    }
    WHEN("I move-construct a new Image") {
      simple_msgs::Image<int16_t> move_image{std::move(random_image)};
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_image.getNumChannels() == 1);
        REQUIRE(move_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_image.getImageSize() == image_size);
        REQUIRE(move_image.getImageOrigin() == random_pose);
        REQUIRE(move_image.getHeader() == random_header);
        REQUIRE(move_image.getImageEncoding() == random_string);
        REQUIRE((move_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing copy-assignment.
  GIVEN("An Image") {
    simple_msgs::Image<int16_t> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data.get(), image_size);
    WHEN("I copy-assign an Image from the original buffer") {
      simple_msgs::Image<int16_t> copy_buffer_image{};
      auto data_ptr = std::make_shared<void*>(random_image.getBufferData()->data());
      copy_buffer_image = data_ptr;
      THEN("The new Image has to be same as the original") { REQUIRE(copy_buffer_image == random_image); }
    }
    WHEN("I copy-assign a new Image") {
      simple_msgs::Image<int16_t> copy_assigned_image{};
      copy_assigned_image = random_image;
      THEN("Both images are the same") { REQUIRE(copy_assigned_image == random_image); }
    }
    WHEN("I move-assign a new Image") {
      simple_msgs::Image<int16_t> move_assigned_image{};
      move_assigned_image = std::move(random_image);
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_assigned_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_assigned_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_assigned_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_assigned_image.getNumChannels() == 1);
        REQUIRE(move_assigned_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_assigned_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_assigned_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_assigned_image.getImageSize() == image_size);
        REQUIRE(move_assigned_image.getImageOrigin() == random_pose);
        REQUIRE(move_assigned_image.getHeader() == random_header);
        REQUIRE(move_assigned_image.getImageEncoding() == random_string);
        REQUIRE((move_assigned_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Images") {
    simple_msgs::Image<int16_t> random_image1{};
    random_image1.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image1.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image1.setImageEncoding(random_string);
    random_image1.setHeader(random_header);
    random_image1.setOrigin(random_pose);
    simple_msgs::Image<int16_t> random_image2{};
    random_image2.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image2.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image2.setImageEncoding(random_string);
    random_image2.setHeader(random_header);
    random_image2.setOrigin(random_pose);
    WHEN("I compare them") {
      THEN("They have to be the same") { REQUIRE(random_image1 == random_image2); }
    }
    WHEN("I change something in the first Image") {
      random_image1.setImageSpacing(random_double_2, random_double_3, random_double_1);
      THEN("They have to be different") { REQUIRE(random_image1 != random_image2); }
    }
  }

  // Testing getters and setters.
  GIVEN("A int16_t Image created from an empty constructor") {
    simple_msgs::Image<int16_t> empty_image{};
    WHEN("I set the Image resolution") {
      empty_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
      THEN("The resolution is correct") {
        REQUIRE(empty_image.getSpacing()[0] == random_double_1);
        REQUIRE(empty_image.getSpacing()[1] == random_double_2);
        REQUIRE(empty_image.getSpacing()[2] == random_double_3);
      }
    }
    WHEN("I set the Image encoding") {
      empty_image.setImageEncoding(random_string);
      THEN("The encoding is correct") { REQUIRE(empty_image.getImageEncoding() == random_string); }
    }
    WHEN("I set the Image dimensions") {
      empty_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
      THEN("The dimensions are correct") {
        REQUIRE(empty_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(empty_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(empty_image.getImageDimensions()[2] == random_int_3);
      }
    }
    WHEN("I set the Image header") {
      empty_image.setHeader(random_header);
      THEN("The header is correct") { REQUIRE(empty_image.getHeader() == random_header); }
    }
    WHEN("I set the Image origin") {
      empty_image.setOrigin(random_pose);
      THEN("The origin is correct") { REQUIRE(empty_image.getImageOrigin() == random_pose); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = empty_image.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "IMAG"); }
    }
  }
}

SCENARIO("Using a float Image Message") {
  simple_msgs::Pose empty_pose{};
  simple_msgs::Header empty_header{};
  double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
  auto random_int_1 = rand() / 100;
  auto random_int_2 = rand() / 100;
  auto random_int_3 = rand() / 100;
  std::string random_string = std::to_string(random_double_1);
  simple_msgs::Header random_header = createRandomHeader();
  simple_msgs::Pose random_pose = createRandomPose();
  // float data
  int image_size{1};
  auto image_data = std::make_shared<float>(static_cast<float>(rand() % 256));

  // Testing constructor.
  GIVEN("A float Image created from an empty constructor") {
    simple_msgs::Image<float> empty_image{};
    WHEN("I check the Image fields") {
      THEN("They have to match the default parameters") {
        REQUIRE(empty_image.getSpacing()[0] == 0.0);
        REQUIRE(empty_image.getSpacing()[1] == 0.0);
        REQUIRE(empty_image.getSpacing()[2] == 0.0);
        REQUIRE(empty_image.getNumChannels() == 1);
        REQUIRE(empty_image.getImageDimensions()[0] == 0);
        REQUIRE(empty_image.getImageDimensions()[1] == 0);
        REQUIRE(empty_image.getImageDimensions()[2] == 0);
        REQUIRE(empty_image.getImageSize() == 0);
        REQUIRE(empty_image.getImageOrigin() == empty_pose);
        REQUIRE(empty_image.getHeader() == empty_header);
        REQUIRE(empty_image.getImageEncoding().empty());
        REQUIRE(empty_image.getImageData() == nullptr);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("An Image") {
    simple_msgs::Image<float> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data, image_size);
    WHEN("I copy-construct a new Image") {
      simple_msgs::Image<float> copy_image{random_image};
      THEN("Both images are the same") { REQUIRE(copy_image == random_image); }
    }
    WHEN("I move-construct a new Image") {
      simple_msgs::Image<float> move_image{std::move(random_image)};
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_image.getNumChannels() == 1);
        REQUIRE(move_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_image.getImageSize() == image_size);
        REQUIRE(move_image.getImageOrigin() == random_pose);
        REQUIRE(move_image.getHeader() == random_header);
        REQUIRE(move_image.getImageEncoding() == random_string);
        REQUIRE((move_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing copy-assignment.
  GIVEN("An Image") {
    simple_msgs::Image<float> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data.get(), image_size);
    WHEN("I copy-assign an Image from the original buffer") {
      simple_msgs::Image<float> copy_buffer_image{};
      auto data_ptr = std::make_shared<void*>(random_image.getBufferData()->data());
      copy_buffer_image = data_ptr;
      THEN("The new Image has to be same as the original") { REQUIRE(copy_buffer_image == random_image); }
    }
    WHEN("I copy-assign a new Image") {
      simple_msgs::Image<float> copy_assigned_image{};
      copy_assigned_image = random_image;
      THEN("Both images are the same") { REQUIRE(copy_assigned_image == random_image); }
    }
    WHEN("I move-assign a new Image") {
      simple_msgs::Image<float> move_assigned_image{};
      move_assigned_image = std::move(random_image);
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_assigned_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_assigned_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_assigned_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_assigned_image.getNumChannels() == 1);
        REQUIRE(move_assigned_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_assigned_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_assigned_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_assigned_image.getImageSize() == image_size);
        REQUIRE(move_assigned_image.getImageOrigin() == random_pose);
        REQUIRE(move_assigned_image.getHeader() == random_header);
        REQUIRE(move_assigned_image.getImageEncoding() == random_string);
        REQUIRE((move_assigned_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Images") {
    simple_msgs::Image<float> random_image1{};
    random_image1.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image1.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image1.setImageEncoding(random_string);
    random_image1.setHeader(random_header);
    random_image1.setOrigin(random_pose);
    simple_msgs::Image<float> random_image2{};
    random_image2.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image2.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image2.setImageEncoding(random_string);
    random_image2.setHeader(random_header);
    random_image2.setOrigin(random_pose);
    WHEN("I compare them") {
      THEN("They have to be the same") { REQUIRE(random_image1 == random_image2); }
    }
    WHEN("I change something in the first Image") {
      random_image1.setImageSpacing(random_double_2, random_double_3, random_double_1);
      THEN("They have to be different") { REQUIRE(random_image1 != random_image2); }
    }
  }

  // Testing getters and setters.
  GIVEN("A float Image created from an empty constructor") {
    simple_msgs::Image<float> empty_image{};
    WHEN("I set the Image resolution") {
      empty_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
      THEN("The resolution is correct") {
        REQUIRE(empty_image.getSpacing()[0] == random_double_1);
        REQUIRE(empty_image.getSpacing()[1] == random_double_2);
        REQUIRE(empty_image.getSpacing()[2] == random_double_3);
      }
    }
    WHEN("I set the Image encoding") {
      empty_image.setImageEncoding(random_string);
      THEN("The encoding is correct") { REQUIRE(empty_image.getImageEncoding() == random_string); }
    }
    WHEN("I set the Image dimensions") {
      empty_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
      THEN("The dimensions are correct") {
        REQUIRE(empty_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(empty_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(empty_image.getImageDimensions()[2] == random_int_3);
      }
    }
    WHEN("I set the Image header") {
      empty_image.setHeader(random_header);
      THEN("The header is correct") { REQUIRE(empty_image.getHeader() == random_header); }
    }
    WHEN("I set the Image origin") {
      empty_image.setOrigin(random_pose);
      THEN("The origin is correct") { REQUIRE(empty_image.getImageOrigin() == random_pose); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = empty_image.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "IMAG"); }
    }
  }
}

SCENARIO("Using an double Image Message") {
  simple_msgs::Pose empty_pose{};
  simple_msgs::Header empty_header{};
  double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
  double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
  auto random_int_1 = rand() / 100;
  auto random_int_2 = rand() / 100;
  auto random_int_3 = rand() / 100;
  std::string random_string = std::to_string(random_double_1);
  simple_msgs::Header random_header = createRandomHeader();
  simple_msgs::Pose random_pose = createRandomPose();
  // double data
  int image_size{1};
  auto image_data = std::make_shared<double>(static_cast<double>(rand() % 256));

  // Testing constructor.
  GIVEN("A double Image created from an empty constructor") {
    simple_msgs::Image<double> empty_image{};
    WHEN("I check the Image fields") {
      THEN("They have to match the default parameters") {
        REQUIRE(empty_image.getSpacing()[0] == 0.0);
        REQUIRE(empty_image.getSpacing()[1] == 0.0);
        REQUIRE(empty_image.getSpacing()[2] == 0.0);
        REQUIRE(empty_image.getNumChannels() == 1);
        REQUIRE(empty_image.getImageDimensions()[0] == 0);
        REQUIRE(empty_image.getImageDimensions()[1] == 0);
        REQUIRE(empty_image.getImageDimensions()[2] == 0);
        REQUIRE(empty_image.getImageSize() == 0);
        REQUIRE(empty_image.getImageOrigin() == empty_pose);
        REQUIRE(empty_image.getHeader() == empty_header);
        REQUIRE(empty_image.getImageEncoding().empty());
        REQUIRE(empty_image.getImageData() == nullptr);
      }
    }
  }

  // Testing copy-constructors.
  GIVEN("An Image") {
    simple_msgs::Image<double> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data, image_size);
    WHEN("I copy-construct a new Image") {
      simple_msgs::Image<double> copy_image{random_image};
      THEN("Both images are the same") { REQUIRE(copy_image == random_image); }
    }
    WHEN("I move-construct a new Image") {
      simple_msgs::Image<double> move_image{std::move(random_image)};
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_image.getNumChannels() == 1);
        REQUIRE(move_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_image.getImageSize() == image_size);
        REQUIRE(move_image.getImageOrigin() == random_pose);
        REQUIRE(move_image.getHeader() == random_header);
        REQUIRE(move_image.getImageEncoding() == random_string);
        REQUIRE((move_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing copy-assignment.
  GIVEN("An Image") {
    simple_msgs::Image<double> random_image{};
    random_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image.setImageEncoding(random_string);
    random_image.setHeader(random_header);
    random_image.setOrigin(random_pose);
    random_image.setImageData(image_data.get(), image_size);
    WHEN("I copy-assign an Image from the original buffer") {
      simple_msgs::Image<double> copy_buffer_image{};
      auto data_ptr = std::make_shared<void*>(random_image.getBufferData()->data());
      copy_buffer_image = data_ptr;
      THEN("The new Image has to be same as the original") { REQUIRE(copy_buffer_image == random_image); }
    }
    WHEN("I copy-assign a new Image") {
      simple_msgs::Image<double> copy_assigned_image{};
      copy_assigned_image = random_image;
      THEN("Both images are the same") { REQUIRE(copy_assigned_image == random_image); }
    }
    WHEN("I move-assign a new Image") {
      simple_msgs::Image<double> move_assigned_image{};
      move_assigned_image = std::move(random_image);
      THEN("The new Image fields are the same as the original") {
        REQUIRE(move_assigned_image.getSpacing()[0] == random_double_1);
        REQUIRE(move_assigned_image.getSpacing()[1] == random_double_2);
        REQUIRE(move_assigned_image.getSpacing()[2] == random_double_3);
        REQUIRE(move_assigned_image.getNumChannels() == 1);
        REQUIRE(move_assigned_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(move_assigned_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(move_assigned_image.getImageDimensions()[2] == random_int_3);
        REQUIRE(move_assigned_image.getImageSize() == image_size);
        REQUIRE(move_assigned_image.getImageOrigin() == random_pose);
        REQUIRE(move_assigned_image.getHeader() == random_header);
        REQUIRE(move_assigned_image.getImageEncoding() == random_string);
        REQUIRE((move_assigned_image.getImageData()) == image_data.get());
      }
    }
  }

  // Testing operations.
  GIVEN("Two identical Images") {
    simple_msgs::Image<double> random_image1{};
    random_image1.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image1.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image1.setImageEncoding(random_string);
    random_image1.setHeader(random_header);
    random_image1.setOrigin(random_pose);
    simple_msgs::Image<double> random_image2{};
    random_image2.setImageSpacing(random_double_1, random_double_2, random_double_3);
    random_image2.setImageDimensions(random_int_1, random_int_2, random_int_3);
    random_image2.setImageEncoding(random_string);
    random_image2.setHeader(random_header);
    random_image2.setOrigin(random_pose);
    WHEN("I compare them") {
      THEN("They have to be the same") { REQUIRE(random_image1 == random_image2); }
    }
    WHEN("I change something in the first Image") {
      random_image1.setImageSpacing(random_double_2, random_double_3, random_double_1);
      THEN("They have to be different") { REQUIRE(random_image1 != random_image2); }
    }
  }

  // Testing getters and setters.
  GIVEN("A double Image created from an empty constructor") {
    simple_msgs::Image<double> empty_image{};
    WHEN("I set the Image resolution") {
      empty_image.setImageSpacing(random_double_1, random_double_2, random_double_3);
      THEN("The resolution is correct") {
        REQUIRE(empty_image.getSpacing()[0] == random_double_1);
        REQUIRE(empty_image.getSpacing()[1] == random_double_2);
        REQUIRE(empty_image.getSpacing()[2] == random_double_3);
      }
    }
    WHEN("I set the Image encoding") {
      empty_image.setImageEncoding(random_string);
      THEN("The encoding is correct") { REQUIRE(empty_image.getImageEncoding() == random_string); }
    }
    WHEN("I set the Image dimensions") {
      empty_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
      THEN("The dimensions are correct") {
        REQUIRE(empty_image.getImageDimensions()[0] == random_int_1);
        REQUIRE(empty_image.getImageDimensions()[1] == random_int_2);
        REQUIRE(empty_image.getImageDimensions()[2] == random_int_3);
      }
    }
    WHEN("I set the Image header") {
      empty_image.setHeader(random_header);
      THEN("The header is correct") { REQUIRE(empty_image.getHeader() == random_header); }
    }
    WHEN("I set the Image origin") {
      empty_image.setOrigin(random_pose);
      THEN("The origin is correct") { REQUIRE(empty_image.getImageOrigin() == random_pose); }
    }
    WHEN("I get the message topic") {
      std::string topic_name = empty_image.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "IMAG"); }
    }
  }
}
