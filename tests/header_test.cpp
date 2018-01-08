#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/header.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE HEADER MESSAGE WRAPPER

SCENARIO("Using a Header Message")
{
  GIVEN("A Header created from an empty constructor")
  {
    simple_msgs::Header header;
    WHEN("We check the Header's fields")
    {
      THEN("They have to be the default ones")
      {
        REQUIRE(header.getSequenceNumber() == 0);
        REQUIRE(header.getFrameID() == "");
        REQUIRE(header.getTimestamp() == 0.0);
      }
    }
  }
  GIVEN("A Header created from a int, a string and a double")
  {
    int i1 = static_cast<int>(rand()) / RAND_MAX;
    double d1 = (rand()) / RAND_MAX;
    std::string s1 = std::to_string(d1);
    simple_msgs::Header header(i1, s1, d1);
    WHEN("We check the Header's fields")
    {
      THEN("It has to be equal to the params from the constructor")
      {
        REQUIRE(header.getSequenceNumber() == i1);
        REQUIRE(header.getFrameID() == s1);
        REQUIRE(header.getTimestamp() == d1);
      }
    }
    WHEN("I construct a new Header from the serialized data of the existing Header")
    {
      simple_msgs::Header header2(header.getBufferData());
      THEN("The new Header has to be equal to the other")
      {
        REQUIRE(header2.getSequenceNumber() == i1);
        REQUIRE(header2.getFrameID() == s1);
        REQUIRE(header2.getTimestamp() == d1);
      }
    }
    WHEN("I copy-construct a new Header")
    {
      simple_msgs::Header header3(header);
      THEN("The new Header is equal to the other")
      {
        REQUIRE(header3.getSequenceNumber() == i1);
        REQUIRE(header3.getFrameID() == s1);
        REQUIRE(header3.getTimestamp() == d1);
      }
    }
    WHEN("I move-construct a new Header")
    {
      simple_msgs::Header header4(std::move(header));
      THEN("The new Header's fields are equal to the previous' one")
      {
        REQUIRE(header4.getSequenceNumber() == i1);
        REQUIRE(header4.getFrameID() == s1);
        REQUIRE(header4.getTimestamp() == d1);
      }
    }
  }

  GIVEN("An instance of a Header.")
  {
    // start a Header
    simple_msgs::Header h;

    WHEN("I set the time stamp of the Header")
    {
      double x = (rand()) / RAND_MAX;
      h.setTimestamp(x);
      THEN("The Header's time stamp is correct") { REQUIRE(h.getTimestamp() == x); }
    }
    WHEN("I set the frame id of the Header")
    {
      std::string s = std::to_string((rand()) / RAND_MAX);
      h.setFrameID(s);
      THEN("The Header's frame id is correct") { REQUIRE(h.getFrameID() == s); }
    }
    WHEN("I set the sequence number of the Header")
    {
      int i = static_cast<int>(rand()) / RAND_MAX;
      h.setSequenceNumber(i);
      THEN("The Header's sequence number is correct") { REQUIRE(h.getSequenceNumber() == i); }
    }
  }
  GIVEN("Two identical Headers")
  {
    int i1 = static_cast<int>(rand()) / RAND_MAX;
    double d1 = (rand()) / RAND_MAX;
    std::string s1 = std::to_string(d1);
    simple_msgs::Header header1(i1, s1, d1);
    simple_msgs::Header header2(i1, s1, d1);
    WHEN("I compare these Headers")
    {
      THEN("They have to be equal") { REQUIRE(header1 == header2); }
    }
  }
  GIVEN("A Header")
  {
    int i1 = static_cast<int>(rand()) / RAND_MAX;
    double d1 = (rand()) / RAND_MAX;
    std::string s1 = std::to_string(d1);
    simple_msgs::Header header1(i1, s1, d1);
    WHEN("I copy-assign from that Header's buffer")
    {
      simple_msgs::Header h2;
      h2 = header1.getBufferData();
      THEN("The new Header has to be same as the original") { REQUIRE(h2 == header1); }
    }
    WHEN("I copy-assign from that Header")
    {
      simple_msgs::Header h3;
      h3 = header1;
      THEN("The new Header has to be same as the original") { REQUIRE(h3 == header1); }
    }
    WHEN("I move-assign from that Header")
    {
      simple_msgs::Header h4;
      h4 = std::move(header1);
      THEN("The new Header has to be same as the original")
      {
        REQUIRE(h4.getSequenceNumber() == i1);
        REQUIRE(h4.getFrameID() == s1);
        REQUIRE(h4.getTimestamp() == d1);
      }
    }
  }
  GIVEN("Two different Header")
  {
    int i1 = static_cast<int>(rand()) / RAND_MAX;
    double d1 = (rand()) / RAND_MAX;
    std::string s1 = std::to_string(d1);
    simple_msgs::Header header1(i1, s1, d1);
    simple_msgs::Header header2(i1 + 1, s1, d1);
    WHEN("I compare these Headers")
    {
      THEN("They have to be different") { REQUIRE(header1 != header2); }
    }
  }
  GIVEN("A Header")
  {
	  int i1 = static_cast<int>(rand()) / RAND_MAX;
	  double d1 = (rand()) / RAND_MAX;
	  std::string s1 = std::to_string(d1);
	  simple_msgs::Header header1(i1, s1, d1);
    WHEN("I get the buffer of that Header and construct a new Header")
    {
      uint8_t* buf = header1.getBufferData();
      simple_msgs::Header h2(buf);
      THEN("The new Header is equal to the original") { REQUIRE(header1 == h2); }
    }
  }
}
