#!/bin/bash

set -e

for tbin in tests/build/bin/ut_*; do
	echo "*** Running:  $tbin ***"
        ./$tbin
        echo "*** DONE ***"
done;


for tbin in tests/build/bin/mut_*; do
	echo "*** Running:  $tbin ***"
        ./$tbin
        echo "*** DONE ***"
done;

