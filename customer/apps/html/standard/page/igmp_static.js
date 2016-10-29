//Static IGMP
var G_StaticGroups = [];
var m = 0;
<?mget :InternetGatewayDevice.X_TWSZ-COM_IGMP. "MaxGroupEntries GroupNumberOfEntries"
`	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.X_TWSZ-COM_IGMP.StaticGroup. "ServiceName MulticastGroupIP STBClientIP STBClientMac LocalPort"
		`	G_StaticGroups[m] = ["InternetGatewayDevice.X_TWSZ-COM_IGMP.StaticGroup.$00.",
								 "$01",  //ServiceName
								 "$02",  //MulticastGroupIP
								 "$03",  //STBClientIP
								 "$04",  //STBClientMac
								 "$05"   //LocalPort
								 ];
			++m;
		`?>
	`?>
`?>

//获取lan config的接口名称//Don't change the order
var G_interface_config = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_Description X_TWSZ-COM_DeviceName"
`	G_interface_config[t] = [];
	G_interface_config[t][0] = "$01";
	G_interface_config[t][1] = "$02";
	t++;
`?>

function uiOnload(){
	//Create the table
	//alert(1234);
	var value_array = [];
	for (var i = 0, len = G_StaticGroups.length; i < len; i++){
		value_array[i] = ['<input type="checkbox" onclick="$id(' + i + ')" />',
						  G_StaticGroups[i][1],  //ServiceName
						  G_StaticGroups[i][2],  //MulticastGroupIP
						  G_StaticGroups[i][3],  //STBClientIP
						  G_StaticGroups[i][4],  //STBClientMac
						  G_StaticGroups[i][5]   //LocalPort
						  ];
	}
	$T('td_MulticastGroupList', value_array);

    var text_lan = [], value_lan = [];
    for (var j = 0; j < G_interface_config.length; j++){
    	text_lan.push(G_interface_config[j][0]);
    	value_lan.push(G_interface_config[j][1]);
    }
    $S('Select_Lan_Port', text_lan, value_lan);
    
	$('addIP').disabled = false;
	$('deleteIP').disabled = false;
	$('EntryInfo').style.display = 'none';
	dealWithError();
}

function uiSubmit(){
	var _nodes = $('GroupName', 'MulticastIP', 'STBClientIP', 'STBClientMac', 'Select_Lan_Port');
	if (addAction == '1'){
		$H({
			'add-obj'			:'InternetGatewayDevice.X_TWSZ-COM_IGMP.StaticGroup.',
			':ServiceName'		:_nodes[0].value,
			':MulticastGroupIP'	:_nodes[1].value,
			':STBClientIP'		:_nodes[2].value,
			':STBClientMac'		:_nodes[3].value,
			':LocalPort'		:_nodes[4].value,
			'obj-action'		:'add-set',
			'var:menu'			:'advanced',
			'var:page'			:'nwtools',
			'var:subpage'		:'igmp_static',
			'var:errorpage'		:'igmp_static',
			'getpage'			:'html/index.html',
			'errorpage'			:'html/index.html',
			'var:CacheLastData'	:ViewState.Save()
		});
	}
	$('uiPostForm').submit();
}

function uiAddEntry(){
	var MContent = $('Content').offsetHeight;
	addAction = 1;
	$('addIP').disabled = true;
	$('deleteIP').disabled = true;
	$('EntryInfo').style.display = 'block';
	var ContentHeigh;
	if (MContent == $('Content').offsetHeight){
		ContentHeigh = FirthContentHeigh + $('EntryInfo').offsetHeight;
		$('Menu').style.height = ContentHeigh + 'px';
		$('Content').style.height = ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}
}

function uiRemoveEntry(){
	if (_array_id.length != 1){
		alert(SEcode[1009]);
		return false;
	}

	var Idx = _array_id[0];
	$H({
		'del-obj'		:G_StaticGroups[Idx][0],
		'obj-action'	:'del',
		'var:menu'		:'advanced',
		'var:page'		:'nwtools',
		'var:subpage'	:'igmp_static',
		'getpage'		:'html/index.html',
		'errorpage'		:'html/index.html',
		'var:errorpage'	:'igmp_static',
		'var:CacheLastData':ViewState.Save()
	});
	$('uiPostForm').submit();
}


function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
