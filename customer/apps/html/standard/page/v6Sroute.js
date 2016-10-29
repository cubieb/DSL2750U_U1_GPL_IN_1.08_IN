//Static Routing List
<?if neq `-` $var:staticRoutingIdx
`	var G_Enable          = '<?get :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.<?echo $var:staticRoutingIdx?>.Enable?>';
	var G_DestIPAddress   = '<?get :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.<?echo $var:staticRoutingIdx?>.DestIPv6Address?>';
	var G_GatewayIPAddress= '<?get :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.<?echo $var:staticRoutingIdx?>.GatewayIPv6Address?>';
	var G_ForwardingMetric= '<?get :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.<?echo $var:staticRoutingIdx?>.ForwardingMetric?>';
`?>
var wanConnection = new Array();
var n = 0;

<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";//path
            wanConnection[n][2] = "$02";
			wanConnection[n][3] = "$03";
			++n;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";//path
            wanConnection[n][2] = "$02";
			wanConnection[n][3] = "$03";
			++n;
		`?>
	`?>
`?>
`?>

//鑾峰彇妗ヨ繛鎺ュ悕
var m = 0;
<?objget :InternetGatewayDevice.LANDevice. "X_TWSZ-COM_DeviceName"
`	wanConnection[n] = new Array();
	wanConnection[n][0] = "LAN Group" + (m+1);//name
	wanConnection[n][1] = "InternetGatewayDevice.LANDevice.$00";//path
	wanConnection[n][2] = "";
	wanConnection[n][3] = "1";
	++n;
	++m;
`?>

//鑾峰彇闈欐�佽矾鐢变俊鎭?
var staticRoutingList = new Array();
var n = 0;
<?objget :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding. "Enable Status DestIPv6Address GatewayIPv6Address Interface ForwardingMetric"
`	staticRoutingList[n] = new Array();
	staticRoutingList[n][0] = "$01"; //Enable
	staticRoutingList[n][1] = "$02"; //Status
	staticRoutingList[n][2] = "$03"; //DestIPv6Address
	staticRoutingList[n][3] = "$04"; //GatewayIPv6Address
	staticRoutingList[n][4] = "$05"; //Interface
	staticRoutingList[n][5] = "$06"; //ForwardingMetric
	staticRoutingList[n][6] = "InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.$00.";	//path
	n++;
`?>

var option = '-';
var path_index = -1;
function uiOnload()
{
	createTable();
	dealWithError();
}

function createTable()
{
	Table.Clear('StaticRoutingList');
	//瀛樺偍鐢熸垚琛ㄦ牸闇�瑕佺殑鏁版嵁
	var array_value = [];
	for (var i = 0; i < staticRoutingList.length; i++)
	{
		{
			array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="edit'+i+'"> ');
			array_value[i].push(staticRoutingList[i][1]);
			array_value[i].push(staticRoutingList[i][2]); //鐩殑IP
			
		    for (var x = 0; x < wanConnection.length; x++)
	        {
		         if(staticRoutingList[i][4] == wanConnection[x][1])
					 array_value[i].push(wanConnection[x][0]); //interface

	        }
		}
	}
	$T('StaticRoutingList', array_value);
}

function uiSubmit()
{
	var _node = $('selectConnectionName','DestinationAddress');
	var _path = wanConnection[_node[0].value][1];

	if (option == 'add')
	{
		$H({
			'add-obj'          : 'InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding.',
			':Enable'          : Form.Checkbox('staticRoutingEnable'),
			':DestIPv6Address' : _node[1].value != '' ? _node[1].value : "undefine",
			':Interface'       : _path,
			':ForwardingMetric': 256,
			'obj-action'       : 'add-set'
		});
	}
	else
	{
		$F(':' + staticRoutingList[path_index][6] + 'Enable', Form.Checkbox('staticRoutingEnable'));
		$F(':' + staticRoutingList[path_index][6] + 'DestIPv6Address', _node[1].value != '' ? _node[1].value : "undefine");
		$F(':' + staticRoutingList[path_index][6] + 'ForwardingMetric', 256);
		$F(':' + staticRoutingList[path_index][6] + 'Interface', _path);
		
		$F('obj-action' ,'set');
	}
	
	$H({
		'var:ConnIdx'      : _node[0].value,
		'getpage'          :'html/index.html',
		'errorpage'        :'html/index.html',
		'var:menu'         :'advanced',
		'var:page'         :'route',
		'var:subpage'      :'v6Sroute',
		'var:errorpage'    :'v6Sroute',
		'var:CacheLastData': ViewState.Save()
	});
	
	$('uiPostForm').submit();
}

