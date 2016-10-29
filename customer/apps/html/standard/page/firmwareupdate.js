var progress = 0;
var firmwareversion = "<?get :InternetGatewayDevice.DeviceInfo.ModemFirmwareVersion?>";
var softwareversion = "<?get :InternetGatewayDevice.DeviceInfo.SoftwareVersion?>";
	
function uiOnload(){
	$('sending').style.display ='none';
    $('lang_firmware1').innerHTML = firmwareversion;
    $('lang_software1').innerHTML = softwareversion;		
	$('lang_datetime').innerHTML = G_Date;	
	dealWithError();
	$('CurrentSoftware_info').style.display = 'none';
}
	
function isInProgress() {
	var node_value = document.getElementById('firmware').value;
	if(node_value == ""){
		alert(uError.lang_select_update_file);
		return false;
	}
	
	if (progress == 0 )  {
		progress = 1;
		$('select_file').style.display ='none';
		$('sending').style.display ='block';
		return true;
	}
	alert(uError.lang_img_file_uploading);
	return false;
}

function dealWithError()
{
	if (G_Error != 1)
	{
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

function uiGetLanguage(){
	for(var i = 0; i < node_self_menu.length; i++){
		$(node_self_menu[i]).innerHTML = self_menu[i];
	}
	chg_language(firmware_language);
}

addListeners(uiOnload);
