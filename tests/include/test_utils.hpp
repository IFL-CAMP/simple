#include "simple_msgs/bool.h"
#include "simple_msgs/int.h"
#include "simple_msgs/float.h"
#include "simple_msgs/double.h"
#include "simple_msgs/string.h"
#include "simple_msgs/header.h"
#include "simple_msgs/point.h"
#include "simple_msgs/point_stamped.h"
#include "simple_msgs/quaternion.h"
#include "simple_msgs/quaternion_stamped.h"
#include "simple_msgs/pose.h"
#include "simple_msgs/pose_stamped.h"
#include "simple_msgs/rotation_matrix.h"
#include "simple_msgs/rotation_matrix_stamped.h"

// Function for random message generation.
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
  double m_11 = static_cast<double>(rand()) / RAND_MAX;
  double m_12 = static_cast<double>(rand()) / RAND_MAX;
  double m_13 = static_cast<double>(rand()) / RAND_MAX;
  double m_21 = static_cast<double>(rand()) / RAND_MAX;
  double m_22 = static_cast<double>(rand()) / RAND_MAX;
  double m_23 = static_cast<double>(rand()) / RAND_MAX;
  double m_31 = static_cast<double>(rand()) / RAND_MAX;
  double m_32 = static_cast<double>(rand()) / RAND_MAX;
  double m_33 = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::RotationMatrix(m_11, m_12, m_13, m_21, m_22, m_23, m_31, m_32, m_33);
}

simple_msgs::Pose createRandomPose()
{
  return simple_msgs::Pose(createRandomPoint(), createRandomQuaternion());
}

simple_msgs::Int createRandomInt()
{
  int x = rand() % 100;
  return simple_msgs::Int(x);
}

simple_msgs::Double createRandomDouble()
{
  double x = static_cast<double>(rand()) / RAND_MAX;
  return simple_msgs::Double(x);
}

simple_msgs::Float createRandomFloat()
{
  float x = static_cast<float>(rand()) / RAND_MAX;
  return simple_msgs::Float(x);
}

simple_msgs::Bool createRandomBool()
{
  bool x((rand() % 1) != 0);
  return simple_msgs::Bool(x);
}

