//Static Routing List
<?if neq `-` $var:staticRoutingIdx
`	var G_Enable          = '<?get :InternetGatewayDevice.Layer3Forwarding.Forwarding.<?echo $var:staticRoutingIdx?>.Enable?>';
	var G_DestIPAddress   = '<?get :InternetGatewayDevice.Layer3Forwarding.Forwarding.<?echo $var:staticRoutingIdx?>.DestIPAddress?>';
	var G_DestSubnetMask  = '<?get :InternetGatewayDevice.Layer3Forwarding.Forwarding.<?echo $var:staticRoutingIdx?>.DestSubnetMask?>';
	var G_GatewayIPAddress= '<?get :InternetGatewayDevice.Layer3Forwarding.Forwarding.<?echo $var:staticRoutingIdx?>.GatewayIPAddress?>';
	var G_ForwardingMetric= '<?get :InternetGatewayDevice.Layer3Forwarding.Forwarding.<?echo $var:staticRoutingIdx?>.ForwardingMetric?>';
`?>
var wanConnection = new Array();
var n = 0;

<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled ConnectionType"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";//path
			wanConnection[n][2] = "$02";
			wanConnection[n][3] = "$03";
			wanConnection[n][4] = "$04";
			++n;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled ConnectionType"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";//path
			wanConnection[n][2] = "$02";
			wanConnection[n][3] = "$03";
			wanConnection[n][4] = "$04";
			++n;
		`?>
	`?>
`?>
`?>

<?objget :InternetGatewayDevice.Layer2Bridging.Bridge. "BridgeName  X_TWSZ-COM_AssociatedLANDevice"
`	<?if eq `1` `<?echo $20?>`
	`	wanConnection[n] = [];
		wanConnection[n][0] = "<?echo $21?>"; 
		wanConnection[n][1] = "<?echo $22?>"; //about path 
		n++;
	`?>
`?>

var staticRoutingList = new Array();
var n = 0;
<?objget :InternetGatewayDevice.Layer3Forwarding.Forwarding. "Enable Status DestIPAddress DestSubnetMask GatewayIPAddress Interface ForwardingMetric StaticRoute X_TWSZ-COM_ErrorInfo"
`	staticRoutingList[n] = new Array();
	staticRoutingList[n][0] = "$01"; //Enable
	staticRoutingList[n][1] = "$02"; //Status
	staticRoutingList[n][2] = "$03"; //DestIPAddress
	staticRoutingList[n][3] = "$04"; //DestSubnetMask
	staticRoutingList[n][4] = "$05"; //GatewayIPAddress
	staticRoutingList[n][5] = "$06"; //Interface
	staticRoutingList[n][6] = "$07"; //ForwardingMetric
	staticRoutingList[n][7] = "$08"; //StaticRoute
	staticRoutingList[n][8] = "$09"; //ErrorInfo
	staticRoutingList[n][9] = "InternetGatewayDevice.Layer3Forwarding.Forwarding.$00.";	//path
	n++;
`?>

var G_ForwardNumberOfEntries  = '<?get :InternetGatewayDevice.Layer3Forwarding.ForwardNumberOfEntries?>';

var option = '-';
var path_index = -1;
function selectConnectionName()
{
    var index = Form.Select('selectConnectionName');

    if(index != '')
    {
	    if(wanConnection[index][1].indexOf('WANPPPConnection') > -1)
	    {
	        $('GatewayIPAddr').style.display = 'none';
	    }
	    else if(wanConnection[index][1].indexOf('LANDevice') > -1)
	    {
	        $('GatewayIPAddr').style.display = 'block';
	    }
	   else
	   {
		 $('GatewayIPAddr').style.display = 'block';
		 $('staticRougingGateway').value = "";//gw
	  }
    }
	else
	{
	    $('GatewayIPAddr').style.display = 'block';
		$('staticRougingGateway').value = "";//gw
	}
}
function uiOnload()
{
	createTable();
	dealWithError();
	checkShowBtn();
}

function createTable()
{
	Table.Clear('StaticRoutingList');
	var array_value = [];
	for(var i = 0; i < staticRoutingList.length; i++)
	{
		{
			array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="edit'+i+'"> ');
			array_value[i].push(staticRoutingList[i][2]); // Destination IP
			array_value[i].push(staticRoutingList[i][3]); // Net mask
			array_value[i].push(staticRoutingList[i][4] == '' ? '-' : staticRoutingList[i][4]); // GatewayIPAddress
			
		    for (var x = 0; x < wanConnection.length; x++)
	        {
		         if(staticRoutingList[i][5] == wanConnection[x][1])
					 array_value[i].push(wanConnection[x][0]); //interface

	         }
			array_value[i].push(staticRoutingList[i][6]); // Metric
		}
	}
	$T('StaticRoutingList',array_value);
}

