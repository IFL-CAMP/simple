#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/point.h"
#include <time.h>
#include <stdlib.h>

// Tests for Point Message.
SCENARIO("Using a Point Message")
{
  double double_1 = static_cast<double>(rand()) / RAND_MAX;
  double double_2 = static_cast<double>(rand()) / RAND_MAX;
  double double_3 = static_cast<double>(rand()) / RAND_MAX;
  std::array<double, 3> double_array{double_1, double_2, double_3};
  std::array<double, 3> twisted_double_array{double_3, double_1, double_2};

  // Testing Constructors.
  GIVEN("An empty point")
  {
    simple_msgs::Point empty_point;
    WHEN("It is constructed")
    {
      THEN("The point coordinates have to be zero")
      {
        REQUIRE(empty_point.getX() == 0);
        REQUIRE(empty_point.getY() == 0);
        REQUIRE(empty_point.getZ() == 0);
      }
    }
  }

  GIVEN("A point created from a double")
  {
    simple_msgs::Point single_double_point(double_1);
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the given value")
      {
        REQUIRE(single_double_point.getX() == double_1);
        REQUIRE(single_double_point.getY() == double_1);
        REQUIRE(single_double_point.getZ() == double_1);
      }
    }
  }

  GIVEN("A point created from 3 double")
  {
    simple_msgs::Point three_double_point(double_1, double_2, double_3);
    WHEN("We check the points coordinates")
    {
      THEN("They have to be equal to the given values")
      {
        REQUIRE(three_double_point.getX() == double_1);
        REQUIRE(three_double_point.getY() == double_2);
        REQUIRE(three_double_point.getZ() == double_3);
      }
    }
  }

  GIVEN("A point created from an array of a double")
  {
    simple_msgs::Point double_array_point(double_array);
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the array content")
      {
        REQUIRE(double_array_point.getX() == double_1);
        REQUIRE(double_array_point.getY() == double_2);
        REQUIRE(double_array_point.getZ() == double_3);
      }
    }
  }

  GIVEN("A point created from moving an array of a double")
  {
    simple_msgs::Point moved_array_point(std::move(double_array));
    WHEN("We check the points coordinates")
    {
      THEN("They all have to be equal to the array content")
      {
        REQUIRE(moved_array_point.getX() == double_1);
        REQUIRE(moved_array_point.getY() == double_2);
        REQUIRE(moved_array_point.getZ() == double_3);
      }
    }
  }

  // Testing Copy-constructors.
  GIVEN("A point")
  {
    simple_msgs::Point single_point(double_array);
    WHEN("I construct a new point from the serialized data of the existing point")
    {
      simple_msgs::Point buffer_point(single_point.getBufferData());
      THEN("The new point has to be equal to the other") { REQUIRE(buffer_point == single_point); }
    }
    WHEN("I copy-construct a new point")
    {
      simple_msgs::Point copied_point(single_point);
      THEN("The new point is equal to the other") { REQUIRE(copied_point == single_point); }
    }
    WHEN("I move-construct a new point")
    {
      simple_msgs::Point moved_point(std::move(single_point));
      THEN("The new point's coordinates are equal to the previous' ones")
      {
        REQUIRE(moved_point.getX() == single_point.getX());
        REQUIRE(moved_point.getY() == single_point.getY());
        REQUIRE(moved_point.getZ() == single_point.getZ());
      }
    }
  }

  // Testing Copy-assignments.
  GIVEN("A point")
  {
    simple_msgs::Point single_point(double_1);
    WHEN("I copy-assign from that point")
    {
      simple_msgs::Point copy_assign_point;
      copy_assign_point = single_point;
      THEN("The new point has to be same as the original") { REQUIRE(copy_assign_point == single_point); }
    }
    WHEN("I move-assign from that point")
    {
      simple_msgs::Point move_assign_point;
      move_assign_point = std::move(single_point);
      THEN("The new point has to be same as the original") { REQUIRE(move_assign_point == single_point); }
    }
    WHEN("I copy-assign from that point's buffer")
    {
      simple_msgs::Point copy_buffer_point = single_point.getBufferData();
      THEN("The new point has to be same as the original") { REQUIRE(copy_buffer_point == single_point); }
    }
    WHEN("I copy-assign a double array to that point")
    {
      single_point = twisted_double_array;
      THEN("The point coordinates are equal to the array")
      {
        REQUIRE(single_point.getX() == twisted_double_array[0]);
        REQUIRE(single_point.getY() == twisted_double_array[1]);
        REQUIRE(single_point.getZ() == twisted_double_array[2]);
      }
    }
    WHEN("I move-assign from an array to that point")
    {
      single_point = std::move(twisted_double_array);
      THEN("The point coordinates are equal to the array")
      {
        REQUIRE(single_point.getX() == twisted_double_array[0]);
        REQUIRE(single_point.getY() == twisted_double_array[1]);
        REQUIRE(single_point.getZ() == twisted_double_array[2]);
      }
    }
  }

  // Testing operations.
  GIVEN("Two points")
  {
    simple_msgs::Point first_point(double_array);
    simple_msgs::Point second_point(double_1 + double_2);
    WHEN("They contain the same values")
    {
      second_point = first_point;
      THEN("Their comparison is correct") { REQUIRE(first_point == second_point); }
    }
    WHEN("They contain different values")
    {
      THEN("Their comparison is correct") { REQUIRE(first_point != second_point); }
    }
    WHEN("I compare these points")
    {
      THEN("They have to be different") { REQUIRE(first_point != second_point); }
    }
    WHEN("I add them")
    {
      simple_msgs::Point sum_point = first_point + second_point;
      THEN("The difference should be correct")
      {
        REQUIRE(sum_point.getX() == (first_point.getX() + second_point.getX()));
        REQUIRE(sum_point.getY() == (first_point.getY() + second_point.getY()));
        REQUIRE(sum_point.getZ() == (first_point.getZ() + second_point.getZ()));
      }
    }
    WHEN("I add the second to the first one")
    {
      first_point += second_point;
      THEN("The first point should have increased by the second point")
      {
        REQUIRE(first_point.getX() == (double_1 + second_point.getX()));
        REQUIRE(first_point.getY() == (double_2 + second_point.getX()));
        REQUIRE(first_point.getZ() == (double_3 + second_point.getX()));
      }
    }
    WHEN("I subtract them")
    {
      simple_msgs::Point difference_point = first_point - second_point;
      THEN("The difference should be correct")
      {
        REQUIRE(difference_point.getX() == (first_point.getX() - second_point.getX()));
        REQUIRE(difference_point.getY() == (first_point.getY() - second_point.getY()));
        REQUIRE(difference_point.getZ() == (first_point.getZ() - second_point.getZ()));
      }
    }
    WHEN("I subtract the second to the first one")
    {
      first_point -= second_point;
      THEN("The first point should have decreased by the second point")
      {
        REQUIRE(first_point.getX() == double_1 - second_point.getX());
        REQUIRE(first_point.getY() == double_2 - second_point.getY());
        REQUIRE(first_point.getZ() == double_3 - second_point.getZ());
      }
    }
    WHEN("I multiply them")
    {
      simple_msgs::Point multiplied_point = first_point * second_point;
      THEN("The result is correct")
      {
        REQUIRE(multiplied_point.getX() == first_point.getX() * second_point.getX());
        REQUIRE(multiplied_point.getY() == first_point.getY() * second_point.getY());
        REQUIRE(multiplied_point.getZ() == first_point.getZ() * second_point.getZ());
      }
    }
    WHEN("I multiply the first times the second")
    {
      first_point *= second_point;
      THEN("The result is correct")
      {
        REQUIRE(first_point.getX() == double_1 * second_point.getX());
        REQUIRE(first_point.getY() == double_2 * second_point.getY());
        REQUIRE(first_point.getZ() == double_3 * second_point.getZ());
      }
    }
    WHEN("I divide them")
    {
      simple_msgs::Point divided_point = first_point / second_point;
      THEN("The result is correct")
      {
        REQUIRE(divided_point.getX() == first_point.getX() / second_point.getX());
        REQUIRE(divided_point.getY() == first_point.getY() / second_point.getY());
        REQUIRE(divided_point.getZ() == first_point.getZ() / second_point.getZ());
      }
    }
    WHEN("I divide the first by the second")
    {
      first_point /= second_point;
      THEN("The result is correct")
      {
        REQUIRE(first_point.getX() == double_1 / second_point.getX());
        REQUIRE(first_point.getY() == double_2 / second_point.getY());
        REQUIRE(first_point.getZ() == double_3 / second_point.getZ());
      }
    }
  }

  // Testing coordinates setters/getters.
  GIVEN("A point")
  {
    simple_msgs::Point single_point;
    WHEN("I set the X coordinate of the point")
    {
      single_point.setX(double_3);
      THEN("The data point has the correct coordinate") { REQUIRE(single_point.getX() == double_3); }
    }
    WHEN("I set the Y coordinate of the point")
    {
      single_point.setY(double_1);
      THEN("The data point has the correct coordinate") { REQUIRE(single_point.getY() == double_1); }
    }
    WHEN("I set the Z coordinate of the point")
    {
      single_point.setZ(double_2);
      THEN("The data point has the correct coordinate") { REQUIRE(single_point.getZ() == double_2); }
    }
  }

  GIVEN("A point")
  {
    simple_msgs::Point single_point(double_array);
    WHEN("I get the double array from the point")
    {
      auto array = single_point.toVector();
      THEN("The array elements are correct")
      {
        REQUIRE(array[0] == single_point.getX());
        REQUIRE(array[1] == single_point.getY());
        REQUIRE(array[2] == single_point.getZ());
      }
    }
    WHEN("I get the message topic")
    {
      std::string topic_name = single_point.getTopic();
      THEN("I get the correct one") { REQUIRE(topic_name == "POIT"); }
    }
  }
}
