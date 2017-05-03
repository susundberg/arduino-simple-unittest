#include "catch.hpp"
#include "Arduino.h"

#include "led.h"

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
   }
}