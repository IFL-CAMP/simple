// Test how the publisher works

#include <iostream>
#include <fstream>
#include <istream>
#include "simple/publisher.hpp"
#include <signal.h>
#include <string>
#include <thread>
#include "simple_msgs/pose.h"
#include "simple_msgs/image.h"
#include "simple_msgs/header.h"

std::pair<char*, int> readImage()
{
  std::ifstream image;
  image.open("C:/Users/ferna/Documents/IFL/SIMPLE/lena.gif", std::ios_base::binary);

  image.seekg(0, std::ios::end);
  int n = image.tellg();
  image.seekg(0, std::ios::beg);

  char* res = new char[n];
  for (int i = 0; i < n; i++) res[i] = '5';

  bool bit = image.eof();

  image.read(res, n);
  return std::make_pair(res, n);
}

int main(int argc, char* argv[])
{
  //auto image = readImage();
  //std::cout << "N is " << std::to_string(image.second) << std::endl;
  //uint8_t* temp = reinterpret_cast<uint8_t*>(image.first);

  simple_msgs::Header h(1, "Image", 100);
  simple_msgs::Pose p;
  //simple_msgs::Image<uint8_t> img;
  //img.setHeader(h);
  //img.setOrigin(p);
  //img.setImageData(temp, image.second);

  //auto check_h = img.getHeader();
  //std::cout << check_h.getFrameID() << std::endl;

  simple::Publisher<simple_msgs::Header> pub("tcp://*:5555");
  //start second publisher
  simple::Publisher<simple_msgs::Header> pub2("tcp://*:5556");

  for (int i = 0; i < 10; i++)
  {
    pub.publish(h);
    std::cout << "Message #" << i << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  //  // create a message, with wrapper
  //  simple_msgs::Point p(5.0, 6.0, 7.0);

  //  //  // create a publisher
  //  simple::Publisher<simple_msgs::Point> pub("tcp://*:5555");
  //  std::cout << "Publish these cordinates: x=5.0, y=6.0, z=7.0" << std::endl;

  //  for (auto i = 0; i < 10; ++i)
  //  {
  //    pub.publish(p);
  //    std::cout << "A Point message #" << i << " has been published. " << std::endl;
  //    std::this_thread::sleep_for(std::chrono::seconds(2));
  //  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
