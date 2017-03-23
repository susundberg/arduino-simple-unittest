

#include "catch.hpp"
#include "main.h"
#include "mock_serial.h"
#include "mock_runtime.h"
#include "Servo.h"

#include <iostream>

static bool fake_hole_get_done = false;
void fake_hole_get( const Runtime* runtime, int* hole, bool* done )
{
   *hole = 10;
   *done = fake_hole_get_done;
}
TEST_CASE( "setup sets servo position if valid structure found", "[main]" ) 
{
   RESET_FAKE( Servo__write );
   
   Runtime__valid_fake.return_val = true;
   Runtime__get_status_fake.custom_fake = fake_hole_get;
   
   SECTION( "not done" ) {
       fake_hole_get_done = false;
       setup();
       REQUIRE( Servo__write_fake.call_count == 1 );
   };
   SECTION( "done" ) {
       fake_hole_get_done = true;
       setup();
       REQUIRE( Servo__write_fake.call_count == 0 );
   };
}