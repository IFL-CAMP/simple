#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/rotation_matrix.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE ROTATION MATRIX MESSAGE WRAPPER

SCENARIO("Using a Rotation Matrix Message")
{
  GIVEN("A Rotation Matrix created from 1 double1")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::RotationMatrix r(d1);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the double from the constructor")
      {
        REQUIRE(r.getRow(0)[0] == d1);
        REQUIRE(r.getRow(0)[1] == d1);
        REQUIRE(r.getRow(0)[2] == d1);
        REQUIRE(r.getRow(1)[0] == d1);
        REQUIRE(r.getRow(1)[1] == d1);
        REQUIRE(r.getRow(1)[2] == d1);
        REQUIRE(r.getRow(2)[0] == d1);
        REQUIRE(r.getRow(2)[1] == d1);
        REQUIRE(r.getRow(2)[2] == d1);
      }
    }
  }
  GIVEN("A Rotation Matrix created from 9 doubles")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = static_cast<double>(rand()) / RAND_MAX;
    double d3 = static_cast<double>(rand()) / RAND_MAX;
    double d4 = static_cast<double>(rand()) / RAND_MAX;
    double d5 = static_cast<double>(rand()) / RAND_MAX;
    double d6 = static_cast<double>(rand()) / RAND_MAX;
    double d7 = static_cast<double>(rand()) / RAND_MAX;
    double d8 = static_cast<double>(rand()) / RAND_MAX;
    double d9 = static_cast<double>(rand()) / RAND_MAX;
    simple_msgs::RotationMatrix r(d1, d2, d3, d4, d5, d6, d7, d8, d9);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles from the constructor")
      {
        REQUIRE(r.getRow(0)[0] == d1);
        REQUIRE(r.getRow(0)[1] == d2);
        REQUIRE(r.getRow(0)[2] == d3);
        REQUIRE(r.getRow(1)[0] == d4);
        REQUIRE(r.getRow(1)[1] == d5);
        REQUIRE(r.getRow(1)[2] == d6);
        REQUIRE(r.getRow(2)[0] == d7);
        REQUIRE(r.getRow(2)[1] == d8);
        REQUIRE(r.getRow(2)[2] == d9);
      }
    }
  }
  GIVEN("A Rotation Matrix created from an array of doubles")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = static_cast<double>(rand()) / RAND_MAX;
    double d3 = static_cast<double>(rand()) / RAND_MAX;
    double d4 = static_cast<double>(rand()) / RAND_MAX;
    double d5 = static_cast<double>(rand()) / RAND_MAX;
    double d6 = static_cast<double>(rand()) / RAND_MAX;
    double d7 = static_cast<double>(rand()) / RAND_MAX;
    double d8 = static_cast<double>(rand()) / RAND_MAX;
    double d9 = static_cast<double>(rand()) / RAND_MAX;
    std::array<double, 9> a{d1, d2, d3, d4, d5, d6, d7, d8, d9};
    simple_msgs::RotationMatrix r(a);
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles")
      {
        REQUIRE(r.getRow(0)[0] == d1);
        REQUIRE(r.getRow(0)[1] == d2);
        REQUIRE(r.getRow(0)[2] == d3);
        REQUIRE(r.getRow(1)[0] == d4);
        REQUIRE(r.getRow(1)[1] == d5);
        REQUIRE(r.getRow(1)[2] == d6);
        REQUIRE(r.getRow(2)[0] == d7);
        REQUIRE(r.getRow(2)[1] == d8);
        REQUIRE(r.getRow(2)[2] == d9);
      }
    }
  }
  GIVEN("A Rotation Matrix created from moving an array of doubles")
  {
	  double d1 = static_cast<double>(rand()) / RAND_MAX;
	  double d2 = static_cast<double>(rand()) / RAND_MAX;
	  double d3 = static_cast<double>(rand()) / RAND_MAX;
	  double d4 = static_cast<double>(rand()) / RAND_MAX;
	  double d5 = static_cast<double>(rand()) / RAND_MAX;
	  double d6 = static_cast<double>(rand()) / RAND_MAX;
	  double d7 = static_cast<double>(rand()) / RAND_MAX;
	  double d8 = static_cast<double>(rand()) / RAND_MAX;
	  double d9 = static_cast<double>(rand()) / RAND_MAX;
	  std::array<double, 9> a{ d1, d2, d3, d4, d5, d6, d7, d8, d9 };
    simple_msgs::RotationMatrix r(std::move(a));
    WHEN("We check the Rotation Matrix's elements")
    {
      THEN("They all have to be equal to the doubles")
      {
		  REQUIRE(r.getRow(0)[0] == d1);
		  REQUIRE(r.getRow(0)[1] == d2);
		  REQUIRE(r.getRow(0)[2] == d3);
		  REQUIRE(r.getRow(1)[0] == d4);
		  REQUIRE(r.getRow(1)[1] == d5);
		  REQUIRE(r.getRow(1)[2] == d6);
		  REQUIRE(r.getRow(2)[0] == d7);
		  REQUIRE(r.getRow(2)[1] == d8);
		  REQUIRE(r.getRow(2)[2] == d9);
      }
    }
    WHEN("I construct a new Rotation Matrix from the serialized data of the existing Rotation Matrix")
    {
      simple_msgs::RotationMatrix r2(r.getBufferData());
      THEN("The new Rotation Matrix has to be equal to the other") { REQUIRE(r2 == r); }
    }
    WHEN("I copy-construct a new Rotation Matrix")
    {
      simple_msgs::RotationMatrix r3(r);
      THEN("The new Rotation Matrix is equal to the other") { REQUIRE(r3 == r); }
    }
    WHEN("I move-construct a new Rotation Matrix")
    {
      simple_msgs::RotationMatrix r4(std::move(r));
      THEN("The new Rotation Matrix's coordinates are equal to the previous' ones")
      {
		  REQUIRE(r4.getRow(0)[0] == d1);
		  REQUIRE(r4.getRow(0)[1] == d2);
		  REQUIRE(r4.getRow(0)[2] == d3);
		  REQUIRE(r4.getRow(1)[0] == d4);
		  REQUIRE(r4.getRow(1)[1] == d5);
		  REQUIRE(r4.getRow(1)[2] == d6);
		  REQUIRE(r4.getRow(2)[0] == d7);
		  REQUIRE(r4.getRow(2)[1] == d8);
		  REQUIRE(r4.getRow(2)[2] == d9);
      }
    }
  }
  GIVEN("An instance of a Rotation Matrix.")
  {
    // start a quaternion
    simple_msgs::RotationMatrix r;

    WHEN("We use the empty constructor")
    {
      THEN("The Rotation Matrix elements have to be zero")
      {
        REQUIRE(r.getRow(0)[0] == 0);
        REQUIRE(r.getRow(0)[1] == 0);
        REQUIRE(r.getRow(0)[2] == 0);
        REQUIRE(r.getRow(1)[0] == 0);
        REQUIRE(r.getRow(1)[1] == 0);
        REQUIRE(r.getRow(1)[2] == 0);
        REQUIRE(r.getRow(2)[0] == 0);
        REQUIRE(r.getRow(2)[1] == 0);
        REQUIRE(r.getRow(2)[2] == 0);
      }
    }
    WHEN("I set the 1st row of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setRow(0, a);
      THEN("The Rotation Matrix row has the correct elements") { REQUIRE(r.getRow(0) == a); }
    }
    WHEN("I set the 1st column of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setColumn(0, a);
      THEN("The Rotation Matrix column has the correct elements") { REQUIRE(r.getColumn(0) == a); }
    }
    WHEN("I set the 2nd row of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setRow(1, a);
      THEN("The Rotation Matrix row has the correct elements") { REQUIRE(r.getRow(1) == a); }
    }
    WHEN("I set the 2nd column of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setColumn(1, a);
      THEN("The Rotation Matrix column has the correct elements") { REQUIRE(r.getColumn(1) == a); }
    }
    WHEN("I set the 3rd row of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setRow(2, a);
      THEN("The Rotation Matrix row has the correct elements") { REQUIRE(r.getRow(2) == a); }
    }
    WHEN("I set the 3rd column of the Rotation Matrix")
    {
      double x = static_cast<double>(rand()) / RAND_MAX;
      double y = static_cast<double>(rand()) / RAND_MAX;
      double z = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 3> a{x, y, z};
      r.setColumn(2, a);
      THEN("The Rotation Matrix column has the correct elements") { REQUIRE(r.getColumn(2) == a); }
    }
  }
  GIVEN("Two identical Rotation Matrixes")
  {
	  double d1 = static_cast<double>(rand()) / RAND_MAX;
	  double d2 = static_cast<double>(rand()) / RAND_MAX;
	  double d3 = static_cast<double>(rand()) / RAND_MAX;
	  double d4 = static_cast<double>(rand()) / RAND_MAX;
	  double d5 = static_cast<double>(rand()) / RAND_MAX;
	  double d6 = static_cast<double>(rand()) / RAND_MAX;
	  double d7 = static_cast<double>(rand()) / RAND_MAX;
	  double d8 = static_cast<double>(rand()) / RAND_MAX;
	  double d9 = static_cast<double>(rand()) / RAND_MAX;
	  simple_msgs::RotationMatrix r(d1, d2, d3, d4, d5, d6, d7, d8, d9);
	  simple_msgs::RotationMatrix r2(d1, d2, d3, d4, d5, d6, d7, d8, d9);
    WHEN("I compare these Rotation Matrixes")
    {
      THEN("They have to be equal") { REQUIRE(r == r2); }
    }
  }
  GIVEN("A Rotation Matrix")
  {
	  double d1 = static_cast<double>(rand()) / RAND_MAX;
	  double d2 = static_cast<double>(rand()) / RAND_MAX;
	  double d3 = static_cast<double>(rand()) / RAND_MAX;
	  double d4 = static_cast<double>(rand()) / RAND_MAX;
	  double d5 = static_cast<double>(rand()) / RAND_MAX;
	  double d6 = static_cast<double>(rand()) / RAND_MAX;
	  double d7 = static_cast<double>(rand()) / RAND_MAX;
	  double d8 = static_cast<double>(rand()) / RAND_MAX;
	  double d9 = static_cast<double>(rand()) / RAND_MAX;
	  simple_msgs::RotationMatrix r(d1, d2, d3, d4, d5, d6, d7, d8, d9);
    WHEN("I copy-assign from that Rotation Matrix's buffer")
    {
      simple_msgs::RotationMatrix r2;
      r2 = r.getBufferData();
      THEN("The new Rotation Matrix has to be same as the original") { REQUIRE(r2 == r); }
    }
    WHEN("I copy-assign from that Rotation Matrix")
    {
      simple_msgs::RotationMatrix r3;
      r3 = r;
      THEN("The new Rotation Matrix has to be same as the original") { REQUIRE(r3 == r); }
    }
    WHEN("I move-assign from that Rotation Matrix")
    {
      simple_msgs::RotationMatrix r4;
      r4 = std::move(r);
      THEN("The new Rotation Matrix has to be same as the original")
      {
		  REQUIRE(r4.getRow(0)[0] == d1);
		  REQUIRE(r4.getRow(0)[1] == d2);
		  REQUIRE(r4.getRow(0)[2] == d3);
		  REQUIRE(r4.getRow(1)[0] == d4);
		  REQUIRE(r4.getRow(1)[1] == d5);
		  REQUIRE(r4.getRow(1)[2] == d6);
		  REQUIRE(r4.getRow(2)[0] == d7);
		  REQUIRE(r4.getRow(2)[1] == d8);
		  REQUIRE(r4.getRow(2)[2] == d9);
      }
    }
  }
  GIVEN("A Rotation Matrix")
  {
    simple_msgs::RotationMatrix r;
    WHEN("I copy-assign an array to that Rotation Matrix")
    {
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = static_cast<double>(rand()) / RAND_MAX;
		double d3 = static_cast<double>(rand()) / RAND_MAX;
		double d4 = static_cast<double>(rand()) / RAND_MAX;
		double d5 = static_cast<double>(rand()) / RAND_MAX;
		double d6 = static_cast<double>(rand()) / RAND_MAX;
		double d7 = static_cast<double>(rand()) / RAND_MAX;
		double d8 = static_cast<double>(rand()) / RAND_MAX;
		double d9 = static_cast<double>(rand()) / RAND_MAX;
      std::array<double, 9> a{d1, d2, d3,d4, d5, d6, d7, d8, d9};
      r = a;
      THEN("The Rotation Matrix's elements are equal to the array")
      {
		  REQUIRE(r.getRow(0)[0] == d1);
		  REQUIRE(r.getRow(0)[1] == d2);
		  REQUIRE(r.getRow(0)[2] == d3);
		  REQUIRE(r.getRow(1)[0] == d4);
		  REQUIRE(r.getRow(1)[1] == d5);
		  REQUIRE(r.getRow(1)[2] == d6);
		  REQUIRE(r.getRow(2)[0] == d7);
		  REQUIRE(r.getRow(2)[1] == d8);
		  REQUIRE(r.getRow(2)[2] == d9);
      }
    }
    WHEN("I move-assign from an array to that Rotation Matrix")
    {
		double d1 = static_cast<double>(rand()) / RAND_MAX;
		double d2 = static_cast<double>(rand()) / RAND_MAX;
		double d3 = static_cast<double>(rand()) / RAND_MAX;
		double d4 = static_cast<double>(rand()) / RAND_MAX;
		double d5 = static_cast<double>(rand()) / RAND_MAX;
		double d6 = static_cast<double>(rand()) / RAND_MAX;
		double d7 = static_cast<double>(rand()) / RAND_MAX;
		double d8 = static_cast<double>(rand()) / RAND_MAX;
		double d9 = static_cast<double>(rand()) / RAND_MAX;
		std::array<double, 9> a{ d1, d2, d3,d4, d5, d6, d7, d8, d9 };
      r = std::move(a);
      THEN("The quaternion's elements are the same as the array")
      {
		  REQUIRE(r.getRow(0)[0] == d1);
		  REQUIRE(r.getRow(0)[1] == d2);
		  REQUIRE(r.getRow(0)[2] == d3);
		  REQUIRE(r.getRow(1)[0] == d4);
		  REQUIRE(r.getRow(1)[1] == d5);
		  REQUIRE(r.getRow(1)[2] == d6);
		  REQUIRE(r.getRow(2)[0] == d7);
		  REQUIRE(r.getRow(2)[1] == d8);
		  REQUIRE(r.getRow(2)[2] == d9);
      }
    }
  }
  GIVEN("Two different Rotation Matrixes")
  {
    double d1 = static_cast<double>(rand()) / RAND_MAX;
    double d2 = d1 + 1.0;
    simple_msgs::RotationMatrix r1(d1);
    simple_msgs::RotationMatrix r2(d2);
    WHEN("I compare these Rotation Matrixes")
    {
      THEN("They have to be different") { REQUIRE(r1 != r2); }
    }
  }
  GIVEN("A Rotation Matrix")
  {
	  double d1 = static_cast<double>(rand()) / RAND_MAX;
	  double d2 = static_cast<double>(rand()) / RAND_MAX;
	  double d3 = static_cast<double>(rand()) / RAND_MAX;
	  double d4 = static_cast<double>(rand()) / RAND_MAX;
	  double d5 = static_cast<double>(rand()) / RAND_MAX;
	  double d6 = static_cast<double>(rand()) / RAND_MAX;
	  double d7 = static_cast<double>(rand()) / RAND_MAX;
	  double d8 = static_cast<double>(rand()) / RAND_MAX;
	  double d9 = static_cast<double>(rand()) / RAND_MAX;
	  simple_msgs::RotationMatrix r(d1, d2, d3, d4, d5, d6, d7, d8, d9);
    WHEN("I get the array from the Rotation Matrix")
    {
      std::array<double, 9> a = r.toVector();
      THEN("The array elements are correct")
      {
        REQUIRE(a[0] == d1);
        REQUIRE(a[1] == d2);
        REQUIRE(a[2] == d3);
        REQUIRE(a[3] == d4);
		REQUIRE(a[4] == d5);
		REQUIRE(a[5] == d6);
		REQUIRE(a[6] == d7);
		REQUIRE(a[7] == d8);
		REQUIRE(a[8] == d9);
      }
    }
	WHEN("I get the transpose from the Rotation Matrix")
	{
		simple_msgs::RotationMatrix transpose = r.getTranspose();
		THEN("The transpose elements are correct")
		{
			REQUIRE(transpose.getRow(0)[0] == d1);
			REQUIRE(transpose.getRow(0)[1] == d4);
			REQUIRE(transpose.getRow(0)[2] == d7);
			REQUIRE(transpose.getRow(1)[0] == d2);
			REQUIRE(transpose.getRow(1)[1] == d5);
			REQUIRE(transpose.getRow(1)[2] == d8);
			REQUIRE(transpose.getRow(2)[0] == d3);
			REQUIRE(transpose.getRow(2)[1] == d6);
			REQUIRE(transpose.getRow(2)[2] == d9);
		}
	}
  }
}
