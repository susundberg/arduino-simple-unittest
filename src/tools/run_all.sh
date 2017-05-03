#!/bin/bash

set -e

for tbin in tests/build/bin/*t_*; do
	echo "*** Running:  $tbin ***"
        ./$tbin
        echo "*** DONE ***"
done;



