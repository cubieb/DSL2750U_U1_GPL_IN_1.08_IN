#! /bin/sh


if [ "$1" == "" ]; then
    echo "Usage: $0 <bin-file>"
    exit 1
fi

ACT_PATH=`pwd`

# convert relative path to absolute
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH_SCRIPT="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
DIR_SCRIPT=`dirname "$ABS_PATH_SCRIPT"`
BASE_SCRIPT=`basename "$ABS_PATH_SCRIPT"`

D=`dirname "$1"`
B=`basename "$1"`
ABS_PATH_BIN="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
DIR_BIN=`dirname "$ABS_PATH_BIN"`
BASE_BIN=`basename "$ABS_PATH_BIN"`

~/projects/buildroot/build_mips/staging_dir/bin/mips-linux-objdump -d "$DIR_BIN/$BASE_BIN" | "$DIR_SCRIPT/checkstack.pl" mips
