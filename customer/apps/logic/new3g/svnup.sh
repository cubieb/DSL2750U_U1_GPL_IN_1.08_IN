#!/bin/sh
#this tools is for uodate the 3g-mngr to main line
#create by scb 2011-8-24

UPDATE_FILES = ""

#open debug log
#set -x

if [ ! -z "$1" ] && [ ! -z "$2" ]; then
	rm -rf tmp_3g-mngr tmp_3g-mngr1
	mkdir -p tmp_3g-mngr tmp_3g-mngr1
	echo "check out 3g-mngr code"
	cd tmp_3g-mngr
	svn co "$2"

	cd ..
	cp -rf 3g-mngr tmp_3g-mngr1
	cd tmp_3g-mngr1/3g-mngr
	#show current path
	pwd
	../../rmsvn.sh  ./
	make clean
	cd ..
	tar -cvf 3g-mngr-tmp.tar 3g-mngr

	cp 3g-mngr-tmp.tar ../tmp_3g-mngr/
	cd ../tmp_3g-mngr
	tar -xvf 3g-mngr-tmp.tar

	cd 3g-mngr
	echo "update to $(svn info) "
	svn st -q >tmp

	cat tmp|sed -n '/.*\.[ch]$/p'>tmp1
	cat tmp|sed -n '/Makefile$/p'>>tmp1

	UPDATE_FILES=$(cat tmp1|awk '{printf("%s ", $2);}')
	if [ ! -z "$UPDATE_FILES" ]; then
		echo "svn ci -m \"$1\" $UPDATE_FILES "
		svn ci -m "$1" $UPDATE_FILES
	else
		echo "No file to update!"
	fi

	rm -rf tmp tmp1
	cd ../..
	rm -rf tmp_3g-mngr tmp_3g-mngr1
else
	echo "svnup 'log' 'svn_path' "
fi
