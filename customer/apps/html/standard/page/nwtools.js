
function toTr064(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=tr064';
}
<?if eq 1 $var:mod_TR069
`
function toTr069(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=tr069';
}

function toCertificates(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=certificate';
}
`?>
function toPortMapping(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=portmapping';
}
function toIgmpproxy(){
	<?if eq 1 $var:mod_IGMP
	`
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=igmp';
	`?>
		
	<?if eq 1 $var:mod_TF_IGMP
	`
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=tf_igmp';
	`?>
}
function toIgmpsnooping(){
	<?if eq 1 $var:mod_IGMP
	`	
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=igmp_snooping';
	`?>
		
	<?if eq 1 $var:mod_TF_IGMP
	`	
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=tf_igmp_snooping';
	`?>
		
}
function toStaticIgmp(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=igmp_static';
}

function toMldCfg(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=mld_cfg';
}

function toUpnp(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=upnp';
}
function toAdsl(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=adslset';
}
function toPvcsearch(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=pvcsearch';
}
function toSnmp(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=snmp';
}
function toprinter(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=print';
}
function toLanPortVlan(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=lpvlan';
}

