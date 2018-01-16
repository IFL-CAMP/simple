
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

// create static header for comparing with data sent
simple_msgs::Header received_header;
bool running_header = false;
int num_receives_header = 0;

// create static point for comparing with data sent
simple_msgs::Point received_point;
bool running_point = false;
int num_receives_point = 0;

// create static pose for comparing with data sent
simple_msgs::Pose received_pose;
bool running_pose = false;
int num_receives_pose = 0;

// create static quaternion for comparing with data sent
simple_msgs::Quaternion received_quaternion;
bool running_quaternion = false;
int num_receives_quaternion = 0;

// create static rotation matrix for comparing with data sent
simple_msgs::RotationMatrix received_rotation_matrix;
bool running_rotation_matrix = false;
int num_receives_rotation_matrix = 0;

// create static string for comparing with data sent
simple_msgs::String received_string;
bool running_string = false;
int num_receives_string = 0;

// create static bool for comparing with data sent
simple_msgs::Bool received_bool;
bool running_bool = false;
int num_receives_bool = 0;

// create static int for comparing with data sent
simple_msgs::NumericType<int> received_int;
bool running_int = false;
int num_receives_int = 0;

// create static double for comparing with data sent
simple_msgs::NumericType<double> received_double;
bool running_double = false;
int num_receives_double = 0;

// create static float for comparing with data sent
simple_msgs::NumericType<float> received_float;
bool running_float = false;
int num_receives_float = 0;

simple_msgs::Header createRandomHeader()
{
  int x = rand() % 100;
  std::string y("Header string:");
  y.append(std::to_string(rand() % 100));
  double z = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Header(x, y, z);
}

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

simple_msgs::RotationMatrix createRandomRotationMatrix()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  double y = static_cast<double>(rand()) / RAND_MAX;
  double z = static_cast<double>(rand()) / RAND_MAX;
  double w = static_cast<double>(rand()) / RAND_MAX;
  double a = static_cast<double>(rand()) / RAND_MAX;
  double b = static_cast<double>(rand()) / RAND_MAX;
  double c = static_cast<double>(rand()) / RAND_MAX;
  double d = static_cast<double>(rand()) / RAND_MAX;
  double e = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::RotationMatrix(x, y, z, w, a, b, c, d, e);
}

simple_msgs::Pose createRandomPose()
{
  return simple_msgs::Pose(createRandomPoint(), createRandomQuaternion());
}

simple_msgs::NumericType<int> createRandomInt()
{
  int x = rand() % 100;
  return simple_msgs::NumericType<int>(x);
}

simple_msgs::NumericType<double> createRandomDouble()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::NumericType<double>(x);
}

simple_msgs::NumericType<float> createRandomFloat()
{
  float x = static_cast<float>(rand()) / RAND_MAX;
  return simple_msgs::NumericType<float>(x);
}

simple_msgs::Bool createRandomBool()
{
  bool x = rand() % 1;
  return simple_msgs::Bool(x);
}

simple_msgs::String createRandomString()
{
  std::string s("Random string:");
  s.append(std::to_string(rand() % 100));
  return simple_msgs::String(s);
}

simple_msgs::PointStamped createRandomPointStamped() {
	return simple_msgs::PointStamped(createRandomHeader(), createRandomPoint());
}

simple_msgs::PoseStamped createRandomPoseStamped() {
	return simple_msgs::PoseStamped(createRandomHeader(), createRandomPose());
}

simple_msgs::QuaternionStamped createRandomQuaternionStamped() {
	return simple_msgs::QuaternionStamped(createRandomHeader(), createRandomQuaternion());
}

simple_msgs::RotationMatrixStamped createRandomRotationMatrixStamped() {
	return simple_msgs::RotationMatrixStamped(createRandomHeader(), createRandomRotationMatrix());
}

// define callback function
void callbackFunctionPoint(simple_msgs::Point& p)
{
  // adds one's to the received point
  simple_msgs::Point point(1, 1, 1);
  p += point;
}

// define callback function
void callbackFunctionHeader(simple_msgs::Header& h)
{
  // fill the header fields
  h.setFrameID("ID");
  h.setSequenceNumber(1);
  h.setTimestamp(1.0);
}

// define callback function
void callbackFunctionPose(simple_msgs::Pose& p)
{
  // add one's to the pose
  p.getPosition() += 1.0;
  p.getQuaternion().setW(p.getQuaternion().getW() + 1);
  p.getQuaternion().setX(p.getQuaternion().getX() + 1);
  p.getQuaternion().setY(p.getQuaternion().getY() + 1);
  p.getQuaternion().setZ(p.getQuaternion().getZ() + 1);
}

