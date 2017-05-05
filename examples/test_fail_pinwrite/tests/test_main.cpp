


#include "catch.hpp"

#include "Arduino.h"

extern void loop();
extern void setup();

TEST_CASE( "__ASSUME__FAIL__", "[main]" )
{
   ARDUINO_TEST.hookup();   
   setup();
   loop();
   
}

