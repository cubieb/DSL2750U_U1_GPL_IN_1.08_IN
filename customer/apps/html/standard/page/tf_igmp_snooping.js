//IGMP SNOOPING
<?mget :InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop. "Enabled AgeEnabled LastMemberQueryInterval HostTimeout MrouterTimeout LeaveTimeout MaxGroups"
`	var G_Enabled = "$01";
	var G_AgeEnabled = "$02";
	var G_LastMemberQueryInterval = "$03";
	var G_HostTimeout = "$04";
	var G_MrouterTimeout = "$05";
	var G_LeaveTimeout = "$06";
	var G_MaxGroups = "$07";
`?>

function uiOnload(){
	//Form.Checkbox('Enabled',G_Enabled);
	if(G_Enabled == '1')
	{
	    $('Enabled').checked = true;
	}
	else
	{
	    $('Enabled').checked = false;
	}
	if(G_AgeEnabled == '1')
	{
	    $('AgeEnabled').checked = true;
	}
	else
	{
	    $('AgeEnabled').checked = false;
	}
	$('LastMemberQueryInterval').value = G_LastMemberQueryInterval;
	$('HostTimeout').value = G_HostTimeout;
	$('MrouterTimeout').value = G_MrouterTimeout;
	$('LeaveTimeout').value = G_LeaveTimeout;
	$('MaxGroups').value = G_MaxGroups;
	//Form.Checkbox('INPUT_ForbidPhyPort1',G_ForbidPhyPort1);
	//Form.Checkbox('INPUT_ForbidPhyPort2',G_ForbidPhyPort2);
	//Form.Checkbox('INPUT_ForbidPhyPort3',G_ForbidPhyPort3);
	//Form.Checkbox('INPUT_ForbidPhyPort4',G_ForbidPhyPort4);
	//dealWithError();
}

function uiSubmit(){
	$H({
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.Enabled':Form.Checkbox('Enabled'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.AgeEnabled':Form.Checkbox('AgeEnabled'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.LastMemberQueryInterval' : $('LastMemberQueryInterval').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.HostTimeout' : $('HostTimeout').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.MrouterTimeout' : $('MrouterTimeout').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.LeaveTimeout' : $('LeaveTimeout').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.MaxGroups' : $('MaxGroups').value,
		//':InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping.ForbidPhyPort1':Form.Checkbox('INPUT_ForbidPhyPort1'),
		//':InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping.ForbidPhyPort2':Form.Checkbox('INPUT_ForbidPhyPort2'),
		//':InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping.ForbidPhyPort3':Form.Checkbox('INPUT_ForbidPhyPort3'),
		//':InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping.ForbidPhyPort4':Form.Checkbox('INPUT_ForbidPhyPort4'),
		'obj-action'   : 'set',
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}



function dealWithError(){
	if(G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
