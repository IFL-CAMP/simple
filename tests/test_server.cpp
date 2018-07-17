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
#include "simple_msgs/point.h"

// Test: Subscriber interface.

//< A dummy callback used to build the subscribers in this test.
int n_received_requests{0};
auto dummy_callback = [](simple_msgs::Point& /*unused*/) { ++n_received_requests; };
simple_msgs::Point dummy{0, 0, 0};

SCENARIO("SIMPLE Server interface") {
  GIVEN("A SIMPLE Server object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{}); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing valid parameters.") {
      THEN("It constructed correctly.") {
        REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{"tcp://*:6666", dummy_callback});
      }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      simple::Server<simple_msgs::Point> copy_server{"tcp://*:6667", dummy_callback};
      simple::Server<simple_msgs::Point> another_server{"tcp://*:6671", dummy_callback};
      THEN("It is moved correctly.") { REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{std::move(copy_server)}); }
      THEN("It works properly.") {
        simple::Client<simple_msgs::Point> client{"tcp://localhost:6671"};
        simple::Server<simple_msgs::Point> server{std::move(another_server)};
        std::this_thread::sleep_for(std::chrono::seconds(2));
        n_received_requests = 0;  //< Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          client.request(dummy);
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        REQUIRE(n_received_requests == 10);
      }
    }

    WHEN("Is is move-constructed from a default constructed Subscriber") {
      simple::Server<simple_msgs::Point> server{};
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Server<simple_msgs::Point>{std::move(server)}); }
    }

    // Move assignment.
    WHEN("It is move-assigned") {
      std::this_thread::sleep_for(std::chrono::seconds(2));
      simple::Server<simple_msgs::Point> copy_server{"tcp://*:6668", dummy_callback};
      simple::Server<simple_msgs::Point> another_server{"tcp://*:6670", dummy_callback};
      THEN("It is moved correctly.") {
        simple::Server<simple_msgs::Point> server;
        REQUIRE_NOTHROW(server = std::move(copy_server));
      }
      THEN("It works properly.") {
        simple::Client<simple_msgs::Point> client{"tcp://localhost:6670"};
        simple::Server<simple_msgs::Point> server = std::move(another_server);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        n_received_requests = 0;  //< Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          client.request(dummy);
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        REQUIRE(n_received_requests == 10);
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
    //    WHEN("A server is stopped") {
    //      std::this_thread::sleep_for(std::chrono::seconds(2));
    //      simple::Client<simple_msgs::Point> client{"tcp://localhost:6669", 10};
    //      simple::Server<simple_msgs::Point> server{"tcp://*:6669", dummy_callback, 100};
    //      n_received_requests = 0;  //< Reset the global variable.
    //      std::this_thread::sleep_for(std::chrono::seconds(2));

    //      for (int i = 0; i < 5; ++i) {
    //        client.request(dummy);
    //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //      }
    //      REQUIRE(n_received_requests == 5);

    //      server.stop();  //< The subscriber is stopped, no messages should be received now.
    //      for (int i = 0; i < 5; ++i) {
    //        client.request(dummy);
    //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //      }
    //      REQUIRE(n_received_requests == 5);
    //      server = simple::Server<simple_msgs::Point>{"tcp://*:6669", dummy_callback, 10};

    //      for (int i = 0; i < 10; ++i) {
    //        client.request(dummy);
    //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //      }
    //      REQUIRE(n_received_requests == 15);
    //    }

    // Failure case.
    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Server<simple_msgs::Point>("invalid_address", dummy_callback), std::runtime_error);
      }
    }
  }
}
