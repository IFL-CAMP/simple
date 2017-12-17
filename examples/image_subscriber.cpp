#include <iostream>
#include <opencv2/opencv.hpp>

#include "simple/subscriber.hpp"
#include "simple_msgs/image.h"

const std::string window_name("Received image");

void example_callback(const simple_msgs::Image<uint8_t>& i)
{
  uint8_t* img = const_cast<uint8_t*>(i.getImageData());
  auto dimensions = i.getImageDimensions();

  cv::Mat received_img(dimensions[0], dimensions[1], CV_8UC1, img);
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
