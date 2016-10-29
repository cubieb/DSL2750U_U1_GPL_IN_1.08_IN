#! /bin/sh

# Used command line argument:
# $1: Specifies the file that includes the used configure options
# $2: Specifies the device that should be used for build (i.e. danube)
# $3: Specifies the log file that is currently used to create statistics

ACT_PATH=`pwd`
ACT_DEVICE="--device=$2"
OPTION=""
TMP_FILE=tmp1.txt
TMP_FILE2=tmp2.txt

# statically used cflags for MIPS build
ADD_APPL_CFLAGS="-Os -G 0 -mlong-calls -finline-limit=100000 -mabi=32 \
 -march=mips32 -Wa,-32 -Wa,-march=mips32 -Wa,-mips32 -Wa,--trap"

delete_tmp()
{
   rm -f $TMP_FILE
   mv $TMP_FILE2 $TMP_FILE
}

remove_empty_lines()
{
   sed "/^ *$/ d" $TMP_FILE > $TMP_FILE2
   delete_tmp
}

remove_option()
{
   sed "s/$OPTION//g" $TMP_FILE > $TMP_FILE2
   delete_tmp
}

#generate temp file for processing
cp $1 $TMP_FILE

# remove options that are invalid for current device
if [ "$2" = "vinax" ]; then
   OPTION="--inc-script=."; remove_option
fi
remove_empty_lines

# dump configure options to log file if specified
if [ "$3" != "" ]; then
    echo $ADD_APPL_CFLAGS >> $3
    cat $TMP_FILE >> $3
fi

# reformat configure options within given config file to be used within
# build process
OPTION="\n"; remove_option
remove_empty_lines

# read final configuration and store it in variable for build
CFG_OPTIONS=`cat "$TMP_FILE"`

# remove temp file of processing
rm $TMP_FILE


# convert relative path to absolute
D=`dirname "$0"`
B=`basename "$0"`
ABS_PATH="`cd \"$D\" 2>/dev/null && pwd || echo \"$D\"`/$B"
DIR=`dirname "$ABS_PATH"`

# call build script with defined settings
cd $DIR
SCRIPT="$DIR/build.sh"

./build.sh $SCRIPT $ACT_DEVICE --arch=mips --add-appl-cflags="$ADD_APPL_CFLAGS" $CFG_OPTIONS

cd $ACT_PATH
