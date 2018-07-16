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
#include "simple_msgs/point.h"

// Test: Client interface

auto dummy_callback = [](simple_msgs::Point& msg) { ++msg; };
simple_msgs::Point dummy{0, 0, 0};

SCENARIO("SIMPLE Client interface") {
  GIVEN("A SIMPLE Client object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Client<simple_msgs::Point> client;
      THEN("It is not able to send requests.") {
        auto result = client.request(dummy);
        REQUIRE(result == false);
      }
    }

    // Parameter ctor.
    WHEN("It is constructed passing an valid address to bind to.") {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      simple::Server<simple_msgs::Point> server{"tcp://*:5555", dummy_callback};
      simple::Client<simple_msgs::Point> client{"tcp://localhost:5555"};

      THEN("It correctly sends its request and receives a reply.") {
        auto result = client.request(dummy);
        REQUIRE(result == true);
        REQUIRE(dummy == simple_msgs::Point{1, 1, 1});
      }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      simple::Server<simple_msgs::Point> server{"tcp://*:5556", dummy_callback};
      simple::Client<simple_msgs::Point> copy_client{"tcp://localhost:5556"};
      simple::Client<simple_msgs::Point> client{std::move(copy_client)};
      std::this_thread::sleep_for(std::chrono::seconds(2));
      THEN("It can send a request") {
        auto result = client.request(dummy);
        REQUIRE(result == true);
        REQUIRE(dummy == simple_msgs::Point{2, 2, 2});
      }
      THEN("The moved one is not able to send requests.") {
        auto result = copy_client.request(dummy);
        REQUIRE(result == false);
      }
    }

    WHEN("Is is move-constructed from a default constructed Publisher") {
      simple::Client<simple_msgs::Point> client{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Client<simple_msgs::Point>{std::move(client)}); }
    }

    // Copy assignment.
    WHEN("It is move-assigned") {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      simple::Server<simple_msgs::Point> server{"tcp://*:5557", dummy_callback};
      simple::Client<simple_msgs::Point> copy_client{"tcp://localhost:5557"};
      simple::Client<simple_msgs::Point> client;
      std::this_thread::sleep_for(std::chrono::seconds(2));
      client = std::move(copy_client);
      THEN("It correctly sends its request and receives a reply.") {
        auto result = client.request(dummy);
        REQUIRE(result == true);
        REQUIRE(dummy == simple_msgs::Point{3, 3, 3});
      }
      THEN("The moved one is not able to send requests.") {
        auto result = copy_client.request(dummy);
        REQUIRE(result == false);
      }
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
        REQUIRE_THROWS_AS(simple::Client<simple_msgs::Point>("invalid_address"), std::runtime_error);
      }
    }
  }
}
