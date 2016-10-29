#!/bin/sh

if [ $# -ne 1 ];then
echo "error format!please use like this:"
echo "csv2wiki.sh fileName"
exit
fi

./bin/formatcsv $1 9|grep -v ',,,,,,,,' |sed '1d'>.tempcsv

while read line;
do 
   
   info=`echo $line|awk -F, '{printf $2}'`;
   if [ "$CurInfo" != "$info" ]; then  
    echo "=="$info"==" 
    CurInfo=$info
   fi

   info=`echo $line|awk -F, '{printf $4}'`;
   echo "==="$info"===" 

   info=`echo $line|awk -F, '{printf $6}'`;
   echo "# '''预设条件'''"
   echo "#:"$info 

   info=`echo $line|awk -F, '{printf $7}'`;
   echo "# '''操作步骤'''"
   echo "#:"$info 

   info=`echo $line|awk -F, '{printf $8}'`;
   echo "# '''预期结果'''"
   echo "#:"$info 

done<.tempcsv

rm -rf .tempcsv
