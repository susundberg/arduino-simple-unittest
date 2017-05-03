/**
 * Unittest file for led. In this example the timer module is faked also
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
   STimer__check_fake.return_val = false; 
   
   SECTION("It runs")
   {
      run_loop(&led);
      REQUIRE( digitalWrite_fake.call_count == 1);
      
      digitalWrite_fake.call_count = 0;
      STimer__check_fake.return_val = true; // after this it will appear to module as the time would be changing always
      run_loop(&led);
      REQUIRE( digitalWrite_fake.call_count == 100);
   }
}