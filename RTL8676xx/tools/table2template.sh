#!/bin/sh
while read line;
do 
 if [ "$line" == "|-" ]; then 
   read line1;
   type=`echo $line1|sed 's/^|//g'`;
   if [ $type"-" != $old_type"-" ];then
     echo "=="$type"=="; 
     old_type=$type
   fi
   read line2;
   info=`echo $line2|sed 's/^|//g'`;
   echo "==="$info"===";
 
   read line3;
   info=`echo $line3|sed 's/^|//g'`;
   echo "# '''预设条件'''";
   echo "#:"$info; 

   read line4;
   echo $line4|grep "^|">/dev/null;

   while [ $? -ne 0  ]
   do
      echo "#:"$line4;
      read line4;
      echo $line4|grep "^|">/dev/null;
   done
 
   info=`echo $line4|sed 's/^|//g'`;
   echo "# '''操作步骤'''";
   echo "#:"$info ;
 

   read line5;
   echo $line5|grep "^|">/dev/null;
   while [ $? -ne 0  ]
   do
      echo "#:"$line5
      read line5;
      echo $line5|grep "^|">/dev/null;
   done

   info=`echo $line5|sed 's/^|//g'`;
   echo "# '''预期结果'''";
   echo "#:"$info ;

 else
      info=`echo $line|sed 's/^|//g'`;
      echo "#:"$info ;
 fi
done<$1
