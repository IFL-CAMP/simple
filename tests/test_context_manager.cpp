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
