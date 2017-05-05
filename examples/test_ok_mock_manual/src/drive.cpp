
#include "moduleT.h"
#include "moduleX.h"

void loop()
{
   ModuleT modt;
   ModuleX modx;
   modx.hello_world();
   
   for ( int loop = 0; loop < 1000; loop ++ )
   {
      modt.drive_step_motor( loop );
   }
   
}
