var activedIpv6rdID = -1;
var Select6rdID = '';
var SelectTunnelID = 0;
var G_IpTunnel = [];
var G_IPv6rd = [];
var m = 0;
var n = 0;
var t = 0;
 
<?objget :InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel. "TunnelName Mode AssociatedWanIfName AssociatedLanIfName Activated TunnelCounter"
`
    G_IpTunnel[m] = [];
    G_IpTunnel[m][0] = "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$00.";
    G_IpTunnel[m][1] = "$01";    //TunnelName
    G_IpTunnel[m][2] = "$02";    //Mode
    G_IpTunnel[m][3] = "$03";    //AssociatedWanIfName
    G_IpTunnel[m][4] = "$04";    //AssociatedLanIfName
	G_IpTunnel[m][5] = "$05";    //Activated
	G_IpTunnel[m][6] = "$06";    //Counter
	G_IpTunnel[m][7] = "$00";    //{i}

	<?if eq  `6in4` "$12"
	`
	  <?objget :InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$20.6in4Tunnel. "Mechanism Dynamic IPv4MaskLen Prefix BorderRelayAddress ConnStatus Select"
       `
	      G_IPv6rd[t] = [];
          G_IPv6rd[t][0] = "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$20.6in4Tunnel.$00.";
		  G_IPv6rd[t][1] = "$01";
		  G_IPv6rd[t][2] = "$02";
		  G_IPv6rd[t][3] = "$03";
		  G_IPv6rd[t][4] = "$04";
		  G_IPv6rd[t][5] = "$05";
		  G_IPv6rd[t][6] = "$06";
		  G_IPv6rd[t][7] = "$07";
		  t++;
	  `?>
	`?>
	m++;
`?>

//WAN Device
var G_Wanconns = [];
n=0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			G_Wanconns[n][5] = "$05";
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			G_Wanconns[n][5] = "$05";
			n++;
		`?>
	`?>
`?>

//LAN Device
<?objget :InternetGatewayDevice.LANDevice. "X_TWSZ-COM_DeviceName"
`	G_Wanconns[n] = [];
	G_Wanconns[n][0] = "$01";
	G_Wanconns[n][1] = "InternetGatewayDevice.LANDevice.$00.LANHostConfigManagement.IPInterface.1";
	G_Wanconns[n][2] = "";
	n++;
