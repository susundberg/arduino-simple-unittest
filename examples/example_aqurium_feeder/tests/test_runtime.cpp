

#include "catch.hpp"
#include "runtime.h"
#include "avr/eeprom.h"
#include <iostream>
#include <string.h>

class RuntimeTester: public Runtime
{
  public:
   
    void set_values( RuntimeEeprom* from )
    {
       this->eeprom = (*from);
    }
    
    void populate_values( bool valid_crc )
    {
      RuntimeEeprom values;
      
      memset( &values, 0x00, sizeof(values));
      values.ndays  = 4;
      values.nholes = 8;
      this->set_values( &values );
      if ( valid_crc )
         values.crc = this->calculate_crc();
      this->set_values( &values );
   }
}; 


TEST_CASE( "Runtime time pass gets new holes ", "[runtime]" ) 
{
   RuntimeTester runtime;
   runtime.populate_values( true );
   REQUIRE( runtime.valid() == true );
   // 4 days 8 holes 12h per hole 
   
   int nchanges = 0;
   int hole_index;
   bool done;
   for (int min_loop = 0; min_loop < 11*60; min_loop ++ )
   {
      nchanges += runtime.time_pass( 60 );
      runtime.get_status( &hole_index, &done );
      REQUIRE( done == false );
      REQUIRE( hole_index == -1 );
   }
   REQUIRE( nchanges > 4 );
   
   // After a while, it will get to hole index 0
   for (int min_loop = 0; min_loop < 2*60; min_loop ++ )
   {
      runtime.time_pass( 60 );
   }
   runtime.get_status( &hole_index, &done );
   REQUIRE( done == false );
   REQUIRE( hole_index == 0 );
   
   // and in the end it will get done
   for (int min_loop = 0; min_loop < 4*24*60; min_loop ++ )
   {
      runtime.time_pass( 60 );
   }
   runtime.get_status( &hole_index, &done );
   REQUIRE( done == true );
   REQUIRE( hole_index == 7 );
   

   
}


TEST_CASE( "Runtime loading is crc protected", "[runtime]" ) 
{
   RuntimeTester runtime;
   fake_eeprom_reset();
   
   SECTION( "invalid crc" ) {
      runtime.load();
      REQUIRE( runtime.valid() == false );
      
   };

   SECTION( "valid crc" ) {
      runtime.populate_values( true );
      runtime.save();
      runtime.populate_values( false );
      runtime.load();
      REQUIRE( runtime.valid() == true );
      
   };   
   
}