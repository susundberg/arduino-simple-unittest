#ifndef SUPA_LED_H
#define SUPA_LED_H

#include "stimer.h"

/** Simple class that will blink the led in cycle of long-short*N, where N is the status given */
class StatusToLed
{
   constexpr static const int INTERVAL_SHORT = 200; // ms
   constexpr static const int INTERVAL_LONG  = 700;
   
   public:
     void setup( int pin ); /// What @param pin to use for output
     void loop();
     void set_status( int blinks );
    
   protected:
     STimer timer;
     int pin;
     int blinks;
     int blink_loop;
};

#endif
