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

#include "simple/client.hpp"
#include "simple/server.hpp"
#include "simple_msgs/point.pb.h"
#include "test_utilities.hpp"

// Test: Subscriber interface.

using namespace simple_tests;

//! A dummy callback used to build the subscribers in this test.
int number_received_messages{0};
auto dummyCallback = [](simple_msgs::Point& /*unused*/) { ++number_received_messages; };

SCENARIO("SIMPLE Server interface") {
  simple_msgs::Point dummy;
  dummy.set_x(1);
  dummy.set_y(1);
  dummy.set_z(1);

  GIVEN("A SIMPLE Server object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Server<simple_msgs::Point> server;
      THEN("Its endpoint string is empty") { REQUIRE(server.endpoint() == ""); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing valid parameters.") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> server{address, dummyCallback};
      THEN("Its endpoint string is correct") { REQUIRE(server.endpoint() == address); }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> copy_server{address, dummyCallback};
      THEN("It is moved correctly.") { REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{std::move(copy_server)}); }
      THEN("It works properly.") {
        simple::Client<simple_msgs::Point> client{address};
        simple::Server<simple_msgs::Point> server{std::move(copy_server)};
        std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForServers));
        number_received_messages = 0;  //< Reset the global variable.
        for (int i = 0; i < kTestMessagesToSend; ++i) {
          client.request(dummy);
          std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
        }
        REQUIRE(number_received_messages == kTestMessagesToSend);
      }
    }

    WHEN("Is is move-constructed from a default constructed Subscriber") {
      simple::Server<simple_msgs::Point> server{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{std::move(server)}); }
    }

    // Move assignment.
    WHEN("It is move-assigned") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Server<simple_msgs::Point> copy_server{address, dummyCallback};
      THEN("It is moved correctly.") {
        simple::Server<simple_msgs::Point> server;
        REQUIRE_NOTHROW(server = std::move(copy_server));
      }
      THEN("It works properly.") {
        simple::Client<simple_msgs::Point> client{address};
        simple::Server<simple_msgs::Point> server = std::move(copy_server);
        std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForServers));
        number_received_messages = 0;  //< Reset the global variable.
        for (int i = 0; i < kTestMessagesToSend; ++i) {
          client.request(dummy);
          std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
        }
        REQUIRE(number_received_messages == kTestMessagesToSend);
      }
    }

    WHEN("Is is move assigned from a default constructed Subscriber") {
      simple::Server<simple_msgs::Point> server{};
      THEN("It assigned correctly.") {
        simple::Server<simple_msgs::Point> another_server{};
        REQUIRE_NOTHROW(another_server = std::move(server));
      }
    }

    // Stop.
    WHEN("A server is stopped") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Client<simple_msgs::Point> client{address, kDefaultTimeoutMilliseconds, kDefaultLingerMilliseconds};
      simple::Server<simple_msgs::Point> server{address, dummyCallback};
      number_received_messages = 0;  //< Reset the global variable.
      std::this_thread::sleep_for(std::chrono::seconds(kWaitTimeForServers));

      for (int i = 0; i < kTestMessagesToSend; ++i) {
        client.request(dummy);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == kTestMessagesToSend);

      server.stop();  //< The subscriber is stopped, no messages should be received now.
      const auto messages_so_far = number_received_messages;
      for (int i = 0; i < kTestMessagesToSend; ++i) {
        client.request(dummy);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == messages_so_far);
      server = simple::Server<simple_msgs::Point>{address, dummyCallback};
      std::this_thread::sleep_for(
          std::chrono::seconds(kWaitTimeForServers));  //! Wait a bit so that the subcriber is connected.

      for (int i = 0; i < kTestMessagesToSend; ++i) {
        client.request(dummy);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == messages_so_far + kTestMessagesToSend);
    }

    // Failure case.
    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Server<simple_msgs::Point>(kInvalidAddress, dummyCallback), std::runtime_error);
      }
    }
  }
}
