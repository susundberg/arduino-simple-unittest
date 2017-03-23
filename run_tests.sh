#!/bin/bash
cd examples

set -e
set -x

for fn in example_*; do
  ./build.sh $fn
done


