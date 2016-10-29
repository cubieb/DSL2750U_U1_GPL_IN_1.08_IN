//wizard 快速配�?

function uiNextPage(){
	$H({
	    'var:menu'	: 'setup',
		'var:subpage'  : 'wizentrance',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html'
		
	},true);
	
	$('uiPostForm').submit();
}
