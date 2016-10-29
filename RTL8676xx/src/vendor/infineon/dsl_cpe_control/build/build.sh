#! /bin/sh

ARCH="mips"
ROOT_DIR=

# set home path of DSL CPE API (convert relative path to absolute is necessary!)
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
MYHOME=`dirname "$ABS_PATH"`/..


# build arguments (and default values if necessary)
DEVICE=
INC_DEBUG=0
INC_DEBUG_PRINTS=0
INC_CLI=0
INC_DSL_CEOC=0
INC_SOAP=0
INC_SCRIPT=0
INC_SCRIPT_NOTIFICATION=0
INC_DSL_PM=0
INC_DSL_PM_CHANNEL_COUNTERS=0
INC_DSL_PM_DATAPATH_COUNTERS=0
INC_DSL_PM_LINE_COUNTERS=0
INC_DSL_PM_CHANNEL_THRESHOLDS=0
INC_DSL_PM_DATAPATH_THRESHOLDS=0
INC_DSL_PM_LINE_THRESHOLDS=0
INC_DSL_PM_TOTAL_COUNTERS=0
INC_DSL_PM_HISTORY=0
INC_DSL_PM_SHOWTIME_COUNTERS=0
INC_DSL_PM_OPTIONAL_PARAMETERS=0
INC_DSL_TCP_DEBUG=0
# additional variables that will be set to fixed values according to given
# device name later on
CHANNELS_PER_LINE=
APPL_CFLAGS=

CONF_DRIVER_INCLUDE=
CONF_DEVICE_DRIVER_INCLUDE=
CONF_APPL_CFLAGS=

EN_DEBUG_PRINTS=
EN_DSL_CEOC=
EN_DSL_PM=
EN_DSL_PM_CHANNEL_COUNTERS=
EN_DSL_PM_DATAPATH_COUNTERS=
EN_DSL_PM_LINE_COUNTERS=
EN_DSL_PM_CHANNEL_THRESHOLDS=
EN_DSL_PM_DATAPATH_THRESHOLDS=
EN_DSL_PM_LINE_THRESHOLDS=
EN_DSL_PM_TOTAL_COUNTERS=
EN_DSL_PM_HISTORY=
EN_DSL_PM_SHOWTIME_COUNTERS=
EN_DSL_TCP_DEBUG=
EN_DSL_PM_OPTIONAL_PARAMETERS=

#############################################################
# helper functions
#############################################################
# print usage description
usage()
{
    echo
    echo "Usage:"
    echo "$0"
    echo "   <--device={dev}>"
    echo "   [--arch={arch}]"
    echo "   [--add-appl-cflags={cflags}]"
    echo "   [--channels={ch}]"
    echo "   [--inc-debug={inc}]"
    echo "   [--inc-debug-prints={inc}]"
    echo "   [--inc-cli={inc}]"
    echo "   [--inc-soap={inc}]"
    echo "   [--inc-script={inc}]"
    echo "   [--inc-script-notification={inc}]"
    echo "   [--inc-dsl-pm={inc}]"
    echo "   [--inc-dsl-pm-channel-counters={inc}]"
    echo "   [--inc-dsl-pm-datapath-counters={inc}]"
    echo "   [--inc-dsl-pm-line-counters={inc}]"
    echo "   [--inc-dsl-pm-channel-thresholds={inc}]"
    echo "   [--inc-dsl-pm-datapath-thresholds={inc}]"
    echo "   [--inc-dsl-pm-line-thresholds={inc}]"
    echo "   [--inc-dsl-pm-total-counters={inc}]"
    echo "   [--inc-dsl-pm-history={inc}]"
    echo "   [--inc-dsl-pm-showtime-counters={inc}]"
    echo "   [--inc-debug-tool-interface={inc}]"
    echo
    echo "with:"
    echo "  dev  (device selection) - danube | vinax | amazon-se"
    echo "  ch  (bearer channels) - 1 | 2"
    echo "  arch (architecture)     - i386 | mips | mipsel | powerpc"
    echo "  cflags (Add CFLAGS)     - any combination of CFLAGS"
    echo "  inc  (include feature)  - 0 | 1  (0:no, 1:yes)"
    echo "remarks:"
    echo "  <..> - necessary parameter"
    echo "  [..] - optional parameter (default value defined)"
    echo
    echo Example: $0 --device=danube

    exit 1
}

