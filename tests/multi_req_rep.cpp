#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple/server.hpp"
#include "simple_msgs/point.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/bool.h"
#include "simple_msgs/string.h"
#include "simple_msgs/numeric_type.hpp"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point_stamped.h"
#include "simple_msgs/quaternion_stamped.h"
#include "simple_msgs/pose_stamped.h"
#include "simple_msgs/rotation_matrix_stamped.h"
#include "simple/client.hpp"
#include "simple_msgs/double.h"
#include "simple_msgs/float.h"
#include "simple_msgs/int.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include "test_utils.hpp"
#include <memory>
#include <functional>

// TEST FOR SEVERAL CLIENTS AND ONE SERVER OF ONE DATA TYPE - POSE STAMPED


void requestInParallel(simple::Client<simple_msgs::PoseStamped>& client, simple_msgs::PoseStamped& pose) {
	client.request(pose);
}

SCENARIO("Several Clients to one Server to a Stamped Pose message.")
{
  GIVEN("An instance of a server and several clients.")
  {
    simple::Server<simple_msgs::PoseStamped> server("tcp://*:5556", callbackFunctionPoseStamped);  // Start a server.
    // Start several clients
	std::vector<simple::Client<simple_msgs::PoseStamped>> clients;
	for (int i = 0; i < 20; i++)
	{
		clients.push_back(simple::Client<simple_msgs::PoseStamped>("tcp://localhost:5556"));
	}

    WHEN("All clients send a request")
    {
		std::vector<simple_msgs::PoseStamped> p;
		std::vector<simple_msgs::PoseStamped> sentPoses;
		for (int i = 0; i < 20; i++)
		{
			p.push_back(createRandomPoseStamped());
			sentPoses.push_back(p.at(i));
		}
	  std::vector<std::thread> threads;
	  for (int i = 0; i < 20; i++)
	  {
		  threads.push_back(std::thread(requestInParallel, std::ref(clients.at(i)), std::ref(p.at(i))));
	  }
      simple_msgs::Point unitPoint(1, 1, 1);
      THEN("All the clients are serviced")
      {
		  for (int i = 0; i < 20; i++)
		  {
			  REQUIRE(p.at(i).getPose().getPosition() == sentPoses.at(i).getPose().getPosition() + unitPoint);
			  REQUIRE(p.at(i).getPose().getQuaternion().getW() == sentPoses.at(i).getPose().getQuaternion().getW() + 1);
			  REQUIRE(p.at(i).getPose().getQuaternion().getX() == sentPoses.at(i).getPose().getQuaternion().getX() + 1);
			  REQUIRE(p.at(i).getPose().getQuaternion().getY() == sentPoses.at(i).getPose().getQuaternion().getY() + 1);
			  REQUIRE(p.at(i).getPose().getQuaternion().getZ() == sentPoses.at(i).getPose().getQuaternion().getZ() + 1);
			  REQUIRE(p.at(i).getHeader().getFrameID() == "ID");
			  REQUIRE(p.at(i).getHeader().getSequenceNumber() == 1);
			  REQUIRE(p.at(i).getHeader().getTimestamp() == 1.0);
		  }
      }
	  //join the threads
	  for (auto& thread : threads) {
		  thread.join();
	  }
    }
  }
}
