#!/bin/bash

set -e


rm -rf tests/build/coverage/
mkdir -p tests/build/coverage/

ALL_INFO_FILES=""

run_single()
{
   lcov --directory=tests/build/src/ --zerocounters
   BIN_NAME=$1
   echo "*** Running: $BIN_NAME"
   $BIN_NAME
   INFO_NAME="tests/build/coverage/$(basename $BIN_NAME).info.part"
   lcov --directory=tests/build/src/ --capture --output-file=$INFO_NAME.extra
   lcov --remove $INFO_NAME.extra "tests/fakes/*" --output-file=$INFO_NAME
   ALL_INFO_FILES="$ALL_INFO_FILES -a $INFO_NAME"
}


for tbin in tests/build/bin/ut_*; do
	run_single $tbin 
done;


for tbin in tests/build/bin/mut_*; do
	run_single $tbin
done;

lcov $ALL_INFO_FILES --output-file=tests/build/coverage/full.info
genhtml --output=tests/build/coverage/ tests/build/coverage/full.info 

echo "*********************************************"
echo "DONE: check tests/build/coverage/index.html"
echo "*********************************************"
