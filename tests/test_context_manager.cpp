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
#include "simple_msgs/bool.hpp"

// Test: Context Manager lifetime using dynamic Linkage.

SCENARIO("SIMPLE ContextManager lifetime using dynamic linkage") {
  GIVEN("A SIMPLE object.") {
    simple::Publisher<simple_msgs::Bool> publisher{"tcp://*:6666"};
    WHEN("The application exits and the ContextManager is destroyed before the dynamic library is unloaded.") {
      THEN("The applications exits cleanly.") {}
    }
  }
  simple::ContextManager::destroy();
}
