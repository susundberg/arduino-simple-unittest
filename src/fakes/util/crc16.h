#ifndef _FAKE_CRC16_H
#define _FAKE_CRC16_H

#include <stdint.h>

// Function directly from : http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
uint16_t _crc16_update(uint16_t crc, uint8_t a);


#endif