function uiCancel()
{
	option = "-";
	$('rtRouteAddInfo').style.display = 'none';
	$('add').disabled = false;
	$('edit').disabled = false;
	$('del').disabled = false;
}

function uiAdd()
{
	var MContent;
	option = "add";
	$('rtRouteAddInfo').style.display = 'block';
	$('add').disabled = true;
	$('edit').disabled = true;
	$('del').disabled = true;

	var _text = [], _value = [];
	//鐢熸垚wan杩炴帴涓嬫媺妗?
	var  k = 0;
	for (var i = 0; i < wanConnection.length; i++)
	{
		if (wanConnection[i][3] == 1)//判断是否为v6的连接
		{
		
			_text[k]  = wanConnection[i][0];
			_value[k] = i;
			k++;
		}
	}
	$S('selectConnectionName', _text, _value);

	$('DestinationAddress').value = ""; //IP
	$('staticRougingGateway').value = ""; //gw
	
	var ContentHeigh;
	if (MContent==$('Content').offsetHeight)
	{
		ContentHeigh = FirthContentHeigh + $('rtRouteAddInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}	

}

function uiEdit()
{
	var MContent;
	$('rtRouteAddInfo').style.display = 'block';
	var ContentHeigh;
	if (MContent==$('Content').offsetHeight)
	{
		ContentHeigh = FirthContentHeigh + $('rtRouteAddInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}	
	$('add').disabled = true;
	$('edit').disabled = true;
	$('del').disabled = true;

    var j = -1;
	for (var i = 0; i < staticRoutingList.length; i++)
	{
		var index = "edit";
		index = index + i;
		if (Form.Checkbox(index) == 1)
		{
			j = i;
		}
	}

	if( -1  == j)
	{
		alert(SEcode[1009]);
		return false;
	}
	
	option = "edit";

	var _text = [], _value = [];
	//create select
	var k = 0;
	for (var i = 0; i < wanConnection.length; i++)
	{
		if (wanConnection[i][3] == 1)//判断是否为v6的连接
		{
			_text[k]  = wanConnection[i][0];
			_value[k] = i;
			k++;
		}
	}
	$S('selectConnectionName', _text, _value);

	for (var i = 0; i < wanConnection.length; i++)
	{
		if(staticRoutingList[j][4] == wanConnection[i][1])
			$('selectConnectionName').value = i;//interface
	}

	Form.Checkbox('staticRoutingEnable', staticRoutingList[j][0]); //Enable
	$('DestinationAddress').value = staticRoutingList[j][2]; //IP

	path_index = j;
}

function uiDelete(id)
{
	var j = -1;

	if (!confirm(SEcode[1008]))
    {
        return false;
    }
	for (var i = 0; i < staticRoutingList.length; i++)
	{
		var index = "edit";
		var indexid = index + i;


		if(Form.Checkbox(indexid) == 1)
		{
			j = i;

	$H({
		'del-obj'    :staticRoutingList[j][6],
		'obj-action' :'del',
		'getpage'    :'html/index.html',
		'errorpage'  :'html/index.html',
		'var:menu'   :'advanced',
		'var:page'   :'route',
		'var:subpage':'v6Sroute',
		'var:errorpage':'v6Sroute',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
		}
	}

	if(-1 == j)
	{
		alert(SEcode[9888]);
		return false;
	}
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, Form.Action);
