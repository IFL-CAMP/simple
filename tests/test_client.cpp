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
#include <thread>

#include "catch.hpp"

#include "simple/client.hpp"
#include "simple/server.hpp"
#include "simple_msgs/point.pb.h"
#include "test_utilities.hpp"

// Test: Client interface

static double random_value{0};
auto dummyCallback = [](simple_msgs::Point& msg) {
  msg.set_x(msg.x() + random_value);
  msg.set_y(msg.y() + random_value);
  msg.set_z(msg.z() + random_value);
};

using namespace simple_tests;

SCENARIO("SIMPLE Client interface") {
  simple_msgs::Point dummy;
  dummy.set_x(1);
  dummy.set_y(1);
  dummy.set_z(1);

  GIVEN("A SIMPLE Client object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Client<simple_msgs::Point> client;
      THEN("It is not able to send requests.") { REQUIRE(client.request(dummy) == false); }
      THEN("Its endpoint string is empty") { REQUIRE(client.endpoint() == ""); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing an valid address to bind to.") {
      auto const address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> server{address, dummyCallback};
      simple::Client<simple_msgs::Point> client{address};

      THEN("It correctly sends its request and receives a reply.") {
        const auto original_message = dummy;
        random_value = double_dist(generator);
        REQUIRE(client.request(dummy) == true);
        REQUIRE(dummy.x() == original_message.x() + random_value);
        REQUIRE(dummy.y() == original_message.y() + random_value);
        REQUIRE(dummy.z() == original_message.z() + random_value);
      }
      THEN("Its endpoint string is correct") { REQUIRE(client.endpoint() == address); }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      auto const address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> server{address, dummyCallback};
      simple::Client<simple_msgs::Point> copy_client{address};
      simple::Client<simple_msgs::Point> client{std::move(copy_client)};
      std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForServers));
      THEN("It can send a request") {
        const auto original_message = dummy;
        random_value = double_dist(generator);
        REQUIRE(client.request(dummy) == true);
        REQUIRE(dummy.x() == original_message.x() + random_value);
        REQUIRE(dummy.y() == original_message.y() + random_value);
        REQUIRE(dummy.z() == original_message.z() + random_value);
      }
      THEN("The moved one is not able to send requests.") { REQUIRE(copy_client.request(dummy) == false); }
    }

    WHEN("Is is move-constructed from a default constructed Publisher") {
      simple::Client<simple_msgs::Point> client{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Client<simple_msgs::Point>{std::move(client)}); }
    }

    // Copy assignment.
    WHEN("It is move-assigned") {
      auto const address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> server{address, dummyCallback};
      simple::Client<simple_msgs::Point> copy_client{address};
      simple::Client<simple_msgs::Point> client;
      std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForServers));
      client = std::move(copy_client);
      THEN("It correctly sends its request and receives a reply.") {
        const auto original_message = dummy;
        random_value = double_dist(generator);
        REQUIRE(client.request(dummy) == true);
        REQUIRE(dummy.x() == original_message.x() + random_value);
        REQUIRE(dummy.y() == original_message.y() + random_value);
        REQUIRE(dummy.z() == original_message.z() + random_value);
      }
      THEN("The moved one is not able to send requests.") { REQUIRE(copy_client.request(dummy) == false); }
    }

    WHEN("Is is move-assigned from a default constructed Client") {
      simple::Client<simple_msgs::Point> copy_client{};
      simple::Client<simple_msgs::Point> client{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(client = std::move(copy_client)); }
    }

    // Failure case.
    WHEN("It is constructed passing an address which is already in use.") {
      simple::Client<simple_msgs::Point> copy_client{"tcp://localhost:5558"};
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Client<simple_msgs::Point>("tcp://*:5558"), std::runtime_error);
      }
    }

    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Client<simple_msgs::Point>(kInvalidAddress), std::runtime_error);
      }
    }
  }
}
