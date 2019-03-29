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
#include <thread>

#include "catch.hpp"

#include "simple/publisher.hpp"
#include "simple_msgs/bool.hpp"

// Test: Publisher interface

SCENARIO("SIMPLE Publisher interface") {
  GIVEN("A SIMPLE Publisher object") {
    // Dummy simple_msgs to test the publish() method.
    simple_msgs::Bool dummy;

    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Publisher<simple_msgs::Bool> publisher;
      THEN("It is not able to send messages.") {
        auto result = publisher.publish(dummy);
        REQUIRE(result == -1);
      }
    }

    // Parameter ctor.
    WHEN("It is constructed passing an valid address to bind to.") {
      simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6666"};
      THEN("It correctly sends messages.") {
        auto result = publisher.publish(dummy);
        REQUIRE(result != -1);
      }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      std::this_thread::sleep_for(std::chrono::seconds(1));  //! Wait a bit so that the subcriber is connected.
      simple::Publisher<simple_msgs::Bool> copy_publisher{"tcp://*:6667"};
      simple::Publisher<simple_msgs::Bool> publisher{std::move(copy_publisher)};
      THEN("It correctly sends messages.") {
        auto result = publisher.publish(dummy);
        REQUIRE(result != -1);
      }
      THEN("The moved one is not able to send messages") {
        auto result = copy_publisher.publish(dummy);
        REQUIRE(result == -1);
      }
    }

    WHEN("Is is move-constructed from a default constructed Publisher") {
      simple::Publisher<simple_msgs::Bool> publisher{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Publisher<simple_msgs::Bool>{std::move(publisher)}); }
    }

    // Copy assignment.
    WHEN("It is move-assigned") {
      std::this_thread::sleep_for(std::chrono::seconds(1));  //! Wait a bit so that the subcriber is connected.
      simple::Publisher<simple_msgs::Bool> copy_publisher{"tcp://*:6668"};
      simple::Publisher<simple_msgs::Bool> publisher;
      publisher = std::move(copy_publisher);
      THEN("It correctly sends messages.") {
        auto result = publisher.publish(dummy);
        REQUIRE(result != -1);
      }
      THEN("The moved one is not able to send messages") {
        auto result = copy_publisher.publish(dummy);
        REQUIRE(result == -1);
      }
    }

    WHEN("Is is move-assigned from a default constructed Publisher") {
      simple::Publisher<simple_msgs::Bool> copy_publisher{};
      simple::Publisher<simple_msgs::Bool> publisher{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(publisher = std::move(copy_publisher)); }
    }

    // Failure case.
    WHEN("It is constructed passing an address which is already in use.") {
      simple::Publisher<simple_msgs::Bool> copy_publisher{"tcp://*:6667"};
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Publisher<simple_msgs::Bool>("tcp://*:6667"), std::runtime_error);
      }
    }

    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Publisher<simple_msgs::Bool>("invalid_address"), std::runtime_error);
      }
    }
  }
}