#print arguments
print_args()
{
    echo
    echo "User defined configurations"
    echo "==========================="
    echo "DEVICE=$DEVICE"
    echo "ARCH=$ARCH"
    echo "CONF_APPL_CFLAGS=$CONF_APPL_CFLAGS"
    echo "CHANNELS_PER_LINE=$CHANNELS_PER_LINE"
    echo "CONF_DRIVER_INCLUDE=$CONF_DRIVER_INCLUDE"
    echo "CONF_DEVICE_DRIVER_INCLUDE=$CONF_DEVICE_DRIVER_INCLUDE"
    echo "INC_DEBUG=$EN_DEBUG"
    echo "INC_DEBUG_PRINTS=$INC_DEBUG_PRINTS"
    echo "INC_CLI=$EN_CLI"
    echo "INC_DSL_CEOC=$EN_DSL_CEOC"
    echo "INC_SOAP=$EN_SOAP"
    echo "INC_SCRIPT=$EN_SCRIPT"
    echo "INC_SCRIPT_NOTIFICATION=$EN_SCRIPT_NOTIFICATION"
    echo "INC_DSL_PM=$EN_DSL_PM"
    echo "INC_DSL_PM_CHANNEL_COUNTERS=$INC_DSL_PM_CHANNEL_COUNTERS"
    echo "INC_DSL_PM_DATAPATH_COUNTERS=$INC_DSL_PM_DATAPATH_COUNTERS"
    echo "INC_DSL_PM_LINE_COUNTERS=$INC_DSL_PM_LINE_COUNTERS"
    echo "INC_DSL_PM_CHANNEL_THRESHOLDS=$INC_DSL_PM_CHANNEL_THRESHOLDS"
    echo "INC_DSL_PM_DATAPATH_THRESHOLDS=$INC_DSL_PM_DATAPATH_THRESHOLDS"
    echo "INC_DSL_PM_LINE_THRESHOLDS=$INC_DSL_PM_LINE_THRESHOLDS"
    echo "INC_DSL_PM_TOTAL_COUNTERS=$INC_DSL_PM_TOTAL_COUNTERS"
    echo "INC_DSL_PM_HISTORY=$INC_DSL_PM_HISTORY"
    echo "INC_DSL_PM_SHOWTIME_COUNTERS=$INC_DSL_PM_SHOWTIME_COUNTERS"
    echo "INC_DSL_TCP_DEBUG=$EN_DSL_TCP_DEBUG"
    echo "INC_DSL_PM_OPTIONAL_PARAMETERS=$INC_DSL_PM_OPTIONAL_PARAMETERS"
    echo
    echo "Internal variables"
    echo "=================="
    echo "MYHOME=$MYHOME"
    echo "APPL_CFLAGS=$APPL_CFLAGS"
    echo
    echo
}


# check the return value and abort the script in case of error
check_return()
{
    if [ $? -ne 0 ]; then
      cd $MYHOME
      echo package build failed
      exit 1
    fi
}


