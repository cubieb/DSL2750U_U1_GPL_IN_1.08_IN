#! /bin/sh

ACT_PATH=`pwd`

# convert relative path to absolute
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"


ADD_APPL_CFLAGS="-G 0 -mlong-calls \
 -finline-limit=100000 -mabi=32 -march=mips32 -Wa,-32 -Wa,-march=mips32 \
 -Wa,-mips32 -Wa,--trap"

# call build script with defined settings
cd `dirname "$ABS_PATH"`
SCRIPT="$ABS_PATH/build.sh"
./build.sh $SCRIPT $1 \
    --arch=mips \
    --add-appl-cflags="$ADD_APPL_CFLAGS" \
    --device=vinax \
    --inc-soap=1 \
    --inc-debug=1 \
    --inc-debug-prints=1 \
    --inc-debug-tool-interface=1\
    --inc-cli=1 \
    --inc-dsl-ceoc=1 \
    --inc-script=1 \
    --inc-dsl-pm=1 \
    --inc-dsl-pm-total-counters=1 \
    --inc-dsl-pm-showtime-counters=1 \
    --inc-dsl-pm-history=1 \
    --inc-dsl-pm-line-counters=1 \
    --inc-dsl-pm-channel-counters=1 \
    --inc-dsl-pm-datapath-counters=1 \
    --inc-dsl-pm-line-thresholds=1 \
    --inc-dsl-pm-channel-thresholds=1 \
    --inc-dsl-pm-datapath-thresholds=1 \
    --inc-dsl-pm-optional-parameters=1
cd $ACT_PATH
