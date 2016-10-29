
var G_MacClone_Inst   = [];
var m = 0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_MacClone.MaccloneObject. "WANConnetPath MacAd MACCloneEnabled"
   `G_MacClone_Inst[m] = [];
	G_MacClone_Inst[m][0] = "$01"; //WANConnetPath
	G_MacClone_Inst[m][1] = "$02"; //MacAd
	G_MacClone_Inst[m][2] = "InternetGatewayDevice.X_TWSZ-COM_MacClone.MaccloneObject.$00.";
    G_MacClone_Inst[m][3] = "$03"; //MACCloneEnabled
	m++;
`?>

var G_Wanconns = [];
var n = 0;
//WAN Device
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries X_TWSZ-COM_DeviceName"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			`	G_Wanconns[n] = [];
				G_Wanconns[n][0] = "$01";
				G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";
				G_Wanconns[n][2] = "$02";
				G_Wanconns[n][3] = "$03";
				n++;
			`?>
		`?>
		<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			`	G_Wanconns[n] = [];
				G_Wanconns[n][0] = "$01";
				G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";
				G_Wanconns[n][2] = "$02";
				G_Wanconns[n][3] = "$03";
				n++;
			`?>
		`?>
	`?>
`?>

var LanHosts = [];               
var k = 0;
<?objget :InternetGatewayDevice.LANDevice. "Hosts.HostNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.LANDevice.$20.Hosts.Host. "IPAddress MACAddress"
		`	LanHosts[k] = [];
			LanHosts[k][0] = "$01";
			LanHosts[k][1] = "$02";
			++k;
		`?>
	`?>
`?>

var G_CurrentIP = "<?echo $var:sys_RemoteAddr ?>";
var G_CurrentMAC = GetMACByIP(G_CurrentIP);
<?setvaronce var:select_wan 0?>
var select_wan = <?echo $var:select_wan?>; 

/* Get Mac By IP */
function GetMACByIP(ip)
{
	for (var i=0; i<LanHosts.length; i++)
	{
		if (LanHosts[i][0] == ip)
		{
			return LanHosts[i][1];
		}
	}
	return "";
}

function uiOnload1(G_Select)
{
	if(G_Select!= '')
	{
		var i = checkMaccloneObject(G_Select);
		if(i != -1)
		{ 
			setJSONValue({
				'INPUT_MACCLONEEnable'  : G_MacClone_Inst[i][3],
				'INPUT_WANMAC'  : G_MacClone_Inst[i][1],
				'INPUT_PCMAC'   : G_CurrentMAC
			});
		}
		else
		{
			setJSONValue({
				'INPUT_MACCLONEEnable'  : 0,
				'INPUT_WANMAC'  : "",
				'INPUT_PCMAC'   : G_CurrentMAC
			});
		  
		}
	}
}

function uiOnload(){
	wanPath();
	var wanConnect = $('selectConnectionName').value;
	$('selectConnectionName').value = select_wan;
	if(wanConnect == "")
	{
		setJSONValue({
			'INPUT_MACCLONEEnable'  : 0,
			'INPUT_WANMAC'  : '',
			'INPUT_PCMAC'   : G_CurrentMAC
		});
		return ;
	}
	var G_Select=G_Wanconns[select_wan][1];
	uiOnload1(G_Select);
}


function wanPath(){
	var text_wan = [],value_wan = [];
	
	for(var i = 0; i < G_Wanconns.length; i++)
	{
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0)
		{
			if (G_Wanconns[i][3]=="IP_Bridged"||G_Wanconns[i][2]=="TR069"||G_Wanconns[i][0]=="pppo3g") 
			{
				continue;
			}
			text_wan.push(G_Wanconns[i][0]);
			value_wan.push(i);
		}		
	}

	$S('selectConnectionName',text_wan,value_wan);
}

function onChangeSelected(){
	select_wan = $('selectConnectionName').value;
	var G_Select=G_Wanconns[select_wan][1];
	uiOnload1(G_Select);
}

function  checkMaccloneObject(wanPath)
{
	for (var i=0; i<G_MacClone_Inst.length; i++)
	{
		if (G_MacClone_Inst[i][0] == wanPath )
		{
			return i;
		}
	}
	return -1;
}

function uiSubmit(){
	var node_macclone= $('INPUT_MACCLONEEnable','INPUT_WANMAC');
	var wanConn = $('selectConnectionName').value;
	var enable_tmp = 0;
	if(wanConn == '')
	{
		alert(SEcode[1002]);
		return false;
	}

	if(node_macclone[1].value=='')
	{
		alert(SEcode[1004]);
		return false;
	}
	if ($('INPUT_MACCLONEEnable').checked)
	{
		var enable_tmp = 1;
	}

	var wanConnpath = G_Wanconns[wanConn][1];
	var wanConninfterface; 
	var i = checkMaccloneObject(wanConnpath);
	if(i != -1)
	{
		$H({
			'var:select_wan' : select_wan,
			'obj-action':'set',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:errorpage': G_Page,
			'var:CacheLastData': ViewState.Save()
		}, true);

		var p = ":" + G_MacClone_Inst[i][2];
		$F(p + 'MACCloneEnabled',enable_tmp);
		$F(p+ 'WANConnetPath',wanConnpath);
		$F(p + 'MacAd',node_macclone[1].value);
		$('uiPostForm').submit();
	}
	else
	{
		$H({					  	
			'add-obj' 	 		: 'InternetGatewayDevice.X_TWSZ-COM_MacClone.MaccloneObject.',
			':MACCloneEnabled'  : enable_tmp,
			':WANConnetPath'    : wanConnpath,
			':MacAd'            : node_macclone[1].value,
			'obj-action' 		: 'add-set',
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:errorpage': G_Page,
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'var:CacheLastData': ViewState.Save()
		},true);
		$('uiPostForm').submit();	
	}
}		

function clonemac()
{
	$('INPUT_WANMAC').value = $('INPUT_PCMAC').value;
}

function dealWithError(){
	if (G_Error != 1){ return false; }
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}
addListeners(uiOnload,dealWithError);


