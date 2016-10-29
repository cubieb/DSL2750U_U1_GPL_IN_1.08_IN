#! /bin/sh

usage()
{
    echo "Usage: $0 --device=<danube|vinax|amazon-se>"
    exit 1
}

if [ "$1" == "" ]; then
   usage
fi

while test $# != 0
do
  case $1 in
  --*=*)
    ac_option=`expr "x$1" : 'x\([^=]*\)='`
    ac_optarg=`expr "x$1" : 'x[^=]*=\(.*\)'`
    ;;
  *) # This is not an option
     ac_option=$1
     ac_need_defaults=false;;
  esac

  case $ac_option in
  --help | --hel | --he | --h | -h )
    usage
    exit 1 ;;
  --device | --dev )
    case $ac_optarg in
      danube | vinax | amazon-se )
        DEVICE="$ac_optarg";;
      *)
        echo "error: unrecognized device: $1"
        exit 1 ;;
    esac ;;

  # This is an error.
  -*)
    echo "error: unrecognized option: $1"
    exit 1 ;;
  esac
  shift
done

ACT_PATH=`pwd`
LOG_FILE=build_mips_all_"$DEVICE"_log.txt
BIN_SIZE=~/projects/buildroot/build_mips/staging_dir/bin/mips-linux-size
CHECKSTACK=./../scripts/mips_checkstack.sh
BIN_OUT_DIR=../build_$DEVICE/src
BIN_OUT_FILE=drv_dsl_cpe_api

#BUILD_ENV=buildroot
BUILD_ENV=open_wrt
export BUILD_ENV

# convert relative path to absolute
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"

cd `dirname "$ABS_PATH"`

echo "**************************************************************" > $LOG_FILE
echo "*                        lite build                          *" >> $LOG_FILE
echo "**************************************************************" >> $LOG_FILE
echo "Build configuration:" >> $LOG_FILE
./build_mips_cfg.sh cfg_lite.sh $DEVICE $LOG_FILE
echo "--------------------------------------------------------" >> $LOG_FILE
echo "Size of generated objects:" >> $LOG_FILE
find $BIN_OUT_DIR/$BIN_OUT_FILE-*.o -type f -print | xargs "$BIN_SIZE" >> $LOG_FILE
echo "Total size of generated binary:" >> $LOG_FILE
$BIN_SIZE $BIN_OUT_DIR/$BIN_OUT_FILE >> $LOG_FILE
echo "--------------------------------------------------------" >> $LOG_FILE
echo "Checkstack:" >> $LOG_FILE
./../scripts/mips_checkstack.sh ../build_$DEVICE/src/drv_dsl_cpe_api >> $LOG_FILE
echo "" >> $LOG_FILE

echo "**************************************************************" >> $LOG_FILE
echo "*                footprint optimized build                   *" >> $LOG_FILE
echo "**************************************************************" >> $LOG_FILE
echo "Build configuration:" >> $LOG_FILE
./build_mips_cfg.sh cfg_footprint.sh $DEVICE $LOG_FILE
echo "--------------------------------------------------------" >> $LOG_FILE
echo "Size of generated objects:" >> $LOG_FILE
find $BIN_OUT_DIR/$BIN_OUT_FILE-*.o -type f -print | xargs "$BIN_SIZE" >> $LOG_FILE
echo "Total size of generated binary:" >> $LOG_FILE
$BIN_SIZE $BIN_OUT_DIR/$BIN_OUT_FILE >> $LOG_FILE
echo "--------------------------------------------------------" >> $LOG_FILE
echo "Checkstack:" >> $LOG_FILE
$CHECKSTACK $BIN_OUT_DIR/$BIN_OUT_FILE >> $LOG_FILE
echo "" >> $LOG_FILE

echo "**************************************************************" >> $LOG_FILE
echo "*                       typical build                        *" >> $LOG_FILE
echo "**************************************************************" >> $LOG_FILE
echo "Build configuration:" >> $LOG_FILE
./build_mips_cfg.sh cfg_typical.sh $DEVICE $LOG_FILE
echo "--------------------------------------------------------------" >> $LOG_FILE
echo "Size of generated objects:" >> $LOG_FILE
find $BIN_OUT_DIR/$BIN_OUT_FILE-*.o -type f -print | xargs "$BIN_SIZE" >> $LOG_FILE
echo "Total size of generated binary:" >> $LOG_FILE
$BIN_SIZE $BIN_OUT_DIR/$BIN_OUT_FILE >> $LOG_FILE
echo "--------------------------------------------------------------" >> $LOG_FILE
echo "Checkstack:" >> $LOG_FILE
./../scripts/mips_checkstack.sh ../build_$DEVICE/src/drv_dsl_cpe_api >> $LOG_FILE
echo "" >> $LOG_FILE

echo "**************************************************************" >> $LOG_FILE
echo "*                         full build                         *" >> $LOG_FILE
echo "**************************************************************" >> $LOG_FILE
echo "Build configuration:" >> $LOG_FILE
./build_mips_cfg.sh cfg_full.sh $DEVICE $LOG_FILE
echo "--------------------------------------------------------------" >> $LOG_FILE
echo "Size of generated objects:" >> $LOG_FILE
find $BIN_OUT_DIR/$BIN_OUT_FILE-*.o -type f -print | xargs "$BIN_SIZE" >> $LOG_FILE
echo "Total size of generated binary:" >> $LOG_FILE
$BIN_SIZE $BIN_OUT_DIR/$BIN_OUT_FILE >> $LOG_FILE
echo "--------------------------------------------------------------" >> $LOG_FILE
echo "Checkstack:" >> $LOG_FILE
./../scripts/mips_checkstack.sh ../build_$DEVICE/src/drv_dsl_cpe_api >> $LOG_FILE
echo "" >> $LOG_FILE
