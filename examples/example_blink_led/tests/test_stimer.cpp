/**
 * Example testcase for timer module, checks that overflow functionality is what it expected to be
 * 
 * All other modules are mocked.
 * 
 */

#include <climits>
#include "catch.hpp"
#include "Arduino.h"


#include "stimer.h"

TEST_CASE( "Basic timer", "[timer]" ) 
{
  STimer timer;
  
  SECTION("Normal functionality")
  {
     REQUIRE( timer.check( 10 ) == true );
     timer.reset();
     REQUIRE( timer.check( 10 ) == false );
     millis_fake.return_val = 1 << 10;
     REQUIRE( timer.check( 10 ) == true );
  }
  SECTION("Overflow")
  {
   
     STimer timer;
     
     millis_fake.return_val = ULLONG_MAX - 1;
     timer.reset();
     millis_fake.return_val = ULLONG_MAX ;
     REQUIRE( timer.check( 10 ) == false );

     millis_fake.return_val = 10 ;
     REQUIRE( timer.check( 10 ) == true );
     
     // Then check the case where we wait for whole timer to go all around over.
     timer.reset();
     REQUIRE( timer.check( 10 ) == false );
     millis_fake.return_val = 0 ; // and the timer went overflow without any check in between
     REQUIRE( timer.check( 10 ) == true );
   }
   
   SECTION("Reset with carry normal and overflow")
   {
      const unsigned long start_value[] = { 0, ULLONG_MAX - 500 };
      for ( int start_loop = 0; start_loop < 2; start_loop ++ )
      {
         millis_fake.return_val = start_value[start_loop];
         timer.reset();
         REQUIRE( timer.check( 500 ) == false);
         millis_fake.return_val +=  1000;
         REQUIRE( timer.check( 500 ) == true );
         timer.reset_with_carry(450);
         REQUIRE( timer.check( 500 ) == true );
      }
   }
      
};
  
