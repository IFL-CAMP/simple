/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
        auto p = createRandomBool();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_bool) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_bool); }
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
        auto p = createRandomInt();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_int) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_int); }
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
        auto p = createRandomFloat();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_float) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_float); }
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
        auto p = createRandomDouble();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_double) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_double); }
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
        auto p = createRandomString();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_string) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_string); }
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
        auto p = createRandomHeader();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_header) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_header); }
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
        auto p = createRandomPoint();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point); }
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
        auto p = createRandomQuaternion();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_quaternion); }
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
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_pose); }
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
        auto p = createRandomRotationMatrix();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_rotation_matrix); }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix == 10);
  }
}

SCENARIO("Publish and subscribe to a PointStamped message.") {
  GIVEN("An instance of a subscriber.") {
    simple::Publisher<simple_msgs::PointStamped> pub("tcp://*:5565");
    simple::Subscriber<simple_msgs::PointStamped> sub("tcp://localhost:5565", callbackFunctionConstPointStamped);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("A publisher publishes data") {
      for (int i = 0; i < 10; ++i) {
        auto p = createRandomPointStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_point_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_point_stamped); }
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
        auto p = createRandomQuaternionStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_quaternion_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_quaternion_stamped); }
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
        auto p = createRandomPoseStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_pose_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_pose_stamped); }
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
        auto p = createRandomRotationMatrixStamped();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (running_rotation_matrix_stamped) {
          THEN("The data received is the same as the one sent") { REQUIRE(p == received_rotation_matrix_stamped); }
        }
      }
    }
    REQUIRE(num_received_rotation_matrix_stamped == 10);
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
        auto p = createRandomPose();
        pub.publish(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
    REQUIRE(num_received_point == 0);
  }
}
