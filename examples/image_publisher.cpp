#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

#include "simple/publisher.hpp"
#include "simple_msgs/image.h"

const std::string data_dir = DATA_DIR;

std::vector<std::pair<cv::Mat, int>> readImage()
{
  const std::string lena_path = data_dir + "lena.ascii.pgm";
  const std::string barbara_path = data_dir + "barbara.ascii.pgm";
  const std::string baboon_path = data_dir + "baboon.ascii.pgm";

  cv::Mat lena = cv::imread(lena_path, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat barbara = cv::imread(barbara_path, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat baboon = cv::imread(baboon_path, CV_LOAD_IMAGE_GRAYSCALE);

  std::vector<std::pair<cv::Mat, int>> return_vector;
  return_vector.push_back(std::make_pair(lena, lena.rows * lena.cols));
  return_vector.push_back(std::make_pair(barbara, barbara.rows * barbara.cols));
  return_vector.push_back(std::make_pair(baboon, baboon.rows * baboon.cols));

  return return_vector;
}

int main()
{
  const int N_RUN = 25;
  const int SLEEP_TIME = 5000;  //<  Milliseconds.

  auto images = readImage();

  simple_msgs::Header h(1, "Image", 0);
  simple_msgs::Pose p;
  simple_msgs::Image<uint8_t> img;
  img.setHeader(h);
  img.setOrigin(p);

  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");

  for (int i = 0; i < N_RUN; i++)
  {
    auto image = images[i % 3];
    img.setImageDimensions(image.first.rows, image.first.cols, 1);
    img.setImageData(image.first.data, image.second);
    pub.publish(img);
    std::cout << "Message #" << i + 1 << " has been published. " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