#############################################################
# scan script arguments
#############################################################
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
  --arch )
    case $ac_optarg in
      i386 | mips | mipsel | powerpc )
        ARCH="$ac_optarg";;
      *)
        echo "error: unrecognized architecture: $1"
        exit 1 ;;
    esac ;;
  --add-appl-cflags )
    CONF_APPL_CFLAGS="$ac_optarg";;
  --device | --dev )
    case $ac_optarg in
      danube | vinax | amazon-se )
        DEVICE="$ac_optarg";;
      *)
        echo "error: unrecognized device: $1"
        exit 1 ;;
    esac ;;
  --drv-include )
    CONF_DRIVER_INCLUDE="$ac_optarg";;
  --device-drv-include )
    CONF_DEVICE_DRIVER_INCLUDE="$ac_optarg";;
  --inc-debug )
    INC_DEBUG="$ac_optarg";;
  --inc-debug-prints )
    INC_DEBUG_PRINTS="$ac_optarg";;
  --channels )
    CHANNELS_PER_LINE="$ac_optarg";;
  --inc-cli )
    INC_CLI="$ac_optarg";;
  --inc-dsl-ceoc )
    INC_DSL_CEOC="$ac_optarg";;
  --inc-soap )
    INC_SOAP="$ac_optarg";;
  --inc-script )
    INC_SCRIPT="$ac_optarg";;
  --inc-script-notification )
    INC_SCRIPT_NOTIFICATION="$ac_optarg";;
  --inc-dsl-pm )
    INC_DSL_PM="$ac_optarg";;
  --inc-dsl-pm-channel-counters )
    INC_DSL_PM_CHANNEL_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-datapath-counters )
    INC_DSL_PM_DATAPATH_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-line-counters )
    INC_DSL_PM_LINE_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-channel-thresholds )
    INC_DSL_PM_CHANNEL_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-datapath-thresholds )
    INC_DSL_PM_DATAPATH_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-line-thresholds )
    INC_DSL_PM_LINE_THRESHOLDS="$ac_optarg";;
  --inc-dsl-pm-total-counters )
    INC_DSL_PM_TOTAL_COUNTERS="$ac_optarg";;
  --inc-dsl-pm-history )
    INC_DSL_PM_HISTORY="$ac_optarg";;
  --inc-dsl-pm-showtime-counters )
    INC_DSL_PM_SHOWTIME_COUNTERS="$ac_optarg";;
  --inc-debug-tool-interface )
    INC_DSL_TCP_DEBUG="$ac_optarg";;
  --inc-dsl-pm-optional-parameters )
    INC_DSL_PM_OPTIONAL_PARAMETERS="$ac_optarg";;

  # This is an error.
  -*)
    echo "error: unrecognized option: $1"
    exit 1 ;;
  esac
  shift
done


if [ "$BUILD_ENV" == "open_wrt" ]; then
	ROOT_DIR=~/projects/open_wrt
else
	ROOT_DIR=~/projects/buildroot
fi

if [ "$ARCH" == "i386" ]; then
    CROSS_PATH=
    CROSS_CC='gcc'
    CROSS_AR='ar'
    CROSS_LD='ld'
    CROSS_STRIP='strip'
    CROSS_HOST='--host=i386-linux'
    CROSS_TARGET='--target=i386-linux'
else
    FILE="$ROOT_DIR"/.build_dir_suffix
    BUILD_DIR_SUFFIX=`cat "$FILE"`
    BUILD_DIR=$ROOT_DIR/build_dirs/build_"$ARCH"_"$BUILD_DIR_SUFFIX"

	case $DEVICE in
	    vinax )
	        KERNEL_INCLUDE=$BUILD_DIR/linux/include
		    CROSS_PATH=$ROOT_DIR/staging_dir_"$ARCH"/"$ARCH"-linux/bin
	        ;;
	    danube )
		    KERNEL_INCLUDE=$ROOT_DIR/linux-2.4.31-easy50712/include
		    CROSS_PATH=$ROOT_DIR/staging_dir/bin/$ARCH-linux-uclibc
	        ;;
	esac

    CROSS_CC=$CROSS_PATH/gcc
    CROSS_AR=$CROSS_PATH/ar
    CROSS_LD=$CROSS_PATH/ld
    CROSS_STRIP=$CROSS_PATH/strip
    CROSS_HOST=--host=$ARCH-linux-uclibc
fi


#############################################################
# sequrity checks and assignment of device specific fixed
# variable settings
#############################################################
# check if device has been specified
if [ "$DEVICE" == " " ]; then
    usage
    exit 1
fi

# set device specific variables here
case $DEVICE in
    danube )
        DEVICE_DRIVER_INCLUDE="$CONF_DEVICE_DRIVER_INCLUDE"
        ;;
    vinax )
         # Device Driver include
         if [ "$CONF_DEVICE_DRIVER_INCLUDE" = "" ]
         then
             DEVICE_DRIVER_INCLUDE="-I$BUILD_DIR/drv_vinax/src"
         fi
        ;;
    amazon-se )
        DEVICE_DRIVER_INCLUDE="$CONF_DEVICE_DRIVER_INCLUDE"
        ;;
