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

#include "simple/subscriber.hpp"
#include "simple_msgs/image.hpp"

const std::string window_name{"Received image"};
cv::Mat buffer{};

// Callback function for the Image Subscriber.
// Every image that is received by the Subscriber is shown in a OpenCV window.
void example_callback(const simple_msgs::Image<uint8_t>& i) {
  // Get the image raw data.
  auto img = const_cast<uint8_t*>(i.getImageData());

  // Get the image dimensions, e.g. 512x512x1.
  const auto dimensions = i.getImageDimensions();

  // Build an OpenCV Mat from those.
  // We need to cast the image dimensions, since OpenCV takes signed ints :(
  cv::Mat received_img{static_cast<int>(dimensions[1]), static_cast<int>(dimensions[0]), CV_8UC(i.getNumChannels()),
                       img};

  received_img.copyTo(buffer);
  cv::imshow(window_name, buffer);
  std::cout << "Message # " << i.getHeader().getSequenceNumber() << " received !" << std::endl;
}

int main() {
  cv::namedWindow(window_name);

  // Created a Subscriber that listens to Images sent by a Publisher on the IP address "localhost" on port 5555.
  std::cout << "Creating a subscriber for Image messages." << std::endl;

  simple::Subscriber<simple_msgs::Image<uint8_t>> sub{"tcp://localhost:5555", example_callback};

  cv::waitKey(0);
  std::cout << "Subscribing ended." << std::endl;

  cv::destroyAllWindows();
  return 0;
}
