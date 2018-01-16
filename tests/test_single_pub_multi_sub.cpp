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

// TEST ONE PUBLISHER WITH MANY SUBSCRIBERS

// create static poses for comparing with data sent
simple_msgs::Pose received_pose01, received_pose02, received_pose03, received_pose04, received_pose05, received_pose06,
    received_pose07, received_pose08, received_pose09, received_pose10, received_pose11, received_pose12,
    received_pose13, received_pose14, received_pose15, received_pose16, received_pose17, received_pose18,
    received_pose19, received_pose20;
bool running_pose01 = false;
bool running_pose02 = false;
bool running_pose03 = false;
bool running_pose04 = false;
bool running_pose05 = false;
bool running_pose06 = false;
bool running_pose07 = false;
bool running_pose08 = false;
bool running_pose09 = false;
bool running_pose10 = false;
bool running_pose11 = false;
bool running_pose12 = false;
bool running_pose13 = false;
bool running_pose14 = false;
bool running_pose15 = false;
bool running_pose16 = false;
bool running_pose17 = false;
bool running_pose18 = false;
bool running_pose19 = false;
bool running_pose20 = false;
int num_receives_pose01 = 0;
int num_receives_pose02 = 0;
int num_receives_pose03 = 0;
int num_receives_pose04 = 0;
int num_receives_pose05 = 0;
int num_receives_pose06 = 0;
int num_receives_pose07 = 0;
int num_receives_pose08 = 0;
int num_receives_pose09 = 0;
int num_receives_pose10 = 0;
int num_receives_pose11 = 0;
int num_receives_pose12 = 0;
int num_receives_pose13 = 0;
int num_receives_pose14 = 0;
int num_receives_pose15 = 0;
int num_receives_pose16 = 0;
int num_receives_pose17 = 0;
int num_receives_pose18 = 0;
int num_receives_pose19 = 0;
int num_receives_pose20 = 0;

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
void callbackFunctionPose02(const simple_msgs::Pose& p)
{
  received_pose02 = p;
  num_receives_pose02++;
  if (!running_pose02)
  {
    running_pose02 = true;
  }
}
void callbackFunctionPose03(const simple_msgs::Pose& p)
{
  received_pose03 = p;
  num_receives_pose03++;
  if (!running_pose03)
  {
    running_pose03 = true;
  }
}
void callbackFunctionPose04(const simple_msgs::Pose& p)
{
  received_pose04 = p;
  num_receives_pose04++;
  if (!running_pose04)
  {
    running_pose04 = true;
  }
}
void callbackFunctionPose05(const simple_msgs::Pose& p)
{
  received_pose05 = p;
  num_receives_pose05++;
  if (!running_pose05)
  {
    running_pose05 = true;
  }
}
void callbackFunctionPose06(const simple_msgs::Pose& p)
{
  received_pose06 = p;
  num_receives_pose06++;
  if (!running_pose06)
  {
    running_pose06 = true;
  }
}
void callbackFunctionPose07(const simple_msgs::Pose& p)
{
  received_pose07 = p;
  num_receives_pose07++;
  if (!running_pose07)
  {
    running_pose07 = true;
  }
}
void callbackFunctionPose08(const simple_msgs::Pose& p)
{
  received_pose08 = p;
  num_receives_pose08++;
  if (!running_pose08)
  {
    running_pose08 = true;
  }
}
void callbackFunctionPose09(const simple_msgs::Pose& p)
{
  received_pose09 = p;
  num_receives_pose09++;
  if (!running_pose09)
  {
    running_pose09 = true;
  }
}
void callbackFunctionPose10(const simple_msgs::Pose& p)
{
  received_pose10 = p;
  num_receives_pose10++;
  if (!running_pose10)
  {
    running_pose10 = true;
  }
}
void callbackFunctionPose11(const simple_msgs::Pose& p)
{
  received_pose11 = p;
  num_receives_pose11++;
  if (!running_pose11)
  {
    running_pose11 = true;
  }
}
void callbackFunctionPose12(const simple_msgs::Pose& p)
{
  received_pose12 = p;
  num_receives_pose12++;
  if (!running_pose12)
  {
    running_pose12 = true;
  }
}
void callbackFunctionPose13(const simple_msgs::Pose& p)
{
  received_pose13 = p;
  num_receives_pose13++;
  if (!running_pose13)
  {
    running_pose13 = true;
  }
}
void callbackFunctionPose14(const simple_msgs::Pose& p)
{
  received_pose14 = p;
  num_receives_pose14++;
  if (!running_pose14)
  {
    running_pose14 = true;
  }
}
void callbackFunctionPose15(const simple_msgs::Pose& p)
{
  received_pose15 = p;
  num_receives_pose15++;
  if (!running_pose15)
  {
    running_pose15 = true;
  }
}
void callbackFunctionPose16(const simple_msgs::Pose& p)
{
  received_pose16 = p;
  num_receives_pose16++;
  if (!running_pose16)
  {
    running_pose16 = true;
  }
}
void callbackFunctionPose17(const simple_msgs::Pose& p)
{
  received_pose17 = p;
  num_receives_pose17++;
  if (!running_pose17)
  {
    running_pose17 = true;
  }
}
void callbackFunctionPose18(const simple_msgs::Pose& p)
{
  received_pose18 = p;
  num_receives_pose18++;
  if (!running_pose18)
  {
    running_pose18 = true;
  }
}
void callbackFunctionPose19(const simple_msgs::Pose& p)
{
  received_pose19 = p;
  num_receives_pose19++;
  if (!running_pose19)
  {
    running_pose19 = true;
  }
}
void callbackFunctionPose20(const simple_msgs::Pose& p)
{
  received_pose20 = p;
  num_receives_pose20++;
  if (!running_pose20)
  {
    running_pose20 = true;
  }
}

