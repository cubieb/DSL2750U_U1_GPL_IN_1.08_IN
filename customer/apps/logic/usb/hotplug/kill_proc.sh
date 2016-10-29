#!/bin/sh

MOUNT_POINT=$*


for i in $(ls /proc/ | grep '^[0-9]\+')
do
   SUM=`ls -l /proc/${i}/cwd | grep -c ${MOUNT_POINT}`
   if [ $SUM -gt 0 ]
     then
       kill -9 $i
   fi
done
