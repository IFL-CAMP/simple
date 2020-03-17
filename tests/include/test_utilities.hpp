/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_TEST_UTILITIES_HPP
#define SIMPLE_TEST_UTILITIES_HPP

#include <array>
#include <thread>

#include "random_generators.hpp"

#include "simple_msgs/bool.pb.h"
#include "simple_msgs/header.pb.h"
#include "simple_msgs/image.pb.h"
#include "simple_msgs/point.pb.h"
#include "simple_msgs/point_stamped.pb.h"
#include "simple_msgs/pose.pb.h"
#include "simple_msgs/pose_stamped.pb.h"
#include "simple_msgs/quaternion.pb.h"
#include "simple_msgs/quaternion_stamped.pb.h"
#include "simple_msgs/rotation_matrix.pb.h"
#include "simple_msgs/rotation_matrix_stamped.pb.h"
#include "simple_msgs/string.pb.h"
#include "simple_msgs/transform.pb.h"
#include "simple_msgs/transform_stamped.pb.h"

namespace simple_tests {

static const std::string kAddressPrefix = "tcp://127.0.0.1:";
static const std::string kInvalidAddress = "Invalid address";
static constexpr size_t kTestMessagesToSend = 5;
static constexpr size_t kWaitTimeForServers = 2;                    //! Seconds.
static constexpr size_t kWaitTimeForSubscribers = 1;                //! Seconds.
static constexpr size_t kWaitTimeBetweenMessagesMilliseconds = 10;  //! Milliseconds.
static constexpr size_t kDefaultTimeoutMilliseconds = 1;            //! Milliseconds.
static constexpr size_t kDefaultLingerMilliseconds = 1;             //! Milliseconds.

//! The total number of messages defined in the simple_msgs namespace.
static constexpr size_t kTotalNumberMessages = 13;

//! Enum of defined messages.
enum MessageType {
  Bool = 0,
  String = 1,
  Header = 2,
  Point = 3,
  Quaternion = 4,
  Pose = 5,
  RotationMatrix = 6,
  Transform = 7,
  PointStamped = 8,
  QuaternionStamped = 9,
  PoseStamped = 10,
  RotationMatrixStamped = 11,
  TransformStamped = 12
};

//! Minimum port number to use for the random generation.
static constexpr size_t kMinPortNumber = 5555;

//! Maximum port number to use for the random generation.
static constexpr size_t kMaxPortNumber = 5700;

//! Random port generation.
//!
//! The following objects are used to randomly assign a port number to pub/sub and client/server tests scenarios.
//! We also keep track of the used ports, so that a non used one is assigned to new test cases. This is to avoid that
//! ZMQ complains about a port being already taken.

//! A random generator of port numbers, this is used to randomly assign a port during pub/sub or client/server tests and
//! avoid that the same number was already assigned to another test scenario within the same test.
static std::uniform_int_distribution<size_t> port_dist(kMinPortNumber, kMaxPortNumber);

//! Keeps track of which port number was already used in a test executable.
static std::vector<size_t> port_pool{};

//! To avoid that multiple test scenario race to generate/check ports.
static std::mutex port_mutex{};

/**
 * @brief Generate a random port number to use and check if it wasn't used yet. If so it adds the new generated number
 * to the pool of used ports and gives it back to the user.
 */
size_t generatePort() {
  std::lock_guard<std::mutex> lock(port_mutex);
  bool success{false};
  while (!success) {
    size_t port = port_dist(generator);
    if (std::find(port_pool.begin(), port_pool.end(), port) == port_pool.end()) {
      port_pool.push_back(port);
      success = true;
      return port;
    }
  }
  return 0;
}

//! END - Random port generation.

//! Following are convenient objects for the pub/sub and client/server tests.
//! Since those tests are performed for each message type, we need the same structures for each message.
//! We keep them in the arrays down here and track which message type uses which elements of the array with the enum.

static std::array<bool, kTotalNumberMessages> active_callback;
static std::array<size_t, kTotalNumberMessages> received_messages;

//! Variables to use in callback functions to receive messages.
static simple_msgs::Bool check_bool{};
static simple_msgs::String check_string{};
static simple_msgs::Header check_header{};
static simple_msgs::Point check_point{};
static simple_msgs::Quaternion check_quaternion{};
static simple_msgs::Pose check_pose{};
static simple_msgs::RotationMatrix check_rotation_matrix{};
static simple_msgs::Transform check_transform{};
static simple_msgs::PointStamped check_point_stamped{};
static simple_msgs::QuaternionStamped check_quaternion_stamped{};
static simple_msgs::PoseStamped check_pose_stamped{};
static simple_msgs::RotationMatrixStamped check_rotation_matrix_stamped{};
static simple_msgs::TransformStamped check_transform_stamped{};

//! Functions to randomly generate simple_msgs.

simple_msgs::Bool createRandomBool() {
  simple_msgs::Bool message;
  message.set_value(bool_dist(generator));
  return message;
}

simple_msgs::String createRandomString() {
  simple_msgs::String message;
  message.set_value("Random string: " + std::to_string(int_dist(generator)));
  return message;
}

simple_msgs::Header createRandomHeader() {
  int id = int_dist(generator);
  std::string frame("Header string:");
  frame.append(std::to_string(int_dist(generator) % 100));
  auto time = static_cast<long long>(double_dist(generator));

  simple_msgs::Header message;
  message.set_id(id);
  message.set_frame(frame);
  message.set_timestamp(time);
  return message;
}

simple_msgs::Point createRandomPoint() {
  simple_msgs::Point message;
  message.set_x(double_dist(generator));
  message.set_y(double_dist(generator));
  message.set_z(double_dist(generator));
  return message;
}

simple_msgs::Quaternion createRandomQuaternion() {
  simple_msgs::Quaternion message;
  message.set_x(double_dist(generator));
  message.set_y(double_dist(generator));
  message.set_z(double_dist(generator));
  message.set_w(double_dist(generator));
  return message;
}

simple_msgs::Pose createRandomPose() {
  auto point = new simple_msgs::Point(createRandomPoint());
  auto quaternion = new simple_msgs::Quaternion(createRandomQuaternion());
  simple_msgs::Pose message;
  message.set_allocated_point(point);
  message.set_allocated_quaternion(quaternion);
  return message;
}

simple_msgs::RotationMatrix createRandomRotationMatrix() {
  simple_msgs::RotationMatrix message;
  message.set_r00(double_dist(generator));
  message.set_r01(double_dist(generator));
  message.set_r02(double_dist(generator));
  message.set_r10(double_dist(generator));
  message.set_r11(double_dist(generator));
  message.set_r12(double_dist(generator));
  message.set_r20(double_dist(generator));
  message.set_r21(double_dist(generator));
  message.set_r22(double_dist(generator));
  return message;
}

simple_msgs::Transform createRandomTransform() {
  auto point = new simple_msgs::Point(createRandomPoint());
  auto matrix = new simple_msgs::RotationMatrix(createRandomRotationMatrix());
  simple_msgs::Transform message;
  message.set_allocated_point(point);
  message.set_allocated_matrix(matrix);
  return message;
}

simple_msgs::PointStamped createRandomPointStamped() {
  auto header = new simple_msgs::Header(createRandomHeader());
  auto point = new simple_msgs::Point(createRandomPoint());
  simple_msgs::PointStamped message;
  message.set_allocated_header(header);
  message.set_allocated_point(point);
  return message;
}

simple_msgs::PoseStamped createRandomPoseStamped() {
  auto header = new simple_msgs::Header(createRandomHeader());
  auto pose = new simple_msgs::Pose(createRandomPose());
  simple_msgs::PoseStamped message;
  message.set_allocated_header(header);
  message.set_allocated_pose(pose);
  return message;
}

simple_msgs::QuaternionStamped createRandomQuaternionStamped() {
  auto header = new simple_msgs::Header(createRandomHeader());
  auto quaternion = new simple_msgs::Quaternion(createRandomQuaternion());
  simple_msgs::QuaternionStamped message;
  message.set_allocated_header(header);
  message.set_allocated_quaternion(quaternion);
  return message;
}

simple_msgs::RotationMatrixStamped createRandomRotationMatrixStamped() {
  auto header = new simple_msgs::Header(createRandomHeader());
  auto matrix = new simple_msgs::RotationMatrix(createRandomRotationMatrix());
  simple_msgs::RotationMatrixStamped message;
  message.set_allocated_header(header);
  message.set_allocated_matrix(matrix);
  return message;
}

simple_msgs::TransformStamped createRandomTransformStamped() {
  auto header = new simple_msgs::Header(createRandomHeader());
  auto transform = new simple_msgs::Transform(createRandomTransform());
  simple_msgs::TransformStamped message;
  message.set_allocated_header(header);
  message.set_allocated_transform(transform);
  return message;
}

//! Set of callback functions for all the simple_msgs.
//! They modify the received message. They are used for client/server tests.

void callbackFunctionBool(simple_msgs::Bool& b) {
  //! Invert the value of the Bool message.
  check_bool.set_value(!b.value());
  b = check_bool;
}

void callbackFunctionString(simple_msgs::String& s) {
  check_string.set_value("String " + std::to_string(int_dist(generator)));
  s = check_string;
}

void callbackFunctionHeader(simple_msgs::Header& h) {
  check_header = createRandomHeader();
  h = check_header;
}

void callbackFunctionPoint(simple_msgs::Point& p) {
  check_point = createRandomPoint();
  p = check_point;
}

void callbackFunctionQuaternion(simple_msgs::Quaternion& q) {
  check_quaternion = createRandomQuaternion();
  q = check_quaternion;
}

void callbackFunctionPose(simple_msgs::Pose& p) {
  check_pose = createRandomPose();
  p = check_pose;
}

void callbackFunctionRotationMatrix(simple_msgs::RotationMatrix& r) {
  check_rotation_matrix = createRandomRotationMatrix();
  r = check_rotation_matrix;
}

void callbackFunctionTransform(simple_msgs::Transform& t) {
  check_transform = createRandomTransform();
  t = check_transform;
}

void callbackFunctionPointStamped(simple_msgs::PointStamped& p) {
  check_point_stamped = createRandomPointStamped();
  p = check_point_stamped;
}

void callbackFunctionQuaternionStamped(simple_msgs::QuaternionStamped& q) {
  check_quaternion_stamped = createRandomQuaternionStamped();
  q = check_quaternion_stamped;
}

void callbackFunctionPoseStamped(simple_msgs::PoseStamped& p) {
  check_pose_stamped = createRandomPoseStamped();
  p = check_pose_stamped;
}

void callbackFunctionRotationMatrixStamped(simple_msgs::RotationMatrixStamped& r) {
  check_rotation_matrix_stamped = createRandomRotationMatrixStamped();
  r = check_rotation_matrix_stamped;
}

void callbackFunctionTransformStamped(simple_msgs::TransformStamped& t) {
  check_transform_stamped = createRandomTransformStamped();
  t = check_transform_stamped;
}

//! Set of const callbacks for all the simple_msgs
//! They receive a message and just save it. They are used for pub/sub tests.

void callbackFunctionConstBool(const simple_msgs::Bool& b) {
  check_bool = b;
  received_messages[MessageType::Bool]++;
  if (!active_callback[MessageType::Bool]) { active_callback[MessageType::Bool] = true; }
}

void callbackFunctionConstString(const simple_msgs::String& s) {
  check_string = s;
  received_messages[MessageType::String]++;
  if (!active_callback[MessageType::String]) { active_callback[MessageType::String] = true; }
}

void callbackFunctionConstHeader(const simple_msgs::Header& h) {
  check_header = h;
  received_messages[MessageType::Header]++;
  if (!active_callback[MessageType::Header]) { active_callback[MessageType::Header] = true; }
}

void callbackFunctionConstPoint(const simple_msgs::Point& p) {
  check_point = p;
  received_messages[MessageType::Point]++;
  if (!active_callback[MessageType::Point]) { active_callback[MessageType::Point] = true; }
}

void callbackFunctionConstQuaternion(const simple_msgs::Quaternion& q) {
  check_quaternion = q;
  received_messages[MessageType::Quaternion]++;
  if (!active_callback[MessageType::Quaternion]) { active_callback[MessageType::Quaternion] = true; }
}

void callbackFunctionConstPose(const simple_msgs::Pose& p) {
  check_pose = p;
  received_messages[MessageType::Pose]++;
  if (!active_callback[MessageType::Pose]) { active_callback[MessageType::Pose] = true; }
}

void callbackFunctionConstRotationMatrix(const simple_msgs::RotationMatrix& r) {
  check_rotation_matrix = r;
  received_messages[MessageType::RotationMatrix]++;
  if (!active_callback[MessageType::RotationMatrix]) { active_callback[MessageType::RotationMatrix] = true; }
}

void callbackFunctionConstTransform(const simple_msgs::Transform& t) {
  check_transform = t;
  received_messages[MessageType::Transform]++;
  if (!active_callback[MessageType::Transform]) { active_callback[MessageType::Transform] = true; }
}

void callbackFunctionConstPointStamped(const simple_msgs::PointStamped& ps) {
  check_point_stamped = ps;
  received_messages[MessageType::PointStamped]++;
  if (!active_callback[MessageType::PointStamped]) { active_callback[MessageType::PointStamped] = true; }
}

void callbackFunctionConstQuaternionStamped(const simple_msgs::QuaternionStamped& qs) {
  check_quaternion_stamped = qs;
  received_messages[MessageType::QuaternionStamped]++;
  if (!active_callback[MessageType::QuaternionStamped]) { active_callback[MessageType::QuaternionStamped] = true; }
}

void callbackFunctionConstPoseStamped(const simple_msgs::PoseStamped& ps) {
  check_pose_stamped = ps;
  received_messages[MessageType::PoseStamped]++;
  if (!active_callback[MessageType::PoseStamped]) { active_callback[MessageType::PoseStamped] = true; }
}

void callbackFunctionConstRotationMatrixStamped(const simple_msgs::RotationMatrixStamped& rs) {
  check_rotation_matrix_stamped = rs;
  received_messages[MessageType::RotationMatrixStamped]++;
  if (!active_callback[MessageType::RotationMatrixStamped]) {
    active_callback[MessageType::RotationMatrixStamped] = true;
  }
}

void callbackFunctionConstTransformStamped(const simple_msgs::TransformStamped& ts) {
  check_transform_stamped = ts;
  received_messages[MessageType::TransformStamped]++;
  if (!active_callback[MessageType::TransformStamped]) { active_callback[MessageType::TransformStamped] = true; }
}

}  // Namespace simple_tests.

#endif  // SIMPLE_TESTS_UTILITIES_HPP.
