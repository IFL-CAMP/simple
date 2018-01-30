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

#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

#include "simple/publisher.hpp"
#include "simple_msgs/image.h"

const std::string data_dir = DATA_DIR;

// Create a vector containing OpenCV images.
std::vector<std::pair<cv::Mat, int>> readImage()
{
  const std::string lena_path = data_dir + "lena.ascii.pgm";
  const std::string barbara_path = data_dir + "barbara.ascii.pgm";
  const std::string baboon_path = data_dir + "baboon.ascii.pgm";
  const std::string lena_color_path = data_dir + "lena512color.tiff";

  cv::Mat lena = cv::imread(lena_path, CV_LOAD_IMAGE_COLOR);
  cv::Mat barbara = cv::imread(barbara_path, CV_LOAD_IMAGE_COLOR);
  cv::Mat baboon = cv::imread(baboon_path, CV_LOAD_IMAGE_COLOR);
  cv::Mat lena_color = cv::imread(lena_color_path, CV_LOAD_IMAGE_COLOR);

  std::vector<std::pair<cv::Mat, int>> return_vector;
  return_vector.push_back(std::make_pair(lena, lena.rows * lena.cols * lena.channels()));
  return_vector.push_back(std::make_pair(barbara, barbara.rows * barbara.cols * barbara.channels()));
  return_vector.push_back(std::make_pair(baboon, baboon.rows * baboon.cols * baboon.channels()));
  return_vector.push_back(std::make_pair(lena_color, lena_color.rows * lena_color.cols * lena_color.channels()));

  return return_vector;
}

int main()
{
  const int N_RUN = 3000;
  const int SLEEP_TIME = 250;  //<  Milliseconds.

  // Obtain the images.
  auto images = readImage();

  // Add dummy Header and Origin to the image message.
  simple_msgs::Header h(1, "Image", 0);
  simple_msgs::Pose p;
  simple_msgs::Image<uint8_t> img;
  img.setHeader(h);
  img.setOrigin(p);

  // Create a Publisher that will send Image messages to any Subscriber listening on port 5555.
  // In this example images are treated as uint8.
  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");

  // Publish an image from the pool of images we read from files, for a total of N_RUN times.
  for (int i = 0; i < N_RUN; i++)
  {
    // At each iteration pick one of the images in rotation.
    auto image = images[i % 4];
    // Set the image dimensions (e.g. 512x512x1).
    img.setImageDimensions(image.first.rows, image.first.cols, 1);
    // Set the image data, giving also the total size in bytes and the number of channels (3 in this case).
    img.setImageData(image.first.data, image.second, 3);
    // Publish the image.
    pub.publish(img);
    std::cout << "Message #" << i + 1 << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    img.getHeader().setSequenceNumber(i + 1);
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
