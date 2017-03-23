
#ifndef SUPA_RUNTIME_H
#define SUPA_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_WRITE_THRESHOLD_MIN 16
#define CRC_DEFAULT_SEED (0xFFFFL)
#define EEPROM_ADDR_RUNTIME ((void*)0x10)

typedef struct 
{
      int16_t  ndays;
      int16_t  nholes;
      int16_t  time_loop;
      uint16_t crc;    
} RuntimeEeprom;

class Runtime
{
   public:
      Runtime();
      ~Runtime();
      
      void setup( );
      void load( );
      void save( )  ; // modifies the crc -> not const
      void stop( );

      /// @returns true if the eeprom time has been updated
      bool time_pass( int done_secs );
      
      bool valid( ) const ;
      void print( ) const;
      int  get_time() const;
      
      /// Stores current hole index to @param hole_index, and stores true to @param done if the hole is last hole.
      void get_status( int* holes, bool* done ) const;
   
   protected:
      uint16_t calculate_crc() const ;
      RuntimeEeprom eeprom;
      int time_acc;
      
};


#endif
