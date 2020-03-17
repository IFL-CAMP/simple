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
#include "simple_msgs/bool.pb.h"
#include "test_utilities.hpp"

// Test: Publisher interface

using namespace simple_tests;

SCENARIO("SIMPLE Publisher interface") {
  GIVEN("A SIMPLE Publisher object") {
    // Dummy simple_msgs to test the publish() method.
    simple_msgs::Bool dummy;
    dummy.set_value(true);

    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Publisher<simple_msgs::Bool> publisher;
      THEN("It is not able to send messages.") { REQUIRE(publisher.publish(dummy) == false); }
      THEN("Its endpoint string is empty") { REQUIRE(publisher.endpoint() == ""); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing an valid address to bind to.") {
      auto const address = kAddressPrefix + std::to_string(generatePort());
      simple::Publisher<simple_msgs::Bool> publisher{address};
      THEN("It correctly sends messages.") { REQUIRE(publisher.publish(dummy) == true); }
      THEN("Its endpoint string is correct") { REQUIRE(publisher.endpoint() == address); }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      simple::Publisher<simple_msgs::Bool> copy_publisher{kAddressPrefix + std::to_string(generatePort())};
      simple::Publisher<simple_msgs::Bool> publisher{std::move(copy_publisher)};
      THEN("It correctly sends messages.") { REQUIRE(publisher.publish(dummy) == true); }
      THEN("The moved one is not able to send messages") { REQUIRE(copy_publisher.publish(dummy) == false); }
    }

    WHEN("Is is move-constructed from a default constructed Publisher") {
      simple::Publisher<simple_msgs::Bool> publisher;
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Publisher<simple_msgs::Bool>{std::move(publisher)}); }
    }

    // Move assignment.
    WHEN("It is move-assigned") {
      simple::Publisher<simple_msgs::Bool> copy_publisher{kAddressPrefix + std::to_string(generatePort())};
      simple::Publisher<simple_msgs::Bool> publisher;
      publisher = std::move(copy_publisher);
      THEN("It correctly sends messages.") { REQUIRE(publisher.publish(dummy) == true); }
      THEN("The moved one is not able to send messages") { REQUIRE(copy_publisher.publish(dummy) == false); }
    }

    WHEN("Is is move-assigned from a default constructed Publisher") {
      simple::Publisher<simple_msgs::Bool> copy_publisher;
      simple::Publisher<simple_msgs::Bool> publisher;
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(publisher = std::move(copy_publisher)); }
    }

    // Failure case.
    WHEN("It is constructed passing an address which is already in use.") {
      const auto port = std::to_string(generatePort());
      simple::Publisher<simple_msgs::Bool> copy_publisher{kAddressPrefix + port};
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Publisher<simple_msgs::Bool>(kAddressPrefix + port), std::runtime_error);
      }
    }

    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Publisher<simple_msgs::Bool>(kInvalidAddress), std::runtime_error);
      }
    }
  }
}