// define callback function
void callbackFunctionQuaternion(simple_msgs::Quaternion& q)
{
  // add one's to the quaternion
  q.setW(q.getW() + 1);
  q.setX(q.getX() + 1);
  q.setY(q.getY() + 1);
  q.setZ(q.getZ() + 1);
}

// define callback function
void callbackFunctionString(simple_msgs::String& s)
{
  // replace string by standard reply
  s.set("REPLY");
  
}

// define callback function
void callbackFunctionRotationMatrix(simple_msgs::RotationMatrix& r)
{
  // set rotation matrix to zero
  r.setColumn(0, {0, 0, 0});
  r.setColumn(1, {0, 0, 0});
  r.setColumn(2, {0, 0, 0});
}

// define callback function
void callbackFunctionBool(simple_msgs::Bool& b)
{
  // set bool to true
  b.set(true);
}

// define callback function
void callbackFunctionInt(simple_msgs::NumericType<int>& i)
{
  // add 1 to the int
  i += 1;
}

// define callback function
void callbackFunctionDouble(simple_msgs::NumericType<double>& d)
{
  // add 1 to the double
  d += 1.0;
}

// define callback function
void callbackFunctionFloat(simple_msgs::NumericType<float>& f)
{
  // add 1 to the float
  f += 1.0f;
}

void callbackFunctionPointStamped(simple_msgs::PointStamped& p) {
	//add one's to the point and set default header
	callbackFunctionPoint(p.getPoint());
	callbackFunctionHeader(p.getHeader());
}

void callbackFunctionPoseStamped(simple_msgs::PoseStamped& p) {
	//add one's to the point and set default header
	callbackFunctionPose(p.getPose());
	callbackFunctionHeader(p.getHeader());
}

void callbackFunctionPoseStampedLazy(simple_msgs::PoseStamped& p) {
	//add one's to the point and set default header
	callbackFunctionPoseStamped(p);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void callbackFunctionQuaternionStamped(simple_msgs::QuaternionStamped& q) {
	//add one's to the point and set default header
	callbackFunctionQuaternion(q.getQuaternion());
	callbackFunctionHeader(q.getHeader());
}

void callbackFunctionRotationMatrixStamped(simple_msgs::RotationMatrixStamped& r) {
	//add one's to the point and set default header
	callbackFunctionRotationMatrix(r.getRotationMatrix());
	callbackFunctionHeader(r.getHeader());
}

// define callback function
void callbackFunctionConstPoint(const simple_msgs::Point& p)
{
	received_point = p;
	num_receives_point++;
	if (!running_point)
	{
		running_point = true;
	}
}

// define callback function
void callbackFunctionConstHeader(const simple_msgs::Header& h)
{
	received_header = h;
	num_receives_header++;
	if (!running_header)
	{
		running_header = true;
	}
}

// define callback function
void callbackFunctionConstPose(const simple_msgs::Pose& p)
{
	received_pose = p;
	num_receives_pose++;
	if (!running_pose)
	{
		running_pose = true;
	}
}

// define callback function
void callbackFunctionConstQuaternion(const simple_msgs::Quaternion& q)
{
	received_quaternion = q;
	num_receives_quaternion++;
	if (!running_quaternion)
	{
		running_quaternion = true;
	}
}

// define callback function
void callbackFunctionConstString(const simple_msgs::String& s)
{
	received_string = s;
	num_receives_string++;
	if (!running_string)
	{
		running_string = true;
	}
}

// define callback function
void callbackFunctionConstRotationMatrix(const simple_msgs::RotationMatrix& r)
{
	received_rotation_matrix = r;
	num_receives_rotation_matrix++;
	if (!running_rotation_matrix)
	{
		running_rotation_matrix = true;
	}
}

// define callback function
void callbackFunctionConstBool(const simple_msgs::Bool& b)
{
	received_bool = b;
	num_receives_bool++;
	if (!running_bool)
	{
		running_bool = true;
	}
}

// define callback function
void callbackFunctionConstInt(const simple_msgs::NumericType<int>& i)
{
	received_int = i;
	num_receives_int++;
	if (!running_int)
	{
		running_int = true;
	}
}

// define callback function
void callbackFunctionConstDouble(const simple_msgs::NumericType<double>& d)
{
	received_double = d;
	num_receives_double++;
	if (!running_double)
	{
		running_double = true;
	}
}

// define callback function
void callbackFunctionConstFloat(const simple_msgs::NumericType<float>& f)
{
	received_float = f;
	num_receives_float++;
	if (!running_float)
	{
		running_float = true;
	}
}