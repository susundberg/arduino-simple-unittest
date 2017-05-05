


#include "catch.hpp"

#include "mock_moduleT.h"

extern void loop();

TEST_CASE( "Manual mocks functions are usable", "[main]" )
{
   REQUIRE( _test_moduleT_init("./output_steps.txt") == 100 );
   loop();
   _test_moduleT_close();
}

