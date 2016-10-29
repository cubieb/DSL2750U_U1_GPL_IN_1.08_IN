var activedGreID = -1;//该变量记录激活隧道的实例号
var SelectGreID = ''; //该变量记录隧道路径
var SelectTunnelID = 0;//该变量记录第一个隧道的索引号
var G_IpTunnel = [];
var m = 0;
var n = 0;
var t = 0;
 
<?objget :InternetGatewayDevice.X_TELEFONICA-ES_GreTunnel. "TunnelName AssociatedWanIfName RemoteIpv4WanAddress PeerLocalAddress PeerRemoteAddress PeerIPv4Address PeerIPv4MaskLen"
`
    G_IpTunnel[m] = [];
    G_IpTunnel[m][0] = "InternetGatewayDevice.X_TELEFONICA-ES_GreTunnel.$00.";
    G_IpTunnel[m][1] = "$01";    //TunnelName
    G_IpTunnel[m][2] = "$02";    //AssociatedWanIfName
    G_IpTunnel[m][3] = "$03";    //RemoteIpv4WanAddress
    G_IpTunnel[m][4] = "$04";    //PeerLocalAddress
    G_IpTunnel[m][5] = "$05";    //PeerRemoteAddress
    G_IpTunnel[m][6] = "$06";    //PeerIPv4Address
    G_IpTunnel[m][7] = "$07";    //PeerIPv4MaskLen

    m++;
`?>

//WAN Device
var G_Wanconns = [];
n=0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			n++;
		`?>
	`?>
