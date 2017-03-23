#!/bin/bash
cd examples

set -e

for fn in example_*; do
   if [ -d $fn ]; then
     ./build.sh $fn
   fi
done


