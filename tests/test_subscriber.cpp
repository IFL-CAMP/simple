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
#include "simple/subscriber.hpp"
#include "simple_msgs/bool.h"
#include "test_utils.hpp"

// Test: Subscriber interface.

//< A dummy callback used to build the subscribers in this test.
int n_received_msg{0};
auto dummy_callback = [](const simple_msgs::Bool& /*unused*/) {
  ++n_received_msg;
  std::cout << "I Received a Messages! N = " << std::to_string(n_received_msg) << std::endl;
};

SCENARIO("SIMPLE Subscriber interface") {
  GIVEN("A SIMPLE Subscriber object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      THEN("It constructed correctly.") { REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{}); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing valid parameters.") {
      THEN("It constructed correctly.") {
        REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{"tcp://127.0.0.1:6666", dummy_callback});
      }
    }

    // Copy ctor.
    WHEN("It is copy-constructed") {
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{"tcp://127.0.0.1:6667", dummy_callback};
      THEN("It constructed correctly.") {
        REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{std::move(copy_subscriber)});
      }
      THEN("The copy works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6667"};
        simple::Subscriber<simple_msgs::Bool> subscriber{std::move(copy_subscriber)};
        std::this_thread::sleep_for(std::chrono::seconds(2));  //< Wait a bit so that the subcriber is connected.
        n_received_msg = 0;
        std::cout << "Sending..." << std::endl;
        //< Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          publisher.publish(createRandomBool());
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "I should have received 10 messages, N = " << std::to_string(n_received_msg) << std::endl;

        REQUIRE(n_received_msg == 10);
      }
    }

    // Copy assignment.
    WHEN("It is copy-constructed") {
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{"tcp://127.0.0.1:6668", dummy_callback};
      THEN("It constructed correctly.") {
        simple::Subscriber<simple_msgs::Bool> subscriber;
        REQUIRE_NOTHROW(subscriber = std::move(copy_subscriber));
      }
      THEN("The copy works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6668"};
        simple::Subscriber<simple_msgs::Bool> subscriber = std::move(copy_subscriber);
        std::this_thread::sleep_for(std::chrono::seconds(2));  //< Wait a bit so that the subcriber is connected.
        n_received_msg = 0;
        std::cout << "Sending..." << std::endl;
        //< Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          publisher.publish(createRandomBool());
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "I should have received 10 messages, N = " << std::to_string(n_received_msg) << std::endl;

        REQUIRE(n_received_msg == 10);
      }
    }

    // Stop.
    WHEN("A subscriber is stopped") {
      simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6669"};
      simple::Subscriber<simple_msgs::Bool> subscriber{"tcp://127.0.0.1:6669", dummy_callback};
      n_received_msg = 0;                                    //< Reset the global variable.
      std::this_thread::sleep_for(std::chrono::seconds(2));  //< Wait a bit so that the subcriber is connected.

      std::cout << "Sending..." << std::endl;
      for (int i = 0; i < 5; ++i) {
        publisher.publish(createRandomBool());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      std::cout << "I should have received 5 messages, N = " << std::to_string(n_received_msg) << std::endl;
      REQUIRE(n_received_msg == 5);

      subscriber.stop();
      std::cout << "Sending..." << std::endl;
      for (int i = 0; i < 5; ++i) {
        publisher.publish(createRandomBool());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      std::cout << "I should have STILL received 5 messages, N = " << std::to_string(n_received_msg) << std::endl;
      REQUIRE(n_received_msg == 5);

      subscriber = simple::Subscriber<simple_msgs::Bool>{"tcp://127.0.0.1:6669", dummy_callback};
      std::this_thread::sleep_for(std::chrono::seconds(2));  //< Wait a bit so that the subcriber is connected.

      std::cout << "Sending 10 messages..." << std::endl;
      std::cout << "So far I received " << std::to_string(n_received_msg) << " messages.." << std::endl;
      for (int i = 0; i < 10; ++i) {
        publisher.publish(createRandomBool());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      REQUIRE(n_received_msg == 15);
    }

    // Failure case.
    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Subscriber<simple_msgs::Bool>("invalid_address", dummy_callback), std::runtime_error);
      }
    }
  }
}