function checkShowBtn()
{
	if (n > 0) {
		$('add').disabled = 0;
		$('del').disabled = 0;
		$('edit').disabled = 0;
	} else {
		$('add').disabled = 0;
		$('del').disabled = 1;
		$('edit').disabled = 1;
	}
}

function uiSubmit()
{
	var _node = $('selectConnectionName','DestinationAddress','SubnetMask','staticRougingGateway','Metric');
	if (_node[0].value == '')
	{
		alert(data_language.dynamic.addwan);
		return false;
	}
	var _path = wanConnection[_node[0].value][1];
	var nodeAddress = $('DestinationAddress').value.split('.');
	var nodeMask = $('SubnetMask').value.split('.');
	//var nodeMetric = $('Metric').value.split('.');
	//alert(nodeAddress[0]);
    var nodeAddress0 = parseInt(nodeAddress[0]);
    var nodeAddress1 = parseInt(nodeAddress[1]);
	var nodeAddress2 = parseInt(nodeAddress[2]);
	var nodeAddress3 = parseInt(nodeAddress[3]);
	var nodeMask0 = parseInt(nodeMask[0]);
	var nodeMask1 = parseInt(nodeMask[1]);
	var nodeMask2 = parseInt(nodeMask[2]);
	var nodeMask3 = parseInt(nodeMask[3]);
	
	var ip_and_mask0 = nodeAddress0&nodeMask0;
	var ip_and_mask1 = nodeAddress1&nodeMask1;
	var ip_and_mask2 = nodeAddress2&nodeMask2;
	var ip_and_mask3 = nodeAddress3&nodeMask3;
	
	if((ip_and_mask0 != nodeAddress0) || (ip_and_mask1 != nodeAddress1) || (ip_and_mask2 != nodeAddress2) || 
	   (ip_and_mask3 != nodeAddress3) || ((nodeAddress3 == 0) && (nodeMask3 == 255)) || ((nodeAddress3 == 255) && (nodeMask3 == 255)))
	{
		alert(data_language.dynamic.destinationaddress);
		return false;
	}

//	for (var z = 0; z < 3; z++)
//	{
//		if (nodeAddress[z] == 0 && nodeMask[z] != 0)
//		{
//			alert("IP address and subnet mask are not valid merge.");
//			return false;
//		}
//	}
	
	var i = (nodeMask0<<24)|(nodeMask1<<16)|(nodeMask2<<8)|(nodeMask3);
	
	
	var b;
	var c;
	var flag = 0;
	//alert('i:'+i);
	for(b=0;b<32;b++)
	{
		if(i&(0x1<<b))
		{
			flag = 1;
		}
		else
		{
			if (flag == 1)
			{
				alert(data_language.dynamic.Subnetmask);
				return false;
			}
		}		
		
		
		
	}

    for(var j = 0; j < staticRoutingList.length; j++)
	{
		if(j == path_index)
		{
			continue;
		}
	    if(($('DestinationAddress').value == staticRoutingList[j][2])
		   && ($('SubnetMask').value == staticRoutingList[j][3]))
		{
			 alert(data_language.dynamic.destinationduplicated);
			 return false;
		}
	}
	
	if(option == 'add')
	{
		$H({
			'add-obj'          : 'InternetGatewayDevice.Layer3Forwarding.Forwarding.',
			':Enable'          : 1,
			':DestIPAddress'   : _node[1].value != '' ? _node[1].value : "undefine",
			':DestSubnetMask'  : _node[2].value != '' ? _node[2].value : "undefine",
			':Interface'       : _path,
			':ForwardingMetric': $('Metric').value,
			'obj-action'       : 'add-set'
		});
		if(_path.indexOf('WANIPConnection') > -1 || _path.indexOf('LANDevice') > -1)
		{
		    $F(':GatewayIPAddress', _node[3].value != '' ? _node[3].value : '');
		}

		if (G_ForwardNumberOfEntries >= 30)
		{
			alert(data_language.dynamic.Maximum);
			return false;
		}
	} else 
	{
		$F(':'+staticRoutingList[path_index][9]+'Enable',1);

		$F(':'+staticRoutingList[path_index][9]+'DestIPAddress',_node[1].value != '' ? _node[1].value : "undefine");
		
		$F(':'+staticRoutingList[path_index][9]+'DestSubnetMask',_node[2].value != '' ? _node[2].value : "undefine");

		if (_path.indexOf('WANIPConnection') > -1 || _path.indexOf('LANDevice') > -1)
			$F(':' + staticRoutingList[path_index][9] + 'GatewayIPAddress', _node[3].value != '' ? _node[3].value : '');
		else if (_path.indexOf('WANPPPConnection') > -1)
			$F(':' + staticRoutingList[path_index][9] + 'GatewayIPAddress', '');

		$F(':'+staticRoutingList[path_index][9]+'ForwardingMetric',$('Metric').value);

		$F(':'+staticRoutingList[path_index][9]+'Interface',_path);
		
		$F('obj-action' ,'set');
	}
	
	$H({
		'var:ConnIdx'      : _node[0].value,
		'getpage'          :'html/index.html',
		'errorpage'        :'html/index.html',
		'var:menu'         :'advanced',
		'var:page'         :'route',
		'var:subpage'     :'staticroute',
		'var:errorpage'    :'staticroute',
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
	var  k = 0;
	for (var i = 0; i < wanConnection.length; i++)
	{   
	    /*if(wanConnection[i][2]=="TR069"){
	        continue;
		}
*/

		if (wanConnection[i][4] == 'IP_Bridged' ||
			wanConnection[i][4] == 'PPPoE_Bridged')
		{
			continue;
		}

		if (wanConnection[i][3] == 1 || wanConnection[i][1].indexOf('LANDevice') > -1)
		{
			_text[k]  = wanConnection[i][0];
			_value[k] = i;
			k++;
		}
	}
	$S('selectConnectionName', _text, _value);

	$('DestinationAddress').value = "";//IP
	
	$('SubnetMask').value = "";//mask

	$('Metric').value = "";//Metric

	selectConnectionName();
	
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('rtRouteAddInfo').offsetHeight;
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
    var j = -1;
	var select_num = 0;
	for(var i = 0; i < staticRoutingList.length; i++)
	{
		var index = "edit";
		index = index + i;
		if(Form.Checkbox(index) == 1)
		{
			j = i;
			select_num = select_num + 1;
			continue;
		}
	}
	if(-1  == j )
	{
		alert(SEcode[9888]);
		return false;
	}
	if(select_num > 1)
	{
		alert(SEcode[1009]);
		return false;
	}
	
	var MContent;
	$('rtRouteAddInfo').style.display = 'block';
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('rtRouteAddInfo').offsetHeight;
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

	
	option = "edit";

	var _text = [], _value = [];
	//create select
	var  k = 0;
	for (var i = 0; i < wanConnection.length; i++)
	{
		if (wanConnection[i][3] == 1 || wanConnection[i][1].indexOf('LANDevice') > -1)
		{
			_text[k]  = wanConnection[i][0];
			_value[k] = i;
			k++;
		}
	}
	$S('selectConnectionName', _text, _value);


	for (var i = 0; i < wanConnection.length; i++)
	{
		if(staticRoutingList[j][5] == wanConnection[i][1])
		{
			$('selectConnectionName').value = i;//interface
			if(wanConnection[i][1].indexOf('WANPPPConnection') > -1)
			{
			    $('GatewayIPAddr').style.display = 'none';
			}
			else
			{
			    $('GatewayIPAddr').style.display = 'block';
			    $('staticRougingGateway').value = staticRoutingList[j][4];//gw
			}
		}
	}

	$('DestinationAddress').value = staticRoutingList[j][2];//IP
	
	$('SubnetMask').value = staticRoutingList[j][3];//mask
	
	$('Metric').value = staticRoutingList[j][6];//Metric

	selectConnectionName;

     path_index = j;
}

function uiDelete(id)
{
	var j = -1;
	var select_num = 0;
	for(var i = 0; i < staticRoutingList.length; i++)
	{
		var index = "edit";
		index = index + i;
		if(Form.Checkbox(index) == 1)
		{
			j = i;
			select_num = select_num + 1;
			continue;
		}
	}
	if(-1  == j )
	{
		alert(SEcode[9888]);
		return false;
	}
	if(select_num > 1)
	{
		alert(SEcode[1009]);
		return false;
	}	
	if(!confirm(SEcode[1008]))
    {
        return false;
    }
	$H({
		'del-obj'    :staticRoutingList[j][9],
		'obj-action' :'del',
		'getpage'    :'html/index.html',
		'errorpage'  :'html/index.html',
		'var:menu'   :'advanced',
		'var:page'   :'route',
		'var:subpage':'staticroute',
		'var:errorpage':'staticroute',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}

	if (G_Error_Msg.indexOf('1002') > 0)
		alert(data_language.dynamic.destinationduplicated);
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, Form.Action);
