/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "simple/client.hpp"
#include "simple/server.hpp"
#include "test_utilities.hpp"

using namespace simple_tests;

// TEST FOR A CLIENT AND SERVER OF ALL DATA TYPES

SCENARIO("Client requests a non-existing server") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("A Client to a Point") {
    simple::Client<simple_msgs::Point> client(address, kDefaultTimeoutMilliseconds, kDefaultLingerMilliseconds);
    WHEN("The client sends a request") {
      auto p = createRandomPoint();
      auto sent_point = p;
      THEN("The timeout is reached and no reply is received") { REQUIRE(client.request(p) == false); }
    }
  }
}

SCENARIO("Two Servers binding to the same address") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("A server") {
    simple::Server<simple_msgs::Point> server(address, callbackFunctionPoint);
    WHEN("Another server tries to bind to the same address") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS(simple::Server<simple_msgs::Point>(address, callbackFunctionPoint));
      }
    }
  }
}

SCENARIO("A Client connecting to a false address") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("A client") {
    WHEN("The address provided is wrong") {
      THEN("An exception is thrown") { REQUIRE_THROWS(simple::Client<simple_msgs::Point>(address)); }
    }
  }
}

SCENARIO("Client-Server to a Bool message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Bool> client(address);
    simple::Server<simple_msgs::Bool> server(address, callbackFunctionBool);

    WHEN("The client sends a request") {
      auto message = createRandomBool();
      auto sentMessage = message;
      client.request(message);

      THEN("The data received is true") { REQUIRE(message.value() == check_bool.value()); }
    }
  }
}

SCENARIO("Client-Server to a String message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::String> client(address);
    simple::Server<simple_msgs::String> server(address, callbackFunctionString);

    WHEN("The client sends a request") {
      auto message = createRandomString();
      client.request(message);
      THEN("The string received back is the default answer") { REQUIRE(message.value() == check_string.value()); }
    }
  }
}

SCENARIO("Client-Server to a Header message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Header> client(address);
    simple::Server<simple_msgs::Header> server(address, callbackFunctionHeader);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("The client sends a request") {
      auto message = createRandomHeader();
      client.request(message);
      THEN("The data received is filled wth the standard values") {
        REQUIRE(message.id() == check_header.id());
        REQUIRE(message.frame() == check_header.frame());
        REQUIRE(message.timestamp() == check_header.timestamp());
      }
    }
  }
}

SCENARIO("Client-Server to a Point message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Point> client(address);
    simple::Server<simple_msgs::Point> server(address, callbackFunctionPoint);

    WHEN("The client sends a request") {
      auto message = createRandomPoint();
      client.request(message);

      THEN("The data received is the equal to the sent point plus one") {
        REQUIRE(message.x() == check_point.x());
        REQUIRE(message.y() == check_point.y());
        REQUIRE(message.z() == check_point.z());
      }
    }
  }
}

SCENARIO("Client-Server to a Quaternion message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Quaternion> client(address);
    simple::Server<simple_msgs::Quaternion> server(address, callbackFunctionQuaternion);

    WHEN("The client sends a request") {
      auto message = createRandomQuaternion();
      client.request(message);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(message.x() == check_quaternion.x());
        REQUIRE(message.y() == check_quaternion.y());
        REQUIRE(message.z() == check_quaternion.z());
        REQUIRE(message.w() == check_quaternion.w());
      }
    }
  }
}

SCENARIO("Client-Server to a Pose message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Pose> client(address);
    simple::Server<simple_msgs::Pose> server(address, callbackFunctionPose);

    WHEN("The client sends a request") {
      auto message = createRandomPose();
      client.request(message);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(message.point().x() == check_pose.point().x());
        REQUIRE(message.point().y() == check_pose.point().y());
        REQUIRE(message.point().z() == check_pose.point().z());
        REQUIRE(message.quaternion().x() == check_pose.quaternion().x());
        REQUIRE(message.quaternion().y() == check_pose.quaternion().y());
        REQUIRE(message.quaternion().z() == check_pose.quaternion().z());
        REQUIRE(message.quaternion().w() == check_pose.quaternion().w());
      }
    }
  }
}

