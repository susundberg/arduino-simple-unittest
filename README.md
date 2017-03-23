# arduino-simple-unittest

Simple framework to provide unittesting for arduino projects

How it works
====================
* The sconscript python build script scans src/ directory for *.h files - that are considered as modules.
* Scans for tests/mocks_man folder for for *.h files and generates mocks (to mocks_gen/) for other modules (that are in src/ but not in mocks_man/)
* Scans for tests/test_*.cpp for module unittests - builds binary for each of them and mocks other modules
* Reads Sconscript MULTI_UNIT_TESTS variable for tests that contains more than one real module. Mocks others.

Why it works
====================
* FFF [https://github.com/meekrosoft/fff] to make mock functions
* CPPclean [https://github.com/myint/cppclean/] to parse the sources to find what kind of functions to mock  
* Catch [https://github.com/philsquared/Catch] to run tests

Usage
====================
* Copy the 'tests' folder into your project 
* The sconscript assumes that you have 'src' directory that contains *.cpp or *c files that each have corresponding .h header file. 
    * For example if you have "src/robot_arm.cpp" you must have also "src/robot_arm.h" that contains declarations of the functions you want to test
* Add building of the tests to your build manager - it works by executing '''scons -Y tests/tools/''' on base path
    * There is also example Makefile on the tests/tools/ directory
* Add file "test_<unit name here>.cpp" that contains Catch tests. It will be done as binary called tests/build/bin/ut_<unit name here> with every other module replaced with mocks.


Examples
====================
* See examples directory that also works as tests for this test framework.

Testing of the test framework
====================
* Tested with travis on ubuntu trusty
