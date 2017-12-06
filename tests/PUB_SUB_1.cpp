// Is noexcept supported?
/*#if defined(__clang__) && __has_feature(cxx_noexcept) || \
	defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
	defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
#  define NOEXCEPT noexcept
#else
#  define NOEXCEPT
#endif*/

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



SCENARIO("PUB SUB POINT") {
	GIVEN("An instance of a subscriber") {
		//start a subscriber
		simple::Subscriber<simple_msgs::Point > sub("tcp://*:5555", callbackFun);
		WHEN("A publisher publishes data") {
			simple::Publisher<simple_msgs::Point> pub("tcp://localhost:5555");
			//randomly generate the data to be sent
			srand(time(NULL));//start random seed
			double x = (double)rand() / RAND_MAX;
			double y = (double)rand() / RAND_MAX;
			double z = (double)rand() / RAND_MAX;
			simple_msgs::Point p(x, y, z);
			pub.publish(p);
			THEN("The data received is the same as the one sent") {
				REQUIRE(p==recvP);
			}
		}
	}
}
