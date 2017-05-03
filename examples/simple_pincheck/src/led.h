#ifndef SUPA_LED_H
#define SUPA_LED_H

#include "stimer.h"

class StatusToLed
{
   constexpr static const int INTERVAL_SHORT = 200;
   constexpr static const int INTERVAL_LONG  = 700;
   
   public:
     void setup( int pin );
     void loop();
     void set_status( int blinks );
    
   protected:
     STimer timer;
     int pin;
     int blinks;
     int blink_loop;
};

#endif
