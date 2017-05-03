#include "Arduino.h"
#include <string>
#include <sstream>

Serial_CLS Serial;

void Serial_CLS::write( const char* buffer, int buffer_n )
{
   
   this->_test_output_string( std::string( buffer, buffer_n ) );   
}

void Serial_CLS::_test_set_input( const char* what )
{
   this->_test_input_buffer = std::queue<char>();
   for ( int loop = 0; loop < (int)strlen(what); loop ++ )
   {
      this->_test_input_buffer.push( what[loop] );
   }
}

void Serial_CLS::_test_clear()
{
   this->_test_output_current = std::string();
   this->_test_output_buffer = std::queue<std::string>();
   this->_test_input_buffer = std::queue<char>();
}

void Serial_CLS::_test_output_string( std::string what )
{
    
    this->_test_output_current += what;
   
    std::size_t index = this->_test_output_current.find( '\n' );
    if ( index == std::string::npos )
       return;
    
    std::string tobuf = this->_test_output_current.substr( 0, index );
    if ( index + 1 >= this->_test_output_current.length() )
    {
       this->_test_output_current = std::string();
    }
    else
    {
      this->_test_output_current = this->_test_output_current.substr( index + 1, std::string::npos  );
    }
    this->_test_output_buffer.push( tobuf );
}


void Serial_CLS::write( const char* buffer )
{
  this->write( buffer, strlen( buffer ) );   
}

template <typename T> std::string tostr(const T& t) { 
   std::ostringstream os; 
   os<<t; 
   return os.str(); 
} 

void Serial_CLS::print( int value )
{
   this->_test_output_string( tostr( value ) );
}

void Serial_CLS::print( double value )
{
   this->_test_output_string( tostr( value ) );
}

void Serial_CLS::begin( int baudrate )
{
   
}

int  Serial_CLS::available()
{
   return this->_test_input_buffer.empty() == false;
}

char Serial_CLS::read()
{
   char ret = this->_test_input_buffer.front();
   this->_test_input_buffer.pop();
   return ret;
}
