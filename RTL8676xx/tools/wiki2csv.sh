#!/bin/sh

if [ $# -ne 1 ];then
echo "error format!please use like this:"
echo "csv2wiki.sh fileName"
exit
fi

ErrorInfo()
{
echo "format error,please check your wiki file"
exit
}

readline()
{
read line
if [ $? -ne 0 ];then
  echo $CurLineInfo
  exit
fi
}

line=" "

sed 's/,/£¬/g' $1|sed '/^$/d'>.tempcsv
CurItem=0;
while [ 1 -eq 1 ];
do
   if [ $CurItem -eq 0 ];then
     read line
   fi
 
   CurItem=`expr $CurItem + 1`
   #CurLineInfo=$CurItem

   echo $line|grep '^=='>/dev/null;
   while [ $? -ne 0 ]
   do
     echo $line|grep "^#">/dev/null;
     if [ $? -ne 0  ];then
      CurLineInfo=$CurLineInfo`echo $line|sed 's/\r//g'|sed 's/\n//g'`
     fi
     read line
   done
   
   if [ $CurItem -ne 1 ];then
      echo $CurLineInfo
      CurLineInfo=""
   fi

   CurLineInfo=$CurItem  
 
   echo $line |grep "===">/dev/null;
   if [ $? -ne 0 ];then  
     echo $line |grep "==">/dev/null;
     if [ $? -eq 0 ];then
       info=`echo $line|sed 's/==//g'|sed 's/\r//g'|sed 's/\n//g'`;
       CurInfo=$info;
       CurLineInfo=$CurLineInfo","$info;
     else
       CurLineInfo=$CurLineInfo",";
     fi
     read line     
   else
     CurLineInfo=$CurLineInfo","$CurInfo;
   fi

   CurLineInfo=$CurLineInfo",ST-"$CurItem
   
   #read line
   info=`echo $line |sed 's/===//g'|sed 's/\n//g'|sed 's/\r//g'`;
   CurLineInfo=$CurLineInfo","$info
   
   CurLineInfo=$CurLineInfo",¸ß"   

   read line
   echo $line|grep -v "^#:\|^##"|grep "^#">/dev/null;
   if [ $? -eq 0  ];then
      CurLineInfo=$CurLineInfo","
      #CurLineInfo=$CurLineInfo","`echo $line|sed 's/^#//g'`
   else
     ErrorInfo; 
   fi 

   read line
   echo $line|grep '^#:\|^##'>/dev/null;
   while [ $? -eq 0 ]
   do
     CurLineInfo=$CurLineInfo`echo $line|sed 's/^#://g'|sed 's/^##//g'|sed 's/\n//g'|sed 's/\r//g'`
     read line
     echo $line|grep '^#:\|^##'>/dev/null;
   done

   echo $line|grep -v "^#:\|^##"|grep "^#">/dev/null;
   if [ $? -eq 0  ];then
      CurLineInfo=$CurLineInfo","
      #CurLineInfo=$CurLineInfo","`echo $line|sed 's/^#//g'|sed 's/\n//g'|sed 's/\r//g'`
   else
     ErrorInfo;
   fi 
   
   read line
   echo $line|grep '^#:\|^##'>/dev/null;
   while [ $? -eq 0 ]
   do
     CurLineInfo=$CurLineInfo`echo $line|sed 's/^#://g'|sed 's/^##//g'|sed 's/\n//g'|sed 's/\r//g'`
     read line
     echo $line|grep '^#:\|^##'>/dev/null;
   done

   echo $line|grep -v "^#:\|^##"|grep "^#">/dev/null;
   if [ $? -eq 0  ];then
      CurLineInfo=$CurLineInfo","
      #CurLineInfo=$CurLineInfo","`echo $line|sed 's/^#//g'|sed 's/\n//g'|sed 's/\r//g'`
   else
     ErrorInfo;
   fi

   read line
   echo $line|grep '^#:\|^##'>/dev/null;
   while [ $? -eq 0 ]
   do
     CurLineInfo=$CurLineInfo`echo $line|sed 's/^#://g'|sed 's/^##//g'|sed 's/\n//g'|sed 's/\r//g'`
     readline;
     echo $line|grep '^#:\|^##'>/dev/null;
   done

   #echo $CurLineInfo
	
done<.tempcsv

echo $CurLineInfo

rm -rf .tempcsv
