#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "simple_msgs/image.h"
#include <time.h>
#include <stdlib.h>

// TEST FOR USING THE IMAGE MESSAGE WRAPPER

SCENARIO("Using an Image Message")
{
  GIVEN("A uint8_t Image created from an empty constructor")
  {
    simple_msgs::Image<uint8_t> image_;
    WHEN("")
    {
      THEN("") {  }
    }
  }
  
}
