#!/bin/sh

#set -x

do_rm() {

	local	LPATH=$(echo $1|sed  's/\(.*\)\/$/\1/g')	

	local	SVN_DIR=$(ls -al $LPATH |sed -n '/.svn/p'|sed 's/.*[ \t]//g' );
	local	SUB_DIRS=$(ls -al $LPATH |sed -n '/^d.*/p'|sed  -n '/[ \t]\.\{1,2\}$/!p'\
				|sed -n '/[ \t]\.svn[ \t]*/!p'|sed 's/.*[ \t]//g');



	echo "======>enter $LPATH"
	
	for svn in $SVN_DIR; do
		echo "======>    rm $LPATH/$svn "
	        rm -rf $LPATH/$svn		
	done

	if [ -z $(echo $SUB_DIRS|awk '{print $1}') ] ; then
		echo "======>leaving  $LPATH"
	        return 0
	fi

	

	for dir in $SUB_DIRS; do
        	do_rm $LPATH/$dir
	done

	echo "======>leaving  $LPATH"

	return 0
}

#main

MYPATH="";

PWD=$(pwd)


if [ $1 = '-h' ] || [ $1 = '--help' ]; then
        echo "rmsvn PATH"
        exit 0;
fi


if [ -z $1 ]; then
	MYPATH=$PWD
else
	if [ -z $(echo $1|sed -n '/^\//p' ) ]; then
		MYPATH=$PWD/$1
	else
		MYPATH=$1
	fi
fi

do_rm $MYPATH

exit 0
