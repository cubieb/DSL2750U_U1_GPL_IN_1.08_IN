#!/bin/sh
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2006 Fokus Fraunhofer <carsten.tittel@fokus.fraunhofer.de>

alias debug=${DEBUG:-:}
alias mount='busybox mount'

# newline
N="
"

_C=0
NO_EXPORT=1
LOAD_STATE=1
LIST_SEP=" "

load_modules() {
	[ -d /etc/modules.d ] && {
        echo "=============Begin Load Modules=============="
	    #cd /etc/modules.d
	    #sed 's/^[^#]/insmod &/' $* | ash 2>&- || :
        for mod in $(cat $* 2>/dev/null | grep -v "^#");do
            echo modprobe $mod
            modprobe $mod || insmod $mod
        done
        echo "=============End Load Modules================"
	}
}

include() {
	local file
	
	for file in $(ls $1/*.sh 2>/dev/null); do
		. $file
	done
}


