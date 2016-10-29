
<?mget :InternetGatewayDevice.ManagementServer. "X_TWSZ-COM_CAname X_TWSZ-COM_CAsubjet X_TWSZ-COM_CAtype X_TWSZ-COM_CAFilePath"            
`       var G_CAname        = "$01"; 
		var G_CAsubjet      = "$02";
		var G_CAtype        = "$03";
		var G_CAFilePath    = "$04";
`?>

function uiOnload(){
	
  if(G_CAFilePath =="")
  {
     return;
  }
  else
  {  
     $('InputCertificate').disabled = true;
     createTable();
  }
}



function createTable(){
	
      Table.Clear('tablecalist');
	  var array_value = [];
	  array_value[0]=[G_CAname,
						   G_CAsubjet.ellipsis(30),
						   G_CAtype.ellipsis(20),
						   "<input type='button'  name='Delete' value = 'Delete' onClick='todeletecafile()'>"
							];
	 $T('tablecalist',array_value);
	  $('tablecalist_' + 0 + '1').title = G_CAsubjet;
	   $('tablecalist_' + 0 + '2').title = G_CAtype;

}
	
	
function 	todeletecafile(){
	
	$("InputCertificate").disabled = false;	
	$H({	
		':InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath' : '',
		'obj-action': 'set',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:menu'  : 'advanced',
		'var:page'  : 'certificate_ca',
		'var:errorpage' : 'certificate_ca',
		'var:CacheLastData' : ViewState.Save()
	});
	$('uiPostForm').submit();
	
}
	
function toCertificate_ca_input(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=certificate_ca_input';
}

function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}
addListeners(uiOnload,dealWithError);