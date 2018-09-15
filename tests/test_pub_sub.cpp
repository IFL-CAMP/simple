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

// Test: Publish and Subscribe to all data types.

static constexpr size_t TEST_MESSAGES_TO_SEND = 10;
static constexpr size_t WAIT_TIME_FOR_SUBSCRIBER = 2;  //! In seconds.
static constexpr size_t TIME_BETWEEN_MESSAGES = 10;    //! In milliseconds.

SCENARIO("Publish and subscribe to a Bool message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Bool> pub{publisher_address};
    simple::Subscriber<simple_msgs::Bool> sub{subscriber_address, callbackFunctionConstBool};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomBool();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));
        if (active_callback[MessageType::Bool]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_bool); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Bool] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Int message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Int> pub{publisher_address};
    simple::Subscriber<simple_msgs::Int> sub{subscriber_address, callbackFunctionConstInt};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomInt();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Int]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_int); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Int] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Float message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Float> pub{publisher_address};
    simple::Subscriber<simple_msgs::Float> sub{subscriber_address, callbackFunctionConstFloat};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomFloat();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Float]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_float); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Float] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Double message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Double> pub{publisher_address};
    simple::Subscriber<simple_msgs::Double> sub{subscriber_address, callbackFunctionConstDouble};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomDouble();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Double]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_double); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Double] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a String message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::String> pub{publisher_address};
    simple::Subscriber<simple_msgs::String> sub{subscriber_address, callbackFunctionConstString};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomString();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::String]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_string); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::String] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Header message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Header> pub{publisher_address};
    simple::Subscriber<simple_msgs::Header> sub{subscriber_address, callbackFunctionConstHeader};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomHeader();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Header]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_header); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Header] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Point message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Point> pub{publisher_address};
    simple::Subscriber<simple_msgs::Point> sub{subscriber_address, callbackFunctionConstPoint};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomPoint();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Point]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_point); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Point] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Quaternion message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Quaternion> pub{publisher_address};
    simple::Subscriber<simple_msgs::Quaternion> sub{subscriber_address, callbackFunctionConstQuaternion};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomQuaternion();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Quaternion]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_quaternion); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Quaternion] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Pose message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Pose> pub{publisher_address};
    simple::Subscriber<simple_msgs::Pose> sub{subscriber_address, callbackFunctionConstPose};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomPose();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Pose]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_pose); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Pose] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Rotation Matrix message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrix> pub{publisher_address};
    simple::Subscriber<simple_msgs::RotationMatrix> sub{subscriber_address, callbackFunctionConstRotationMatrix};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomRotationMatrix();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::RotationMatrix]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_rotation_matrix); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::RotationMatrix] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a Transform message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Transform> pub{publisher_address};
    simple::Subscriber<simple_msgs::Transform> sub{subscriber_address, callbackFunctionConstTransform};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomTransform();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::Transform]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_transform); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::Transform] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a PointStamped message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PointStamped> pub{publisher_address};
    simple::Subscriber<simple_msgs::PointStamped> sub{subscriber_address, callbackFunctionConstPointStamped};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomPointStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::PointStamped]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_point_stamped); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::PointStamped] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a QuaternionStamped message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::QuaternionStamped> pub{publisher_address};
    simple::Subscriber<simple_msgs::QuaternionStamped> sub{subscriber_address, callbackFunctionConstQuaternionStamped};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomQuaternionStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::QuaternionStamped]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_quaternion_stamped); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::QuaternionStamped] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a PoseStamped message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PoseStamped> pub{publisher_address};
    simple::Subscriber<simple_msgs::PoseStamped> sub{subscriber_address, callbackFunctionConstPoseStamped};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomPoseStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::PoseStamped]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_pose_stamped); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::PoseStamped] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a RotationMatrixStamped message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrixStamped> pub{publisher_address};
    simple::Subscriber<simple_msgs::RotationMatrixStamped> sub{subscriber_address,
                                                               callbackFunctionConstRotationMatrixStamped};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomRotationMatrixStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::RotationMatrixStamped]) {
          THEN("The data received is the same as the one sent") {
            REQUIRE(message == received_rotation_matrix_stamped);
          }
        }
      }
    }
    REQUIRE(received_messages[MessageType::RotationMatrixStamped] == TEST_MESSAGES_TO_SEND);
  }
}

SCENARIO("Publish and subscribe to a TransformStamped message.") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::TransformStamped> pub{publisher_address};
    simple::Subscriber<simple_msgs::TransformStamped> sub{subscriber_address, callbackFunctionConstTransformStamped};
    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomTransformStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));

        if (active_callback[MessageType::TransformStamped]) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_transform_stamped); }
        }
      }
    }
    REQUIRE(received_messages[MessageType::TransformStamped] == TEST_MESSAGES_TO_SEND);
  }
}

// Connecting a subscriber to a wrong type of publisher.
SCENARIO("Publish a Pose and subscribe to a Point message") {
  const auto port = generatePort();
  const auto publisher_address = "tcp://*:" + std::to_string(port);
  const auto subscriber_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a subscriber") {
    size_t wrong_received_messages{0};  // Reset this variable;
    simple::Publisher<simple_msgs::Pose> pub{publisher_address};
    simple::Subscriber<simple_msgs::Point> sub{subscriber_address, callbackFunctionConstPoint};

    std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME_FOR_SUBSCRIBER));
    WHEN("A publisher publishes data") {
      for (size_t i = 0; i < TEST_MESSAGES_TO_SEND; ++i) {
        auto message = createRandomPose();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MESSAGES));
      }
    }
    REQUIRE(wrong_received_messages == 0);
  }
}
