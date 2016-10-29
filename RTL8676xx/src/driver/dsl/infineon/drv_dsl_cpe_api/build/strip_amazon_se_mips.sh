#! /bin/sh

ARCH='mips'
BUILD_ROOT=~/projects/buildroot/build_$ARCH

# set home path of DSL CPE API (convert relative path to absolute is necessary!)
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
MYHOME=`dirname "$ABS_PATH"`/..

$BUILD_ROOT/staging_dir/bin/$ARCH-linux-uclibc-strip $MYHOME/build_amazon_se/src/drv_dsl_cpe_api --strip-unneeded
#$BUILD_ROOT/staging_dir/bin/$ARCH-linux-uclibc-strip $MYHOME/build_amazon_se/src/drv_dsl_cpe_api --strip-debug

