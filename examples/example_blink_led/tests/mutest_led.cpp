/** 
 * Unittesting file for led.h - since the functionality requires stuff from timer, lets not fake it but rather
 * use it as real unit
 * 
 * __UNITTEST__SOURCES_ = runtime.cpp, main.cpp
 */

#include "catch.hpp"
#include "Arduino.h"

#include "led.h" // include the unit under test
#include "mock_stimer.h" // include the faked module (so we can set the return values)


void run_loop( StatusToLed* led )
{
   for ( int loop = 0; loop < 100; loop ++ )
   {
      led->loop();
   }
}


TEST_CASE( "Led blinking works", "[led]" ) 
{
   StatusToLed led;
   
   ARDUINO_TEST.hookup(); 
   led.setup( 10 );
   
   SECTION("It runs")
   {
      run_loop();
      REQUIRE( digitalWrite_fake.call_count == 1);
      
      digitalWrite_fake.call_count = 0;
      STimer__check_fake.return_val = true; // after this it will appear to module as the time would be changing always
      run_loop();
      REQUIRE( digitalWrite_fake.call_count == 10);
   }
}