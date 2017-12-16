#include <iostream>
#include <string>

#include "CImg.h"

#include "simple/publisher.hpp"
#include "simple_msgs/image.h"

const std::string data_dir = DATA_DIR;

std::vector<std::pair<uint8_t*, int>> CImgReadImage()
{
  const std::string lena_path = data_dir + "lena.ascii.pgm";
  const std::string barbara_path = data_dir + "barbara.ascii.pgm";
  const std::string baboon_path = data_dir + "baboon.ascii.pgm";


  cimg_library::CImg<uint8_t> lena(lena_path.c_str());
  cimg_library::CImg<uint8_t> barbara(barbara_path.c_str());
  cimg_library::CImg<uint8_t> baboon(baboon_path.c_str());

  std::vector<std::pair<uint8_t*, int>> return_vector;
  return_vector.push_back(std::make_pair(lena.data(), 512*512));
  return_vector.push_back(std::make_pair(barbara.data(), 512 * 512));
  return_vector.push_back(std::make_pair(baboon.data(), 512 * 512));

  return return_vector;
}

int main()
{
  const int N_RUN = 25;
  const int SLEEP_TIME = 10000;  //<  Milliseconds.
  
  auto images = CImgReadImage();
  //uint8_t* temp = (image.first);

  simple_msgs::Header h(1, "Image", 0);
  simple_msgs::Pose p;
  simple_msgs::Image<uint8_t> img;
  img.setHeader(h);
  img.setOrigin(p);
  img.setImageDimensions(512, 512, 1);

  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");

  for (int i = 0; i < N_RUN; i++)
  {
	auto image = images[i % 3];
	img.setImageData(image.first, image.second);
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    pub.publish(img);
    std::cout << "Message #" << i+1 << " has been published. " << std::endl;
  }

  std::cout << "Publishing ended." << std::endl;
  return 0;
}