esac 

# check configuration of used bearer channels and set default
# value if not already set
if [ "$CHANNELS_PER_LINE" = "" ]
then
    CHANNELS_PER_LINE=1
fi

#############################################################
# set configure strings
#############################################################

# Debug suppport
if [ "$INC_DEBUG" = "1" ]
then
    EN_DEBUG="--enable-debug=yes"
else
    EN_DEBUG="--enable-debug=no"
fi

# Debug prints suppport
if [ "$INC_DEBUG_PRINTS" = "1" ]
then
    EN_DEBUG_PRINTS="--enable-debug-prints=yes"
else
    EN_DEBUG_PRINTS="--enable-debug-prints=no"
fi

# CLI suppport
if [ "$INC_CLI" = "1" ]
then
    EN_CLI="--enable-cli-support=yes"
else
    EN_CLI="--enable-cli-support=no"
fi

# DSL CEOC suppport
if [ "$INC_DSL_CEOC" = "1" ]
then
    EN_DSL_CEOC="--enable-dsl-ceoc"
else
    EN_DSL_CEOC=""
fi

# SOAP suppport
if [ "$INC_SOAP" = "1" ]
then
    EN_SOAP="--enable-soap-support=yes"
else
    EN_SOAP="--enable-soap-support=no"
fi

# Debug scripts suppport
if [ "$INC_SCRIPT" = "1" ]
then
    EN_SCRIPT="--enable-cmv-scripts=yes"
else
    EN_SCRIPT="--enable-cmv-scripts=no"
fi

# Script notification suppport
if [ "$INC_SCRIPT_NOTIFICATION" = "1" ]
then
    EN_SCRIPT_NOTIFICATION="--enable-script-notification=yes"
else
    EN_SCRIPT_NOTIFICATION="--enable-script-notification=no"
fi

# DSL PM suppport
if [ "$INC_DSL_PM" = "1" ]
then
    EN_DSL_PM="--enable-dsl-pm"
else
    EN_DSL_PM=""
fi

# DSL PM channel counters suppport
if [ "$INC_DSL_PM_CHANNEL_COUNTERS" = "1" ]
then
    EN_DSL_PM_CHANNEL_COUNTERS="--enable-dsl-pm-channel-counters"
else
    EN_DSL_PM_CHANNEL_COUNTERS=""
fi

# DSL PM datapath counters suppport
if [ "$INC_DSL_PM_DATAPATH_COUNTERS" = "1" ]
then
    EN_DSL_PM_DATAPATH_COUNTERS="--enable-dsl-pm-datapath-counters"
else
    EN_DSL_PM_DATAPATH_COUNTERS=""
fi

# DSL PM line counters suppport
if [ "$INC_DSL_PM_LINE_COUNTERS" = "1" ]
then
    EN_DSL_PM_LINE_COUNTERS="--enable-dsl-pm-line-counters"
else
    EN_DSL_PM_LINE_COUNTERS=""
fi