`?>


function SelectTunnelEntry(index)
{
    Table.Clear('Ipv6rdList');
	createIpv6rdTable(index);
	return 0;
}

function ctrlSelectItem(_id)
{
   if(G_IPv6rd[_id][7] == "1" )	
   {
	   Select6rdID = '';

		$H({
			'obj-action'    : 'set',
			'var:menu'      : 'advanced',
			'var:page'      : 'iptunnel',
			'var:subpage'   :'ipv6rd',
			'var:errorpage' : 'ipv6rd',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);
		$F(':'+G_IPv6rd[_id][0] + 'Select','0');
		$('uiPostForm').submit();
   }
   else 
   {
	   if(Select6rdID != '')
	   {	
		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'ipv6rd',
				'var:errorpage' : 'ipv6rd',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
            $F(':'+Select6rdID + 'Select','0');
			$F(':'+G_IPv6rd[_id][0] + 'Select','1');
			$('uiPostForm').submit();
			Select6rdID = G_IPv6rd[_id][0];    
	   }
	   else
	   {
		   Select6rdID = G_IPv6rd[_id][0];

		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'ipv6rd',
				'var:errorpage' : 'ipv6rd',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
			$F(':'+G_IPv6rd[_id][0] + 'Select','1');
			$('uiPostForm').submit(); 
	   }
	}
}
function createIpv6rdTable(TunnelIndex)
{
	var array_value = [];
	var idx=0;
	var flag = -1;
    for(var i=0; i< G_IPv6rd.length ; i++)
	{
        if(G_IPv6rd[i][7] == "1")
		{
			Select6rdID = G_IPv6rd[i][0];
		}
		if((G_IPv6rd[i][0].indexOf(G_IpTunnel[TunnelIndex][0])>-1) )
		{

            flag  = 1;
			array_value[i] = [];
			array_value[i].push('<input type="radio" id="ip6rd_index'+i+'" name="ip6rd_index" value="'+i+'" >');
			array_value[i].push(G_IPv6rd[i][1]+'&nbsp;'); //Mechanism
			array_value[i].push(G_IPv6rd[i][2]+'&nbsp;'); //Dynamic
			array_value[i].push(G_IPv6rd[i][3]+'&nbsp;'); //IPv4MaskLen
			array_value[i].push(G_IPv6rd[i][4]+'&nbsp;'); //Prefix
			array_value[i].push(G_IPv6rd[i][5]+'&nbsp;'); //BorderRelayAddress
			array_value[i].push(G_IPv6rd[i][6]+'&nbsp;'); //ConnStatus
			array_value[i].push('<input type="checkbox" id="INPUT_Select' + i + '" onclick="ctrlSelectItem(' + i + ')"' + ("1" == G_IPv6rd[i][7] ? 'checked' : '') + '>'); //Select
		}
	}
	$T('Ipv6rdList',array_value);
	
	if(G_IpTunnel.length==0)
	{
		$('addIpv6rd').disabled=true;
		$('editIpv6rd').disabled=true;
		$('deleteIpv6rd').disabled=true;
		$('editTunnel').disabled=true;
		$('removeTunnel').disabled=true;
	}
	else
	{
		$('editTunnel').disabled=false;
		$('removeTunnel').disabled=false;
		if(flag == 1)
		{
			$('editIpv6rd').disabled=false;
		    $('deleteIpv6rd').disabled=false;
            $('addIpv6rd').disabled=false;     
		}
		else
		{
		   $('editIpv6rd').disabled=true;
		   $('deleteIpv6rd').disabled=true;
           $('addIpv6rd').disabled=false;   
		}
	}
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
		if(G_IpTunnel[i][2] == "6in4")
		{
			if(G_IpTunnel[i][5] == "1")
			{
				activedIpv6rdID = G_IpTunnel[i][7];
			}
		  
			if(flagCheck == -1)
		    {
			    array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'" onclick="SelectTunnelEntry(' + i + ')" checked>');
		        flagCheck = 0;
				SelectTunnelID = i;
			}
		    else
		    {
			    array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'" onclick="SelectTunnelEntry(' + i + ')">');
		    }
           
		    array_value[m].push(G_IpTunnel[i][1]+'&nbsp;'); //Name
		    array_value[m].push(G_IpTunnel[i][2]+'&nbsp;'); //Mode
		    array_value[m].push(transform_name(G_IpTunnel[i][3])+'&nbsp;'); //WanInterfaceName
		    array_value[m].push("LAN:"+transform_name(G_IpTunnel[i][4])+'&nbsp;'); //LanInterfaceName
            array_value[m].push('<input type="checkbox" id="INPUT_Actived' + i + '" onclick="ctrlActived(' + i + ')"' + ("1" == G_IpTunnel[i][5] ? 'checked' : '') + '>'); //actived
		    array_value[m].push((G_IpTunnel[i][6])+'&nbsp;'); //Counter
		    m++;
		}
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

function ctrlActived(_id)
{
   var path;
   var paht_ex;

   if(G_IpTunnel[_id][5] == "1" )	
   {
	   activedIpv6rdID = -1;
	   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		$H({
			'obj-action'    : 'set',
			'var:menu'      : 'advanced',
			'var:page'      : 'iptunnel',
			'var:subpage'   :'ipv6rd',
			'var:errorpage' : 'ipv6rd',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);
		$F(path + '.Activated','0');
		$('uiPostForm').submit();
   }
   else 
   {
	   if(activedIpv6rdID != -1)
	   {
		   var tmpid = activedIpv6rdID; 
		   activedIpv6rdID = G_IpTunnel[_id][7];
		   path_ex = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+tmpid;
		   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'ipv6rd',
				'var:errorpage' : 'ipv6rd',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
			$F(path_ex + '.Activated','0');
			$F(path + '.Activated','1');
			$('uiPostForm').submit();
           
	   }
	   else
	   {
		   activedIpv6rdID = G_IpTunnel[_id][7];		   
		   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'ipv6rd',
				'var:errorpage' : 'ipv6rd',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
			$F(path + '.Activated','1');
			$('uiPostForm').submit(); 
	   }
   }
}

function uiOnload()
{
	selectIpv6rdmode();
	wanAndLanPath();
	createTunnelTable();
	createIpv6rdTable(SelectTunnelID);

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
	$S('SELECT_LanInterface', text_lan, value_lan);
}

function selectIpv6rdmode()
{
    var text_Ipv6rd_mode = [], value_Ipv6rd_mode = [];
	text_Ipv6rd_mode.push('6in4');
	value_Ipv6rd_mode.push('6in4');
   $S('SELECT_TunnelMode', text_Ipv6rd_mode, value_Ipv6rd_mode);
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
	$('addIpv6rd').disabled=true;
	$('tunnel_setup').style.display='block';
	$('Ipv6rdSetting').style.display='none';
	$('Ipv6rd_setup').style.display='none';

}

function EditTunnel(){
	
	addTunnelAction = 2;
	editTunnelIdx=Form.Radio('Tunnel_index');
	$('addTunnel').disabled=true;	
	$('editTunnel').disabled=true;
	$('removeTunnel').disabled=true;
	$('tunnel_setup').style.display='block';

	$('Ipv6rdSetting').style.display='none';
	$('Ipv6rd_setup').style.display='none';
    $('addIpv6rd').disabled=true;
	
	$('INPUT_TunnelName').value=G_IpTunnel[editTunnelIdx][1];
	$('SELECT_TunnelMode').value=G_IpTunnel[editTunnelIdx][2];
	$('SELECT_WanInterface').value=G_IpTunnel[editTunnelIdx][3];
    $('SELECT_LanInterface').value=G_IpTunnel[editTunnelIdx][4];
	
}

function RemoveTunnel(){
	

	if(!confirm(SEcode[1008])){return false;}

	var DelIndex=Form.Radio('Tunnel_index');

	
	$H({
		'del-obj'   :G_IpTunnel[DelIndex][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'iptunnel',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'   :'ipv6rd',
		'var:errorpage':'ipv6rd'
	});
	$('uiPostForm').submit();
}

function uiTunnelSubmit(){

	var Value_Nodes = $('INPUT_TunnelName', 'SELECT_TunnelMode', 'SELECT_WanInterface',
						'SELECT_LanInterface');

    var i=0;
	for(i=0;i<G_IpTunnel.length;i++)
	{

		if(addTunnelAction != 2 && Value_Nodes[0].value==G_IpTunnel[i][1] )
			break;

	}
	if(i!=G_IpTunnel.length)
	{
		alert("Error: This Tunnel has exists in Tunnel table!");
		return false;
	}
	if(addTunnelAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.',
			':TunnelName'	: 	Value_Nodes[0].value,
			':Mode'	:  Value_Nodes[1].value,
			':AssociatedWanIfName'   : Value_Nodes[2].value,
			':AssociatedLanIfName'   : Value_Nodes[3].value,

			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'ipv6rd',
			'var:page' : 'iptunnel',
			'var:errorpage'     : 'ipv6rd',
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
			'var:subpage'   :'ipv6rd',
			'var:page' : 'iptunnel',
			'var:errorpage'    : 'ipv6rd',
			'var:CacheLastData': ViewState.Save()
		},true);		

		var path=':'+G_IpTunnel[editTunnelIdx][0];
		$F(path+'TunnelName',Value_Nodes[0].value);
		$F(path+'Mode',Value_Nodes[1].value);
		$F(path+'AssociatedWanIfName',Value_Nodes[2].value);
		$F(path+'AssociatedLanIfName',Value_Nodes[3].value);
	}
	$('uiPostForm').submit();
	addTunnelAction = 0;
}


var addIpv6rdAction=0;
var editIpv6rdIdx=0;

function uiAddIpv6rd()
{
	addIpv6rdAction = 1;
	$('TunnelSetting').style.display='none';
	$('tunnel_setup').style.display='none';

	$('addIpv6rd').disabled=true;
	$('editIpv6rd').disabled=true;
	$('deleteIpv6rd').disabled=true;
	

	$('Ipv6rdSetting').style.display='block';
	$('Ipv6rd_setup').style.display='block';

}

function uiEditIpv6rd()
{
	var ip6rdIdx=Form.Radio('ip6rd_index');
	addIpv6rdAction = 2;
    if(G_IPv6rd.length==0)
	{
    	return;
	}
	editIpv6rdIdx = ip6rdIdx;
	$('TunnelSetting').style.display='none';
	$('tunnel_setup').style.display='none';
	$('Ipv6rdSetting').style.display='none';
	$('Ipv6rd_setup').style.display='block';

	setJSONValue({	
					'SELECT_Ipv6rdMechanism'      : G_IPv6rd[editIpv6rdIdx][1],
					'SELECT_Ipv6rdDynamic'     : G_IPv6rd[editIpv6rdIdx][2],
					'INPUT_IPv4MaskLen'      : G_IPv6rd[editIpv6rdIdx][3],
					'INPUT_Prefix'      : G_IPv6rd[editIpv6rdIdx][4],
					'INPUT_BorderRelayAddress'     : G_IPv6rd[editIpv6rdIdx][5]

				});
		
}

function uiDeleteIpv6rd()
{
	if(!confirm(SEcode[1008])){return false;}


	var Index =Form.Radio('ip6rd_index');

	if(Index==undefined){
		alert("[error] Please Select one item!");
		return false;
	}
	$H({
		'del-obj'   :G_IPv6rd[Index][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'iptunnel',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'   :'ipv6rd',
		'var:errorpage':'ipv6rd'
	});
	$('uiPostForm').submit();
}

function btnApplyIpv6rd()
{
    var TunnelIdx=Form.Radio('Tunnel_index');
	var Path=G_IpTunnel[TunnelIdx][0]+'6in4Tunnel.';

	if(addIpv6rdAction==1){
		if(('SELECT_Ipv6rdDynamic').value == '1')
		{
			$H({
				'add-obj' 	  	: Path,
				':Mechanism'	: $('SELECT_Ipv6rdMechanism').value ,
				':Dynamic'	    : $('SELECT_Ipv6rdDynamic').value,
				'obj-action' 		: 'add-set',
				'getpage'  : 'html/index.html',
				'errorpage': 'html/index.html',
				'var:menu' : 'advanced',
				'var:subpage'   :'ipv6rd',
				'var:page' : 'iptunnel',
				'var:errorpage'    : 'ipv6rd',
				'var:CacheLastData': ViewState.Save()
			},true);
		}
		else
		{
			$H({
				'add-obj' 	  	: Path,
				':Mechanism'	: $('SELECT_Ipv6rdMechanism').value ,
				':Dynamic'	    : $('SELECT_Ipv6rdDynamic').value,
				':IPv4MaskLen':		$('INPUT_IPv4MaskLen').value,
				':Prefix':          $('INPUT_Prefix').value,
				':BorderRelayAddress'	:$('INPUT_BorderRelayAddress').value,
				
				'obj-action' 		: 'add-set',
				'getpage'  : 'html/index.html',
				'errorpage': 'html/index.html',
				'var:menu' : 'advanced',
				'var:subpage'   :'ipv6rd',
				'var:page' : 'iptunnel',
				'var:errorpage'    : 'ipv6rd',
				'var:CacheLastData': ViewState.Save()
			},true);
		}
	}else if(addIpv6rdAction== 2){     //edit
		var Ipv6rdIdx= 0;
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'ipv6rd',
			'var:page' : 'iptunnel',
			'var:errorpage'    : 'ipv6rd',
			'var:CacheLastData': ViewState.Save()
		},true);		
		
		var path=':'+G_IPv6rd[Ipv6rdIdx][0];
		$F(path+'Mechanism',$('SELECT_Ipv6rdMechanism').value);
		$F(path+'Dynamic',$('SELECT_Ipv6rdDynamic').value);
		if($('SELECT_Ipv6rdDynamic').value == "0")
		{
		    $F(path+'IPv4MaskLen',$('INPUT_IPv4MaskLen').value);
		    $F(path+'BorderRelayAddress',$('INPUT_BorderRelayAddress').value);	
		    $F(path+'Prefix',$('INPUT_Prefix').value);
		}
	}
	$('uiPostForm').submit();
	addIpv6rdAction=0;
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
