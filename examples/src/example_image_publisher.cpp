/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

#include "simple/publisher.hpp"
#include "simple_msgs/image.pb.h"

const std::string kDataDirectory{DATA_DIR};
const std::string kLenaPath = kDataDirectory + "lena.ascii.pgm";
const std::string kBarbaraPath = kDataDirectory + "barbara.ascii.pgm";
const std::string kBaboonPath = kDataDirectory + "baboon.ascii.pgm";
const std::string kLenaColorPath = kDataDirectory + "lena512color.tiff";

// Create a vector containing OpenCV images.
std::vector<cv::Mat> readImages() {
  return {cv::imread(kLenaPath, cv::IMREAD_GRAYSCALE), cv::imread(kBarbaraPath, cv::IMREAD_GRAYSCALE),
          cv::imread(kBaboonPath, cv::IMREAD_GRAYSCALE), cv::imread(kLenaColorPath, cv::IMREAD_COLOR)};
}

int main() {
  constexpr int kNumberRuns{30000};
  constexpr int kSleepTimeMilliseconds{200};  //!  Milliseconds.
  const std::string kAddress{"tcp://*:5555"};

  // Obtain the images.
  auto images = readImages();

  // Add dummy Header and Origin to the image message.
  simple_msgs::Header header;
  header.set_id(1);
  header.set_frame("Image");
  header.set_timestamp(0);

  simple_msgs::Pose pose;
  pose.mutable_point()->set_x(0);
  pose.mutable_point()->set_y(0);
  pose.mutable_point()->set_z(0);

  pose.mutable_quaternion()->set_x(0);
  pose.mutable_quaternion()->set_y(0);
  pose.mutable_quaternion()->set_z(0);
  pose.mutable_quaternion()->set_w(1);

  simple_msgs::Image image_message;
  image_message.set_allocated_header(&header);
  image_message.set_allocated_origin(&pose);

  // Create a Publisher that will send Image messages to any Subscriber listening on port 5555.
  simple::Publisher<simple_msgs::Image> publisher{kAddress};

  // Publish an image from the pool of images we read from files, for a total of N_RUN times.
  for (size_t i = 0; i < kNumberRuns; i++) {
    // At each iteration pick one of the images in rotation.
    auto image = images[i % images.size()];

    // Set the image dimensions.
    image_message.set_height(image.rows);
    image_message.set_width(image.cols);
    image_message.set_depth(1);

    // Set the image data. In this example images are treated as uint8.
    image_message.set_uint8_data(image.data, image.total() * image.elemSize());
    image_message.set_data_size(image.total() * image.elemSize());
    image_message.set_channels(image.channels());

    // Publish the image.
    publisher.publish(image_message);

    std::cout << "Message #" << i + 1 << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(kSleepTimeMilliseconds));
    image_message.mutable_header()->set_id(i + 1);
  }

  std::cout << "Quitting..." << std::endl;
  return 0;
}
