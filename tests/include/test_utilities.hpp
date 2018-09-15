/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_TESTS_UTILITIES_HPP
#define SIMPLE_TESTS_UTILITIES_HPP

#include <thread>

#include "random_generators.hpp"
#include "simple_msgs/all_messages.h"

//! The total number of messages defined in the simple_msgs namespace.
static constexpr size_t TOTAL_NUM_MESSAGES = 16;

//! Minimum port number to use for the random generation.
static constexpr size_t MIN_PORT_NUMBER = 5555;

//! Maximum port number to use for the random generation.
static constexpr size_t MAX_PORT_NUMBER = 5700;

//! Random port generation.
//! The following objects are used to ranomly assign a port number to pub/sub and client/server tests scenarios.
//! We also keep track of the used ports, so that a non used one is assigned to new test cases. This is to avoid that
//! ZMQ complains about a port being already taken.

//! A random generator of port numbers, this is used to randomly assign a port during pub/sub or client/server tests and
//! avoid that the same number was already assigned to another test scenario within the same test.
static std::uniform_int_distribution<size_t> port_dist(MIN_PORT_NUMBER, MAX_PORT_NUMBER);

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

//! Following are convininet objects for the pub/sub and client/server tests.
//! Since those tests are performed for each message type, we need the same structures for each message.
//! We keep them in the arrays down here and track which message type uses which elements of the array with the enum.

static std::array<bool, TOTAL_NUM_MESSAGES> active_callback{};
static std::array<size_t, TOTAL_NUM_MESSAGES> received_messages{};

enum MessageType {
  Bool = 0,
  Int = 1,
  Double = 2,
  Float = 3,
  String = 4,
  Header = 5,
  Point = 6,
  Quaternion = 7,
  Pose = 8,
  RotationMatrix = 9,
  Transform = 10,
  PointStamped = 11,
  QuaternionStamped = 12,
  PoseStamped = 13,
  RotationMatrixStamped = 14,
  TransformStamped = 15
};

//! Variables to use in callback functions to receive messages.
static simple_msgs::Bool received_bool{};
static simple_msgs::Int received_int{};
static simple_msgs::Double received_double{};
static simple_msgs::Float received_float{};
static simple_msgs::String received_string{};
static simple_msgs::Header received_header{};
static simple_msgs::Point received_point{};
static simple_msgs::Quaternion received_quaternion{};
static simple_msgs::Pose received_pose{};
static simple_msgs::RotationMatrix received_rotation_matrix{};
static simple_msgs::Transform received_transform{};
static simple_msgs::PointStamped received_point_stamped{};
static simple_msgs::QuaternionStamped received_quaternion_stamped{};
static simple_msgs::PoseStamped received_pose_stamped{};
static simple_msgs::RotationMatrixStamped received_rotation_matrix_stamped{};
static simple_msgs::TransformStamped received_transform_stamped{};

//! Functions to randomly generate simple_msgs.

simple_msgs::Bool createRandomBool() { return simple_msgs::Bool(bool_dist(generator)); }

simple_msgs::Int createRandomInt() { return simple_msgs::Int(int_dist(generator)); }

simple_msgs::Double createRandomDouble() { return simple_msgs::Double(double_dist(generator)); }

simple_msgs::Float createRandomFloat() { return simple_msgs::Float(static_cast<float>(double_dist(generator))); }

simple_msgs::String createRandomString() {
  return simple_msgs::String("Random string: " + std::to_string(int_dist(generator)));
}

simple_msgs::Header createRandomHeader() {
  int x = int_dist(generator);
  std::string y("Header string:");
  y.append(std::to_string(int_dist(generator) % 100));
  long long z = static_cast<long long>(double_dist(generator));
  return simple_msgs::Header(x, y, z);
}

simple_msgs::Point createRandomPoint() {
  return simple_msgs::Point(double_dist(generator), double_dist(generator), double_dist(generator));
}

simple_msgs::Quaternion createRandomQuaternion() {
  return simple_msgs::Quaternion(double_dist(generator), double_dist(generator), double_dist(generator),
                                 double_dist(generator));
}

