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
  GIVEN("A Client to a Point") {
    simple::Client<simple_msgs::Point, simple_msgs::Point> client("tcp://localhost:" + std::to_string(generatePort()),
                                                                  3000, 3000);
    WHEN("The client sends a request") {
      auto request = createRandomPoint();
      auto reply = request;
      THEN("The timeout is reached and no reply is received") { REQUIRE(client.request(request, reply) == false); }
    }
  }
}

SCENARIO("Two Servers binding to the same address") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  GIVEN("A server") {
    simple::Server<simple_msgs::Point, simple_msgs::Point> server(server_address, callbackFunctionPoint);
    WHEN("Another server tries to bind to the same address") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS(simple::Server<simple_msgs::Point, simple_msgs::Point>(server_address, callbackFunctionPoint));
      }
    }
  }
}

SCENARIO("A Client connecting to a false address") {
  const auto port = generatePort();
  const auto client_address = "tcp://*:" + std::to_string(port);
  GIVEN("A client") {
    WHEN("The address provided is wrong") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS(simple::Client<simple_msgs::Point, simple_msgs::Point>(client_address));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning Bool messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Bool, simple_msgs::Bool> client(client_address);
    simple::Server<simple_msgs::Bool, simple_msgs::Bool> server(server_address, callbackFunctionBool);

    WHEN("The client sends a request") {
      auto message = createRandomBool();
      auto sentMessage = message;
      client.request(message, message);

      THEN("The data received is true") { REQUIRE(message == !sentMessage); }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Int messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Int, simple_msgs::Int> client(client_address);
    simple::Server<simple_msgs::Int, simple_msgs::Int> server(server_address, callbackFunctionInt);

    WHEN("The client sends a request") {
      auto i = createRandomInt();
      auto sentInt = i;
      client.request(i, i);
      THEN("The data received is the same as the one sent plus 1") { REQUIRE(i == sentInt.get() + 1); }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Float messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Float, simple_msgs::Float> client(client_address);
    simple::Server<simple_msgs::Float, simple_msgs::Float> server(server_address, callbackFunctionFloat);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("The client sends a request") {
      auto f = createRandomFloat();
      auto sentFloat = f;
      client.request(f, f);

      THEN("The data received is the same as the one sent plus 1") {
        REQUIRE(f.get() == Approx(sentFloat.get() + 1.0f));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Double messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Double, simple_msgs::Double> client(client_address);
    simple::Server<simple_msgs::Double, simple_msgs::Double> server(server_address, callbackFunctionDouble);

    WHEN("The client sends a request") {
      auto d = createRandomDouble();
      auto sentDouble = d;
      client.request(d, d);
      THEN("The data received is the same as the one sent plus 1") {
        REQUIRE(d.get() == Approx(sentDouble.get() + 1.0));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a String messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::String, simple_msgs::String> client(client_address);
    simple::Server<simple_msgs::String, simple_msgs::String> server(server_address, callbackFunctionString);

    WHEN("The client sends a request") {
      auto s = createRandomString();
      client.request(s, s);
      THEN("The string received back is the default answer") { REQUIRE(s == "REPLY"); }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Header messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Header, simple_msgs::Header> client(client_address);
    simple::Server<simple_msgs::Header, simple_msgs::Header> server(server_address, callbackFunctionHeader);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    WHEN("The client sends a request") {
      auto h = createRandomHeader();
      client.request(h, h);
      THEN("The data received is filled wth the standard values") {
        REQUIRE(h.getFrameID() == "ID");
        REQUIRE(h.getSequenceNumber() == 1);
        REQUIRE(h.getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Point messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Point, simple_msgs::Point> client(client_address);
    simple::Server<simple_msgs::Point, simple_msgs::Point> server(server_address, callbackFunctionPoint);

    WHEN("The client sends a request") {
      auto p = createRandomPoint();
      auto sentPoint = p;
      client.request(p, p);

      THEN("The data received is the equal to the sent point plus one") {
        REQUIRE(p.getX() == Approx(sentPoint.getX() + 1.0));
        REQUIRE(p.getY() == Approx(sentPoint.getY() + 1.0));
        REQUIRE(p.getZ() == Approx(sentPoint.getZ() + 1.0));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Quaternion messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Quaternion, simple_msgs::Quaternion> client(client_address);
    simple::Server<simple_msgs::Quaternion, simple_msgs::Quaternion> server(server_address, callbackFunctionQuaternion);

    WHEN("The client sends a request") {
      auto q = createRandomQuaternion();
      auto sentQuaternion = q;
      client.request(q, q);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(q.getW() == Approx(sentQuaternion.getW() + 1.0));
        REQUIRE(q.getX() == Approx(sentQuaternion.getX() + 1.0));
        REQUIRE(q.getY() == Approx(sentQuaternion.getY() + 1.0));
        REQUIRE(q.getZ() == Approx(sentQuaternion.getZ() + 1.0));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Pose messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::Pose, simple_msgs::Pose> client(client_address);
    simple::Server<simple_msgs::Pose, simple_msgs::Pose> server(server_address, callbackFunctionPose);

    WHEN("The client sends a request") {
      auto p = createRandomPose();
      auto sentPose = p;
      client.request(p, p);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(p.getPosition().getX() == Approx(sentPose.getPosition().getX() + 1.0));
        REQUIRE(p.getPosition().getY() == Approx(sentPose.getPosition().getY() + 1.0));
        REQUIRE(p.getPosition().getZ() == Approx(sentPose.getPosition().getZ() + 1.0));
        REQUIRE(p.getQuaternion().getW() == Approx(sentPose.getQuaternion().getW() + 1.0));
        REQUIRE(p.getQuaternion().getX() == Approx(sentPose.getQuaternion().getX() + 1.0));
        REQUIRE(p.getQuaternion().getY() == Approx(sentPose.getQuaternion().getY() + 1.0));
        REQUIRE(p.getQuaternion().getZ() == Approx(sentPose.getQuaternion().getZ() + 1.0));
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Rotation Matrix messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server.
    simple::Client<simple_msgs::RotationMatrix, simple_msgs::RotationMatrix> client(client_address);
    simple::Server<simple_msgs::RotationMatrix, simple_msgs::RotationMatrix> server(server_address,
                                                                                    callbackFunctionRotationMatrix);

    WHEN("The client sends a request") {
      auto r = createRandomRotationMatrix();
      client.request(r, r);
      THEN("The data received is the the identity matrix") { REQUIRE(r == simple_msgs::RotationMatrix()); }
    }
  }
}

SCENARIO("Client-Server accepting and returning a Transform messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::Transform, simple_msgs::Transform> client(client_address);
    simple::Server<simple_msgs::Transform, simple_msgs::Transform> server(server_address, callbackFunctionTransform);
    WHEN("The client sends a request") {
      auto t = createRandomTransform();
      client.request(t, t);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(t.getTranslation() == simple_msgs::Point{1, 1, 1});
        REQUIRE(t.getRotation() == simple_msgs::RotationMatrix{1, 1, 1, 1, 1, 1, 1, 1, 1});
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a PointStamped messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::PointStamped, simple_msgs::PointStamped> client(client_address);
    simple::Server<simple_msgs::PointStamped, simple_msgs::PointStamped> server(server_address,
                                                                                callbackFunctionPointStamped);

    WHEN("The client sends a request") {
      auto p = createRandomPointStamped();
      auto sentPoint = p;
      client.request(p, p);
      simple_msgs::Point unitPoint(1, 1, 1);

      THEN("The data received is the equal to the sent point plus one's") {
        REQUIRE(p.getPoint().getX() == Approx(sentPoint.getPoint().getX() + 1.0));
        REQUIRE(p.getPoint().getY() == Approx(sentPoint.getPoint().getY() + 1.0));
        REQUIRE(p.getPoint().getZ() == Approx(sentPoint.getPoint().getZ() + 1.0));
        REQUIRE(p.getHeader().getFrameID() == "ID");
        REQUIRE(p.getHeader().getSequenceNumber() == 1);
        REQUIRE(p.getHeader().getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a QuaternionStamped messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::QuaternionStamped, simple_msgs::QuaternionStamped> client(client_address);
    simple::Server<simple_msgs::QuaternionStamped, simple_msgs::QuaternionStamped> server(
        server_address, callbackFunctionQuaternionStamped);

    WHEN("The client sends a request") {
      auto q = createRandomQuaternionStamped();
      auto sentQuaternion = q;
      client.request(q, q);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(q.getQuaternion().getW() == Approx(sentQuaternion.getQuaternion().getW() + 1.0));
        REQUIRE(q.getQuaternion().getX() == Approx(sentQuaternion.getQuaternion().getX() + 1.0));
        REQUIRE(q.getQuaternion().getY() == Approx(sentQuaternion.getQuaternion().getY() + 1.0));
        REQUIRE(q.getQuaternion().getZ() == Approx(sentQuaternion.getQuaternion().getZ() + 1.0));
        REQUIRE(q.getHeader().getFrameID() == "ID");
        REQUIRE(q.getHeader().getSequenceNumber() == 1);
        REQUIRE(q.getHeader().getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a PoseStamped messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server
    simple::Client<simple_msgs::PoseStamped, simple_msgs::PoseStamped> client(client_address);
    simple::Server<simple_msgs::PoseStamped, simple_msgs::PoseStamped> server(server_address,
                                                                              callbackFunctionPoseStamped);

    WHEN("The client sends a request") {
      auto p = createRandomPoseStamped();
      auto sentPose = p;
      client.request(p, p);
      simple_msgs::Point unitPoint(1, 1, 1);
      THEN("The data received is the equal to the sent pose plus one's") {
        REQUIRE(p.getPose().getPosition().getX() == Approx(sentPose.getPose().getPosition().getX() + 1.0));
        REQUIRE(p.getPose().getPosition().getY() == Approx(sentPose.getPose().getPosition().getY() + 1.0));
        REQUIRE(p.getPose().getPosition().getZ() == Approx(sentPose.getPose().getPosition().getZ() + 1.0));
        REQUIRE(p.getPose().getQuaternion().getW() == Approx(sentPose.getPose().getQuaternion().getW() + 1.0));
        REQUIRE(p.getPose().getQuaternion().getX() == Approx(sentPose.getPose().getQuaternion().getX() + 1.0));
        REQUIRE(p.getPose().getQuaternion().getY() == Approx(sentPose.getPose().getQuaternion().getY() + 1.0));
        REQUIRE(p.getPose().getQuaternion().getZ() == Approx(sentPose.getPose().getQuaternion().getZ() + 1.0));
        REQUIRE(p.getHeader().getFrameID() == "ID");
        REQUIRE(p.getHeader().getSequenceNumber() == 1);
        REQUIRE(p.getHeader().getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a RotationMatrixStamped messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    // start a server.
    simple::Client<simple_msgs::RotationMatrixStamped, simple_msgs::RotationMatrixStamped> client(client_address);
    simple::Server<simple_msgs::RotationMatrixStamped, simple_msgs::RotationMatrixStamped> server(
        server_address, callbackFunctionRotationMatrixStamped);

    WHEN("The client sends a request") {
      auto r = createRandomRotationMatrixStamped();
      client.request(r, r);
      THEN("The data received is the the identity matrix") {
        REQUIRE(r.getRotationMatrix() == simple_msgs::RotationMatrix());
        REQUIRE(r.getHeader().getFrameID() == "ID");
        REQUIRE(r.getHeader().getSequenceNumber() == 1);
        REQUIRE(r.getHeader().getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting and returning a TransformStamped messages.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::TransformStamped, simple_msgs::TransformStamped> client(client_address);
    simple::Server<simple_msgs::TransformStamped, simple_msgs::TransformStamped> server(
        server_address, callbackFunctionTransformStamped);

    WHEN("The client sends a request") {
      auto t = createRandomTransformStamped();
      client.request(t, t);
      THEN("The data received is the the identity matrix") {
        REQUIRE(t.getTransform().getTranslation() == simple_msgs::Point{1, 1, 1});
        REQUIRE(t.getTransform().getRotation() == simple_msgs::RotationMatrix{1, 1, 1, 1, 1, 1, 1, 1, 1});
        REQUIRE(t.getHeader().getFrameID() == "ID");
        REQUIRE(t.getHeader().getSequenceNumber() == 1);
        REQUIRE(t.getHeader().getTimestamp() == 10);
      }
    }
  }
}

SCENARIO("Client-Server accepting a RotationMatrix and returning a Bool message.") {
  const auto port = generatePort();
  const auto server_address = "tcp://*:" + std::to_string(port);
  const auto client_address = "tcp://localhost:" + std::to_string(port);
  GIVEN("An instance of a server.") {
    simple::Client<simple_msgs::RotationMatrix, simple_msgs::Bool> client(client_address);
    simple::Server<simple_msgs::RotationMatrix, simple_msgs::Bool> server(server_address,
                                                                          callbackIsRotationMatrixIdentity);
    simple_msgs::Bool b;  //< Reply message.

    WHEN("The client sends an Identity matrix as request") {
      auto identity = simple_msgs::RotationMatrix::Identity();
      client.request(identity, b);
      THEN("The server returns a simple::Bool message set to true.") { REQUIRE(b == true); }
    }
    WHEN("The client sends an Identity matrix as request") {
      auto r = createRandomRotationMatrix();
      client.request(r, b);
      THEN("The server returns a simple::Bool message set to true.") { REQUIRE(b == false); }
    }
  }
}
