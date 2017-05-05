#include "catch.hpp"


#include "mock_led.h" // get the led _fake objects


extern void loop();
extern void setup();

TEST_CASE( "Main functionality", "[main]" ) 
{
   
   RESET_FAKE( StatusToLed__setup );
   RESET_FAKE( StatusToLed__loop );
   RESET_FAKE( StatusToLed__set_status );
   setup();
   loop();
   loop();
   loop();
   REQUIRE( StatusToLed__loop_fake.call_count == 3);
   
}