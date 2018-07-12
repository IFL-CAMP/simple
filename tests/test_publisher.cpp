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

#include "simple/publisher.hpp"
#include "simple_msgs/bool.h"

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

    WHEN("Is is move-constructed from a default constructed Publish") {
      simple::Publisher<simple_msgs::Bool> publisher{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Publisher<simple_msgs::Bool>{std::move(publisher)}); }
    }

    // Copy assignment.
    WHEN("It is move-assigned") {
      simple::Publisher<simple_msgs::Bool> copy_publisher{"tcp://*:6668"};
      simple::Publisher<simple_msgs::Bool> publisher = std::move(copy_publisher);
      THEN("It correctly sends messages.") {
        auto result = publisher.publish(dummy);
        REQUIRE(result != -1);
      }
      THEN("The moved one is not able to send messages") {
        auto result = copy_publisher.publish(dummy);
        REQUIRE(result == -1);
      }
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
