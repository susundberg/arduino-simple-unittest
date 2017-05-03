#!/bin/bash


set -e

usage_exit()
{
   echo "$0 <target directory>"
   exit 1
}


if [ -z $1 ]; then
  usage_exit;
fi



SOURCE_DIR=$1
TARGET_DIR="build/$1"
BASE_PATH=$(pwd)


rm -rf $TARGET_DIR
mkdir -p $TARGET_DIR
cp -a "../src/" "$TARGET_DIR/tests/"
cp -a "$SOURCE_DIR/src" $TARGET_DIR
cp -a "$SOURCE_DIR/tests" $TARGET_DIR
cd $TARGET_DIR
PATCH_FILE="../../$SOURCE_DIR.patch"
if [ -e $PATCH_FILE ]; then
   patch -p0 < $PATCH_FILE
fi
make -f tests/tools/Makefile test_build test_run test_coverage

