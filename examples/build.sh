#!/bin/bash


usage_exit()
{
   echo "$0 <target directory>"
   exit 1
}


if [ -z $1 ]; then
  usage_exit;
fi

set -e
set -x


SOURCE_DIR=$1
TARGET_DIR="build/$1"
BASE_PATH=$(pwd)


rm -rf $TARGET_DIR
mkdir -p $TARGET_DIR
cp -a "../tests/" $TARGET_DIR
cp -a "$SOURCE_DIR/src" $TARGET_DIR
cp -a "$SOURCE_DIR/tests" $TARGET_DIR
cd $TARGET_DIR
make -f tests/tools/Makefile test_build test_run test_coverage

