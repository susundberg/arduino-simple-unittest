#include <assert.h>
#include <string.h>

#include "avr/eeprom.h"
#define EEPROM_SIZE 1024
static char EEPROM[EEPROM_SIZE];


void _test_eeprom_reset()
{
   memset( EEPROM, 0xFE, sizeof(EEPROM ));
}

void eeprom_update_block( const void * src, void* dst, size_t n )
{
   char* target = (char*)EEPROM + (uintptr_t)(dst);
   assert( target + n < EEPROM + EEPROM_SIZE );
   
   memcpy( target, src, n );
   
}

void eeprom_read_block(  void * dest, const void * source, size_t n )
{
   char* target = EEPROM + (uintptr_t)source;
   
   assert( target + n < EEPROM + EEPROM_SIZE );
   memcpy( dest, target, n );
}