SCENARIO("Publish a pose and have many subscribers.")
{
  GIVEN("An instance of a publisher and 20 subscribers.")
  {
    // start a publisher
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5555");
    // start 20 subscribers
    simple::Subscriber<simple_msgs::Pose> sub01("tcp://localhost:5555", callbackFunctionPose01);
    simple::Subscriber<simple_msgs::Pose> sub02("tcp://localhost:5555", callbackFunctionPose02);
    simple::Subscriber<simple_msgs::Pose> sub03("tcp://localhost:5555", callbackFunctionPose03);
    simple::Subscriber<simple_msgs::Pose> sub04("tcp://localhost:5555", callbackFunctionPose04);
    simple::Subscriber<simple_msgs::Pose> sub05("tcp://localhost:5555", callbackFunctionPose05);
    simple::Subscriber<simple_msgs::Pose> sub06("tcp://localhost:5555", callbackFunctionPose06);
    simple::Subscriber<simple_msgs::Pose> sub07("tcp://localhost:5555", callbackFunctionPose07);
    simple::Subscriber<simple_msgs::Pose> sub08("tcp://localhost:5555", callbackFunctionPose08);
    simple::Subscriber<simple_msgs::Pose> sub09("tcp://localhost:5555", callbackFunctionPose09);
    simple::Subscriber<simple_msgs::Pose> sub10("tcp://localhost:5555", callbackFunctionPose10);
    simple::Subscriber<simple_msgs::Pose> sub11("tcp://localhost:5555", callbackFunctionPose11);
    simple::Subscriber<simple_msgs::Pose> sub12("tcp://localhost:5555", callbackFunctionPose12);
    simple::Subscriber<simple_msgs::Pose> sub13("tcp://localhost:5555", callbackFunctionPose13);
    simple::Subscriber<simple_msgs::Pose> sub14("tcp://localhost:5555", callbackFunctionPose14);
    simple::Subscriber<simple_msgs::Pose> sub15("tcp://localhost:5555", callbackFunctionPose15);
    simple::Subscriber<simple_msgs::Pose> sub16("tcp://localhost:5555", callbackFunctionPose16);
    simple::Subscriber<simple_msgs::Pose> sub17("tcp://localhost:5555", callbackFunctionPose17);
    simple::Subscriber<simple_msgs::Pose> sub18("tcp://localhost:5555", callbackFunctionPose18);
    simple::Subscriber<simple_msgs::Pose> sub19("tcp://localhost:5555", callbackFunctionPose19);
    simple::Subscriber<simple_msgs::Pose> sub20("tcp://localhost:5555", callbackFunctionPose20);
    // wait so the subscribers get every message
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data")
    {
      for (int i = 0; i < 10; ++i)
      {
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::seconds(1));
			REQUIRE(received_pose01 == p);
			REQUIRE(received_pose02 == p);
			REQUIRE(received_pose03 == p);
			REQUIRE(received_pose04 == p);
			REQUIRE(received_pose05 == p);
			REQUIRE(received_pose06 == p);
			REQUIRE(received_pose07 == p);
			REQUIRE(received_pose08 == p);
			REQUIRE(received_pose09 == p);
			REQUIRE(received_pose10 == p);
			REQUIRE(received_pose11 == p);
			REQUIRE(received_pose12 == p);
			REQUIRE(received_pose13 == p);
			REQUIRE(received_pose14 == p);
			REQUIRE(received_pose15 == p);
			REQUIRE(received_pose16 == p);
			REQUIRE(received_pose17 == p);
			REQUIRE(received_pose18 == p);
			REQUIRE(received_pose19 == p);
			REQUIRE(received_pose20 == p);
		
      }
	  REQUIRE(num_receives_pose01 == 10);
	  REQUIRE(num_receives_pose02 == 10);
	  REQUIRE(num_receives_pose03 == 10);
	  REQUIRE(num_receives_pose04 == 10);
	  REQUIRE(num_receives_pose05 == 10);
	  REQUIRE(num_receives_pose06 == 10);
	  REQUIRE(num_receives_pose07 == 10);
	  REQUIRE(num_receives_pose08 == 10);
	  REQUIRE(num_receives_pose09 == 10);
	  REQUIRE(num_receives_pose10 == 10);
	  REQUIRE(num_receives_pose11 == 10);
	  REQUIRE(num_receives_pose12 == 10);
	  REQUIRE(num_receives_pose13 == 10);
	  REQUIRE(num_receives_pose14 == 10);
	  REQUIRE(num_receives_pose15 == 10);
	  REQUIRE(num_receives_pose16 == 10);
	  REQUIRE(num_receives_pose17 == 10);
	  REQUIRE(num_receives_pose18 == 10);
	  REQUIRE(num_receives_pose19 == 10);
	  REQUIRE(num_receives_pose20 == 10);
    }
    
  }
}