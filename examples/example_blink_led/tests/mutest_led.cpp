/** 
 * Unittesting file for led.h - since the functionality requires stuff from timer, lets not fake it but rather
 * use it as real unit
 * 
 * __UNITTEST__SOURCES_ = led.cpp, stimer.cpp
 * 
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
   led.set_status( 2 );
   
   SECTION("It runs")
   {
      run_loop(&led);
      REQUIRE( digitalWrite_fake.call_count == 1);
      
      for ( int loop = 0; loop < 10; loop ++ )
      {
         digitalWrite_fake.call_count = 0;
         millis_fake.return_val += 1000;  // lets fake this timer so that the STimer class will work as it would if 1sec has passed since last call.
         run_loop(&led);
         REQUIRE( digitalWrite_fake.call_count == 1);
         REQUIRE( digitalWrite_fake.arg1_val == ((loop+1)%2) );
      }
   }
}

