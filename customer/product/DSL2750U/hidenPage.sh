#!/bin/sh
version=$1
auth=$2
lastRev=$3
revurl=$4
flashList=$5

date=`date -R|cut -b1-26`
echo $date > build/romfs/rootfs/usr/www/firmware_date.txt
echo "" > test_version.html

echo -e "	<td>\n\
			<p>Version Information</p>\n\
			<p width=\"16%\" align=\"left\">Version:&nbsp;$version</p>\n \
                   <p>Subversion rev:&nbsp;$lastRev</p>\n \
                   <p>Build By:&nbsp;$auth</p>\n \
                   <p>Build Date:&nbsp;$date</p>\n \
                   <p>Changed Rev URL:&nbsp;$revurl</p>\n \
				   <p>Flash support List:&nbsp;$flashList</p>\n \
		 </td>\n\
	</tr>\n\
</table>\n" >> test_version.html

mv test_version.html build/romfs/rootfs/usr/www/ -f