`?>

var lanip = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress?>";
var lanmask = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask?>";

function IPtoInt(ip) {
	var a = ip.split('.');
	var b = parseInt(a[0], 10) << 24;
	var c = parseInt(a[1], 10) << 16;
	var d = parseInt(a[2], 10) << 8;
	var e = parseInt(a[3], 10);

	return (b + c + d + e);
}

function createTunnelTable()
{	
	Table.Clear('Iptunnel_table');

	var array_value = [];
	var m = 0;
	var flagCheck = -1;
	for(var i = 0; i < G_IpTunnel.length; i++)
	{
		array_value[m] = [];
		if(flagCheck == -1)
		{
			array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'" checked>');
			flagCheck = 0;
			SelectTunnelID = i;
		}
		else
		{
			array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'">');
		}

		array_value[m].push(G_IpTunnel[i][1]+'&nbsp;'); //Name
		array_value[m].push(transform_name(G_IpTunnel[i][2])+'&nbsp;'); //WanInterfaceName
		array_value[m].push(G_IpTunnel[i][3]+'&nbsp;'); //Remote address
		array_value[m].push((G_IpTunnel[i][4])+'&nbsp;'); //PeerLocalAddress
		array_value[m].push((G_IpTunnel[i][5])+'&nbsp;'); //PeerRemoteAddress
		array_value[m].push((G_IpTunnel[i][6])+'/'+(G_IpTunnel[i][7]) + '&nbsp;'); //TunnelAddress
		m++;
	}
	$T('Iptunnel_table',array_value);
	if(m > 0)
	{
		$('editTunnel').disabled=false;
		$('removeTunnel').disabled=false;
	}
	else
	{
       	$('editTunnel').disabled=true;
		$('removeTunnel').disabled=true;
	}
}

function uiOnload()
{
	wanAndLanPath();
	createTunnelTable();

}

function wanAndLanPath()
{
	var text_wan = [], value_wan = [];
	var text_lan = [], value_lan = [];
	
	for(var i = 0; i < G_Wanconns.length; i++)
	{
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0)
		{
			if (G_Wanconns[i][2].indexOf('IP_Bridged') < 0 && G_Wanconns[i][2].indexOf('PPP_Bridged') < 0)
			{
				if (G_Wanconns[i][4] == '0')
				{
					continue;
				}

				if (G_Wanconns[i][3] == 'TR069')
				{
					continue;
				}
				
				text_wan.push(G_Wanconns[i][0]);
				value_wan.push(G_Wanconns[i][1]);
			}
		}
		else if (G_Wanconns[i][1].indexOf('LANDevice') > 0)
		{
			if (G_Wanconns[i][0].indexOf('br1') < 0 && G_Wanconns[i][0].indexOf('br0') < 0 )
			{
				continue;
			}
			text_lan.push("LAN:"+G_Wanconns[i][0]);
			value_lan.push(G_Wanconns[i][1]);
		}
	}
	$S('SELECT_WanInterface', text_wan, value_wan);
	// $S('SELECT_LanInterface', text_lan, value_lan);
}

function transform_name(path)
{
	for (var i = 0; i < G_Wanconns.length; i++)
	{
		if (G_Wanconns[i][1] == path)
			return G_Wanconns[i][0];
	}

	return "unknown";
}


var addTunnelAction=0;
var editTunnelIdx=0;

function onChgRelay()
{
	var value_relay = $('SELECT_Ipv6rdDynamic').value;

	if (value_relay == "0")
	{
		$("div_ipv6rd_dynamic").style.display = "block";
	}
	else if (value_relay == "1")
	{
		$("div_ipv6rd_dynamic").style.display = "none";
	}
}

function AddTunnel(){
	addTunnelAction = 1;
  	$('addTunnel').disabled=true;
	$('editTunnel').disabled=true;
	$('removeTunnel').disabled=true;
	$('tunnel_setup').style.display='block';

}

function EditTunnel(){
	addTunnelAction = 2;
	editTunnelIdx=Form.Radio('Tunnel_index');
	$('addTunnel').disabled=true;
	$('editTunnel').disabled=true;
	$('removeTunnel').disabled=true;
	$('tunnel_setup').style.display='block';

	$('INPUT_TunnelName').value=G_IpTunnel[editTunnelIdx][1];
	$('SELECT_WanInterface').value=G_IpTunnel[editTunnelIdx][2];
	$('INPUT_RemoteAddress').value=G_IpTunnel[editTunnelIdx][3];
	$('INPUT_PeerLocalAddress').value=G_IpTunnel[editTunnelIdx][4];
	$('INPUT_PeerRemoteAddress').value=G_IpTunnel[editTunnelIdx][5];
	$('INPUT_PeerIPv4Address').value=G_IpTunnel[editTunnelIdx][6];
	$('INPUT_PeerIPv4MaskLen').value=G_IpTunnel[editTunnelIdx][7];
}

function RemoveTunnel(){

	if(!confirm(SEcode[1008])){return false;}

	var DelIndex=Form.Radio('Tunnel_index');

	$H({
		'del-obj'   :G_IpTunnel[DelIndex][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'gre',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:errorpage':'gre'
	});
	$('uiPostForm').submit();
}

/*隧道提交处理函数*/
function uiTunnelSubmit(){

	var Value_Nodes = $('INPUT_TunnelName', 'SELECT_WanInterface',
			'INPUT_RemoteAddress', 'INPUT_PeerLocalAddress',
			'INPUT_PeerRemoteAddress', 'INPUT_PeerIPv4Address',
			'INPUT_PeerIPv4MaskLen');

    var i=0;
	for(i=0;i<G_IpTunnel.length;i++)
	{

		if(addTunnelAction != 2 && Value_Nodes[0].value==G_IpTunnel[i][1] )
			break;

	}
	/*如果是添加隧道，并且名字由重复，则错误退出*/
	if(i!=G_IpTunnel.length)
	{
		alert("Error: This Tunnel has exists in Tunnel table!");
		return false;
	}

	/* IP合法性检查 */
	var nodeAddr = $('INPUT_PeerIPv4Address').value.split('.');
	var nodeAddr0 = parseInt(nodeAddr[0]);
	var nodeAddr1 = parseInt(nodeAddr[1]);
	var nodeAddr2 = parseInt(nodeAddr[2]);
	var nodeAddr3 = parseInt(nodeAddr[3]);
	var addr = (nodeAddr0 << 24) + (nodeAddr1 << 16) + (nodeAddr2 << 8) + nodeAddr3;
	var mask = 0xFFFFFFFF - (1 << (32 - $('INPUT_PeerIPv4MaskLen').value)) + 1;

	if ((addr & mask) != addr) {
		alert("Please enter the right address and mask length!");
		return false;
	}

	/* 私有地址范围检查 */
	var peerAddr = IPtoInt($('INPUT_PeerIPv4Address').value);
	var lanAddr = IPtoInt(lanip);
	var lanMask = IPtoInt(lanmask);

	if ((peerAddr & lanMask) == (lanAddr & lanMask)) {
		alert("Private address and the local network in the same network!");
		return false;
	}

    /*添加隧道*/
	if(addTunnelAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TELEFONICA-ES_GreTunnel.',
			':TunnelName'	: 	Value_Nodes[0].value,
			':AssociatedWanIfName'   : Value_Nodes[1].value,
			':RemoteIpv4WanAddress'   : Value_Nodes[2].value,
			':PeerLocalAddress'   : Value_Nodes[3].value,
			':PeerRemoteAddress'   : Value_Nodes[4].value,
			':PeerIPv4Address'   : Value_Nodes[5].value,
			':PeerIPv4MaskLen'   : Value_Nodes[6].value,

			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:page' : 'gre',
			'var:errorpage'     : 'gre',
			'var:CacheLastData': ViewState.Save()
		},true);

	}
	else if(addTunnelAction=='2')     //edit
	{
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:page' : 'gre',
			'var:errorpage'    : 'gre',
			'var:CacheLastData': ViewState.Save()
		},true);		

		var path=':'+G_IpTunnel[editTunnelIdx][0];
		$F(path+'TunnelName',Value_Nodes[0].value);
		$F(path+'AssociatedWanIfName',Value_Nodes[1].value);
		$F(path+'RemoteIpv4WanAddress', Value_Nodes[2].value);
		$F(path+'PeerLocalAddress', Value_Nodes[3].value);
		$F(path+'PeerRemoteAddress', Value_Nodes[4].value);
		$F(path+'PeerIPv4Address', Value_Nodes[5].value);
		$F(path+'PeerIPv4MaskLen', Value_Nodes[6].value);
	}
	$('uiPostForm').submit();
	addTunnelAction = 0;
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

addListeners(uiOnload, dealWithError);
