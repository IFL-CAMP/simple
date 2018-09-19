/**
 * S.I.M.P.L.E. - Smart Intuitive Messaging Platform with Less Effort
 * Copyright (C) 2018 Salvatore Virga - salvo.virga@tum.de, Fernanda Levy
 * Langsch - fernanda.langsch@tum.de
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SIMPLE_TESTS_RANDOM_GENERATORS_HPP
#define SIMPLE_TESTS_RANDOM_GENERATORS_HPP

#include <cfloat>
#include <random>

namespace simple_tests {

static std::random_device rd;
static std::minstd_rand generator(rd());
static std::uniform_int_distribution<int> int_dist(1, 1000);
static std::uniform_real_distribution<double> double_dist(1, std::nextafter(1000, DBL_MAX));
static std::bernoulli_distribution bool_dist;

}  // Namespace simple_tests.

#endif  // SIMPLE_TESTS_RANDOM_GENERATORS_HPP.
