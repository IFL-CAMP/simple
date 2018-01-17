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

#include <iostream>
#include <opencv2/opencv.hpp>

#include "simple/subscriber.hpp"
#include "simple_msgs/image.h"

const std::string window_name("Received image");

void example_callback(const simple_msgs::Image<uint8_t>& i)
{
  uint8_t* img = const_cast<uint8_t*>(i.getImageData());
  auto dimensions = i.getImageDimensions();
  cv::Mat received_img(dimensions[0], dimensions[1], CV_8UC3, img);
  cv::imshow(window_name, received_img);
}

int main()
{
  const int SLEEP_TIME = 60000;  //<  Milliseconds.

  cv::namedWindow(window_name);

  std::cout << "Creating a subscriber for Image messages." << std::endl;
  simple::Subscriber<simple_msgs::Image<uint8_t>> sub("tcp://localhost:5555", example_callback);

  cv::waitKey(0);

  std::cout << "Subscribing ended." << std::endl;

  cv::destroyAllWindows();
  return 0;
}
