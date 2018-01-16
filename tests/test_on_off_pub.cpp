#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include <iostream>
#include "simple/publisher.hpp"
#include "simple_msgs/point.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/bool.h"
#include "simple_msgs/string.h"
#include "simple_msgs/numeric_type.hpp"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/header.h"
#include "simple/subscriber.hpp"
#include "simple_msgs/double.h"
#include "simple_msgs/float.h"
#include "simple_msgs/int.h"
#include <time.h>
#include <stdlib.h>

// TEST ONE PUBLISHER DISCONNECT AND RECONNECT

// create static poses for comparing with data sent
simple_msgs::Pose received_pose01;
bool running_pose01 = false;
int num_receives_pose01 = 0;

simple_msgs::Point createRandomPoint()
{
	double x = static_cast<double>(rand()) / RAND_MAX;
	double y = static_cast<double>(rand()) / RAND_MAX;
	double z = static_cast<double>(rand()) / RAND_MAX;
	return simple_msgs::Point(x, y, z);
}

simple_msgs::Quaternion createRandomQuaternion()
{
	double x = static_cast<double>(rand()) / RAND_MAX;
	double y = static_cast<double>(rand()) / RAND_MAX;
	double z = static_cast<double>(rand()) / RAND_MAX;
	double w = static_cast<double>(rand()) / RAND_MAX;
	return simple_msgs::Quaternion(x, y, z, w);
}

simple_msgs::Pose createRandomPose()
{
	simple_msgs::Point p = createRandomPoint();
	simple_msgs::Quaternion q = createRandomQuaternion();
	return simple_msgs::Pose(p, q);
}

// define callback functions
void callbackFunctionPose01(const simple_msgs::Pose& p)
{
	received_pose01 = p;
	num_receives_pose01++;
	if (!running_pose01)
	{
		running_pose01 = true;
	}
}

SCENARIO("Publish a pose and have many subscribers.")
{
	GIVEN("An instance of a publisher and 1 subscriber.")
	{
		// start a publisher
		simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
		// start subscriber
		simple::Subscriber<simple_msgs::Pose> sub01("tcp://localhost:5555", callbackFunctionPose01);
		// wait so the subscribers get every message
		std::this_thread::sleep_for(std::chrono::seconds(5));
		WHEN("A publisher publishes data")
		{
			for (int i = 0; i < 20; ++i)
			{
				auto p = createRandomPose();
				pub.publish(p);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				REQUIRE(received_pose01 == p);

			}
			REQUIRE(num_receives_pose01 == 20);
		}
		WHEN("The publisher is disconnected and then reconnected.") {
			pub.~Publisher();
			std::this_thread::sleep_for(std::chrono::seconds(5));
			simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
			std::this_thread::sleep_for(std::chrono::seconds(5));
			THEN("The subscriber will go back to receiving messages") {
				for (int i = 0; i < 20; ++i)
				{
					auto p = createRandomPose();
					pub.publish(p);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					REQUIRE(received_pose01 == p);

				}
				REQUIRE(num_receives_pose01 == 40);
			}
		}
	}
}