SCENARIO("Client-Server to a Rotation Matrix message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server.
    simple::Client<simple_msgs::RotationMatrix> client(address);
    simple::Server<simple_msgs::RotationMatrix> server(address, callbackFunctionRotationMatrix);

    WHEN("The client sends a request") {
      auto message = createRandomRotationMatrix();
      client.request(message);
      THEN("The data received is the the identity matrix") {
        REQUIRE(message.r00() == check_rotation_matrix.r00());
        REQUIRE(message.r01() == check_rotation_matrix.r01());
        REQUIRE(message.r02() == check_rotation_matrix.r02());
        REQUIRE(message.r10() == check_rotation_matrix.r10());
        REQUIRE(message.r11() == check_rotation_matrix.r11());
        REQUIRE(message.r12() == check_rotation_matrix.r12());
        REQUIRE(message.r20() == check_rotation_matrix.r20());
        REQUIRE(message.r21() == check_rotation_matrix.r21());
        REQUIRE(message.r22() == check_rotation_matrix.r22());
      }
    }
  }
}

SCENARIO("Client-Server to a Transform message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Transform> client(address);
    simple::Server<simple_msgs::Transform> server(address, callbackFunctionTransform);
    WHEN("The client sends a request") {
      auto message = createRandomTransform();
      client.request(message);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(message.point().x() == check_transform.point().x());
        REQUIRE(message.point().y() == check_transform.point().y());
        REQUIRE(message.point().z() == check_transform.point().z());
        REQUIRE(message.matrix().r00() == check_transform.matrix().r00());
        REQUIRE(message.matrix().r01() == check_transform.matrix().r01());
        REQUIRE(message.matrix().r02() == check_transform.matrix().r02());
        REQUIRE(message.matrix().r10() == check_transform.matrix().r10());
        REQUIRE(message.matrix().r11() == check_transform.matrix().r11());
        REQUIRE(message.matrix().r12() == check_transform.matrix().r12());
        REQUIRE(message.matrix().r20() == check_transform.matrix().r20());
        REQUIRE(message.matrix().r21() == check_transform.matrix().r21());
        REQUIRE(message.matrix().r22() == check_transform.matrix().r22());
      }
    }
  }
}

SCENARIO("Client-Server to a PointStamped message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::PointStamped> client(address);
    simple::Server<simple_msgs::PointStamped> server(address, callbackFunctionPointStamped);

    WHEN("The client sends a request") {
      auto message = createRandomPointStamped();
      client.request(message);
      THEN("The data received is the equal to the sent point plus one's") {
        REQUIRE(message.header().id() == check_point_stamped.header().id());
        REQUIRE(message.header().frame() == check_point_stamped.header().frame());
        REQUIRE(message.header().timestamp() == check_point_stamped.header().timestamp());
        REQUIRE(message.point().x() == check_point_stamped.point().x());
        REQUIRE(message.point().y() == check_point_stamped.point().y());
        REQUIRE(message.point().z() == check_point_stamped.point().z());
      }
    }
  }
}

SCENARIO("Client-Server to a QuaternionStamped message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::QuaternionStamped> client(address);
    simple::Server<simple_msgs::QuaternionStamped> server(address, callbackFunctionQuaternionStamped);

    WHEN("The client sends a request") {
      auto message = createRandomQuaternionStamped();
      client.request(message);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(message.header().id() == check_quaternion_stamped.header().id());
        REQUIRE(message.header().frame() == check_quaternion_stamped.header().frame());
        REQUIRE(message.header().timestamp() == check_quaternion_stamped.header().timestamp());
        REQUIRE(message.quaternion().x() == check_quaternion_stamped.quaternion().x());
        REQUIRE(message.quaternion().y() == check_quaternion_stamped.quaternion().y());
        REQUIRE(message.quaternion().z() == check_quaternion_stamped.quaternion().z());
        REQUIRE(message.quaternion().w() == check_quaternion_stamped.quaternion().w());
      }
    }
  }
}

