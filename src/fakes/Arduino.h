#ifndef FAKE_Arduino_h
#define FAKE_Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "fff.h"

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

typedef unsigned int word;
typedef bool boolean;
typedef uint8_t byte;


void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
unsigned long millis();
void analogWrite(uint8_t, int);
void delay(unsigned long);

DECLARE_FAKE_VOID_FUNC( pinMode, uint8_t, uint8_t );
DECLARE_FAKE_VOID_FUNC( digitalWrite, uint8_t, uint8_t );
DECLARE_FAKE_VALUE_FUNC( int, digitalRead, uint8_t );
DECLARE_FAKE_VALUE_FUNC( unsigned long, millis );
DECLARE_FAKE_VALUE_FUNC( int, analogRead, uint8_t );
DECLARE_FAKE_VOID_FUNC( analogWrite, uint8_t, int );
DECLARE_FAKE_VOID_FUNC( delay, unsigned long );



#include <queue>
#include <string>
#include <map>


class Serial_CLS
{
   
   typedef std::queue<std::string> Buffer; 
   
   public:
      void write( const char* buffer, int buffer_n );
      void write( const char* buffer );
      void print( int value );
      void print( double value );
      void begin( int baudrate );
      int available();
      char read();
   
      // any printing will be appended to this vector
      Buffer _test_output_buffer;
      std::string _test_output_current; // current output line
      std::queue<char> _test_input_buffer;
      
      void _test_clear(); // remove and reset everything from the buffers
      void _test_set_input( const char* what );
   protected:
      void _test_output_string( std::string what );
};


class Arduino_TEST
{
   public:
     enum class  Check_mode { Full, None }; // Mode FULL for all checks (check that digital write is output and digital read is input), Defaults to FULL
     constexpr static const int MAX_PINS = 128;
     
     void hookup(); // Reset and hook the arduino functions (digitalRead, digitalWrite, pinMode, analogRead, analogWrite)
     void reset(); // clear all values and custom hookups.
     void set_mode( Check_mode target );
     void check_write(uint8_t pin);
     void check_read(uint8_t pin);
     
     int     pin_value[ MAX_PINS ];
     uint8_t pin_mode [ MAX_PINS ];
     
     Check_mode check_mode;
};

extern Arduino_TEST ARDUINO_TEST;

extern Serial_CLS Serial;

typedef std::string String ;

static const int A0 = 100;

#define LED_BUILTIN 13

/** TODO 
// undefine stdlib's abs if encountered
// #ifdef abs
// #undef abs
// #endif
// 
// #ifdef max
// #undef max
// #endif
// 
// #ifdef min
// #undef min
// #endif
// 
// #define min(a,b) ((a)<(b)?(a):(b))
// #define max(a,b) ((a)>(b)?(a):(b))
// #define abs(x) ((x)>0?(x):-(x))
// These seems to cause troubles with queue or string include -- one should figure out why and do we need these.

void analogReference(uint8_t mode);

unsigned long millis(void);
unsigned long micros(void);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);


// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

*/


#endif
