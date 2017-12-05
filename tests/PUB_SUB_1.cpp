#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include "simple/publisher.h"
#include "simple_msgs/point.h"
#include "simple/subscriber.h"
#include <time.h>
#include <stdlib.h>

//TEST FOR THE PUBLISHING AND SUBSCRIPTION OF A POINT

//start a publisher
simple::Publisher<simple_msgs::Point > pub("tcp://localhost:5555");

//randomly generate the data to be sent
srand(time(NULL));//start random seed
double x = (double)rand()/RAND_MAX;
double y = (double)rand() / RAND_MAX;
double z = (double)rand() / RAND_MAX;
simple_msgs::Point p(x, y, z);

SCENARIO("PUB SUB POINT") {
	GIVEN("An instance of a subscriber") {
		WHEN("A negative number is given") {
			int negative_number = -10;
			THEN("The sign is -1") {
				REQUIRE(sign(negative_number) == -1);
			}
		}
		WHEN("A positive number is given") {
			int positive_number = 22;
			THEN("The sign is 1") {
				REQUIRE(sign(positive_number) == 1);
			}
		}
		WHEN("Zero is given") {
			THEN("The sign is 0") {
				REQUIRE(sign(0) == 0);
			}
		}
	}
}
