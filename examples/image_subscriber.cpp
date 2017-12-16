#include <iostream>

#include "CImg.h"

#include "simple/subscriber.hpp"
#include "simple_msgs/image.h"

cimg_library::CImgDisplay main_disp(512, 512, "Show the received image.");

void example_callback(const simple_msgs::Image<uint8_t>& i)
{
  auto img = i.getImageData();
  auto dimensions = i.getImageDimensions();

  cimg_library::CImg<uint8_t> final_image(img, dimensions[0], dimensions[1], dimensions[2]);
  main_disp.display(final_image);
}

int main(int argc, char* argv[])
{
  const int SLEEP_TIME = 60000;  //<  Milliseconds.

  std::cout << "Creating a subscriber for Image messages." << std::endl;
  simple::Subscriber<simple_msgs::Image<uint8_t>> sub("tcp://localhost:5555", example_callback);

  main_disp.show();
  while (!main_disp.is_closed())
  {
	  main_disp.wait();
  }

  std::cout << "Subscribing ended." << std::endl;
  return 0;
}
