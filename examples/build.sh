#!/bin/bash


usage_exit()
{
   echo "$0 <target directory>"
}


if [ -z $1 ]; then
  usage_exit;
fi

set -e
set -x


SOURCE_DIR=$1
TARGET_DIR="build/$1"


rm -rf $TARGET_DIR
mkdir -p $TARGET_DIR
cp -a $SOURCE_DIR $TARGET_DIR