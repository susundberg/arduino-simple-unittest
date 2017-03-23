# arduino-simple-unittest


Why use it
====================
I found some arduino mock/test frameworks but they were either hacky or just hard to use, since you could not mock any modules. As professional software guy, i cannot stand bad tools or untested code (its just compiling before testing!). And i want to be able to step the code with gdb to find out what is going on rather than embed my code with tons of printfs - and then remove then when preparing to production. And i want test coverage raport.

So what you get is a simple way to unittest your code.

How it works
====================
* The sconscript python build script scans src/ directory for *.h files - that are considered as modules.
* Scans for tests/mocks_man folder for for *.h files and generates mocks (to mocks_gen/) for other modules (that are in src/ but not in mocks_man/)
* Scans for tests/test_*.cpp for module unittests - builds binary for each of them and mocks other modules
* Reads Sconscript MULTI_UNIT_TESTS variable for tests that contains more than one real module. Mocks others.

Why it works
====================
* [FFF](https://github.com/meekrosoft/fff) to make mock functions
* [CPPclean](https://github.com/myint/cppclean/) to parse the sources to find what kind of functions to mock  
* [Catch](https://github.com/philsquared/Catch) to run tests

Usage
====================
Its not currently working with Arduino IDE code, but i welcome pull requests to make it happen. It should not be that much. But it does work fine with platformio projects.

* Copy the 'tests' folder into your project 
* The sconscript assumes that you have 'src' directory that contains *.cpp or *c files that each have corresponding .h header file. 
    * For example if you have "src/robot_arm.cpp" you must have also "src/robot_arm.h" that contains declarations of the functions you want to test
* Add building of the tests to your build manager - it works by executing '''scons -Y tests/tools/''' on base path
    * There is also example Makefile on the tests/tools/ directory
* Add file "test_<unit name here>.cpp" that contains Catch tests. It will be done as binary called tests/build/bin/ut_<unit name here> with every other module replaced with mocks.
* Modify the sconscript file variable MULTI_UNIT_TESTS to contain multi-unit tests
* See tools/Makefile for example makefile targets - or use it directly.


Examples
====================
* See examples directory that also works as tests for this test framework.
* See my [aquarium feeder](https://github.com/susundberg/arduino-aquarium-feeder) project for full platformio example  

Testing of the test framework
====================
* Tested with travis on ubuntu trusty ![build status](https://travis-ci.org/susundberg/arduino-simple-unittest.svg?branch=master)
