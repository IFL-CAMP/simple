#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/image.h"
#include "test_utils.hpp"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE IMAGE MESSAGE WRAPPER

SCENARIO("Using an Image Message")
{
	simple_msgs::Pose empty_pose;
	simple_msgs::Header empty_header;
	double random_double_1 = static_cast<double>(rand()) / RAND_MAX;
	double random_double_2 = static_cast<double>(rand()) / RAND_MAX;
	double random_double_3 = static_cast<double>(rand()) / RAND_MAX;
	int random_int_1 = static_cast<int>(rand() / 100);
	int random_int_2 = static_cast<int>(rand() / 100);
	int random_int_3 = static_cast<int>(rand() / 100);
	std::string random_string = std::to_string(random_double_1);
	//Testing constructor.
  GIVEN("A uint8_t Image created from an empty constructor")
  {
    simple_msgs::Image<uint8_t> empty_image;
    WHEN("I check the image's fields")
    {
      THEN("They have to match the default parameters") { 
		  REQUIRE(empty_image.getResolution()[0] == 0.0);
		  REQUIRE(empty_image.getResolution()[1] == 0.0);
		  REQUIRE(empty_image.getResolution()[2] == 0.0);
		  REQUIRE(empty_image.getNumChannels() == 1);
		  REQUIRE(empty_image.getImageDimensions()[0] == 0);
		  REQUIRE(empty_image.getImageDimensions()[1] == 0);
		  REQUIRE(empty_image.getImageDimensions()[2] == 0);
		  REQUIRE(empty_image.getImageSize() == 0);
		  REQUIRE(empty_image.getImageOrigin() == empty_pose);
		  REQUIRE(empty_image.getHeader() == empty_header);
		  REQUIRE(empty_image.getImageEncoding() == "");
		  REQUIRE(empty_image.getImageData() == nullptr);
	  }
    }
  }

  //Testing copy-constructors.
  GIVEN("An image") {
	  simple_msgs::Image<uint8_t> random_image;
	  random_image.setImageResolution(random_double_1, random_double_2, random_double_3);
	  random_image.setImageDimensions(random_int_1, random_int_2, random_int_3);
	  random_image.setImageEncoding(random_string);
	  random_image.setHeader(createRandomHeader());
	  random_image.setOrigin(createRandomPose());
	  WHEN("I copy-construct a new image") {
		  simple_msgs::Image<uint8_t> copy_image(random_image);
		  THEN("Both images are the same") {
			  REQUIRE(copy_image == random_image);
		  }
	  }
	  WHEN("I move-construct a new image") {
		  simple_msgs::Image<uint8_t> move_image(std::move(random_image));
		  THEN("The new image's fields are the same as the original") {
			  REQUIRE(move_image.getResolution()[0] == random_double_1);
			  REQUIRE(move_image.getResolution()[1] == random_double_2);
			  REQUIRE(move_image.getResolution()[2] == random_double_3);
			  REQUIRE(move_image.getNumChannels() == 1);
			  REQUIRE(move_image.getImageDimensions()[0] == 0);
			  REQUIRE(move_image.getImageDimensions()[1] == 0);
			  REQUIRE(move_image.getImageDimensions()[2] == 0);
			  REQUIRE(move_image.getImageSize() == 0);
			  REQUIRE(move_image.getImageOrigin() == empty_pose);
			  REQUIRE(move_image.getHeader() == empty_header);
			  REQUIRE(move_image.getImageEncoding() == "");
			  REQUIRE(move_image.getImageData() == nullptr);
		  }
	  }
  }

  //Testing getters and setters.
  GIVEN("A uint8_t Image created from an empty constructor")
  {
	  simple_msgs::Image<uint8_t> empty_image;
	  WHEN("I check the image's fields")
	  {
		  THEN("They have to match the default parameters") {
			  REQUIRE(empty_image.getResolution()[0] == 0.0);
			  REQUIRE(empty_image.getResolution()[1] == 0.0);
			  REQUIRE(empty_image.getResolution()[2] == 0.0);
			  REQUIRE(empty_image.getNumChannels() == 1);
			  REQUIRE(empty_image.getImageDimensions()[0] == 0);
			  REQUIRE(empty_image.getImageDimensions()[1] == 0);
			  REQUIRE(empty_image.getImageDimensions()[2] == 0);
			  REQUIRE(empty_image.getImageSize() == 0);
			  REQUIRE(empty_image.getImageOrigin() == empty_pose);
			  REQUIRE(empty_image.getHeader() == empty_header);
			  REQUIRE(empty_image.getImageEncoding() == "");
			  REQUIRE(empty_image.getImageData() == nullptr);
		  }
	  }
	  WHEN("I set the image fields") {

		  THEN("The fields are correct") {

		  }
	  }
  }
  
}
