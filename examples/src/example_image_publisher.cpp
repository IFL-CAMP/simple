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
#include "simple_msgs/image.hpp"

const std::string DATA_DIRECTORY{DATA_DIR};

// Create a vector containing OpenCV images.
std::vector<cv::Mat> readImages() {
  const std::string lena_path = DATA_DIRECTORY + "lena.ascii.pgm";
  const std::string barbara_path = DATA_DIRECTORY + "barbara.ascii.pgm";
  const std::string baboon_path = DATA_DIRECTORY + "baboon.ascii.pgm";
  const std::string lena_color_path = DATA_DIRECTORY + "lena512color.tiff";

  return {cv::imread(lena_path, CV_LOAD_IMAGE_COLOR), cv::imread(barbara_path, CV_LOAD_IMAGE_COLOR),
          cv::imread(baboon_path, CV_LOAD_IMAGE_COLOR), cv::imread(lena_color_path, CV_LOAD_IMAGE_COLOR)};
}

int main() {
  const int N_RUN{30000};
  const int SLEEP_TIME{200};  //!  Milliseconds.

  // Obtain the images.
  auto images = readImages();

  // Add dummy Header and Origin to the image message.
  simple_msgs::Header h{1, "Image", 0};
  simple_msgs::Pose p{};
  simple_msgs::Image<uint8_t> img{};
  img.setHeader(h);
  img.setOrigin(p);

  // Create a Publisher that will send Image messages to any Subscriber listening on port 5555.
  // In this example images are treated as uint8.
  simple::Publisher<simple_msgs::Image<uint8_t>> pub{"tcp://*:5555"};

  // Publish an image from the pool of images we read from files, for a total of N_RUN times.
  for (int i = 0; i < N_RUN; i++) {
    // At each iteration pick one of the images in rotation.
    auto image = images[i % 4];
    // Set the image dimensions (e.g. 512x512x1).
    img.setImageDimensions(image.cols, image.rows, 1);
    // Set the image data, giving also the total size in bytes and the number of channels
    img.setImageData(image.data, static_cast<int>(image.total() * image.elemSize()),
                     static_cast<int>(image.channels()));
    // Publish the image.
    pub.publish(img);

    std::cout << "Message #" << i + 1 << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    img.getHeader().setSequenceNumber(i + 1);
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
