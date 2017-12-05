#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include "simple/publisher.h"
#include "simple_msgs/point.h"
#include "simple/subscriber.h"
#include <time.h>
#include <stdlib.h>

//TEST FOR THE PUBLISHING AND SUBSCRIPTION OF A POINT

//create static point for comparing with data sent
static simple_msgs::Point recvP(0.0, 0.0, 0.0);

//define callback function
void callbackFun(const simple_msgs::Point& pt){
	recvP = pt;
}

//start a subscriber
simple::Subscriber<simple_msgs::Point > sub("tcp://*:5555",callbackFun);

//randomly generate the data to be sent
srand(time(NULL));//start random seed
double x = (double)rand()/RAND_MAX;
double y = (double)rand() / RAND_MAX;
double z = (double)rand() / RAND_MAX;
simple_msgs::Point p(x, y, z);

SCENARIO("PUB SUB POINT") {
	GIVEN("An instance of a subscriber") {
		WHEN("A publisher publishes data") {
			simple::Publisher<simple_msgs::Point> pub("tcp://localhost:5555");
			THEN("The data received is the same as the one sent") {
				REQUIRE(p==recvP);
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
