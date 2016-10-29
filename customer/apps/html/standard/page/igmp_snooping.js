//IGMP SNOOPING
<?mget :InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping. "Enable ForbidPhyPort1 ForbidPhyPort2 ForbidPhyPort3 ForbidPhyPort4"
`	var G_DNSRelayEnable = "$01";
	var G_ForbidPhyPort1 = "$02";
	var G_ForbidPhyPort2 = "$03";
	var G_ForbidPhyPort3 = "$04";
	var G_ForbidPhyPort4 = "$05";
`?>

function uiOnload(){
	Form.Checkbox('INPUT_IGMPSnoopingEnable',G_DNSRelayEnable);
	//Form.Checkbox('INPUT_ForbidPhyPort1',G_ForbidPhyPort1);
	//Form.Checkbox('INPUT_ForbidPhyPort2',G_ForbidPhyPort2);
	//Form.Checkbox('INPUT_ForbidPhyPort3',G_ForbidPhyPort3);
	//Form.Checkbox('INPUT_ForbidPhyPort4',G_ForbidPhyPort4);
	dealWithError();
}

function uiSubmit(){
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Snooping.Enable':Form.Checkbox('INPUT_IGMPSnoopingEnable'),
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

function autoCheck(){
	var node_chb = $('INPUT_IGMPSnoopingEnable','INPUT_ForbidPhyPort1','INPUT_ForbidPhyPort2','INPUT_ForbidPhyPort3','INPUT_ForbidPhyPort4');
	var Bool     = node_chb[0].checked;
	
	for(var i = 1; i < node_chb.length; i++){
		if(node_chb[i] != null){
			node_chb[i].checked = Bool;
		}
	}
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	arrayHint['Enable'] = 'INPUT_IGMPSnoopingEnable';
	arrayHint['ForbidPhyPort1'] = 'INPUT_ForbidPhyPort1';
	arrayHint['ForbidPhyPort2'] = 'INPUT_ForbidPhyPort2';
	arrayHint['ForbidPhyPort3'] = 'INPUT_ForbidPhyPort3';
	arrayHint['ForbidPhyPort4'] = 'INPUT_ForbidPhyPort4';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
