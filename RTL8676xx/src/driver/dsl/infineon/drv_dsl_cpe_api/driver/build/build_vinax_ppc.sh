#! /bin/sh

ACT_PATH=`pwd`

# convert relative path to absolute
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"

ADD_DRV_CFLAGS="-DMODULE -ggdb -O1"

BUILD_ENV=open_wrt
export BUILD_ENV

# call build script with defined settings
cd `dirname "$ABS_PATH"`
SCRIPT="$ABS_PATH/build.sh"
./build.sh $SCRIPT $1 \
    --arch=powerpc \
    --add-drv-cflags="$ADD_DRV_CFLAGS" \
    --enable-linux-26 \
    --device=vinax \
    --channels=1 \
    --inc-debug=1 \
    --inc-adsl-mib=0 \
    --inc-adsl-led=0 \
    --inc-dsl-ceoc=0 \
    --inc-dsl-static-delt=0 \
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
