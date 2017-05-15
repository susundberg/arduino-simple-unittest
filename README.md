
What it is
====================
Unittesting 'framework' for arduino projects, currently works with platform.io projects. It generates host-pc binaries that contains tests, that can be runned and debuged, no flashing required, super fast. I call it framework, since it provides building, mock-generation and testing in single package, and requires you to copy folder to your project folder.


Why use it instead of other testing frameworks
====================

* Automatic generation of mock/stub functions from source-files headers and automatic compiling of test binaries with specified units as real implementantions.
* Basic Arduino functions fakes (so that compiling works, this is not an emulator)
* Compiles to HOST-PC runnable binary, that you can easily debug with your preferred IDE or plain gdb. 
* Batteries included: test-framework & mocking picked from two lightweight alternatives

Examples why to do unittesting on your arduino code
====================
* Timer class that has to deal with overflow. Manually testing is imposible, and testing on real hw is doable, but you have to inject somehow millis() to return proper test values.
     * Create separate My_Timer.h module, that is unit under tests, and other modules, (include millis() function) are mocked, and user can define the return codes as one pleases.
* Serial communication with another device, and you want to make sure the output looks proper 
     * Create separate My_Device.h module, that is unit under test, make the Serial.read() return the input you want and check that Serial.write output is proper.
* Stepper motor driving with specified ramp - assume you are controlling a stepper motor, that speeds up with smooth ramp and travels certain number of steps. You want to make sure that the ramp has no 'clitches' and it looks proper. 
     * Create My_Driver.h module that contanis the driving functions and My_Stepper.h that contains the actual stepper controlling functions. Now when testing module My_Driver you can set the MyStepper.set_steps() to output a file, and you can analyze the file with say python to find possible spikes.   

Usage
====================

First you might want to see the examples folder - the [blink led](examples/example_blink_led) is currently the best to get quick overview what this is about.


### Set up the framework for your project:

Currently you need to copy this projects 'src' folder to your project. This is because Scons is not very happy with out-of-tree builds. Sorry.

1. Copy the 'src' folder as 'tests' folder into your project.
2. Add building of the tests to your build manager - it works by executing ```scons -Y tests/tools/``` on base path
    * There is also example Makefile on the tests/tools/ directory

### Start writing unit tests

These tests will be linked so that they will have only the unit under test real source file and other modules are mocked. See below for more info.

1. Create a test file called ```tests/test_<any name here>.cpp```

That is if you have ```src/foo.cpp``` and ```src/bar.cpp``` the test file for foo should
be called ```tests/tests_foo.cpp``` and there will be binary called ```tests/build/bin/test_foo``` created after compiling is done.

The test binary will have real implementations from ```src/foo.cpp``` but module Bar will be implemented in mock - from ```tests/mocks_gen/mock_bar.cpp```. 
The arduino specific stuff is mostly mocked away, but some extra checking is availble. See [src/Arduino.h](src/Arduino.h) and 'Fakes provided' later on this document.
 

### Do some multi-unit tests

These binaries will be called ```tests/build/bin/mutest_xxx``` and they will contain real implementation from the files listed. Other modules will be mocked away.

1. Create a test file called ```tests/mutest_<any name here>.cpp```
2. In that file create line containing '__UNITTEST__SOURCES_ = <source files comma separated>'. For example if you have ```src/foo.cpp'' and ```src/bar.cpp'', this line should be ```__UNITTEST__SOURCES_ = foo.cpp, bar.cpp```.

The minimal test file (and this is enough! It compiles to test binary!) would be something like:
```
#include "catch.hpp"
#include "Arduino.h"

#include "led.h" 


TEST_CASE( "Led blinking works", "[led]" ) 
{
   Led led;
   led.setup(1);
   REQUIRE( digitalWrite_fake.call_count == 1);
   led.loop();
   REQUIRE( digitalWrite_fake.call_count == 2);
}
```

### Extra: Define common test modules

These common files will be included in all tests. They can be used to contain global variables (```external Foo XXX``` stuff) or just common test code (```initialize_output_csv()```).
1. Create files ```tests/common_<any name here>.cpp``` as needed. 

### Extra: Define manual mocks

For manually mocked modules we do not generate 'automatic' mocks. To define a module as manually mocked do:
1. Create ```tests/mocks_man/mock_<source filename>.cpp``` - that is if you have ```src/foo.cpp``` create file ```tests/mocks_man/mock_foo.cpp``` that contains the mocked functions 


Fakes provided
====================

* Arduino pin functionality. Its defined in ```src/fakes/Arduino.h``` and implemented in corresponding .cpp. It provides simple checks (can be disabled) that no un-initialized pin gets read or writes.
* Serial functionality that stores and received lines from buffer. 
 

Examples
====================
* See [examples](examples) directory that also works as tests for this test framework.
* See my [aquarium feeder](https://github.com/susundberg/arduino-aquarium-feeder) project for full platformio example  

#### Screenshot: Debugger on arduino code
![screenshot debugger](https://rawgit.com/susundberg/arduino-simple-unittest/master/website/screen_debug.png)

#### Screenshot: Coverage of the tests on firefox (generates html)
![screenshot coverage](https://rawgit.com/susundberg/arduino-simple-unittest/master/website/screen_coverage.png)


Testing of the test framework
====================
* Tested with travis on ubuntu trusty ![build status](https://travis-ci.org/susundberg/arduino-simple-unittest.svg?branch=master)

What makes the heavy lifting
====================
* [FFF](https://github.com/meekrosoft/fff) to make mock functions
* [CPPclean](https://github.com/myint/cppclean/) to parse the sources to find what kind of functions to mock  
* [Catch](https://github.com/philsquared/Catch) to run tests
* [Scons](http://scons.org/) to run building scripts 
* [Lcov](http://ltp.sourceforge.net/coverage/lcov.php) to generate coverage report.


TODO
====================

### Structure: build tree
I would like to have the tests/ folder out from the source tree, but currently it would require some hacking for Scons to support out of tree build or change of build system. 

### Structure: separate platforms
I am working on ESP8266 support, and it would be neat to have it somewhat separated from the basic Arduino tree, and one would configure the platform used in tests/test_config.ini or similar. 

### Mock generation: C++
* C++: Support for static member functions
* C++: Support for overloaded functions
* C++: Support for references in parameters
* C++: Support for types that are defined inside classes/structures

### Arduino IDE
Like said before, currently this works only with platformio projects. So its not working with Arduino IDE code, but i welcome pull requests to make it happen. With Arduino IDE one still needs the source files to be splitted to multiple files (for mock generatation). 