simple_msgs::String createRandomString()
{
  std::string s("Random string: " + std::to_string(rand() % 100));
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

// Set of callback functions for all the simple_msgs.
// They modify the received point.

void callbackFunctionPoint(simple_msgs::Point& p)
{
  simple_msgs::Point point(1, 1, 1);
  p += point;
}

void callbackFunctionHeader(simple_msgs::Header& h)
{
  h.setFrameID("ID");
  h.setSequenceNumber(1);
  h.setTimestamp(1.0);
}

void callbackFunctionPose(simple_msgs::Pose& p)
{
  p.getPosition() += 1.0;
  p.getQuaternion().setW(p.getQuaternion().getW() + 1);
  p.getQuaternion().setX(p.getQuaternion().getX() + 1);
  p.getQuaternion().setY(p.getQuaternion().getY() + 1);
  p.getQuaternion().setZ(p.getQuaternion().getZ() + 1);
}

void callbackFunctionQuaternion(simple_msgs::Quaternion& q)
{
  q.setW(q.getW() + 1);
  q.setX(q.getX() + 1);
  q.setY(q.getY() + 1);
  q.setZ(q.getZ() + 1);
}

void callbackFunctionString(simple_msgs::String& s)
{
  // Replace the string by a standard reply.
  s.set("REPLY");
}

void callbackFunctionRotationMatrix(simple_msgs::RotationMatrix& r)
{
  r.setColumn(0, {0, 0, 0});
  r.setColumn(1, {0, 0, 0});
  r.setColumn(2, {0, 0, 0});
}

void callbackFunctionBool(simple_msgs::Bool& b)
{
  b = !b; //< Invert the value of the Bool message.
}

void callbackFunctionInt(simple_msgs::Int& i)
{
  i += 1; //< Add 1.
}

// define callback function
void callbackFunctionDouble(simple_msgs::Double& d)
{
  d += 1.0; //< Add 1.
}

// define callback function
void callbackFunctionFloat(simple_msgs::Float& f)
{
  f += 1.0f; //< Add 1.
}

void callbackFunctionPointStamped(simple_msgs::PointStamped& p) {
  // Add one to the point and set a default header.
	callbackFunctionPoint(p.getPoint());
	callbackFunctionHeader(p.getHeader());
}

void callbackFunctionPoseStamped(simple_msgs::PoseStamped& p) {
  // Add one to the point and set a default header.
  callbackFunctionPose(p.getPose());
	callbackFunctionHeader(p.getHeader());
}

void callbackFunctionQuaternionStamped(simple_msgs::QuaternionStamped& q) {
  // Add one to the point and set a default header.
  callbackFunctionQuaternion(q.getQuaternion());
	callbackFunctionHeader(q.getHeader());
}

void callbackFunctionRotationMatrixStamped(simple_msgs::RotationMatrixStamped& r) {
  // Add one to the point and set a default header.
  callbackFunctionRotationMatrix(r.getRotationMatrix());
	callbackFunctionHeader(r.getHeader());
}

// Set of const callbacks for all the simple_msgs
// They receive a message and just save it.

bool running_bool = false, running_int = false, running_float = false,
     running_double = false, running_string = false, running_header = false,
     running_point =  false, running_quaternion = false, running_pose = false,
     running_rotation_matrix = false, running_point_stamped = false,
     running_quaternion_stamped = false, running_pose_stamped = false,
     running_rotation_matrix_stamped = false;

int num_received_bool = 0, num_received_int = 0, num_received_float = 0,
    num_received_double = 0, num_received_string = 0, num_received_header = 0,
    num_received_point = 0, num_received_quaternion = 0, num_received_pose = 0,
    num_received_rotation_matrix = 0, num_received_point_stamped = 0,
    num_received_quaternion_stamped = 0, num_received_pose_stamped = 0,
    num_received_rotation_matrix_stamped = 0;

simple_msgs::Bool received_bool;
simple_msgs::Int received_int;
simple_msgs::Double received_double;
simple_msgs::Float received_float;
simple_msgs::String received_string;
simple_msgs::Header received_header;
simple_msgs::Point received_point;
simple_msgs::PointStamped received_point_stamped;
simple_msgs::Quaternion received_quaternion;
simple_msgs::QuaternionStamped received_quaternion_stamped;
simple_msgs::Pose received_pose;
simple_msgs::PoseStamped received_pose_stamped;
simple_msgs::RotationMatrix received_rotation_matrix;
simple_msgs::RotationMatrixStamped received_rotation_matrix_stamped;

void callbackFunctionConstBool(const simple_msgs::Bool& b)
{
  received_bool = b;
  num_received_bool++;
  if (!running_bool)
  {
    running_bool = true;
  }
}

void callbackFunctionConstInt(const simple_msgs::Int& i)
{
  received_int = i;
  num_received_int++;
  if (!running_int)
  {
    running_int = true;
  }
}

void callbackFunctionConstFloat(const simple_msgs::Float& f)
{
  received_float = f;
  num_received_float++;
  if (!running_float)
  {
    running_float = true;
  }
}

void callbackFunctionConstDouble(const simple_msgs::Double& d)
{
  received_double = d;
  num_received_double++;
  if (!running_double)
  {
    running_double = true;
  }
}

void callbackFunctionConstString(const simple_msgs::String& s)
{
	received_string = s;
  num_received_string++;
	if (!running_string)
	{
		running_string = true;
	}
}


void callbackFunctionConstHeader(const simple_msgs::Header& h)
{
  received_header = h;
  num_received_header++;
  if (!running_header)
  {
    running_header = true;
  }
}

void callbackFunctionConstPoint(const simple_msgs::Point& p)
{
  received_point = p;
  num_received_point++;
  if (!running_point)
  {
    running_point = true;
  }
}

void callbackFunctionConstPointStamped(const simple_msgs::PointStamped& ps)
{
  received_point_stamped = ps;
  num_received_point_stamped++;
  if (!running_point_stamped)
  {
    running_point_stamped = true;
  }
}

void callbackFunctionConstQuaternion(const simple_msgs::Quaternion& q)
{
  received_quaternion = q;
  num_received_quaternion++;
  if (!running_quaternion)
  {
    running_quaternion = true;
  }
}


void callbackFunctionConstQuaternionStamped(const simple_msgs::QuaternionStamped& qs)
{
  received_quaternion_stamped = qs;
  num_received_quaternion_stamped++;
  if (!running_quaternion_stamped)
  {
    running_quaternion_stamped = true;
  }
}

void callbackFunctionConstPose(const simple_msgs::Pose& p)
{
  received_pose = p;
  num_received_pose++;
  if (!running_pose)
  {
    running_pose = true;
  }
}

void callbackFunctionConstPoseStamped(const simple_msgs::PoseStamped& ps)
{
  received_pose_stamped = ps;
  num_received_pose_stamped++;
  if (!running_pose_stamped)
  {
    running_pose_stamped = true;
  }
}

void callbackFunctionConstRotationMatrix(const simple_msgs::RotationMatrix& r)
{
  received_rotation_matrix = r;
  num_received_rotation_matrix++;
  if (!running_rotation_matrix)
  {
    running_rotation_matrix = true;
  }
}

void callbackFunctionConstRotationMatrixStamped(const simple_msgs::RotationMatrixStamped& rs)
{
  received_rotation_matrix_stamped = rs;
  num_received_rotation_matrix_stamped++;
  if (!running_rotation_matrix_stamped)
  {
    running_rotation_matrix_stamped = true;
  }
}
