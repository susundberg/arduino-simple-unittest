Examples
==================


### example_blink_led

Features:
* Multi unit test example - see mutest_led.cpp file. It has two modules as real modules and others mocked. See the test_led.cpp for the same test with all modules mocked
* Arduino_test hookup, providing check that we do not write on input or un-initialized pins.
* Class mock generation: mocks are generated for both classes. The Led class mocks is used in test_main.cpp


### example_serial_comm
Features:
* Use the Serial-fake object to test the serial communication
* Uses variable args







