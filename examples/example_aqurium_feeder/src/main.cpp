/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */


#include <Arduino.h>
#include <Servo.h>

#include "serial.h"
#include "runtime.h"
#include "main.h"

#define PIN_SERVO_ENABLE 8
#define N_SECONDS_IN_MINUTE (60)

// Declared as non-static since we want to use it in unittests.
Runtime RUNTIME;

static Servo SERVO;  
static int SERVO_old_value;

static void servo_set_hole( int index );

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode( PIN_SERVO_ENABLE, OUTPUT );
  digitalWrite( PIN_SERVO_ENABLE, LOW);
  
  serial_setup( "Feeder terminal" );
  serial_print(" * type 'setup' to configure and start\n");
  serial_print(" * type 'stop' to cancel\n" );
  
  SERVO.attach( 9 );
  SERVO_old_value = 0xFF;
  
  RUNTIME.load( );
  
  if ( RUNTIME.valid() == true )
  {
     int curr_hole;
     bool done;
     
     RUNTIME.get_status( &curr_hole, &done ) ;
     
     if (done == false )
     {
       servo_set_hole( curr_hole );
     }
  }
  
}



void servo_set_hole( int index )
{
   
   static const int n_steps = 20;
   
   int target_value = index * 125 + 700;

   if (target_value < 0 )
      target_value = 0;
   if (target_value > 1650)
      target_value = 1650;

   // If value has not changed, do nothing
   if ( SERVO_old_value == target_value )
      return;
   
   
   digitalWrite( PIN_SERVO_ENABLE, HIGH );
   delay(10);
   
   // On the first round we just set the value on what it supposed to be
   if ( SERVO_old_value  == 0xFF )
   {
      SERVO.write( target_value / 10 );
      SERVO_old_value  = target_value ;
   }
   else
   {
      // otherwise make 'smooth' transition
      int value_inc = ( target_value - SERVO_old_value  )/(n_steps-1);
      for ( int loop_steps = 0; loop_steps < n_steps; loop_steps ++ )
      {
         int value = SERVO_old_value  + value_inc*loop_steps;
         SERVO.write( (value + 5) / 10 );
         delay( 10 );
      }
   }
   delay( 200 );
   digitalWrite( PIN_SERVO_ENABLE, LOW );
   SERVO_old_value  = target_value;
}


static void toggle_led()
{
  static bool state = 0;
  digitalWrite(LED_BUILTIN, state);
  state = !state;
}

bool handle_receive( Runtime* runtime,  int sleep_time_ms )
{
   for ( int loop = 0; loop < sleep_time_ms; loop += 100 )
   {
        int input_n = 0;
        char* input = serial_receive( &input_n );
        
        toggle_led();
        
        delay( 100 );
        
        if (input == NULL )
           continue;
        
        if (strcmp( input, "setup") == 0 )
        {
           runtime->setup(  );
           servo_set_hole( -1 );
           return true;
        }
        else if (strcmp( input, "print") == 0 )
        {
           runtime->print( );
           int  hole_index;
           bool done;
           RUNTIME.get_status( &hole_index, &done );
           serial_print("Current time: %d hole: %d done: %d\n", RUNTIME.get_time(), hole_index, done );
        }
        else if (strcmp( input, "stop") == 0 )
        {
           runtime->stop( );
           serial_print("Runtime cleared.\n");
           return true;
        }
        else if (strcmp( input, "set") == 0 )
        {
           serial_print("Input hole\n");
           int hole = serial_receive_number( 0, 10 );
           servo_set_hole( hole );
           delay(1000);
        }
        else
        {
           serial_print("Invalid command '%s'\n>", input );
        }
   }
   return false;
}


void loop()
{
  toggle_led();
  
  // If the runtime is not valid, make led blinking noticable different
  if ( RUNTIME.valid() == false )
  {
     handle_receive( &RUNTIME, 300 );
     delay(300);
     return;
  }
  
  // wait for one minute
  int sec_loop = 0;
  for ( sec_loop= 0; sec_loop < N_SECONDS_IN_MINUTE; sec_loop ++ )
  {
     if ( handle_receive( &RUNTIME, 1000 ) )
        break;
  }   
  
  serial_print("Acc time: %d\n", RUNTIME.get_time() );
  
  // check if we did enter new eeprom time 
  if ( RUNTIME.time_pass( sec_loop ) == false )
     return;
  
  // we did. Move it!
  int  hole_index;
  bool done;
  RUNTIME.get_status( &hole_index, &done );
  serial_print("Move to hole: %d\n", hole_index );
  servo_set_hole( hole_index );
  if ( done == true )
  {
     serial_print("End position reached, clearing the configuration.");
     RUNTIME.stop( );
  }
}  
