//get User Interface
var wanConnection = new Array();
var n = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`    <?if eq $var:G_IPv6Enable 0
			  `   <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			       `wanConnection[n] = new Array();
				    wanConnection[n][0] = "$01";//name
				    wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$00";//path
	               	    wanConnection[n][2] = "$02";
				    wanConnection[n][3] = "$03";
				    ++n;
			       `?>
			 `?>
			 <?if eq $var:G_IPv6Enable 1
			  `     <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled ConnectionType"
			`	    wanConnection[n] = new Array();
				    wanConnection[n][0] = "$01";//name
				    wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$00";//path
	                	    wanConnection[n][2] = "$02";
				    wanConnection[n][3] = "$03";
				    wanConnection[n][4] = "$04";
				    ++n;
			       `?>
			 `?>
		`?>
		<?if gt $12 0
		`	 <?if eq $var:G_IPv6Enable 0
		      `    <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			       `wanConnection[n] = new Array();
				    wanConnection[n][0] = "$01";//name
				    wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$00";//path
	               	    wanConnection[n][2] = "$02";
				    wanConnection[n][3] = "$03";
				    ++n;
			     `?>
			  `?>
			  <?if eq $var:G_IPv6Enable 1
		      `    <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled ConnectionType"
			       `wanConnection[n] = new Array();
				    wanConnection[n][0] = "$01";//name
				    wanConnection[n][1] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$00";//path
	                	    wanConnection[n][2] = "$02";
				    wanConnection[n][3] = "$03";
				    wanConnection[n][4] = "$04";
				    ++n;
			     `?>
			  `?>
		`?>
	`?>
`?>	

//获取桥连接名
//var m = 0;
<?objget :InternetGatewayDevice.LANDevice. "X_TWSZ-COM_DeviceName"
`	wanConnection[n] = new Array();
	wanConnection[n][0] = "$01";//name
	wanConnection[n][1] = "InternetGatewayDevice.LANDevice.$00";//path
	++n;
//	++m;
`?>

//get the configuration
var G_UserInIf   = "<?get :InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService?>";
<?if eq $var:G_IPv6Enable 1
`
var G_UserIp6If  = "<?get :InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.DefaultConnectionService?>";
`?>

function uiOnload()
{
	var text = [], value = [];
	var ip6text = [], ip6value = [];
	var i = 0;
	var j = 0;
	var k = 0;
	//生成wan连接下拉框

	for (i = 0; i < wanConnection.length; i++)
	{   
	    if(wanConnection[i][2]=="TR069")
		{
	       continue;
		}
		if (wanConnection[i][0].indexOf("br") > -1)
		{
	       continue;
		}
		<?if eq $var:G_IPv6Enable 0
		`
		if((wanConnection[i][3]=="IP_Bridged") || (wanConnection[i][3]=="PPPoE_Bridged"))
		{
		   continue;
		}
		`?>
		<?if eq $var:G_IPv6Enable 1
		`
		if((wanConnection[i][4]=="IP_Bridged") || (wanConnection[i][4]=="PPPoE_Bridged"))
		{
		   continue;
		}
		`?>
		text[k]  = wanConnection[i][0];
		value[k] = i;
		k++;
	}
	$S('selectConnectionName', text, value);
	
	for (i = 0; i < wanConnection.length; i++)
	{   
		if(G_UserInIf == wanConnection[i][1])
		$('selectConnectionName').value = i;//interface
	}

	/* IPv6 */
	<?if eq $var:G_IPv6Enable 1
	`
	for (i = 0; i < wanConnection.length; i++)
	{
		if (wanConnection[i][2] == "TR069")
			continue;
 
		if (wanConnection[i][3] != "1")
			continue;

		ip6text[j] = wanConnection[i][0];
		ip6value[j] = i;
		j++;
	}
	$S('selectIp6ConnectionName', ip6text, ip6value);

	for (i = 0; i < wanConnection.length; i++)
	{
		if (G_UserIp6If == wanConnection[i][0])
			$('selectIp6ConnectionName').value = i;
	}
	`?>
}

function uiSubmit()
{
	var wanpath;
	var wanIp6Name;
	var i = 0;

	for (i = 0; i < wanConnection.length; i++)
	{
		if($('selectConnectionName').value == i)
			wanpath = wanConnection[i][1];
	}
    <?if eq $var:G_IPv6Enable 1
	`
	for (i = 0; i < wanConnection.length; i++)
	{
		if ($('selectIp6ConnectionName').value == "")
			wanIp6Name = undefined;

		if ($('selectIp6ConnectionName').value == i)
			wanIp6Name = wanConnection[i][0];
	}
	`?>	
	$H({
	   ':InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService': wanpath,
	  <?if eq $var:G_IPv6Enable 1
	   `
	   ':InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.DefaultConnectionService': wanIp6Name,
	   `?>
	   'obj-action'        :'set',
	   'getpage'           :'html/index.html',
       'errorpage'         :'html/index.html',
	   'var:menu'          :'advanced',
	   'var:page'          :'route',
	   'var:subpage'       :'defaultgw',
	   'var:errorpage'     :'defaultgw',
	   'var:CacheLastData' : ViewState.Save()
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

addListeners(uiOnload, dealWithError);
