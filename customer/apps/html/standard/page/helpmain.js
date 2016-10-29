
function uiOnload(){
	if(userLevel == 2)
		{
		   $('div_FirmwareUpdate').style.display = 'none';
	          $('div_Diagnostics').style.display = 'none';
		   $('div_SystemLog').style.display = 'none';

		}
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

