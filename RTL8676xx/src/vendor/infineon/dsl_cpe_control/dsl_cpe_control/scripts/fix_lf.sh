#! /bin/bash

FILES=`find . -name *.[ch] -exec file {} \; | grep CRLF | cut -d":" -f1`

# check the files out
cleartool co -nc $FILES

# convert the files to UNIX line feeds
dos2unix $FILES