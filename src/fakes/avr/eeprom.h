
#ifndef FAKE_AVR_EEPROM_H
#define FAKE_AVR_EEPROM_H

#include <stdint.h>
#include <stdlib.h>

void eeprom_update_block( const void * src, void * dst, size_t n );
void eeprom_read_block(  void * dest, const void * source, size_t n );

void _test_eeprom_reset();

#endif