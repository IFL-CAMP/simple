
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

// TEST FOR A CLIENT AND SERVER OF ALL DATA TYPES

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

