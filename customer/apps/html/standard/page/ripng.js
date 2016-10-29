<?setvaronce var:state 0?>
//get wan connection
var G_wanConnction = [];
var m = 0;
var G_InternetWanType = "Ethernet";
<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection. "X_TWSZ-COM_IPv6Config.ConnectionStatus Name Enable AddressingType RIPngEnabled RipPassive X_TELEFONICA-ES_IPv6Enabled ConnectionType"
			`	G_wanConnction[m] = [];

				G_wanConnction[m][0] = m + 1;    //Index;
				G_wanConnction[m][1] = "<?echo $11?>";   //ConnectionStatus
                G_wanConnction[m][2] = "<?echo $12?>";   //name;
				G_wanConnction[m][3] = "<?echo $13?>";   //Enable;
                G_wanConnction[m][4] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $10?>";  // about Path
				G_wanConnction[m][5] = "<?echo $14?>";   //static or dhcp;
				G_wanConnction[m][6] = "<?echo $15?>";   //RIPngEnabled
				G_wanConnction[m][7] = "-";   // not used
				G_wanConnction[m][8] = "N/A";
				G_wanConnction[m][9] = "<?echo $17?>";   //X_TWSZ-COM_ProtocolType
				G_wanConnction[m][10] = "<?echo $18?>";   // ConnectionType
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
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection. "X_TWSZ-COM_IPv6Config.ConnectionStatus Name Enable ConnectionTrigger RIPngEnabled RipPassive X_TELEFONICA-ES_IPv6Enabled ConnectionType"
			`	G_wanConnction[m] = [];
				G_wanConnction[m][0] = m + 1;    //Index;
				G_wanConnction[m][1] = "<?echo $11?>";   //ConnectionStatus
                G_wanConnction[m][2] = "<?echo $12?>";   //name;
				G_wanConnction[m][3] = "<?echo $13?>";   //Enable;
                G_wanConnction[m][4] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $10?>";  // about Path;
				G_wanConnction[m][5] = "<?echo $14?>";   //pppoe
				G_wanConnction[m][6] = "<?echo $15?>";   //RIPngEnabled
				G_wanConnction[m][7] = "-";   // not used
				G_wanConnction[m][8] = "N/A";
				G_wanConnction[m][9] = "<?echo $17?>";   //X_TWSZ-COM_ProtocolType
				G_wanConnction[m][10] = "<?echo $18?>";   // ConnectionType
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
		if ((G_wanConnction[i][2].indexOf('br') > -1) || (G_wanConnction[i][10].indexOf('Bridged') > -1)) {
			continue;
		}

        // skip non-IPv6 interfaces
		if (G_wanConnction[i][9] == '0') {
			continue;
		}

		value_array[i].push(G_wanConnction[i][2]);
		value_array[i].push(G_wanConnction[i][8]);
		if("0" == G_wanConnction[i][6] || "" == G_wanConnction[i][6])
		    value_array[i].push('<input type="checkbox" id="index' + i + '">');
		else
		   value_array[i].push('<input type="checkbox" id="index' + i + '" checked>');
	}

	$T('td_wanripnglist',value_array);

}

function uiEdit()
{
	var path;
	for(var i = 0; i < G_wanConnction.length; i++)
	{
		if ((G_wanConnction[i][2].indexOf('br') > -1) || (G_wanConnction[i][10].indexOf('Bridged') > -1)) {
			continue;
		}

        // skip non-IPv6 interfaces
		if (G_wanConnction[i][9] == '0') {
			continue;
		}

		path =":"+ G_wanConnction[i][4]+".RIPngEnabled";
		$F(path,Form.Checkbox("index" + i));
	}

	$H({
		'obj-action'       :'set',
		'var:menu'         :'advanced',
		'var:page'         :'route',
		'var:subpage'      :'ripng',
		'var:errorpage'    :'ripng',
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
