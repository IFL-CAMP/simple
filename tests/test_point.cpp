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

#include <iostream>

#include "random_generators.hpp"
#include "simple_msgs/point.hpp"

using namespace simple_tests;

// Tests for Point Message.
SCENARIO("Using a Point Message") {
  double double_1 = double_dist(generator);
  double double_2 = double_dist(generator);
  double double_3 = double_dist(generator);
  std::array<double, 3> double_array{{double_1, double_2, double_3}};
  std::array<double, 3> flipped_double_array{{double_3, double_1, double_2}};

  // Testing Constructors.
  GIVEN("An empty Point") {
    simple_msgs::Point empty_point{};
    WHEN("It is constructed") {
      THEN("The Point coordinates have to be zero") {
        REQUIRE(empty_point.getX() == Approx(0));
        REQUIRE(empty_point.getY() == Approx(0));
        REQUIRE(empty_point.getZ() == Approx(0));
      }
    }
  }

  GIVEN("A Point created from a double") {
    simple_msgs::Point single_double_point{double_1};
    WHEN("We check the Point coordinates") {
      THEN("They all have to be equal to the given value") {
        REQUIRE(single_double_point.getX() == Approx(double_1));
        REQUIRE(single_double_point.getY() == Approx(double_1));
        REQUIRE(single_double_point.getZ() == Approx(double_1));
      }
    }
  }

  GIVEN("A Point created from 3 double") {
    simple_msgs::Point three_double_point{double_1, double_2, double_3};
    WHEN("We check the Point coordinates") {
      THEN("They have to be equal to the given values") {
        REQUIRE(three_double_point.getX() == Approx(double_1));
        REQUIRE(three_double_point.getY() == Approx(double_2));
        REQUIRE(three_double_point.getZ() == Approx(double_3));
      }
    }
  }

  GIVEN("A Point created from an array of 3 double") {
    simple_msgs::Point double_array_point{double_array};
    WHEN("We check the Point coordinates") {
      THEN("They all have to be equal to the array content") {
        REQUIRE(double_array_point.getX() == Approx(double_1));
        REQUIRE(double_array_point.getY() == Approx(double_2));
        REQUIRE(double_array_point.getZ() == Approx(double_3));
      }
    }
  }

  GIVEN("A Point created from moving an array of 3 double") {
    simple_msgs::Point moved_array_point{std::move(double_array)};
    WHEN("We check the Point coordinates") {
      THEN("They all have to be equal to the array content") {
        REQUIRE(moved_array_point.getX() == Approx(double_1));
        REQUIRE(moved_array_point.getY() == Approx(double_2));
        REQUIRE(moved_array_point.getZ() == Approx(double_3));
      }
    }
  }

  // Testing copy/move constructors.
  GIVEN("A Point") {
    simple_msgs::Point single_point{double_array};
    simple_msgs::Point reference_point{single_point};
    WHEN("I copy-construct a new Point") {
      simple_msgs::Point copied_point{single_point};
      THEN("The new Point is equal to the other") { REQUIRE(copied_point == single_point); }
    }
    WHEN("I move-construct a new Point") {
      simple_msgs::Point moved_point{std::move(single_point)};
      THEN("The new Point coordinates are equal to the previous' ones") {
        REQUIRE(moved_point.getX() == Approx(reference_point.getX()));
        REQUIRE(moved_point.getY() == Approx(reference_point.getY()));
        REQUIRE(moved_point.getZ() == Approx(reference_point.getZ()));
      }
    }
  }

  // Testing copy/move assignments.
  GIVEN("A Point") {
    simple_msgs::Point single_point{double_1};
    simple_msgs::Point reference_point{double_1};
    WHEN("I copy-assign from that Point") {
      simple_msgs::Point copy_assign_point{};
      copy_assign_point = single_point;
      THEN("The new Point has to be same as the original") { REQUIRE(copy_assign_point == single_point); }
    }
    WHEN("I move-assign from that Point") {
      simple_msgs::Point move_assign_point{};
      move_assign_point = std::move(single_point);
      THEN("The new Point has to be same as the original") { REQUIRE(move_assign_point == reference_point); }
    }
    WHEN("I copy-assign a double array to that point") {
      single_point = flipped_double_array;
      THEN("The Point coordinates are equal to the array") {
        REQUIRE(single_point.getX() == Approx(flipped_double_array[0]));
        REQUIRE(single_point.getY() == Approx(flipped_double_array[1]));
        REQUIRE(single_point.getZ() == Approx(flipped_double_array[2]));
      }
    }
    WHEN("I move-assign from an array to that Point") {
      single_point = std::move(flipped_double_array);
      THEN("The Point coordinates are equal to the array") {
        REQUIRE(single_point.getX() == Approx(flipped_double_array[0]));
        REQUIRE(single_point.getY() == Approx(flipped_double_array[1]));
        REQUIRE(single_point.getZ() == Approx(flipped_double_array[2]));
      }
    }
  }

  // Testing operations.
  GIVEN("One point") {
    simple_msgs::Point point{double_array};
    WHEN("I use the unary ++ operator") {
      point++;
      ++point;
      THEN("The point has the correct values") {
        REQUIRE(point.getX() == Approx(double_array[0] + 2));
        REQUIRE(point.getY() == Approx(double_array[1] + 2));
        REQUIRE(point.getZ() == Approx(double_array[2] + 2));
      }
    }
    WHEN("I use the unary -- operator") {
      point--;
      --point;
      THEN("The point has the correct values") {
        REQUIRE(point.getX() == Approx(double_array[0] - 2));
        REQUIRE(point.getY() == Approx(double_array[1] - 2));
        REQUIRE(point.getZ() == Approx(double_array[2] - 2));
      }
    }
  }

  GIVEN("Two Points") {
    simple_msgs::Point first_point{double_array};
    simple_msgs::Point second_point{double_1 + double_2};
    WHEN("They contain the same values") {
      second_point = first_point;
      THEN("They are equal") { REQUIRE(first_point == second_point); }
    }
    WHEN("They contain different values") {
      THEN("They are different") { REQUIRE(first_point != second_point); }
    }
    WHEN("I add them") {
      simple_msgs::Point sum_point = first_point + second_point;
      THEN("The sum should be correct") {
        REQUIRE(sum_point.getX() == Approx(first_point.getX() + second_point.getX()));
        REQUIRE(sum_point.getY() == Approx(first_point.getY() + second_point.getY()));
        REQUIRE(sum_point.getZ() == Approx(first_point.getZ() + second_point.getZ()));
      }
    }
    WHEN("I add the second to the first one") {
      first_point += second_point;
      THEN("The first Point should have increased by the second Point") {
        REQUIRE(first_point.getX() == Approx(double_1 + second_point.getX()));
        REQUIRE(first_point.getY() == Approx(double_2 + second_point.getX()));
        REQUIRE(first_point.getZ() == Approx(double_3 + second_point.getX()));
      }
    }
    WHEN("I subtract them") {
      simple_msgs::Point difference_point = first_point - second_point;
      THEN("The difference should be correct") {
        REQUIRE(difference_point.getX() == Approx(first_point.getX() - second_point.getX()));
        REQUIRE(difference_point.getY() == Approx(first_point.getY() - second_point.getY()));
        REQUIRE(difference_point.getZ() == Approx(first_point.getZ() - second_point.getZ()));
      }
    }
    WHEN("I subtract the second to the first one") {
      first_point -= second_point;
      THEN("The first Point should have decreased by the second Point") {
        REQUIRE(first_point.getX() == Approx(double_1 - second_point.getX()));
        REQUIRE(first_point.getY() == Approx(double_2 - second_point.getY()));
        REQUIRE(first_point.getZ() == Approx(double_3 - second_point.getZ()));
      }
    }
    WHEN("I multiply them") {
      simple_msgs::Point multiplied_point = first_point * second_point;
      THEN("The result is correct") {
        REQUIRE(multiplied_point.getX() == Approx(first_point.getX() * second_point.getX()));
        REQUIRE(multiplied_point.getY() == Approx(first_point.getY() * second_point.getY()));
        REQUIRE(multiplied_point.getZ() == Approx(first_point.getZ() * second_point.getZ()));
      }
    }
    WHEN("I multiply the first times the second") {
      first_point *= second_point;
      THEN("The result is correct") {
        REQUIRE(first_point.getX() == Approx(double_1 * second_point.getX()));
        REQUIRE(first_point.getY() == Approx(double_2 * second_point.getY()));
        REQUIRE(first_point.getZ() == Approx(double_3 * second_point.getZ()));
      }
    }
    WHEN("I divide them") {
      simple_msgs::Point divided_point = first_point / second_point;
      THEN("The result is correct") {
        REQUIRE(divided_point.getX() == Approx(first_point.getX() / second_point.getX()));
        REQUIRE(divided_point.getY() == Approx(first_point.getY() / second_point.getY()));
        REQUIRE(divided_point.getZ() == Approx(first_point.getZ() / second_point.getZ()));
      }
    }
    WHEN("I divide the first by the second") {
      first_point /= second_point;
      THEN("The result is correct") {
        REQUIRE(first_point.getX() == Approx(double_1 / second_point.getX()));
        REQUIRE(first_point.getY() == Approx(double_2 / second_point.getY()));
        REQUIRE(first_point.getZ() == Approx(double_3 / second_point.getZ()));
      }
    }
  }

  // Testing coordinates setters/getters.
  GIVEN("A Point") {
    simple_msgs::Point single_point{};
    WHEN("I set the X coordinate of the Point") {
      single_point.setX(double_3);
      THEN("The data point has the correct coordinate") { REQUIRE(single_point.getX() == Approx(double_3)); }
    }
    WHEN("I set the Y coordinate of the Point") {
      single_point.setY(double_1);
      THEN("The data point has the correct coordinate") { REQUIRE(single_point.getY() == Approx(double_1)); }
    }
    WHEN("I set the Z coordinate of the Point") {
      single_point.setZ(double_2);
      THEN("The data Point has the correct coordinate") { REQUIRE(single_point.getZ() == Approx(double_2)); }
    }
  }

  // Testing vectorization, message topic and stream operator.
  GIVEN("A Point") {
    simple_msgs::Point single_point{double_array};
    WHEN("I get the double array from the Point") {
      auto array = single_point.toVector();
      THEN("The array elements are correct") {
        REQUIRE(array[0] == Approx(single_point.getX()));
        REQUIRE(array[1] == Approx(single_point.getY()));
        REQUIRE(array[2] == Approx(single_point.getZ()));
      }
    }
    WHEN("I get the message topic") {
      std::string topic_name = simple_msgs::Point::getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POIT"); }
    }
    WHEN("I print the Point") {
      std::ostringstream out;
      out << single_point;
      THEN("The output is correct") {
        REQUIRE(out.str() == "Point \n \tx: " + std::to_string(single_point.getX()) + "\n \t" +
                                 "y: " + std::to_string(single_point.getY()) + "\n \t" +
                                 "z: " + std::to_string(single_point.getZ()) + "\n");
      }
    }
  }
}
