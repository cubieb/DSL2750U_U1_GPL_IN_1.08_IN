#! /bin/sh

ARCH='powerpc'
ROOT_DIR=~/projects/open_wrt
CROSS_PATH=$ROOT_DIR/staging_dir_"$ARCH"/"$ARCH"-linux/bin

# set home path of DSL CPE API (convert relative path to absolute is necessary!)
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
MYHOME=`dirname "$ABS_PATH"`/..

$CROSS_PATH/strip $MYHOME/build_vinax/src/drv_dsl_cpe_api --strip-unneeded
