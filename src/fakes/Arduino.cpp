#include "Arduino.h"
#include <assert.h>

DEFINE_FAKE_VOID_FUNC( pinMode, uint8_t, uint8_t );
DEFINE_FAKE_VOID_FUNC( digitalWrite, uint8_t, uint8_t );
DEFINE_FAKE_VALUE_FUNC( int, digitalRead, uint8_t );
DEFINE_FAKE_VALUE_FUNC( int, analogRead, uint8_t );
DEFINE_FAKE_VOID_FUNC( analogWrite, uint8_t, int );
DEFINE_FAKE_VOID_FUNC( delay, unsigned long );
DEFINE_FAKE_VALUE_FUNC( unsigned long, millis );



Arduino_TEST ARDUINO_TEST;


void _arduino_test_pinMode( uint8_t pin, uint8_t mode )
{
   assert( pin < Arduino_TEST::MAX_PINS );
   ARDUINO_TEST.pin_mode[ pin ] = mode;
}

void _arduino_test_digitalWrite( uint8_t pin, uint8_t value)
{
   ARDUINO_TEST.check_write( pin );
   ARDUINO_TEST.pin_value[ pin ] = (value != 0);
}

int _arduino_test_digitalRead( uint8_t pin )
{
   ARDUINO_TEST.check_read( pin );
   return (ARDUINO_TEST.pin_value[ pin ] != 0);
}  

int _arduino_test_analogRead( uint8_t pin )
{
   ARDUINO_TEST.check_read( pin );
   return (ARDUINO_TEST.pin_value[ pin ]);
}  

void _arduino_test_analogWrite( uint8_t pin, int value)
{
   ARDUINO_TEST.check_write( pin );
   ARDUINO_TEST.pin_value[ pin ] = value;
}

void Arduino_TEST::set_mode( Arduino_TEST::Check_mode mode )
{
   this->check_mode = mode;
}

void  Arduino_TEST::check_read( uint8_t pin )
{
   assert( pin < Arduino_TEST::MAX_PINS );
   
   if ( this->check_mode !=  Arduino_TEST::Check_mode::Full )
      return;
   
   assert( (this->pin_mode[ pin ] == INPUT) || (this->pin_mode[ pin ] == INPUT_PULLUP ));
   
}

void  Arduino_TEST::check_write( uint8_t pin )
{
   assert( pin < Arduino_TEST::MAX_PINS );
   
   if ( this->check_mode !=  Arduino_TEST::Check_mode::Full )
      return;
   
   assert( this->pin_mode[ pin ] == OUTPUT );
   
}



void Arduino_TEST::hookup()
{
   memset( ARDUINO_TEST.pin_value, 0x00, sizeof(ARDUINO_TEST.pin_value));
   memset( ARDUINO_TEST.pin_mode, 0xFF, sizeof(ARDUINO_TEST.pin_mode));
   this->check_mode = Arduino_TEST::Check_mode::Full;
   pinMode_fake.custom_fake = _arduino_test_pinMode;
   digitalWrite_fake.custom_fake = _arduino_test_digitalWrite;
   digitalRead_fake.custom_fake = _arduino_test_digitalRead;
   analogRead_fake.custom_fake = _arduino_test_analogRead;
   analogWrite_fake.custom_fake = _arduino_test_analogWrite;

}
void Arduino_TEST::reset()
{
   pinMode_fake.custom_fake = NULL;
   digitalWrite_fake.custom_fake = NULL;
   digitalRead_fake.custom_fake = NULL;
   analogRead_fake.custom_fake = NULL;
   analogWrite_fake.custom_fake = NULL;
}
