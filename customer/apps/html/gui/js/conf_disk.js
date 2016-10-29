
function usbAccept() {
	closeDialog();
}

function collectData(serialNumber,id) {
	getUSBInfo(serialNumber,id,processData);
}

function processData(success, usbinfo, error) {
	var nameUsb;
	var vendor;
	var freeSpace;
	var usedSpace;
	if (success) {
		$("#usbname").html("<span><b>"+usbinfo.nameUSB+"</b></span>");
		$("#usbvendor").html("<span>"+usbinfo.vendor+"</span>");
		processPartitions(usbinfo.PARTITION);		
	} else {
		
	}
}

function processPartitions(partitions) {
	var part;
	var ind=0;
	var free=0;
	var used=0;
	for (;ind<partitions.length;ind++) {
		part=partitions[ind];
		free=free+part.freeSpace;
		used=used+part.usedSpace;
	}
	editSpaces(free,'#usb_freespace');
	editSpaces(used,'#usb_usedspace');
	drawPie([free,used]);
}

function editSpaces(bytes,id) {
	var unit="bytes";
	if (bytes>1023) {
		bytes=bytes/1024;
		bytes=Math.round(bytes*100)/100;
		unit="Kbytes";
		if (bytes>1023) {
			bytes=bytes/1024;
			bytes=Math.round(bytes*100)/100;
			unit="Mbytes";
			if (bytes>1023) {
				bytes=bytes/1024;
				bytes=Math.round(bytes*100)/100;
				unit="Gbytes";
			}
		}
	}
	$(id).html(bytes+" "+unit);
}

function drawPie(pieData) {

	plot3 = $.jqplot('chart4', [pieData], {
	  series:[
	        {renderer:$.jqplot.PieRenderer
	        , color:'#cccccc'
	        , rendererOptions:{sliceMargin:4, fill:false, shadow:false, lineWidth:4, diameter:90 }
	        }
	    ]
	    ,legend:{show:false, location: 'w'}
	    ,grid: {
	            drawGridLines: false        
	            ,gridLineColor: '#ffffff'
	            ,background: '#ffffff'
	        ,borderColor: '#ffffff'
	        ,borderWidth: 0
	        ,shadow: false
	    }
	});
}

function setDiskDownload() {	
		var os=getOS();
		var href='';var key='';
		if (os=='win') {
			href='scripts/openUSB.exe';
			key='CONF_DISK_DESC_WIN';
		}
		else if (os=='mac') {
			href='scripts/ClickToOpenUSB.dmg';
			key='CONF_DISK_DESC_MAC';
		}
		else if (os=='linux') {
			href='scripts/openUSB.exe';	//never happens
			key='CONF_DISK_DESC_WIN';
		}
		$("a#usbdownlink").each( function(i){ $(this).attr('href',href);});
		$("#desc_cont").attr('key',key);
		reloadTextsDiv("desc_cont");
}