SCENARIO("Client-Server to a PoseStamped message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::PoseStamped> client(address);
    simple::Server<simple_msgs::PoseStamped> server(address, callbackFunctionPoseStamped);

    WHEN("The client sends a request") {
      auto message = createRandomPoseStamped();
      client.request(message);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(message.header().id() == check_pose_stamped.header().id());
        REQUIRE(message.header().frame() == check_pose_stamped.header().frame());
        REQUIRE(message.header().timestamp() == check_pose_stamped.header().timestamp());
        REQUIRE(message.pose().point().x() == check_pose_stamped.pose().point().x());
        REQUIRE(message.pose().point().y() == check_pose_stamped.pose().point().y());
        REQUIRE(message.pose().point().z() == check_pose_stamped.pose().point().z());
        REQUIRE(message.pose().quaternion().x() == check_pose_stamped.pose().quaternion().x());
        REQUIRE(message.pose().quaternion().y() == check_pose_stamped.pose().quaternion().y());
        REQUIRE(message.pose().quaternion().z() == check_pose_stamped.pose().quaternion().z());
        REQUIRE(message.pose().quaternion().w() == check_pose_stamped.pose().quaternion().w());
      }
    }
  }
}

SCENARIO("Client-Server to a RotationMatrixStamped message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    // start a server.
    simple::Client<simple_msgs::RotationMatrixStamped> client(address);
    simple::Server<simple_msgs::RotationMatrixStamped> server(address, callbackFunctionRotationMatrixStamped);

    WHEN("The client sends a request") {
      auto message = createRandomRotationMatrixStamped();
      client.request(message);
      THEN("The data received is the the identity matrix") {
        REQUIRE(message.header().id() == check_rotation_matrix_stamped.header().id());
        REQUIRE(message.header().frame() == check_rotation_matrix_stamped.header().frame());
        REQUIRE(message.header().timestamp() == check_rotation_matrix_stamped.header().timestamp());
        REQUIRE(message.matrix().r00() == check_rotation_matrix_stamped.matrix().r00());
        REQUIRE(message.matrix().r01() == check_rotation_matrix_stamped.matrix().r01());
        REQUIRE(message.matrix().r02() == check_rotation_matrix_stamped.matrix().r02());
        REQUIRE(message.matrix().r10() == check_rotation_matrix_stamped.matrix().r10());
        REQUIRE(message.matrix().r11() == check_rotation_matrix_stamped.matrix().r11());
        REQUIRE(message.matrix().r12() == check_rotation_matrix_stamped.matrix().r12());
        REQUIRE(message.matrix().r20() == check_rotation_matrix_stamped.matrix().r20());
        REQUIRE(message.matrix().r21() == check_rotation_matrix_stamped.matrix().r21());
        REQUIRE(message.matrix().r22() == check_rotation_matrix_stamped.matrix().r22());
      }
    }
  }
}

SCENARIO("Client-Server to a TransformStamped message.") {
  const auto port = generatePort();
  const auto address = kAddressPrefix + std::to_string(port);

  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::TransformStamped> client(address);
    simple::Server<simple_msgs::TransformStamped> server(address, callbackFunctionTransformStamped);

    WHEN("The client sends a request") {
      auto message = createRandomTransformStamped();
      client.request(message);
      THEN("The data received is the the identity matrix") {
        REQUIRE(message.header().id() == check_transform_stamped.header().id());
        REQUIRE(message.header().frame() == check_transform_stamped.header().frame());
        REQUIRE(message.header().timestamp() == check_transform_stamped.header().timestamp());
        REQUIRE(message.transform().point().x() == check_transform_stamped.transform().point().x());
        REQUIRE(message.transform().point().y() == check_transform_stamped.transform().point().y());
        REQUIRE(message.transform().point().z() == check_transform_stamped.transform().point().z());
        REQUIRE(message.transform().matrix().r00() == check_transform_stamped.transform().matrix().r00());
        REQUIRE(message.transform().matrix().r01() == check_transform_stamped.transform().matrix().r01());
        REQUIRE(message.transform().matrix().r02() == check_transform_stamped.transform().matrix().r02());
        REQUIRE(message.transform().matrix().r10() == check_transform_stamped.transform().matrix().r10());
        REQUIRE(message.transform().matrix().r11() == check_transform_stamped.transform().matrix().r11());
        REQUIRE(message.transform().matrix().r12() == check_transform_stamped.transform().matrix().r12());
        REQUIRE(message.transform().matrix().r20() == check_transform_stamped.transform().matrix().r20());
        REQUIRE(message.transform().matrix().r21() == check_transform_stamped.transform().matrix().r21());
        REQUIRE(message.transform().matrix().r22() == check_transform_stamped.transform().matrix().r22());
      }
    }
  }
}
