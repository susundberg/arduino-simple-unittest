#!/bin/bash
cd examples
rm -rf build

set -e

for fn in example_*; do
   if [ -d $fn ]; then
     ./build.sh $fn normal
   fi
done

for fn in test_fail_*; do
   if [ -d $fn ]; then
     ./build.sh $fn assume_fail
   fi
done

for fn in test_ok_*; do
   if [ -d $fn ]; then
     ./build.sh $fn normal
   fi
done



