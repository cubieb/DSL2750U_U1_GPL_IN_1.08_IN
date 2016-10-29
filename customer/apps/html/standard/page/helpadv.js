
function uiOnload(){
	if(userLevel == 2)
		{
		   $('div_DDNS').style.display = 'none';
	          $('div_PortForwarding').style.display = 'none';
		   $('div_ParentalControl').style.display = 'none';
		   $('div_FilteringOptions').style.display = 'none';
		   $('div_FirewallSettings').style.display = 'none';
		   $('div_NetworkTools').style.display = 'none';

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

