#ifndef FAKE_SERVO_H
#define FAKE_SERVO_H

#include "fff.h"


class Servo;

void Servo__write( Servo*, int );
DECLARE_FAKE_VOID_FUNC( Servo__write, Servo*, int );

void Servo__attach( Servo*, int );
DECLARE_FAKE_VOID_FUNC( Servo__attach, Servo*, int );


class Servo 
{
  public:
     void write( int angle ) { Servo__write(this, angle); };
     void attach( int pin ) {  Servo__attach(this, pin ); } ;
};

#endif
