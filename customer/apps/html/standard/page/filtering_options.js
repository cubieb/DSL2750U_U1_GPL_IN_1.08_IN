//wireless entrance
function toInboundIpfiltering(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=filtering_options&var:subpage=inbound_ipfiltering';
}

function toOutboundIpfiltering(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=filtering_options&var:subpage=outbound_ipfiltering';
}

function toIPList(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=filtering_options&var:subpage=ip_list';
}

function toTFIPList(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=filtering_options&var:subpage=tf_ipsecurity';
}

function toBridgeList(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=filtering_options&var:subpage=bridge_list';
}