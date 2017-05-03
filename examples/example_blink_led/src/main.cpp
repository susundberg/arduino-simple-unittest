#include <Arduino.h>

#include "led.h"

StatusToLed LED;

void setup()
{
   LED.setup(13);
   LED.set_status( 2 );
}

void loop()
{
   LED.loop();
}