
#include <avr/eeprom.h>
#include <util/crc16.h>
#include <string.h>

#include "runtime.h"
#include "serial.h"

void Runtime::print( ) const
{
   if ( this->eeprom.ndays == 0)
   {
      serial_print("RuntimeEeprom: not valid.\n");
      return;
   }
   serial_print("RuntimeEeprom: valid.\n");
   
   serial_print( "   ndays: %d\n" , this->eeprom.ndays );   
   serial_print( "   nholes: %d\n", this->eeprom.nholes);
   serial_print( "   tloop: %d\n" , this->eeprom.time_loop );
   
}


Runtime::Runtime()
{
   memset( &(this->eeprom), 0x00, sizeof(RuntimeEeprom ));
   this->time_acc = 0;
}

Runtime::~Runtime()
{
}

bool Runtime::valid( ) const
{
   return ( this->eeprom.ndays > 0 && this->eeprom.ndays < 60 );
}

bool Runtime::time_pass( int done_secs)
{
   this->time_acc += (done_secs)/60;
   
   if (this->time_acc >= EEPROM_WRITE_THRESHOLD_MIN )
   {
      this->eeprom.time_loop += this->time_acc;
      Runtime::save( );
      this->time_acc = 0;
      return true;
   }
   return false;
   
}


uint16_t Runtime::calculate_crc( ) const
{
   uint16_t crc_ret = CRC_DEFAULT_SEED;
   const char* eeprom_array = (const char*)(&this->eeprom);
   
   int calc_size = sizeof(RuntimeEeprom) - sizeof(uint16_t);
   for ( int cloop = 0; cloop < calc_size; cloop ++ )
   {
      crc_ret = _crc16_update( crc_ret , eeprom_array[cloop] );
   }
   return crc_ret;
}

void Runtime::save(   )
{
   this->eeprom.crc = calculate_crc(  );
   eeprom_update_block( (const char*)(&this->eeprom), EEPROM_ADDR_RUNTIME, sizeof(RuntimeEeprom) );
}


void Runtime::stop(  )
{
   memset( &this->eeprom, 0x00, sizeof(RuntimeEeprom));
   Runtime::save();
}

void Runtime::load( )
{
   eeprom_read_block( &this->eeprom, EEPROM_ADDR_RUNTIME,  sizeof(RuntimeEeprom) );
   uint16_t crc_calc  = this->calculate_crc();
   if (crc_calc != this->eeprom.crc )
   {
      serial_print("Invalid eeprom content -> clearing.");
      Runtime::stop( );
   }
   Runtime::print( );
}

void Runtime::setup()
{
   serial_print("Input number of days to stay\n");
   int ndays = serial_receive_number( 1, 255);

   serial_print("Input number holes filled\n");
   int nholes = serial_receive_number( 1, 9 );
   

   float holes_per_day = (float)nholes / (float)ndays ;
   float sleep_hours = (24.0f ) / holes_per_day ;
   
   serial_print("-> %f holes per day -> sleep of %f hours between holes\n", holes_per_day, sleep_hours );
   this->eeprom.time_loop = 0;
   this->eeprom.ndays     = ndays;
   this->eeprom.nholes    = nholes;
   Runtime::save();
}

int Runtime::get_time() const
{
   return this->eeprom.time_loop + this->time_acc;
}


void Runtime::get_status( int* hole_index, bool* done ) const
{
   
   //  We could use also 
   /// the acculumated time, but would mean that after power shortage we would
   /// reverse for one hole. Lets not do that.
   
   uint32_t time = this->eeprom.time_loop;
   
   uint32_t time_per_hole = (this->eeprom.ndays*24L*60L) / this->eeprom.nholes;
   
   *hole_index = (time/time_per_hole) - 1;
   if ( *hole_index >= (this->eeprom.nholes - 1 ) )
   {
      *done = true;
      (*hole_index) = (this->eeprom.nholes - 1 ); 
   }
   else
   {
      *done = false;
   }
   
   
}
