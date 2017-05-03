


#include <Arduino.h>
#include "serial.h"

void serial_setup( const char* name)
{
  Serial.begin(9600); 
  Serial.write("***********************************\n");  
  serial_print("Welcome to %s!\n", name );
  Serial.write("***********************************\n");
}


bool serial_process_number( const char* buffer, int buffer_n, int* convert )
{
    char* end_ptr;
    int value = strtol( (const char*)buffer, &end_ptr, 10  );
    if ( end_ptr != buffer + buffer_n )
    {
        Serial.write(" Invalid: '" );
        Serial.write( (const char*)buffer);
        Serial.write("'\n"); 
        return false;
    }
    
    *convert = value;
    return true;
}



char* serial_receive( int* buffer_len)
{
  static char buffer[64];
  static int loop = 0;
  
  // if there's any serial available, read it:
  while (Serial.available() > 0) 
  {
    // do it again:
    buffer[ loop ] = Serial.read();

    if (buffer[loop] == '\n')
    {
      buffer[loop] = 0x00;
      *buffer_len = loop;
      loop = 0;
      return buffer;
    }

    loop += 1;
    if ( loop >= 64 )
    {
      Serial.write("E: Too long line\n");
      loop = 0;
    }
    
  }
  return NULL;
}

void serial_print( const char* format, ... )
{
   va_list arg_list;
   va_start(arg_list, format);
   
   int full_len = strlen( format );
   int loop_offset = 0;
   int loop;
   
   for ( loop = 0; loop < full_len; loop ++ )
   {
      if (format[loop] != '%')
         continue;
      
      Serial.write( format + loop_offset, loop - loop_offset ); 
      
      if ( full_len == loop -1 )
      {
         Serial.write("\ERROR_INVALID_FORMAT\n");
         return;
      }
      
      char output_format = format[loop + 1];
      if (output_format == 'd' )
      {
         int value = va_arg( arg_list, int );
         Serial.print( value );
      }
      else if (output_format == 'f' )
      {
         double value = va_arg( arg_list, double );
         Serial.print( value );
      }  
      else if (output_format == 's' )
      {
         const char* value = (char*)va_arg( arg_list, void* );
         Serial.write( value );
      }
      else  
      {
         Serial.write("\ERROR_INVALID_FORMAT\n");
         return;
      }
      // now loop is '%d'
      loop_offset = loop + 2;
      loop = loop + 1;
   }
   va_end(arg_list);
   if ( loop_offset < full_len )
   {
      Serial.write( format + loop_offset, full_len - loop_offset ); 
   }
      
}

int serial_receive_number(int min_value, int max_value)
{
   int buffer_n = 0;
   int number;
   bool print_prompt = true;
   
   while ( true ) 
   {
      if ( print_prompt )
      {
        Serial.write(">");
        print_prompt = false;
      }
      char* buffer = serial_receive( &buffer_n );
      if ( buffer == NULL )
        continue;
      Serial.write(buffer);
      Serial.write("\n");
     
      print_prompt = true;
      
      if ( serial_process_number( buffer, buffer_n , &number ) == false )
        continue;
      
      if ( number < min_value )
      {
         serial_print("Too small number. Min %d\n", min_value );
      }   
      else if ( number > max_value)
      {
         serial_print("Too large number. Max %d\n", max_value );
      }
      else
      {
         return number;
      }
   }
}
