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

#include "simple/publisher.hpp"
#include "simple/subscriber.hpp"
#include "simple_msgs/bool.hpp"

// Test: Subscriber interface.

//! A dummy callback used to build the subscribers in this test.
static int n_received_msg{0};
auto dummy_callback = [](const simple_msgs::Bool& /*unused*/) { ++n_received_msg; };

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

    // Move ctor.
    WHEN("It is move-constructed") {
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{"tcp://127.0.0.1:6667", dummy_callback};
      THEN("It is moved correctly.") {
        REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{std::move(copy_subscriber)});
      }
      THEN("It works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6667"};
        simple::Subscriber<simple_msgs::Bool> subscriber{std::move(copy_subscriber)};
        std::this_thread::sleep_for(std::chrono::seconds(2));  //! Wait a bit so that the subcriber is connected.
        n_received_msg = 0;                                    //! Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          publisher.publish(simple_msgs::Bool{false});
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        REQUIRE(n_received_msg == 10);
      }
    }

    WHEN("Is is move-constructed from a default constructed Subscriber") {
      simple::Subscriber<simple_msgs::Bool> subscriber{};
      THEN("It constructed correctly.") {
        REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{std::move(subscriber)});
      }
    }

    // Move assignment.
    WHEN("It is move-assigned") {
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{"tcp://127.0.0.1:6668", dummy_callback};
      THEN("It is moved correctly.") {
        simple::Subscriber<simple_msgs::Bool> subscriber;
        REQUIRE_NOTHROW(subscriber = std::move(copy_subscriber));
      }
      THEN("It works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6668"};
        simple::Subscriber<simple_msgs::Bool> subscriber = std::move(copy_subscriber);
        std::this_thread::sleep_for(std::chrono::seconds(2));  //! Wait a bit so that the subcriber is connected.
        n_received_msg = 0;                                    //! Reset the global variable.
        for (int i = 0; i < 10; ++i) {
          publisher.publish(simple_msgs::Bool{true});
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        REQUIRE(n_received_msg == 10);
      }
    }

    WHEN("Is is move assigned from a default constructed Subscriber") {
      simple::Subscriber<simple_msgs::Bool> subscriber{};
      THEN("It assigned correctly.") {
        simple::Subscriber<simple_msgs::Bool> another_subscriber{};
        REQUIRE_NOTHROW(another_subscriber = std::move(subscriber));
      }
    }

    // Stop.
    WHEN("A subscriber is stopped") {
      simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6669"};
      simple::Subscriber<simple_msgs::Bool> subscriber{"tcp://127.0.0.1:6669", dummy_callback};
      n_received_msg = 0;                                    //! Reset the global variable.
      std::this_thread::sleep_for(std::chrono::seconds(2));  //! Wait a bit so that the subcriber is connected.

      for (int i = 0; i < 5; ++i) {
        publisher.publish(simple_msgs::Bool{false});
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      REQUIRE(n_received_msg == 5);

      subscriber.stop();  //! The subscriber is stopped, no messages should be received now.
      for (int i = 0; i < 5; ++i) {
        publisher.publish(simple_msgs::Bool{true});
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      REQUIRE(n_received_msg == 5);

      subscriber = simple::Subscriber<simple_msgs::Bool>{"tcp://127.0.0.1:6669", dummy_callback};
      std::this_thread::sleep_for(std::chrono::seconds(2));  //! Wait a bit so that the subcriber is connected.

      for (int i = 0; i < 10; ++i) {
        publisher.publish(simple_msgs::Bool{false});
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
