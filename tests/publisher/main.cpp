// Test how the publisher works

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include "simple/publisher.hpp"
#include <signal.h>
#include <string>
#include <thread>
#include "CImg.h"

#include "simple_msgs/pose.h"
#include "simple_msgs/image.h"
#include "simple_msgs/header.h"

std::pair<uint8_t*, int> CImgReadImage()
{
  cimg_library::CImg<uint8_t> image("/Users/jack/Downloads/lena.ascii.pgm");
  return std::make_pair(image.data(), 512 * 512);
}

int main()
{
  auto image = CImgReadImage();
  std::cout << "N is " << std::to_string(image.second) << std::endl;
  uint8_t* temp = (image.first);

  simple_msgs::Header h(1, "Image", 100);
  simple_msgs::Pose p;
  simple_msgs::Image<uint8_t> img;
  img.setHeader(h);
  img.setOrigin(p);
  img.setImageData(temp, image.second);
  img.setImageDimensions(512, 512, 1);

  auto check_h = img.getHeader();
  std::cout << check_h.getFrameID() << std::endl;

  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");

  for (int i = 0; i < 10; i++)
  {
    pub.publish(img);
    std::cout << "Message #" << i << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
