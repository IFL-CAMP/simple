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

#include <chrono>

#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "test_utilities.hpp"

using namespace simple_tests;

// Test: Publish and Subscribe to all data types.

SCENARIO("Publish and subscribe to a Bool message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Bool> pub{address};
    simple::Subscriber<simple_msgs::Bool> sub{address, callbackFunctionConstBool};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomBool();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
        if (active_callback[MessageType::Bool]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message.value() == check_bool.value()); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Bool] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a String message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::String> pub{address};
    simple::Subscriber<simple_msgs::String> sub{address, callbackFunctionConstString};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomString();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::String]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message.value() == check_string.value()); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::String] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Header message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Header> pub{address};
    simple::Subscriber<simple_msgs::Header> sub{address, callbackFunctionConstHeader};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomHeader();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::Header]) {
          THEN("The data received is the same as the one sent") {
            REQUIRE(message.id() == check_header.id());
            REQUIRE(message.frame() == check_header.frame());
            REQUIRE(message.timestamp() == check_header.timestamp());
          }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Header] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Point message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Point> pub{address};
    simple::Subscriber<simple_msgs::Point> sub{address, callbackFunctionConstPoint};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomPoint();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::Point]) {
          THEN("The data received is the same as the one sent") {
            REQUIRE(message.x() == check_point.x());
            REQUIRE(message.y() == check_point.y());
            REQUIRE(message.z() == check_point.z());
          }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Point] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Quaternion message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Quaternion> pub{address};
    simple::Subscriber<simple_msgs::Quaternion> sub{address, callbackFunctionConstQuaternion};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomQuaternion();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::Quaternion]) {
          THEN("The data received is the same as the one sent") {
            REQUIRE(message.x() == check_quaternion.x());
            REQUIRE(message.y() == check_quaternion.y());
            REQUIRE(message.z() == check_quaternion.z());
            REQUIRE(message.w() == check_quaternion.w());
          }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Quaternion] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Pose message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Pose> pub{address};
    simple::Subscriber<simple_msgs::Pose> sub{address, callbackFunctionConstPose};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomPose();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::Pose]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::Pose] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Rotation Matrix message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrix> pub{address};
    simple::Subscriber<simple_msgs::RotationMatrix> sub{address, callbackFunctionConstRotationMatrix};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomRotationMatrix();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::RotationMatrix]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::RotationMatrix] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a Transform message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Transform> pub{address};
    simple::Subscriber<simple_msgs::Transform> sub{address, callbackFunctionConstTransform};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomTransform();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::Transform]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::Transform] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a PointStamped message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PointStamped> pub{address};
    simple::Subscriber<simple_msgs::PointStamped> sub{address, callbackFunctionConstPointStamped};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomPointStamped();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::PointStamped]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::PointStamped] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a QuaternionStamped message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::QuaternionStamped> pub{address};
    simple::Subscriber<simple_msgs::QuaternionStamped> sub{address, callbackFunctionConstQuaternionStamped};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomQuaternionStamped();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::QuaternionStamped]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::QuaternionStamped] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a PoseStamped message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PoseStamped> pub{address};
    simple::Subscriber<simple_msgs::PoseStamped> sub{address, callbackFunctionConstPoseStamped};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomPoseStamped();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::PoseStamped]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::PoseStamped] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a RotationMatrixStamped message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrixStamped> pub{address};
    simple::Subscriber<simple_msgs::RotationMatrixStamped> sub{address, callbackFunctionConstRotationMatrixStamped};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomRotationMatrixStamped();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::RotationMatrixStamped]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::RotationMatrixStamped] == kTestMessagesToSend);
  }
}

SCENARIO("Publish and subscribe to a TransformStamped message.") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::TransformStamped> pub{address};
    simple::Subscriber<simple_msgs::TransformStamped> sub{address, callbackFunctionConstTransformStamped};
    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomTransformStamped();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));

        if (active_callback[MessageType::TransformStamped]) {
          THEN("The data received is the same as the one sent") {
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
    REQUIRE(received_messages[MessageType::TransformStamped] == kTestMessagesToSend);
  }
}

// Connecting a subscriber to a wrong type of publisher.
SCENARIO("Publish a Pose and subscribe to a Point message") {
  const auto address = kAddressPrefix + std::to_string(generatePort());

  GIVEN("An instance of a subscriber") {
    size_t wrong_received_messages{0};  // Reset this variable;
    simple::Publisher<simple_msgs::Pose> pub{address};
    simple::Subscriber<simple_msgs::Point> sub{address, callbackFunctionConstPoint};

    std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForSubscribers));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < kTestMessagesToSend; ++i) {
        auto message = createRandomPose();
        REQUIRE(pub.publish(message) == true);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
    }
    REQUIRE(wrong_received_messages == 0);
  }
}
