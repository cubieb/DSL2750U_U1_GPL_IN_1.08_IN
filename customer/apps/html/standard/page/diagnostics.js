function DSLtest(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=maintenance&var:page=diagnostics&var:subpage=DSLtest';
}
<?if eq 1 $var:mod_TRACERT
`
function Traceroute(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=maintenance&var:page=diagnostics&var:subpage=traceroute';;
}
`?>