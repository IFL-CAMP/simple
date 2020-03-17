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
#include "simple_msgs/bool.pb.h"
#include "test_utilities.hpp"

// Test: Subscriber interface.

using namespace simple_tests;

//! A dummy callback used to build the subscribers in this test.
static int number_received_messages{0};
auto dummyCallback = [](const simple_msgs::Bool& /*unused*/) { ++number_received_messages; };

SCENARIO("SIMPLE Subscriber interface") {
  simple_msgs::Bool false_boolean;
  false_boolean.set_value(false);

  GIVEN("A SIMPLE Subscriber object") {
    // Default ctor.
    WHEN("It is default constructed.") {
      simple::Subscriber<simple_msgs::Bool> subscriber;
      THEN("Its endpoint string is empty") { REQUIRE(subscriber.endpoint() == ""); }
    }

    // Parameter ctor.
    WHEN("It is constructed passing valid parameters.") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Subscriber<simple_msgs::Bool> subscriber{address, dummyCallback};
      THEN("Its endpoint string is correct") { REQUIRE(subscriber.endpoint() == address); }
    }

    // Move ctor.
    WHEN("It is move-constructed") {
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{address, dummyCallback};
      THEN("It is moved correctly.") {
        REQUIRE_NOTHROW(simple::Subscriber<simple_msgs::Bool>{std::move(copy_subscriber)});
      }
      THEN("It works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{address};
        simple::Subscriber<simple_msgs::Bool> subscriber{std::move(copy_subscriber)};
        std::this_thread::sleep_for(
            std::chrono::seconds(kWaitTimeForSubscribers));  //! Wait a bit so that the subcriber is connected.
        number_received_messages = 0;                        //! Reset the global variable.
        for (int i = 0; i < kTestMessagesToSend; ++i) {
          publisher.publish(false_boolean);
          std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
        }
        REQUIRE(number_received_messages == kTestMessagesToSend);
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
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Subscriber<simple_msgs::Bool> copy_subscriber{address, dummyCallback};
      THEN("It is moved correctly.") {
        simple::Subscriber<simple_msgs::Bool> subscriber;
        REQUIRE_NOTHROW(subscriber = std::move(copy_subscriber));
      }
      THEN("It works properly.") {
        simple::Publisher<simple_msgs::Bool> publisher{address};
        simple::Subscriber<simple_msgs::Bool> subscriber = std::move(copy_subscriber);
        std::this_thread::sleep_for(
            std::chrono::seconds(kWaitTimeForSubscribers));  //! Wait a bit so that the subcriber is connected.
        number_received_messages = 0;                        //! Reset the global variable.
        for (int i = 0; i < kTestMessagesToSend; ++i) {
          publisher.publish(false_boolean);
          std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
        }
        REQUIRE(number_received_messages == kTestMessagesToSend);
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
      const auto address = kAddressPrefix + std::to_string(generatePort());
      simple::Publisher<simple_msgs::Bool> publisher{address};
      simple::Subscriber<simple_msgs::Bool> subscriber{address, dummyCallback};
      number_received_messages = 0;  //! Reset the global variable.
      std::this_thread::sleep_for(
          std::chrono::seconds(kWaitTimeForSubscribers));  //! Wait a bit so that the subcriber is connected.

      for (int i = 0; i < kTestMessagesToSend; ++i) {
        publisher.publish(false_boolean);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == kTestMessagesToSend);

      subscriber.stop();  //! The subscriber is stopped, no messages should be received now.
      const auto messages_so_far = number_received_messages;
      for (int i = 0; i < kTestMessagesToSend; ++i) {
        publisher.publish(false_boolean);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == messages_so_far);

      subscriber = simple::Subscriber<simple_msgs::Bool>{address, dummyCallback};
      std::this_thread::sleep_for(
          std::chrono::seconds(kWaitTimeForSubscribers));  //! Wait a bit so that the subcriber is connected.

      for (int i = 0; i < kTestMessagesToSend; ++i) {
        publisher.publish(false_boolean);
        std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeBetweenMessagesMilliseconds));
      }
      REQUIRE(number_received_messages == messages_so_far + kTestMessagesToSend);
    }

    // Failure case.
    WHEN("It is constructed passing an invalid address.") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(simple::Subscriber<simple_msgs::Bool>(kInvalidAddress, dummyCallback), std::runtime_error);
      }
    }
  }
}
