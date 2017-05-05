#!/bin/bash

set -e

for tbin in tests/build/bin/*test_*; do
	echo "*** Running:  $tbin ***"
        ./$tbin
        echo "*** DONE ***"
done;



