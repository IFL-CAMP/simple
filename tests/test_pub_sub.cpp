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

#include <ctime>

#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "test_utils.hpp"

// Test: Publish and Subscribe to all data types.

SCENARIO("Publish and subscribe to a Bool message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Bool> pub("tcp://*:5555");
    simple::Subscriber<simple_msgs::Bool> sub("tcp://localhost:5555", callbackFunctionConstBool);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomBool();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_bool) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_bool); }
        }
      }
    }
    REQUIRE(num_received_bool == 10);
  }
}

SCENARIO("Publish and subscribe to a Int message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Int> pub("tcp://*:5556");
    simple::Subscriber<simple_msgs::Int> sub("tcp://localhost:5556", callbackFunctionConstInt);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomInt();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_int) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_int); }
        }
      }
    }
    REQUIRE(num_received_int == 10);
  }
}

SCENARIO("Publish and subscribe to a Float message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Float> pub("tcp://*:5557");
    simple::Subscriber<simple_msgs::Float> sub("tcp://localhost:5557", callbackFunctionConstFloat);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomFloat();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_float) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_float); }
        }
      }
    }
    REQUIRE(num_received_float == 10);
  }
}

SCENARIO("Publish and subscribe to a Double message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Double> pub("tcp://*:5558");
    simple::Subscriber<simple_msgs::Double> sub("tcp://localhost:5558", callbackFunctionConstDouble);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomDouble();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_double) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_double); }
        }
      }
    }
    REQUIRE(num_received_double == 10);
  }
}

SCENARIO("Publish and subscribe to a String message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::String> pub("tcp://*:5559");
    simple::Subscriber<simple_msgs::String> sub("tcp://localhost:5559", callbackFunctionConstString);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomString();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_string) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_string); }
        }
      }
    }
    REQUIRE(num_received_string == 10);
  }
}

SCENARIO("Publish and subscribe to a Header message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Header> pub("tcp://*:5560");
    simple::Subscriber<simple_msgs::Header> sub("tcp://localhost:5560", callbackFunctionConstHeader);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomHeader();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_header) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_header); }
        }
      }
    }
    REQUIRE(num_received_header == 10);
  }
}

SCENARIO("Publish and subscribe to a Point message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Point> pub("tcp://*:5561");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5561", callbackFunctionConstPoint);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomPoint();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_point); }
        }
      }
    }
    REQUIRE(num_received_point == 10);
  }
}

SCENARIO("Publish and subscribe to a Quaternion message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Quaternion> pub("tcp://*:5562");
    simple::Subscriber<simple_msgs::Quaternion> sub("tcp://localhost:5562", callbackFunctionConstQuaternion);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomQuaternion();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_quaternion); }
        }
      }
    }
    REQUIRE(num_received_quaternion == 10);
  }
}

SCENARIO("Publish and subscribe to a Pose message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5563");
    simple::Subscriber<simple_msgs::Pose> sub("tcp://localhost:5563", callbackFunctionConstPose);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomPose();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_pose); }
        }
      }
    }
    REQUIRE(num_received_pose == 10);
  }
}

SCENARIO("Publish and subscribe to a Rotation Matrix message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrix> pub("tcp://*:5564");
    simple::Subscriber<simple_msgs::RotationMatrix> sub("tcp://localhost:5564", callbackFunctionConstRotationMatrix);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomRotationMatrix();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_rotation_matrix); }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix == 10);
  }
}

SCENARIO("Publish and subscribe to a Transform message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::Transform> pub("tcp://*:5570");
    simple::Subscriber<simple_msgs::Transform> sub("tcp://localhost:5570", callbackFunctionConstTransform);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomTransform();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_transform) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_transform); }
        }
      }
    }
    REQUIRE(num_received_transform == 10);
  }
}

SCENARIO("Publish and subscribe to a PointStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PointStamped> pub("tcp://*:5565");
    simple::Subscriber<simple_msgs::PointStamped> sub("tcp://localhost:5565", callbackFunctionConstPointStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomPointStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_point_stamped); }
        }
      }
    }
    REQUIRE(num_received_point_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a QuaternionStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::QuaternionStamped> pub("tcp://*:5566");
    simple::Subscriber<simple_msgs::QuaternionStamped> sub("tcp://localhost:5566",
                                                           callbackFunctionConstQuaternionStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomQuaternionStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_quaternion_stamped); }
        }
      }
    }
    REQUIRE(num_received_quaternion_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a PoseStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PoseStamped> pub("tcp://*:5567");
    simple::Subscriber<simple_msgs::PoseStamped> sub("tcp://localhost:5567", callbackFunctionConstPoseStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomPoseStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_pose_stamped); }
        }
      }
    }
    REQUIRE(num_received_pose_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a RotationMatrixStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::RotationMatrixStamped> pub("tcp://*:5568");
    simple::Subscriber<simple_msgs::RotationMatrixStamped> sub("tcp://localhost:5568",
                                                               callbackFunctionConstRotationMatrixStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomRotationMatrixStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix_stamped) {
          THEN("The data received is the same as the one sent") {
            REQUIRE(message == received_rotation_matrix_stamped);
          }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix_stamped == 10);
  }
}

SCENARIO("Publish and subscribe to a TransformStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::TransformStamped> pub("tcp://*:5571");
    simple::Subscriber<simple_msgs::TransformStamped> sub("tcp://localhost:5571",
                                                          callbackFunctionConstTransformStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomTransformStamped();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_transform_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(message == received_transform_stamped); }
        }
      }
    }
    REQUIRE(num_received_transform_stamped == 10);
  }
}

// Connecting a subscriber to a wrong type of publisher.
SCENARIO("Publish a Pose and subscribe to a Point message") {
  GIVEN("An instance of a subscriber") {
    num_received_point = 0;  // Reset this variable;
    simple::Publisher<simple_msgs::Pose> pub("tcp://*:5569");
    simple::Subscriber<simple_msgs::Point> sub("tcp://localhost:5569", callbackFunctionConstPoint);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto message = createRandomPose();
        pub.publish(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
    REQUIRE(num_received_point == 0);
  }
}
