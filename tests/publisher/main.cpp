// Test how the publisher works

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include "simple/publisher.hpp"
#include <signal.h>
#include <string>
#include <thread>
#include "simple_msgs/pose.h"
#include "simple_msgs/image.h"
#include "simple_msgs/header.h"

std::pair<uint8_t*, int> readImage()
{
	std::ifstream infile("C:/Users/ferna/Documents/IFL/SIMPLE/lena.ascii.pgm");
	std::stringstream ss;
	std::string inputLine = "";

	// First line : version
	getline(infile, inputLine);
	if (inputLine.compare("P2") != 0) {
		std::cerr << "Version error" << std::endl;
	}
	else {
		std::cout << "Version : " << inputLine << std::endl;
	}
	// Second line : comment
	getline(infile, inputLine);
	std::cout << "Comment : " << inputLine << std::endl;

	// Continue with a stringstream
	ss << infile.rdbuf();
	int numcols=0, numrows=0;
	// Third line : size
	ss >> numcols >> numrows;
	std::cout << numcols << " columns and " << numrows << " rows" << std::endl;

	const int size = numrows * numcols;
	uint8_t* buffer = (uint8_t*)std::malloc(size);

	// Following lines : data
	for (int row = 0; row < numrows; ++row)
		for (int col = 0; col < numcols; ++col) ss >> buffer[row*numrows+col];

	infile.close();

	return std::make_pair(buffer, size);
}

int main(int argc, char* argv[])
{
  auto image = readImage();
  std::cout << "N is " << std::to_string(image.second) << std::endl;
  uint8_t* temp = (image.first);

  simple_msgs::Header h(1, "Image", 100);
  simple_msgs::Pose p;
  simple_msgs::Image<uint8_t> img;
  img.setHeader(h);
  img.setOrigin(p);
  img.setImageData(temp, image.second);
  

  auto check_h = img.getHeader();
  std::cout << check_h.getFrameID() << std::endl;

  simple::Publisher<simple_msgs::Image<uint8_t>> pub("tcp://*:5555");
  //start second publisher
  //simple::Publisher<simple_msgs::Header> pub2("tcp://*:5556");

  for (int i = 0; i < 100; i++)
  {
    pub.publish(img);
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
