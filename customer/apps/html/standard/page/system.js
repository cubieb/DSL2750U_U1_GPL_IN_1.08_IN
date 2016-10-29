// JavaScript Document

var progress = 0;

function isInProgress() {
	var node_value = $('config').value;
	if(node_value == ""){
		alert(data_language.dynamic.lang_select_update_file);
		return false;
	}
	
	if (progress == 0 )  {
		progress = 1;
		return true;
	}
	alert(data_language.dynamic.lang_config_file_uploading);
	return false;
}


function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}


function saveConfiguration(){
	
	$H({
	  ':InternetGatewayDevice.DeviceConfig.PersistentData' : 'SaveConfig',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'system',
		'var:errorpage' : 'system',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true,'uiPostForm');

	$('uiPostForm').submit();	
	
}

function CofirmRestore(){
	var confirmRestore = 'You confirm that you want to restore the default configuration?';
	 if(!confirm(confirmRestore)){
		return false;
	 }else{
		return true;
	 }	
}

function rebootFromTheLastSavedSettings(){
	
	$H({
	  ':InternetGatewayDevice.DeviceConfig.PersistentData': 'RebootFromLastConfig',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'system',
		'var:errorpage' : 'system',
		'getpage'       :  'html/page/restarting.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true,'uiPostForm');

	$('uiPostForm').submit();	
}
