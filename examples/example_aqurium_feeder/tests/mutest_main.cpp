
#include "catch.hpp"

#include "Servo.h"
#include "main.h"
#include "runtime.h"
#include "avr/eeprom.h"
#include "mock_serial.h"

void do_loop_a_while()
{
  for ( int i = 0; i < EEPROM_WRITE_THRESHOLD_MIN; i ++ )
     loop();  
}



extern Runtime RUNTIME;

void run_setup()
{
       // we could have some neat check of serial output here, but lets go blind folded
       serial_receive_fake.return_val = (char*)"setup";
       int seria_return_vals[2] = { 3, 8 }; // three days, 8 holes
       SET_RETURN_SEQ(serial_receive_number, seria_return_vals, 2);
       
       REQUIRE( Servo__write_fake.call_count == 0 );
       
       loop();
       serial_receive_fake.return_val = NULL;
       // Now the runtime should be valid
       REQUIRE( RUNTIME.valid() == true );

}

TEST_CASE( "setup sets servo position if valid structure found", "[integration],[main],[runtime]" ) 
{
   RESET_FAKE( Servo__write );
   fake_eeprom_reset();
   RESET_FAKE( serial_receive );
   RESET_FAKE( serial_receive_number );
   
   setup();

   SECTION( "nothing is done on invalid eeprom" ) {
       do_loop_a_while();
       
       REQUIRE( Servo__write_fake.call_count == 0 );
       REQUIRE( RUNTIME.valid() == false );
   };
   
   SECTION( "With valid setup we start running" ) {
       
       do_loop_a_while();
       REQUIRE( RUNTIME.valid() == false );
       run_setup();
       
       // and now when the loop is running the servo starts to get write calls
       REQUIRE( Servo__write_fake.call_count == 1 );
       
       // until at some point the program gets done and the structure gets reseted again.
       for ( int tloop = 0; tloop < 4*24*60; tloop ++ )
       {
          loop();
       }
       
       REQUIRE( Servo__write_fake.call_count > 16 );
       REQUIRE( RUNTIME.valid() == false );
       
   };
   SECTION( "With valid eeprom we continue running" ) {
      run_setup();
      RESET_FAKE( Servo__write );
      setup(); // simulate restart - but now we have valid eeprom
      REQUIRE( RUNTIME.valid() == true );
      REQUIRE( Servo__write_fake.call_count == 1 );
      
      // until at some point the program gets done and the structure gets reseted again.
      for ( int tloop = 0; tloop < 4*24*60; tloop ++ )
      {
         loop();
      }
      REQUIRE( Servo__write_fake.call_count > 16 );
   };
}


