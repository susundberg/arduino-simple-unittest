/**
 * Simple test to check that invalid pinwrite does fail
 */

#include <Arduino.h>


void setup()
{
   pinMode( 12, OUTPUT );
}

void loop()
{
   digitalWrite( 10, 1 );
   delay(1000);
   digitalWrite( 10, 0 );
}