# DSL PM channel thresholds suppport
if [ "$INC_DSL_PM_CHANNEL_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_CHANNEL_THRESHOLDS="--enable-dsl-pm-channel-thresholds"
else
    EN_DSL_PM_CHANNEL_THRESHOLDS=""
fi

# DSL PM datapath thresholds suppport
if [ "$INC_DSL_PM_DATAPATH_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_DATAPATH_THRESHOLDS="--enable-dsl-pm-datapath-thresholds"
else
    EN_DSL_PM_DATAPATH_THRESHOLDS=""
fi

# DSL PM line thresholds suppport
if [ "$INC_DSL_PM_LINE_THRESHOLDS" = "1" ]
then
    EN_DSL_PM_LINE_THRESHOLDS="--enable-dsl-pm-line-thresholds"
else
    EN_DSL_PM_LINE_THRESHOLDS=""
fi

# DSL PM total counters suppport
if [ "$INC_DSL_PM_TOTAL_COUNTERS" = "1" ]
then
    EN_DSL_PM_TOTAL_COUNTERS="--enable-dsl-pm-total"
else
    EN_DSL_PM_TOTAL_COUNTERS=""
fi

# DSL PM history suppport
if [ "$INC_DSL_PM_HISTORY" = "1" ]
then
    EN_DSL_PM_HISTORY="--enable-dsl-pm-history"
else
    EN_DSL_PM_HISTORY=""
fi

# DSL PM showtime counters suppport
if [ "$INC_DSL_PM_SHOWTIME_COUNTERS" = "1" ]
then
    EN_DSL_PM_SHOWTIME_COUNTERS="--enable-dsl-pm-showtime"
else
    EN_DSL_PM_SHOWTIME_COUNTERS=""
fi

# DSL PM optional parameters suppport
if [ "$INC_DSL_PM_OPTIONAL_PARAMETERS" = "1" ]
then
    EN_DSL_PM_OPTIONAL_PARAMETERS="--enable-dsl-pm-optional-parameters"
else
    EN_DSL_PM_OPTIONAL_PARAMETERS=""
fi

# DSL TCP debug suppport
if [ "$INC_DSL_TCP_DEBUG" = "1" ]
then
    EN_DSL_TCP_DEBUG="--enable-debug-tool-interface"
else
    EN_DSL_TCP_DEBUG=""
fi

# Driver include
if [ "$CONF_DRIVER_INCLUDE" = "" ]
then
    DRIVER_INCLUDE=-I$MYHOME/../../driver/src/include
fi

# Additional CFLAGS
if [ "$CONF_APPL_CFLAGS" != "" ]
then
    APPL_CFLAGS=$CONF_APPL_CFLAGS
fi


#############################################################
# preparation of configure/make process
#############################################################

# take care that NO makefile exists in the top level directory of the DSL CPE API
#echo clean the package directory
if [ -f $MYHOME/Makefile ]
then
   make -s -C $MYHOME distclean
fi

echo remove the build directories
rm -Rf $MYHOME/build_$DEVICE

echo create the build directories
mkdir -p $MYHOME/build_$DEVICE


#############################################################
# start configure and make process with specified settings
#############################################################

echo "configure $DEVICE"

cd $MYHOME/build_$DEVICE

$MYHOME/configure \
    CC=$CROSS_CC \
    AR=$CROSS_AR \
    LD=$CROSS_LD \
    STRIP=$CROSS_STRIP \
    $CROSS_HOST \
    $CROSS_TARGET \
    --enable-$DEVICE \
    --with-channels-per-line=$CHANNELS_PER_LINE \
    --enable-driver-include="$DRIVER_INCLUDE" \
    --enable-device-driver-include="$DEVICE_DRIVER_INCLUDE" \
    --enable-add-appl-cflags="$APPL_CFLAGS" \
    $CONF_DRIVER_INCLUDE \
    $EN_DEBUG_PRINTS \
    $EN_CLI \
    $EN_DSL_CEOC \
    $EN_SOAP \
    $EN_SCRIPT \
    $EN_SCRIPT_NOTIFICATION \
    $EN_DSL_PM \
    $EN_DSL_PM_CHANNEL_COUNTERS \
    $EN_DSL_PM_DATAPATH_COUNTERS \
    $EN_DSL_PM_LINE_COUNTERS \
    $EN_DSL_PM_CHANNEL_THRESHOLDS \
    $EN_DSL_PM_DATAPATH_THRESHOLDS \
    $EN_DSL_PM_LINE_THRESHOLDS \
    $EN_DSL_PM_TOTAL_COUNTERS \
    $EN_DSL_PM_HISTORY \
    $EN_DSL_PM_SHOWTIME_COUNTERS \
    $EN_DSL_TCP_DEBUG \
    $EN_DSL_PM_OPTIONAL_PARAMETERS

check_return

echo "build $DEVICE"
make -s
check_return

cd $MYHOME

exit 0
