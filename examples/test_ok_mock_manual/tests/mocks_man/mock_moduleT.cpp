
#include <Arduino.h>
#include "moduleT.h"


static FILE* OUTPUT_FID  = NULL;

int _test_moduleT_init( const char* filename )
{
   OUTPUT_FID = fopen( filename, "wb" );
   if ( OUTPUT_FID == NULL )
      return -1;
   return 100;
}

void _test_moduleT_close( )
{
   fclose( OUTPUT_FID );
   OUTPUT_FID = NULL;
}


void ModuleT::drive_step_motor( int steps ) 
{
   fprintf( OUTPUT_FID, "%d %d\n", (int)millis_fake.return_val, steps );
}