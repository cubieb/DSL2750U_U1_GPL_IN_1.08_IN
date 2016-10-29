#!/bin/sh
#this tools is for uodown the 3g-mngr to main line
#Create by scb at 2011-8-24

#open debug
#set -x

if [ ! -z "$1" ] && [ ! -z "$2" ]; then
	rm -rf tmp
	mkdir tmp

	cd tmp
	svn co $2
	if [ ! -d "3g-mngr" ] ; then
		echo "svn co fail"
		cd ..
		rm -rf tmp 
		exit 0;
	fi
	cd ..

	svn del 3g-mngr
	sleep 1;
	svn ci -m "$1" 3g-mngr

	cp -rf tmp/3g-mngr ./
	./rmsvn.sh 3g-mngr
	cd 3g-mngr
	make clean
	cd ..

	svn add 3g-mngr
	echo "svn info:"
	echo "$(svn st -q)"

	svn ci -m "$1" 3g-mngr	
	rm -rf tmp		
else
	echo "svndown svn_path ci_log"
fi

exit 0;
