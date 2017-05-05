
Why use it
====================
I found some arduino mock/test frameworks but they were either hacky or just hard to use, since you could not mock any modules. As professional software guy, i cannot stand bad tools or untested code (its just compiling before testing!). And i want to be able to step the code with gdb to find out what is going on rather than embed my code with tons of printfs - and then remove then when preparing to production. And i want test coverage report. 

So what you get is a simple way to unittest your code.

### Features:
* Automatic generation of mock/stub functions from source-files headers
* Basic Arduino functions fakes (so that compiling works, this is not an emulator)
* Batteries included: test-framework & mocking picked from two lightweight alternatives   
* Automatic compling of binary per source file, with other sources files stubbed amd binary per set of defined source files, with other sources stubbed.

How it works
====================
* The sconscript python build script scans src/ directory for *.h files - that are considered as modules.
* Scans for tests/mocks_man folder for for *.h files and generates mocks (to mocks_gen/) for other modules (that are in src/ but not in mocks_man/ - these are manual created mocks/fakes. )
* Scans for tests/mocks_man/fake_*.cpp for other extra files to be included in all builds (provides for example 'external XXX' linkage)
* Scans for tests/test_*.cpp for module unittests - builds binary for each of them and mocks other modules
* Scans for tests/mutest_*.cpp for multi unit tests, the file must contain magic string to name the real source modules (others mocked).


What makes the heavy lifting
====================
* [FFF](https://github.com/meekrosoft/fff) to make mock functions
* [CPPclean](https://github.com/myint/cppclean/) to parse the sources to find what kind of functions to mock  
* [Catch](https://github.com/philsquared/Catch) to run tests
* [Scons](http://scons.org/) to run building scripts 
* [Lcov](http://ltp.sourceforge.net/coverage/lcov.php) to generate coverage report.

Usage
====================
Its not currently working with Arduino IDE code, but i welcome pull requests to make it happen. It should not be that much. But it does work fine with platformio projects.

* Copy the 'src' folder as 'tests' folder into your project 
* The sconscript assumes that you have 'src' directory that contains *.cpp files that each have corresponding .h header file. 
    * You need to split your project into separate files - i call them on this text modules - every module own functions are real and every other module is mocked - that is when the function is called, rather than the real code is executed a stub or mock function is called (that records the parameters and returns a given value by default) 
    * For example if you have "src/robot_arm.cpp" you must have also "src/robot_arm.h" that contains declarations of the functions that are called outside of the module. And the "src/main.cpp" that calls "robot_arm_init()" function will be calling actually a stub "robot_arm_init()" function when running main.cpp unittests (defined in file "tests/test_main.cpp").
* Add building of the tests to your build manager - it works by executing '''scons -Y tests/tools/''' on base path
    * There is also example Makefile on the tests/tools/ directory
* Add file "test_<unit name here>.cpp" that contains Catch tests. It will be done as binary called "tests/build/bin/ut_<unit name here>" with every other module replaced with mocks.
* Add file "mutest_<any name here>.cpp" that contains Catch tests. It must contain magic string '__UNITTEST__SOURCES_ = <source files comma separated>'.


Fakes provided
====================

* Arduino pin functionality. Its defined in '''src/fakes/Arduino.h''' and implemented in corresponding .cpp. It provides simple checks (can be disabled) that no un-initialized pin gets read or writes.
* Serial functionality that stores and received lines from buffer. 
 

Examples
====================
* See examples directory that also works as tests for this test framework.
* See my [aquarium feeder](https://github.com/susundberg/arduino-aquarium-feeder) project for full platformio example  

#### Screenshot: Debugger on arduino code
![screenshot debugger](https://rawgit.com/susundberg/arduino-simple-unittest/master/website/screen_debug.png)

#### Screenshot: Coverage of the tests on firefox (generates html)
![screenshot coverage](https://rawgit.com/susundberg/arduino-simple-unittest/master/website/screen_coverage.png)


Testing of the test framework
====================
* Tested with travis on ubuntu trusty ![build status](https://travis-ci.org/susundberg/arduino-simple-unittest.svg?branch=master)

TODO
====================
* Support for static member functions
* Support for overloaded functions
* Support for references in parameters
* Support for types that are defined inside classes/structures


