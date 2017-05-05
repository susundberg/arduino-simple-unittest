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


if [[ $2 != *"keep"* ]]; then
   echo "Build clean"
   rm -rf $TARGET_DIR
fi


mkdir -p $TARGET_DIR
cp -a "../src/" "$TARGET_DIR/tests/"
cp -a "$SOURCE_DIR/src" $TARGET_DIR
cp -a "$SOURCE_DIR/tests" $TARGET_DIR
cd $TARGET_DIR
PATCH_FILE="../../$SOURCE_DIR.patch"
if [ -e $PATCH_FILE ]; then
   patch -p0 < $PATCH_FILE
fi

if [[ $2 == *"assume_fail"* ]]; then
   echo "Fail run, assume build ok, running fails"
   make -f tests/tools/Makefile test_build 
   for tb in tests/build/bin/*; do
      set +e
      ./$tb > /dev/null # Hide the output as its hidious since it fails
      ret=$?
      set -e
      if [[ $ret == 0 ]];then 
         echo "The test '$tb' did not fail, though it should!"
         return 1
      fi;
   done;
else
   echo "Normal run - assume all ok"
   make -f tests/tools/Makefile test_build test_run test_coverage
fi

echo "ALL DONE, BYE BYE"
