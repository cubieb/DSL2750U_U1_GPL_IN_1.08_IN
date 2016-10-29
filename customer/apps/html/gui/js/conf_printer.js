function printerReady() {
	setPrinterDownload();
	//setMACimg();
	$('#accept_printer').click(function(){
		printAccept();
	});
	effectHover('print', 'print_icon');
}

function printAccept() {
	closeDialog();
}
function setPrinterDownload() {
	var href=getPrinterHref();
	$("a#down_link").each( function(i){ $(this).attr('href',href);});
}

function getPrinterHref() {
	var navInfo = window.navigator.appVersion.toLowerCase();  
	var href = 'scripts/installPrinter.exe';
	if(navInfo.indexOf('win') != -1){  
		href = 'scripts/installPrinter.exe';  
	}  
	else if(navInfo.indexOf('linux') != -1){  
		href = '????????';  
	}  
	else if(navInfo.indexOf('mac') != -1){  
		href = 'scripts/installPrinter.mac';  
	}
	return href;  
}
/*
function setMACimg() {
	$("#print_img").attr('src','./images/networkMap/MACPrinter_'+LANG+'.png');
}*/