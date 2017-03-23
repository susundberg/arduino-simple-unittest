

#include "catch.hpp"
#include "serial.h"
#include <iostream>
#include <string.h>

#include "Arduino.h"


Serial_CLS help;

// void foo()
// {
// 
// help._test_output_buffer.size()
// 
//    help._test_input_buffer
// };


TEST_CASE( "input works", "[serial]" ) 
{
   Serial._test_clear();
   
   SECTION( "plain string" ) 
   {
      int len = 0;
      char* buffer;
      buffer = serial_receive( &len );
      REQUIRE( buffer == NULL );
      Serial._test_set_input("setup");
      buffer = serial_receive( &len );
      REQUIRE( buffer == NULL );
      Serial._test_set_input("\n");
      buffer = serial_receive( &len );
      REQUIRE( buffer != NULL );
      REQUIRE( len == 5 );
      REQUIRE( std::string(buffer) == "setup" );
   }
   
   SECTION( "input number" ) 
   {
      Serial._test_set_input("Invalid\n-3\n10\n0\n");
      int ret = serial_receive_number( -1, 1);
      REQUIRE( ret == 0 );
      REQUIRE( Serial._test_input_buffer.size() == 0 );
   }
}
   
TEST_CASE( "prints some format", "[serial]" ) 
{
   Serial._test_clear();
   REQUIRE( Serial._test_output_buffer.size() == 0);
   
   
   SECTION( "strings" ) 
   {
     serial_print("HELLO %s\n", "WORLD");
     REQUIRE( Serial._test_output_buffer.size() == 1);
     REQUIRE( Serial._test_output_buffer.front() == "HELLO WORLD" );
   }      
   
   SECTION("setup")
   {
      serial_setup( "TESTFOO" );
   }
   
   SECTION( "ints and floats" ) 
   {
      
     serial_print("FOO %d %d\nMORE", 10, 20 );
     REQUIRE( Serial._test_output_buffer.size() == 1);
     REQUIRE( Serial._test_output_buffer.front() == "FOO 10 20" );
     Serial._test_output_buffer.pop();
     serial_print(" %f TOCOME\n", 0.15 );
     REQUIRE( Serial._test_output_buffer.size() == 1);
     REQUIRE( Serial._test_output_buffer.front() == "MORE 0.15 TOCOME" );
   }
   
}