simple_msgs::Pose createRandomPose() { return simple_msgs::Pose(createRandomPoint(), createRandomQuaternion()); }

simple_msgs::RotationMatrix createRandomRotationMatrix() {
  return simple_msgs::RotationMatrix(double_dist(generator), double_dist(generator), double_dist(generator),
                                     double_dist(generator), double_dist(generator), double_dist(generator),
                                     double_dist(generator), double_dist(generator), double_dist(generator));
}

simple_msgs::Transform createRandomTransform() {
  return simple_msgs::Transform(createRandomPoint(), createRandomRotationMatrix());
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

simple_msgs::TransformStamped createRandomTransformStamped() {
  return simple_msgs::TransformStamped(createRandomHeader(), createRandomTransform());
}

//! Set of callback functions for all the simple_msgs.
//! They modify the received point. They are used for client/server tests.

void callbackFunctionBool(simple_msgs::Bool& b) {
  b = !b;  //! Invert the value of the Bool message.
}

void callbackFunctionInt(simple_msgs::Int& i) {
  i.set(i.get() + 1);  //! Add 1.
}

void callbackFunctionDouble(simple_msgs::Double& d) {
  d.set(d.get() + 1);  //! Add 1.
}

void callbackFunctionFloat(simple_msgs::Float& f) {
  f.set(f.get() + 1);  //! Add 1.
}

void callbackFunctionString(simple_msgs::String& s) {
  s.set("REPLY");  //! Replace the string by a standard reply.
}

void callbackFunctionHeader(simple_msgs::Header& h) {
  h.setFrameID("ID");
  h.setSequenceNumber(1);
  h.setTimestamp(10);
}

void callbackFunctionPoint(simple_msgs::Point& p) { ++p; }

void callbackFunctionQuaternion(simple_msgs::Quaternion& q) {
  q.setW(q.getW() + 1);
  q.setX(q.getX() + 1);
  q.setY(q.getY() + 1);
  q.setZ(q.getZ() + 1);
}

void callbackFunctionPose(simple_msgs::Pose& p) {
  p.getPosition() += 1.0;
  p.getQuaternion().setW(p.getQuaternion().getW() + 1);
  p.getQuaternion().setX(p.getQuaternion().getX() + 1);
  p.getQuaternion().setY(p.getQuaternion().getY() + 1);
  p.getQuaternion().setZ(p.getQuaternion().getZ() + 1);
}

void callbackFunctionRotationMatrix(simple_msgs::RotationMatrix& r) {
  r.setColumn(0, {{0, 0, 0}});
  r.setColumn(1, {{0, 0, 0}});
  r.setColumn(2, {{0, 0, 0}});
}

void callbackFunctionTransform(simple_msgs::Transform& t) {
  t.setTranslation({1, 1, 1});
  t.setRotation({1, 1, 1, 1, 1, 1, 1, 1, 1});
}

void callbackFunctionPointStamped(simple_msgs::PointStamped& p) {
  callbackFunctionPoint(p.getPoint());
  callbackFunctionHeader(p.getHeader());
}

void callbackFunctionQuaternionStamped(simple_msgs::QuaternionStamped& q) {
  callbackFunctionQuaternion(q.getQuaternion());
  callbackFunctionHeader(q.getHeader());
}

void callbackFunctionPoseStamped(simple_msgs::PoseStamped& p) {
  callbackFunctionPose(p.getPose());
  callbackFunctionHeader(p.getHeader());
}

void callbackFunctionRotationMatrixStamped(simple_msgs::RotationMatrixStamped& r) {
  callbackFunctionRotationMatrix(r.getRotationMatrix());
  callbackFunctionHeader(r.getHeader());
}

void callbackFunctionTransformStamped(simple_msgs::TransformStamped& t) {
  callbackFunctionTransform(t.getTransform());
  callbackFunctionHeader(t.getHeader());
}

//! Set of const callbacks for all the simple_msgs
//! They receive a message and just save it. They are used for pub/sub tests.

void callbackFunctionConstBool(const simple_msgs::Bool& b) {
  received_bool = b;
  received_messages[MessageType::Bool]++;
  if (!active_callback[MessageType::Bool]) { active_callback[MessageType::Bool] = true; }
}

void callbackFunctionConstInt(const simple_msgs::Int& i) {
  received_int = i;
  received_messages[MessageType::Int]++;
  if (!active_callback[MessageType::Int]) { active_callback[MessageType::Int] = true; }
}

void callbackFunctionConstFloat(const simple_msgs::Float& f) {
  received_float = f;
  received_messages[MessageType::Float]++;
  if (!active_callback[MessageType::Float]) { active_callback[MessageType::Float] = true; }
}

void callbackFunctionConstDouble(const simple_msgs::Double& d) {
  received_double = d;
  received_messages[MessageType::Double]++;
  if (!active_callback[MessageType::Double]) { active_callback[MessageType::Double] = true; }
}

void callbackFunctionConstString(const simple_msgs::String& s) {
  received_string = s;
  received_messages[MessageType::String]++;
  if (!active_callback[MessageType::String]) { active_callback[MessageType::String] = true; }
}

void callbackFunctionConstHeader(const simple_msgs::Header& h) {
  received_header = h;
  received_messages[MessageType::Header]++;
  if (!active_callback[MessageType::Header]) { active_callback[MessageType::Header] = true; }
}

void callbackFunctionConstPoint(const simple_msgs::Point& p) {
  received_point = p;
  received_messages[MessageType::Point]++;
  if (!active_callback[MessageType::Point]) { active_callback[MessageType::Point] = true; }
}

void callbackFunctionConstQuaternion(const simple_msgs::Quaternion& q) {
  received_quaternion = q;
  received_messages[MessageType::Quaternion]++;
  if (!active_callback[MessageType::Quaternion]) { active_callback[MessageType::Quaternion] = true; }
}

void callbackFunctionConstPose(const simple_msgs::Pose& p) {
  received_pose = p;
  received_messages[MessageType::Pose]++;
  if (!active_callback[MessageType::Pose]) { active_callback[MessageType::Pose] = true; }
}

void callbackFunctionConstRotationMatrix(const simple_msgs::RotationMatrix& r) {
  received_rotation_matrix = r;
  received_messages[MessageType::RotationMatrix]++;
  if (!active_callback[MessageType::RotationMatrix]) { active_callback[MessageType::RotationMatrix] = true; }
}

void callbackFunctionConstTransform(const simple_msgs::Transform& t) {
  received_transform = t;
  received_messages[MessageType::Transform]++;
  if (!active_callback[MessageType::Transform]) { active_callback[MessageType::Transform] = true; }
}

void callbackFunctionConstPointStamped(const simple_msgs::PointStamped& ps) {
  received_point_stamped = ps;
  received_messages[MessageType::PointStamped]++;
  if (!active_callback[MessageType::PointStamped]) { active_callback[MessageType::PointStamped] = true; }
}

void callbackFunctionConstQuaternionStamped(const simple_msgs::QuaternionStamped& qs) {
  received_quaternion_stamped = qs;
  received_messages[MessageType::QuaternionStamped]++;
  if (!active_callback[MessageType::QuaternionStamped]) { active_callback[MessageType::QuaternionStamped] = true; }
}

void callbackFunctionConstPoseStamped(const simple_msgs::PoseStamped& ps) {
  received_pose_stamped = ps;
  received_messages[MessageType::PoseStamped]++;
  if (!active_callback[MessageType::PoseStamped]) { active_callback[MessageType::PoseStamped] = true; }
}

void callbackFunctionConstRotationMatrixStamped(const simple_msgs::RotationMatrixStamped& rs) {
  received_rotation_matrix_stamped = rs;
  received_messages[MessageType::RotationMatrixStamped]++;
  if (!active_callback[MessageType::RotationMatrixStamped]) {
    active_callback[MessageType::RotationMatrixStamped] = true;
  }
}

void callbackFunctionConstTransformStamped(const simple_msgs::TransformStamped& ts) {
  received_transform_stamped = ts;
  received_messages[MessageType::TransformStamped]++;
  if (!active_callback[MessageType::TransformStamped]) { active_callback[MessageType::TransformStamped] = true; }
}

#endif  // SIMPLE_TESTS_UTILITIES_HPP.
