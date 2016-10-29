<?setvaronce var:state 0?>
//get wan connection
var G_wanConnction = [];
var m = 0;
var G_InternetWanType = "Ethernet";

<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection. "ConnectionStatus Name Enable AddressingType RouteProtocolRx RipPassive X_TELEFONICA-ES_IPv4Enabled ConnectionType"
			`	G_wanConnction[m] = [];
				G_wanConnction[m][0] = m + 1;    //Index;
				G_wanConnction[m][1] = "<?echo $11?>";   //ConnectionStatus
                G_wanConnction[m][2] = "<?echo $12?>";   //name;
				G_wanConnction[m][3] = "<?echo $13?>";   //Enable;
                G_wanConnction[m][4] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $10?>";  // about Path
				G_wanConnction[m][5] = "<?echo $14?>";   //static or dhcp;
				G_wanConnction[m][6] = "<?echo $15?>";   //RIP
				if ("off" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "off";
				else if ("ripv1" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "RIPv1";
				else if ("ripv2" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "RIPv2";
				G_wanConnction[m][7] = "<?echo $16?>";   //RIP Passive
				G_wanConnction[m][8] = "N/A";
				G_wanConnction[m][9] = "<?echo $17?>";
				G_wanConnction[m][10] = "<?echo $18?>";
			`?>
				
			<?if eq `DSL` `<?echo $41?>`
			`	<?mget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.<?echo $40?>.WANDSLLinkConfig. "DestinationAddress"
				`	G_wanConnction[m][8] = "<?echo $11?>";   //PVC
				`?>	
				G_InternetWanType = "DSL";
			`?>


			++m;
		`?>
			
		<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger RouteProtocolRx RipPassive X_TELEFONICA-ES_IPv4Enabled ConnectionType"
			`	G_wanConnction[m] = [];
				G_wanConnction[m][0] = m + 1;    //Index;
				G_wanConnction[m][1] = "<?echo $11?>";   //ConnectionStatus
                G_wanConnction[m][2] = "<?echo $12?>";   //name;
				G_wanConnction[m][3] = "<?echo $13?>";   //Enable;
                G_wanConnction[m][4] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $10?>";  // about Path;
				G_wanConnction[m][5] = "<?echo $14?>";   //pppoe
				G_wanConnction[m][6] = "<?echo $15?>";   //RIP
				if ("off" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "off";
				else if ("ripv1" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "RIPv1";
				else if ("ripv2" == G_wanConnction[m][6].toLowerCase())
					G_wanConnction[m][6] = "RIPv2";
				G_wanConnction[m][7] = "<?echo $16?>";   //RIP Passive
				G_wanConnction[m][8] = "N/A";
				G_wanConnction[m][9] = "<?echo $17?>";
				G_wanConnction[m][10] = "<?echo $18?>";
			`?>
				
			<?if eq `DSL` `<?echo $41?>`
			`	<?mget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.<?echo $40?>.WANDSLLinkConfig. "DestinationAddress"
				`	G_wanConnction[m][8] = "<?echo $11?>";   //PVC
				`?>	
				G_InternetWanType = "DSL";
			`?>

			++m;
		`?>
		
	`?>
`?>

//鑾峰彇LAN渚ф帴鍙?

<?objget :InternetGatewayDevice.Layer2Bridging.Bridge. "BridgeName  X_TWSZ-COM_AssociatedLANDevice"
`	<?if eq `1` `<?echo $20?>`
	`	G_wanConnction[m] = [];
		G_wanConnction[m][0] = m + 1;
		G_wanConnction[m][1] = "Connected"; 
		G_wanConnction[m][2] = "<?echo $21?>"; 
			G_wanConnction[m][3] = "";   //去掉InternetGatewayDevice.LANDevice.{i}.RIPEnable这个无效的节点，与之前兼容将它设成空
		G_wanConnction[m][4] = "<?echo $22?>"; //about path
		G_wanConnction[m][5] = ""; 
		G_wanConnction[m][6] = "<?get :<?echo $32?>.RouteProtocolRx?>"; //RIP
		if ("off" == G_wanConnction[m][6].toLowerCase())
			G_wanConnction[m][6] = "off";
		else if ("ripv1" == G_wanConnction[m][6].toLowerCase())
			G_wanConnction[m][6] = "RIPv1";
		else if ("ripv2" == G_wanConnction[m][6].toLowerCase())
			G_wanConnction[m][6] = "RIPv2";
		G_wanConnction[m][7] = "<?get :<?echo $32?>.RipPassive?>"; //RIP Passive
		G_wanConnction[m][8] = "-";
		G_wanConnction[m][10] = "-";  
		m++;
	`?>
`?>



function createTable()
{ // uiOnload;
	var value_array = [];
	var input_value;
	var selectindex;
	for(var i = 0; i < G_wanConnction.length; i++)
	{
		value_array[i] = [];


		//if("Lan1" == G_wanConnction[i][2])
		//{
			//continue;//skip bridge mode
		//}
		if (G_wanConnction[i][10].indexOf('IP_Bridged') > -1) {
			continue;
		}
		
        // skip IPv6 interfaces
		if (G_wanConnction[i][9] == '0') {
			continue;
		}		

		value_array[i].push(G_wanConnction[i][2]);
		value_array[i].push(G_wanConnction[i][8]);
		if("RIPv1" ==G_wanConnction[i][6] )
		{
			value_array[i].push('<select  id="select_index'+i+'"><option value="RIPv1" selected>1 </option><option value="RIPv2">2 </option></select>');
		}
         else if("RIPv2" ==G_wanConnction[i][6] )
		{
			value_array[i].push('<select  id="select_index'+i+'"><option value="RIPv1">1 </option><option value="RIPv2"selected>2 </option></select>');
		}
         else 
		{
			value_array[i].push('<select  id="select_index'+i+'" ><option value="RIPv1">1 </option><option value="RIPv2">2 </option></select>');
		}

		value_array[i].push(data_language.dynamic.active);
		
		if("off" == G_wanConnction[i][6] || "" == G_wanConnction[i][6])
		    value_array[i].push('<input type="checkbox" id="index' + i + '">');
		else
		   value_array[i].push('<input type="checkbox" id="index' + i + '" checked>');

		if("0" == G_wanConnction[i][7] || "" == G_wanConnction[i][7])
		    value_array[i].push('<input type="checkbox" id="rip_index' + i + '">');
		else
		   value_array[i].push('<input type="checkbox" id="rip_index' + i + '" checked>');        

		
	}

	$T('td_wanriplist',value_array);

}

function createTable_Ethnet()
{ // uiOnload;
	var value_array = [];
	var input_value;
	var selectindex;
	for(var i = 0; i < G_wanConnction.length; i++)
	{
		value_array[i] = [];


		if("Lan1" == G_wanConnction[i][2])
		{
			continue;//skip bridge mode
		}
		if (G_wanConnction[i][2].indexOf('br') > -1) {
			continue;
		}

        // skip IPv6 interfaces
		if (G_wanConnction[i][9] == '0') {
			continue;
		}				

		value_array[i].push(G_wanConnction[i][2]);
		value_array[i].push(G_wanConnction[i][8]);
		if("RIPv1" ==G_wanConnction[i][6] )
		{
			value_array[i].push('<select  id="select_index'+i+'"><option value="RIPv1" selected>1 </option><option value="RIPv2">2 </option></select>');
		}
         else if("RIPv2" ==G_wanConnction[i][6] )
		{
			value_array[i].push('<select  id="select_index'+i+'"><option value="RIPv1">1 </option><option value="RIPv2"selected>2 </option></select>');
		}
         else 
		{
			value_array[i].push('<select  id="select_index'+i+'" ><option value="RIPv1">1 </option><option value="RIPv2">2 </option></select>');
		}

		value_array[i].push("Active");
		
		if("off" == G_wanConnction[i][6] || "" == G_wanConnction[i][6])
		    value_array[i].push('<input type="checkbox" id="index' + i + '">');
		else
		   value_array[i].push('<input type="checkbox" id="index' + i + '" checked>');

		
	}

	$T('td_wanriplist',value_array);

}


function uiEdit()
{
	var path;
    var path1;
	for(var i = 0; i < G_wanConnction.length; i++)
	{
		var index = "select_index"+i;
        var index1 = "rip_index"+i;
		var enable = "index" +i;
		
		path =":"+ G_wanConnction[i][4]+".RouteProtocolRx";
        path1 =":"+ G_wanConnction[i][4]+".RipPassive";

		var s=document.getElementById(enable);

		if(s)
		{
			if(!Form.Checkbox(enable))
			{
				$F(path,"off");
                $F(path1,"1");
			}
			else
			{
				$F(path,$(index).value);
                $F(path1,$(index1).checked);
			}
		}
		else
			continue;
	}

		$H({
		'obj-action'       :'set',
		'var:menu'         :'advanced',
		'var:page'         :'route',
		'var:subpage'      :'rip',
		'var:errorpage'    :'rip',
		'getpage'          :'html/index.html',
		'errorpage'        :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});

	
	$('uiPostForm').submit();
}

function uiOnload()
{ 
	createTable();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
