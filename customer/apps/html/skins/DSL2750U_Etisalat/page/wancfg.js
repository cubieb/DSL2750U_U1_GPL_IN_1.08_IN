
<?setvaronce var:wanSel 0?>
var wanSel = <?echo $var:wanSel?>; 
var ajax;
var G_DefaultRouter = "<?get :InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW?>";  //默认路由;
//ATM

var G_ATM_num = 0;
var G_WanConns_num = 0;
var temp_num = 0;
var G_WanConns3g_num = 0;
var select_DSL_all = 0;
var select_DSL = 0;
var temp = [];
var G_ATM = [];
var G_WanConns = [];
var G_WanConns3g = [];
var G_AtmValidArry = [];
var G_EthValidArry = [];

<?if eq 1 $var:mod_ETHWAN
`
	var G_ETH_num = 0;
	var select_Eth_all = 0;
	var ethTemp_num = 0;
	var G_EthWanConns_num = 0;
	var ethTemp = [];
	var G_ETH = [];
	var select_Eth = 0;
	var G_EthWanConns = [];
	var G_WANETHEnable= "<?get :InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable?>";
`?>

<?if eq `DSL` `<?get :InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANAccessType?>`
`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS X_TWSZ-COM_VLANID"
	`	 G_ATM[G_ATM_num] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
					"$01",//LinkType
					"$02",//DestinationAddress
					"$03",//ATMEncapsulation
					"$04",//ATMQoS
					"$05" //X_TWSZ-COM_VLANID
				];
				G_ATM_num++;
	`?>
	<?if eq 1 $var:mod_ETHWAN
	`
		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "X_TWSZ-COM_VLANID"
		`	G_ETH[G_ETH_num] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$00.",//Path
						"$01" //X_TWSZ-COM_VLANID
							];
						G_ETH_num++;
		`?>
	`?>
`?>
<?if  eq $var:G_IPv6Enable 0
	`<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`<?if gt $11 0
			`<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType ConnectionType X_TELEFONICA-ES_IPv4Enabled X_TWSZ-COM_ServiceList Backup3G"
				`G_WanConns[G_WanConns_num] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
								"$01",      //status
								"$02",      //name
								"$03",      //enable
								"$04",      //addressingtype
								"$05",      //connectiontype
								"$06",      //ipv4 enable
								"$07",      //WAN service Type
								"$08"       //Backup3G
								];
						++G_WanConns_num;
			`?>
		`?>
	`?>
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger ConnectionType  X_TWSZ-COM_ServiceList Backup3G"
			`		temp[temp_num] =[];
					temp[temp_num][0] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.";//Path
					temp[temp_num][1] =	"$01";    //ConnectionStatus
					temp[temp_num][2] =	"$02";    //name;
					temp[temp_num][3] =	"$03";    //Enable;
					temp[temp_num][4] =	"$04";    //ConnectionTrigger
					temp[temp_num][5] =	"$05";    //ConnectionType
					temp[temp_num][6] =	"PPPoE";
					temp[temp_num][7] = "$06";    //WAN service Type
					temp[temp_num][8] = "$07";      //Backup3G
					++temp_num;
			`?>
		`?>
	`?>
	<?if eq 1 $var:mod_ETHWAN
	`
		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
			`<?if gt $11 0
				`<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType ConnectionType  X_TWSZ-COM_ServiceList"
					`G_EthWanConns[G_EthWanConns_num] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
													"$01",       //ConnectionStatus
													"$02",       //Name
													"$03",       //Enable
													"$04",       //AddressingType
													"$05",       //ConnectionType
													"$06"        // X_TWSZ-COM_ServiceList
													];
													++G_EthWanConns_num;
				`?>
			`?>
		`?>

		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
			`<?if gt $12 0
				`<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger ConnectionType X_TELEFONICA-ES_IPv4Enabled X_TWSZ-COM_ServiceList"
					`ethTemp[ethTemp_num] =[];
					ethTemp[ethTemp_num][0] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00.";//Path
					ethTemp[ethTemp_num][1] = "$01";    //ConnectionStatus
					ethTemp[ethTemp_num][2] = "$02";    //name;
					ethTemp[ethTemp_num][3] = "$03";    //Enable;
					ethTemp[ethTemp_num][4] = "$04";    //ConnectionTrigger
					ethTemp[ethTemp_num][5] = "$05";    //ConnectionType
					ethTemp[ethTemp_num][6] = "PPPoE";  
					ethTemp[ethTemp_num][7] = "$06";    // X_TELEFONICA-ES_IPv4Enabled
					ethTemp[ethTemp_num][8] = "$07";    //X_TWSZ-COM_ServiceList
					++ethTemp_num;
				`?>
			`?>
		`?>
	`?>
`?>

<?if  eq $var:G_IPv6Enable 1
	`<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`<?if gt $11 0
			`<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType ConnectionType  X_TWSZ-COM_ServiceList Backup3G X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.ConnectionStatus X_TELEFONICA-ES_IPv6ConnStatus"
				`G_WanConns[G_WanConns_num] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
							"$01",       //status
							"$02",       //name
							"$03",       //enable
							"$04",       //addressingtype
							"$05",       //connectiontype
							"$06",       //WAN service Type
							"$07",       //Backup3G
							"$08",       //ipv6enable
							"$09",       //ipv6 connectionstatus status
							"$0a"        //ipv6status
							];
							++G_WanConns_num;
			`?>
		`?>
	`?>
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`<?if gt $12 0
			`<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger ConnectionType X_TWSZ-COM_ServiceList Backup3G X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.ConnectionStatus X_TELEFONICA-ES_IPv6ConnStatus"
				`	temp[temp_num] =[];
					temp[temp_num][0] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.";//Path
					temp[temp_num][1] = "$01";      //ConnectionStatus
					temp[temp_num][2] = "$02";      //name;
					temp[temp_num][3] = "$03";      //Enable;
					temp[temp_num][4] = "$04";      //ConnectionTrigger
					temp[temp_num][5] = "$05";      //ConnectionType
					temp[temp_num][6] = "PPPoE";
					temp[temp_num][7] = "$06";      //WAN service Type
					temp[temp_num][8] = "$07";      //Backup3G
					temp[temp_num][9] = "$08";      //ipv6 enable
					temp[temp_num][10]= "$09";     //ipv6 connectionstatus
					temp[temp_num][11]= "$0a";     //IPv6ConnStatus
					++temp_num;
			`?>
		`?>
	`?>
	<?if eq 1 $var:mod_ETHWAN
	`
		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`	<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.ConnectionStatus"
					`	G_EthWanConns[G_EthWanConns_num] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
														"$01",   //ConnectionStatus
														"$02",   //Name
														"$03",   //Enable
														"$04",   //AddressingType
														"$05",   //ConnectionType
														"$06",   //X_TWSZ-COM_ServiceList 
														"$07",   //X_TELEFONICA-ES_IPv6Enabled
														"$08",   //X_TWSZ-COM_IPv6Config.ConnectionStatus
														];
														++G_EthWanConns_num;
				`?>
			`?>
		`?>
		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`	<?if gt $12 0
			`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.ConnectionStatus "
				`	ethTemp[ethTemp_num]    =[];
					ethTemp[ethTemp_num][0] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00.";//Path
					ethTemp[ethTemp_num][1] = "$01";    //ConnectionStatus
					ethTemp[ethTemp_num][2] = "$02";    //name;
					ethTemp[ethTemp_num][3] = "$03";    //Enable;
					ethTemp[ethTemp_num][4] = "$04";    //ConnectionTrigger
					ethTemp[ethTemp_num][5] = "$05";    //ConnectionType
					ethTemp[ethTemp_num][6] = "PPPoE";  
					ethTemp[ethTemp_num][7] = "$06";    //X_TWSZ-COM_ServiceList
					ethTemp[ethTemp_num][8] = "$07";    //X_TELEFONICA-ES_IPv6Enabled
					ethTemp[ethTemp_num][9] = "$08";    //X_TWSZ-COM_IPv6Config.ConnectionStatus
					++ethTemp_num;
				`?>
			`?>
		`?>
	`?>
`?>

function initWanData(){
		for(var i = 0; i < temp.length; i++){
			if(("IP_Routed" == temp[i][5] )){
				G_WanConns[G_WanConns_num]    = [];
				G_WanConns[G_WanConns_num][0] = temp[i][0];
				G_WanConns[G_WanConns_num][1] = temp[i][1];
				G_WanConns[G_WanConns_num][2] = temp[i][2];
				G_WanConns[G_WanConns_num][3] = temp[i][3];
				G_WanConns[G_WanConns_num][4] = temp[i][4];
				G_WanConns[G_WanConns_num][5] = "PPPOE";
				G_WanConns[G_WanConns_num][6] = temp[i][7];
				G_WanConns[G_WanConns_num][7] = temp[i][8];
				<?if  eq $var:G_IPv6Enable 1
				`
					G_WanConns[G_WanConns_num][8] = temp[i][9];
					G_WanConns[G_WanConns_num][9] = temp[i][10];
					G_WanConns[G_WanConns_num][10] = temp[i][11];
				`?>
				++G_WanConns_num;
			}
			else if(( temp[i][5] == "PPPoE_Bridged") && (temp[i][2] != "pppo3g")){
				G_WanConns[G_WanConns_num]    = [];
				G_WanConns[G_WanConns_num][0] = temp[i][0];
				G_WanConns[G_WanConns_num][1] = temp[i][1];
				G_WanConns[G_WanConns_num][2] = temp[i][2];
				G_WanConns[G_WanConns_num][3] = temp[i][3];
				G_WanConns[G_WanConns_num][4] = temp[i][4];
				G_WanConns[G_WanConns_num][5] = "PPPoE_Bridged";
				G_WanConns[G_WanConns_num][6] = temp[i][7];
				G_WanConns[G_WanConns_num][7] = temp[i][8];
				<?if  eq $var:G_IPv6Enable 1
				`
					G_WanConns[G_WanConns_num][8] = temp[i][9];
					G_WanConns[G_WanConns_num][9] = temp[i][10];
					G_WanConns[G_WanConns_num][10]= temp[i][11];
				`?>
				++G_WanConns_num;
			}
			else if("PPPoU" == temp[i][5]){
				G_WanConns3g[G_WanConns3g_num]    = [];
				G_WanConns3g[G_WanConns3g_num][0] = temp[i][0];
				G_WanConns3g[G_WanConns3g_num][1] = temp[i][1];
				G_WanConns3g[G_WanConns3g_num][2] = temp[i][2];
				G_WanConns3g[G_WanConns3g_num][3] = temp[i][3];
				G_WanConns3g[G_WanConns3g_num][4] = temp[i][4];
				G_WanConns3g[G_WanConns3g_num][5] = "PPPOE";
				<?if  eq $var:G_IPv6Enable 1
				`
					G_WanConns3g[G_WanConns3g_num][6] = temp[i][8];
					G_WanConns3g[G_WanConns3g_num][7] = temp[i][9];
				`?>
				++G_WanConns3g_num;
			}
		}
		<?if eq 1 $var:mod_ETHWAN
		`
			for(var i = 0; i < ethTemp.length; i++){
				if(("IP_Routed" == ethTemp[i][5] )){
					G_EthWanConns[G_EthWanConns_num]     = [];
					G_EthWanConns[G_EthWanConns_num] [0] = ethTemp[i][0];
					G_EthWanConns[G_EthWanConns_num] [1] = ethTemp[i][1];
					G_EthWanConns[G_EthWanConns_num] [2] = ethTemp[i][2];
					G_EthWanConns[G_EthWanConns_num] [3] = ethTemp[i][3];
					G_EthWanConns[G_EthWanConns_num] [4] = ethTemp[i][4];
					G_EthWanConns[G_EthWanConns_num] [5] = "PPPOE";
					G_EthWanConns[G_EthWanConns_num] [6] = ethTemp[i][7];
					<?if  eq $var:G_IPv6Enable 1
					`
						G_EthWanConns[G_EthWanConns_num] [7] = ethTemp[i][8];
						G_EthWanConns[G_EthWanConns_num] [8] = ethTemp[i][9];
					`?>
					++G_EthWanConns_num;
				}
				else if(( ethTemp[i][5] == "PPPoE_Bridged") && (ethTemp[i][2] != "pppo3g")){
					G_EthWanConns[G_EthWanConns_num]     = [];
					G_EthWanConns[G_EthWanConns_num] [0] = ethTemp[i][0];
					G_EthWanConns[G_EthWanConns_num] [1] = ethTemp[i][1];
					G_EthWanConns[G_EthWanConns_num] [2] = ethTemp[i][2];
					G_EthWanConns[G_EthWanConns_num] [3] = ethTemp[i][3];
					G_EthWanConns[G_EthWanConns_num] [4] = ethTemp[i][4];
					G_EthWanConns[G_EthWanConns_num] [5] = "PPPoE_Bridged";
					G_EthWanConns[G_EthWanConns_num] [6] = ethTemp[i][7];
					<?if  eq $var:G_IPv6Enable 1
					`
						G_EthWanConns[G_EthWanConns_num] [7] = ethTemp[i][8];
						G_EthWanConns[G_EthWanConns_num] [8] = ethTemp[i][9];
					`?>
					++G_EthWanConns_num;	
				}  
			}
		`?>
}
function initAtmValidArry()
{
	for(var i = 0; i < G_ATM.length; i++)
	{
		G_AtmValidArry[i] = ["invalid",0];
	}
}
function getInvalidAtmCnt()
{
	var i = 0;
	for(var j = 0; j < G_ATM.length; j++)
	{
		if(G_AtmValidArry[j][0] == "invalid")
			i++;
	}
	return i;
}
function initEthValidArry()
{
	for(var i = 0; i < G_ETH.length; i++)
	{
		G_EthValidArry[i] = ["invalid",0];
	}
}
function getInvalidEthCnt()
{
	var i = 0;
	for(var j = 0; j < G_ETH.length; j++)
	{
		if(G_EthValidArry[j][0] == "invalid")
			i++;
	}
	return i;
}


function uiOnload(){
	// reboot only both these two conditions met:
	//1.want to reboot,this means tag had been set value as 'reboot'
	//2.wan type changed successfully,this means error code is 0

	if(userLevel == '2')
	{
		$('add').style.display = 'none';
		$('del').style.display = 'none';
		$('lang_introduction_admin').style.display = 'none';
		$('lang_introduction_user').style.display = 'block';
	}
	<?if eq 1 $var:mod_ETHWAN
	`
		Form.Radio('INPUT_WanAcsType',wanSel);
		select_Eth_all = 0;
	`?>
	select_DSL_all = 0;
	initWanData();
	if(('reboot' == '<?echo $var:tag?>')
		&&("<?error found?>" == "0")){
		$H({
			'obj-action'     : 'reboot',
			'var:page'       : 'system',
			'var:noredirect' : '1',
			'getpage'        : 'html/page/restarting.html'
		});
		$('uiPostForm').submit();
	}

	$('3g1').style.display = 'none';
	$('3g2').style.display = 'none';
	$('3g3').style.display = 'none';
	$('lang_config2').style.display = 'none';
	initAtmValidArry();	
	/*createDSLTable() 和 createEthernetTable()这两个函数在这里的位置不能调换，调换之后会影响到单选框的序列号 */
	createDSLTable();
	
	<?if eq 1 $var:mod_ETHWAN
	`
		initEthValidArry();
		createEthernetTable();
	`?>
	
	checkShowBtn();
}

function select_choose()
{
	<?if neq 1 $var:mod_ETHWAN
	`
		for(var i = 0; i < (G_WanConns.length + getInvalidAtmCnt()); i++){
			var tmpid_dsl = "choose" + i;
			if (Form.Checkbox(tmpid_dsl) == 1)
			{
				select_DSL = i;
				select_Eth_all = 0;
				select_DSL_all = 1;
				return;
			}		
		}
	`?>
	
	<?if eq 1 $var:mod_ETHWAN
	`
		for(var i = 0; i < (G_WanConns.length + getInvalidAtmCnt()) ; i++){
			var tmpid_dsl = "choose" + i;
			if (Form.Checkbox(tmpid_dsl) == 1)
			{
				select_DSL = i;
				select_Eth_all = 0;
				select_DSL_all = 1;
				return;
			}		
		}

		for(var i = 0; i < (G_EthWanConns.length + getInvalidEthCnt()); i++){
			var select_tmp = G_WanConns.length + getInvalidAtmCnt() + i;
			var tmpid_eth = "choose" + select_tmp;
			if (Form.Checkbox(tmpid_eth) == 1)
			{
				select_Eth = i;
				select_Eth_all = 1;
				select_DSL_all = 0;
				return;
			}
		}
	`?>
}

//以太网上?
function createEthernetTable(){
	var value_array = [];
	var connect;
	var invalidAtmCnt = getInvalidAtmCnt();
	var i = 0;
	var select_eth_tmp = G_WanConns.length + invalidAtmCnt;
	
	for(var j = 0, _len = G_EthWanConns.length; i < _len; i++){
	j = getAtmParam_Eth(i);	
	value_array[i] = [((G_WanConns.length + invalidAtmCnt) == 0 && i == 0) ? '<input type="radio" id="choose'+select_eth_tmp+'" name="choose" value="'+select_eth_tmp+'" checked >' : '<input type="radio" id="choose'+select_eth_tmp+'" name="choose" value="'+select_eth_tmp+'" >',
					 G_ETH[j][1], //VLAN ID
					 G_EthWanConns[i][2], //Service Name
					 G_EthWanConns[i][5] == 'PPPOE' ? G_EthWanConns[i][5] : (G_EthWanConns[i][5] == "IP_Bridged"? "Bridge" : G_EthWanConns[i][4]), //Protocol
					 G_EthWanConns[i][3], //Enable
					 G_EthWanConns[i][1] == 'Disconnected' ? data_language.dynamic.disconnection : data_language.dynamic.connection, //Status
					 G_EthWanConns[i][0].indexOf('PPP') > 0 ? '<input type="button" value=' + 
										(G_EthWanConns[i][1] == 'Connected' ? data_language.dynamic.disconnect : data_language.dynamic.connect) 
										  + ' onclick="ctrlEthConnEnable(' + i + ')" ' + (G_EthWanConns[i][4] == 'Manual' ? '' : 'disabled') + '>' : '-'
					 ];

	G_EthValidArry[j][0] = "valid";	
	
	<?if  eq $var:G_IPv6Enable 1
	`						  
		if (G_EthWanConns[i][7] == "1") {
			if(G_EthWanConns[i][4] == 'OnDemand'){
				value_array[i][5] = ((G_EthWanConns[i][8] == 'Disconnected' || G_EthWanConns[i][8] == 'LinkLocalConnected') 
									&& ((G_EthWanConns[i][1] == 'Disconnected') || (G_EthWanConns[i][1] == 'Unconfigured'))) 
									? data_language.dynamic.disconnection : data_language.dynamic.connection ; //Status
			 }
			else{
				value_array[i][5] = ((G_EthWanConns[i][8] == 'Disconnected' || G_EthWanConns[i][8] == 'LinkLocalConnected') 
									&& (G_EthWanConns[i][1] == 'Disconnected')) ? data_language.dynamic.disconnection : data_language.dynamic.connection ; //Status
				}
			// IPv4_6
			value_array[i][6] = G_EthWanConns[i][0].indexOf('PPP') > 0 ? '<input type="button" value=' + 
							((G_EthWanConns[i][1] == 'Connected' || G_EthWanConns[i][8] == 'GlobalConnected') 
							? data_language.dynamic.disconnect: data_language.dynamic.connect ) + ' onclick="ctrlConnEnable(' + i + ')" '
							+ (G_EthWanConns[i][4] == 'Manual' ? '' : 'disabled') + '>' : '-';
		}
	`?>
	  select_eth_tmp++;
	}
	/*display the invalid WANConnectionDevice instance*/
	for(var j=0; j < G_ETH.length; j++)
	{
		if(G_EthValidArry[j][0] != "valid")
		{
			value_array[i]= [((G_WanConns.length + invalidAtmCnt) == 0 && i == 0) ? '<input type="radio" id="choose'+select_eth_tmp+'" name="choose" value="'+select_eth_tmp+'" checked >' : '<input type="radio" id="choose'+select_eth_tmp+'" name="choose" value="'+select_eth_tmp+'" >',
					 G_ETH[j][1], //VLAN ID
					 "Unknown", //Service Name
					 "Unknown", //Protocol
					 "Unknown", //Enable
					 "Unknown", //Status
					 "Unknown"
					 ];
			G_EthValidArry[j][1]=i;
			i++;
			select_eth_tmp++;
		}
	}	
	$T('wan_eth_conns_list', value_array);
}

//DSL
function createDSLTable(){ //生成WAN连接列表
	var value_array = [];
	var value_array3g = [];
	var connect;
	var i = 0;

	for(var j = 0, _len = G_WanConns.length; i < _len; i++){
		
	j = getAtmParam_DSL(i);
	value_array[i] = [
					 (i == 0)? '<input type="radio" id="choose'+i+'" name="choose" value="'+i+'" checked>' : '<input type="radio" id="choose'+i+'" name="choose" value="'+i+'">',
					G_ATM[j][2].split(':')[1], //VPI/VCI
					G_ATM[j][5], //VLAN ID
					G_ATM[j][3], //VLAN Mux
					G_WanConns[i][2].ellipsis(18), //Service Name
					(G_ATM[j][1] == 'EoA' && G_WanConns[i][0].indexOf('IP') > -1) ?
					G_WanConns[i][5] == 'IP_Bridged' ? 'Bridge' : G_WanConns[i][4] :
					(G_ATM[j][1] == 'EoA' && G_WanConns[i][0].indexOf('PPP') > -1) ? 
					(G_WanConns[i][5] == 'PPPoE_Bridged'? 'Bridge' : 'PPPoE' ): G_ATM[j][1], //Protocol
					G_WanConns[i][3], //Enable
					G_WanConns[i][1] == 'Connected' ? data_language.dynamic.connection : data_language.dynamic.disconnection, //Status
						 
					//scb+ 2012-8-1
					<?if eq 1 $var:mod_NEW3G
					`
						G_WanConns[i][5] == 'IP_Bridged' ? '-' : G_WanConns[i][7],
					`?>
					(G_WanConns[i][0].indexOf('PPP') > 0 &&  G_WanConns[i][5]!='PPPoE_Bridged')? 
					'<input type="button" value=' + (G_WanConns[i][1] == 'Connected' ? data_language.dynamic.disconnect : data_language.dynamic.connect) + ' onclick="ctrlConnEnable(' + i + ')" ' + (G_WanConns[i][4] == 'Manual' ? '' : 'disabled') + '>' : '-'
					];
	G_AtmValidArry[j][0] = "valid";

	<?if  eq $var:G_IPv6Enable 1
	`	
		if (G_WanConns[i][8] == "1") {
			if(G_WanConns[i][4] == 'OnDemand'){
				value_array[i][7] = ((G_WanConns[i][10] == 'Disconnected' || G_WanConns[i][10] == 'LinkLocalConnected') 
									&& ((G_WanConns[i][1] == 'Disconnected') || (G_WanConns[i][1] == 'Unconfigured'))) 
									? data_language.dynamic.disconnection : data_language.dynamic.connection ; //Status
			 }
			else{
				value_array[i][7] = ((G_WanConns[i][9] == 'Disconnected' || G_WanConns[i][9] == 'LinkLocalConnected') 
									&& (G_WanConns[i][1] == 'Disconnected')) ? data_language.dynamic.disconnection : data_language.dynamic.connection ; //Status
				}
			// IPv4_6
			value_array[i][8] = G_WanConns[i][0].indexOf('PPP') > 0 ? '<input type="button" value=' + 
								((G_WanConns[i][1] == 'Connected' || G_WanConns[i][9] == 'GlobalConnected') 
								? data_language.dynamic.disconnect: data_language.dynamic.connect ) + ' onclick="ctrlConnEnable(' + i + ')" '
								+ (G_WanConns[i][4] == 'Manual' ? '' : 'disabled') + '>' : '-';
		}
	`?>
	}

	/*display the invalid WANConnectionDevice instance*/
	for(var j=0; j < G_ATM.length; j++)
	{
		if(G_AtmValidArry[j][0] != "valid")
		{
			value_array[i]= [
					(i == 0)? '<input type="radio" id="choose'+i+'" name="choose" value="'+i+'" checked>' : '<input type="radio" id="choose'+i+'" name="choose" value="'+i+'">',
					G_ATM[j][2].split(':')[1], //VPI/VCI
					G_ATM[j][5], //VLAN ID
					G_ATM[j][3], //VLAN Mux
					"Unknown", //Service Name
					"Unknown", //Protocol
					"Unknown", //Enable
					"Unknown", //Status
					"Unknown"
					];
			G_AtmValidArry[j][1]=i;
			i++;
		}
	}

	$T('wan_conns_list', value_array);
	
	for(var i = 0; i < G_WanConns.length; i++){
		$('wan_conns_list_' + i + '4').title = G_WanConns[i][2];
	}
	
	<?if eq 1 $var:mod_NEW3G
	`
		for(var i = 0, j, _len = G_WanConns3g.length; i < _len; i++){
			j = getAtmParam3g(i);
			
			value_array3g[i] = ['<input type="radio" id="choose'+i+'" name="choose" value="'+i+'">',
							G_WanConns3g[i][2], //Service Name
							(G_ATM[j][1] == 'EoA' && G_WanConns3g[i][0].indexOf('IP') > -1) ?
							G_WanConns3g[i][5] == 'IP_Bridged' ? 'Bridge' : G_WanConns3g[i][4] :
													(G_ATM[j][1] == 'EoA' && G_WanConns3g[i][0].indexOf('PPP') > -1) ? 'PPPoE' : G_ATM[j][1], //Protocol
							G_WanConns3g[i][3], //Enable
							G_WanConns3g[i][1] == 'Connected' ? data_language.dynamic.connection : data_language.dynamic.disconnection, //Status
							G_WanConns3g[i][5] == 'IP_Bridged' ? '-' : '<input type="checkbox" id="INPUT_DefRoutingBtn' + i 
													+ '" onclick="ctrlDefaultRouting3g(' + i + ')"' + ((G_DefaultRouter + '.') == G_WanConns3g[i][0] ? 'checked' : '') 
													+ '>', //Default Gateway
							G_WanConns3g[i][0].indexOf('PPP') > 0 ? '<input type="button" value=' 
													+ (G_WanConns3g[i][1] == 'Connected' ? data_language.dynamic.disconnect : data_language.dynamic.connect)
													+ ' onclick="ctrlConnEnable3g(' + i + ')" ' + (G_WanConns3g[i][4] == 'Manual' ? '' : 'disabled') + '>' : '-'
							];
		}
		$T('wan_conns_list_3g', value_array3g);
	`?>
}

function checkShowBtn()
{
	<?if eq 1 $var:mod_ETHWAN
	`
		if (G_EthWanConns.length != 0 ||  G_WanConns.length != 0) {
			$('add').disabled = 0;
			$('del').disabled = 0;
			$('edit').disabled = 0;
		} else {
			$('add').disabled = 0;
			$('del').disabled = 1;
			$('edit').disabled = 1;
		}
		//if wan connection is invalid, delete button should be enabled
		if(G_ETH_num > 0)
		{
			$('del').disabled = 0;
		}
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`
		if (G_WanConns.length != 0 ) {
			$('add').disabled = 0;
			$('del').disabled = 0;
			$('edit').disabled = 0;
		} else {
			$('add').disabled = 0;
			$('del').disabled = 1;
			$('edit').disabled = 1;
		}
		//if wan connection is invalid, delete button should be enabled
		if(G_ATM_num > 0)
		{
			$('del').disabled = 0;
		}
	`?>
}
<?if eq 1 $var:mod_ETHWAN
`
function changetypeSelect(value){
	wanSel= value;
	$H({
		'var:wanSel'   : wanSel,
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}
`?>
//管理WAN连接
function ctrlConnEnable(_i){
	var st_conn_DSL;
//	var st_conn3g;
	<?if  eq $var:G_IPv6Enable 0
	`
		if (G_WanConns[_i]) {
			if(G_WanConns[_i] && G_WanConns[_i][1] == 'Connected'){
				st_conn_DSL = 'Disconnect';
			} else if(G_WanConns[_i] && G_WanConns[_i][1] != 'Connected'){
				st_conn_DSL = 'Connect';
			} else {
				return false;
			}
		}
	`?>
	<?if  eq $var:G_IPv6Enable 1               
	`
		if (G_WanConns[_i] && G_WanConns[_i][8] == '0') {         //若IPV6未开启
			if(G_WanConns[_i] && G_WanConns[_i][1] == 'Connected'){   //ipv4已连接
				st_conn_DSL = 'Disconnect';
			} else if(G_WanConns[_i] && G_WanConns[_i][1] != 'Connected'){  //ipv4未连接
				st_conn_DSL = 'Connect';
			} else {
				return false;
			}
		} else {
			if(G_WanConns[_i] && G_WanConns[_i][1] == 'Connected') // 若IPV6已开启,且ipv4已连接
				st_conn_DSL = 'Disconnect';
			else if(G_WanConns[_i] && G_WanConns[_i][9] == 'Disconnected') //且ipv4未连接，IPV6也未连接
				st_conn_DSL = 'Connect';
			else
				st_conn_DSL = 'Disconnect';
		}
	`?>
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);
	//Enable
	$F(':' + G_WanConns[_i][0] + 'ConnectionTrigger', 'Manual');
	$F(':' + G_WanConns[_i][0] + 'X_TWSZ-COM_ConnectionAction', st_conn_DSL);
	 
	$('uiPostForm').submit();
}

function ctrlEthConnEnable(_i){
	var st_conn_Eth;
	<?if  eq $var:G_IPv6Enable 0
	`
		<?if eq 1 $var:mod_ETHWAN
		`
			if (G_EthWanConns[_i]) {
				if(G_EthWanConns[_i] && G_EthWanConns[_i][1] == 'Connected'){
					st_conn_Eth = 'Disconnect';
				} else if(G_EthWanConns[_i] && G_EthWanConns[_i][1] != 'Connected'){
					st_conn_Eth = 'Connect';
				} else {
					return false;
				}
			}
		`?>
	`?>
	<?if  eq $var:G_IPv6Enable 1               
	`
		<?if eq 1 $var:mod_ETHWAN
		`
			if (G_EthWanConns[_i] && G_EthWanConns[_i][7] == '0') {
				if(G_EthWanConns[_i] && G_EthWanConns[_i][1] == 'Connected'){
					st_conn_Eth = 'Disconnect';
				} else if(G_EthWanConns[_i] && G_EthWanConns[_i][1] != 'Connected'){
					st_conn_Eth = 'Connect';
				} else {
					return false;
				}
			} else {
				if(G_EthWanConns[_i] && G_EthWanConns[_i][1] == 'Connected') // Judge when enable IPv4_6
					st_conn_Eth = 'Disconnect';
				else if(G_EthWanConns[_i] && G_EthWanConns[_i][8] == 'Disconnected')
					st_conn_Eth = 'Connect';
				else
					st_conn_Eth = 'Disconnect';
			}
		`?>
	`?>
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);
	//Enable
	<?if eq 1 $var:mod_ETHWAN
	`
		$F(':' + G_EthWanConns[_i][0] + 'ConnectionTrigger', 'Manual');
		$F(':' + G_EthWanConns[_i][0] + 'X_TWSZ-COM_ConnectionAction', st_conn_Eth);
	`?>
	
	$('uiPostForm').submit();
}	

//通过对应ATM, 查找相应的数组索?
function getAtmParam_DSL(_i){
	var path_wan = G_WanConns[_i][0];
	
	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}
//通过对应ATM, 查找相应的数组索?
function getAtmParam_Eth(_i){
	var path_wan = G_EthWanConns[_i][0];
	
	for(var i = 0; i < G_ETH.length; i++){
		if(path_wan.indexOf(G_ETH[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}
function ctrlConnEnable3g(_i){
	var st_conn;

	if(G_WanConns3g[_i] && G_WanConns3g[_i][1] == 'Connected'){
		st_conn = 'Disconnect';
	} else if(G_WanConns3g[_i] && G_WanConns3g[_i][1] != 'Connected'){
		st_conn = 'Connect';
	} else {
		return false;
	}
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(':' + G_WanConns3g[_i][0] + 'ConnectionTrigger', 'Manual');
	$F(':' + G_WanConns3g[_i][0] + 'X_TWSZ-COM_ConnectionAction', st_conn);
	
	$('uiPostForm').submit();
}

function getAtmParam3g(_i){
	var path_wan = G_WanConns3g[_i][0];
	
	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}

//控制默认路由的函?
function ctrlDefaultRouting3g(_id){
	var _path = G_WanConns3g[_id][0].replace(/(\.*$)/g,'');
	if(_path == G_DefaultRouter){
		_path = '';
	}
	
	$H({
		':InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService' : _path,
		'obj-action'    : 'set',
		'var:menu'      : 'setup',
		'var:page'      : 'wancfg',
		'var:errorpage' : 'wancfg',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);
	$('uiPostForm').submit();
}

function uiAdd(){
	$('add').disabled = 1;
	$('edit').disabled = 1;
	$('del').disabled = 1;
	var _url;
	<?if eq 1 $var:mod_ETHWAN
	`
		if(wanSel == 1){
			_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanethernet';
		} else {
			_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanadsl';
		}  
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`
		_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanadsl';
	`?>
	document.location.href = _url;
}

function uiEdit(){
	$('add').disabled = 1;
	$('edit').disabled = 1;
	$('del').disabled = 1;
	select_choose();
<?if eq 1 $var:mod_ETHWAN
	`
	if(1 == select_Eth_all){
		var wan_conn = G_EthWanConns[select_Eth][0];
	
		$H({
			'var:conn_type' : G_EthWanConns[select_Eth][5] == 'PPPOE' ? G_EthWanConns[select_Eth][5] : (G_EthWanConns[select_Eth][5] == "IP_Bridged"? "Bridge" : G_EthWanConns[select_Eth][4]),
			'var:secIdx'    : wan_conn.split('.')[4],
			'var:thdIdx'    : wan_conn.split('.')[6],
			'var:menu'      : '<?echo $var:menu?>',
			'var:page'      : '<?echo $var:page?>',
			'var:subpage'   : 'wanethernet',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		});
	}
	else if(1 == select_DSL_all){
		var wan_conn = G_WanConns[select_DSL][0];
	
		$H({
			'var:conn_type' : wan_conn.indexOf('PPP') > -1 ? 'PPP' : 'IP', //IP/PPP
			'var:secIdx'    : wan_conn.split('.')[4],
			'var:thdIdx'    : wan_conn.split('.')[6],
			'var:menu'      : '<?echo $var:menu?>',
			'var:page'      : '<?echo $var:page?>',
			'var:subpage'   : 'wanadsl',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		});	
	}
`?>
<?if neq 1 $var:mod_ETHWAN
`
	var wan_conn = G_WanConns[select_DSL][0];
	if(userLevel == '2')
	{
		if(G_WanConns[select_DSL][2].indexOf('5/55') < 0 && G_WanConns[select_DSL][2].indexOf('8/88') <0 && 
			G_WanConns[select_DSL][5].indexOf('PPPOE') > -1)
		{
			$H({
				'var:conn_type' : wan_conn.indexOf('PPP') > -1 ? 'PPP' : 'IP', //IP/PPP
				'var:secIdx'    : wan_conn.split('.')[4],
				'var:thdIdx'    : wan_conn.split('.')[6],
				'var:menu'      : '<?echo $var:menu?>',
				'var:page'      : '<?echo $var:page?>',
				'var:subpage'   : 'wanadsl_user',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			});
		}
		else
		{
			alert(" Permission denied");
			$('edit').disabled = 0;
			return false;
		}

	}
	else
	{
		$H({
			'var:conn_type' : wan_conn.indexOf('PPP') > -1 ? 'PPP' : 'IP', //IP/PPP
			'var:secIdx'    : wan_conn.split('.')[4],
			'var:thdIdx'    : wan_conn.split('.')[6],
			'var:menu'      : '<?echo $var:menu?>',
			'var:page'      : '<?echo $var:page?>',
			'var:subpage'   : 'wanadsl',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		});	
	}
`?>
	$('uiPostForm').submit();
}

function uiDelete(){
	$('add').disabled = 1;
	$('edit').disabled = 1;
	$('del').disabled = 1;
	select_choose();
<?if eq 1 $var:mod_ETHWAN
`
	if(1 == select_Eth_all){
		if(1 == G_EthWanConns_num){
			uiAjax_Set();
		}else{
			ajax_Del_eth();
		}
	}
	if(1 == select_DSL_all){
		ajax_Del_adsl();
	}
`?>
<?if neq 1 $var:mod_ETHWAN
`
	ajax_Del_adsl();
`?>
}

function uiAjax_Set(){
	$H({
		':InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable' : "0",
		'getpage'       : 'html/page/wiz_ajax.js',
		'var:page'      : 'wancfg',
		'obj-action'    : 'set'
	}, true);	
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_Del_eth);
	ajax.post($('uiPostForm'));	
	
}
function ajax_Del_eth(){
	var selectPath = '';
	if(select_Eth >= G_EthWanConns.length)
	{
		for(var i = 0; i < G_ETH.length; i++)
		{
			if(G_EthValidArry[i][1] == select_Eth)
			{
				//select a invalid wan connection
				selectPath = G_ETH[i][0];
			}
		}
	}
	else
	{
		selectPath = G_EthWanConns[select_Eth][0]
	}

	if(selectPath == '')
	{
		alert("wan connection selected is invalid!");
		return;
	}
	
	$H({
		'mid'           : '0430',
		'del-obj'       : selectPath,
		'obj-action'    : 'del',
		'getpage'       : 'html/page/wiz_ajax.js',
		'var:page'      : 'wancfg'
	},true);	

	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_SetProc);
	ajax.post($('uiPostForm'));
}

function ajax_Del_adsl(){
	var selectPath = '';
	if(select_DSL >= G_WanConns.length)
	{
		for(var i = 0; i < G_ATM.length; i++)
		{
			if(G_AtmValidArry[i][1] == select_DSL)
			{
				//select a invalid wan connection
				selectPath = G_ATM[i][0];
			}
		}
	}
	else
	{
		selectPath = G_WanConns[select_DSL][0]
	}

	if(selectPath == '')
	{
		alert("wan connection selected is invalid!");
		return;
	}

	$H({
		'mid'           : '0430',
		'del-obj'       : selectPath,
		'obj-action'    : 'del',
		'getpage'       : 'html/page/wiz_ajax.js',
		'var:page'      : 'wancfg'
	},true);
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_SetProc);
	ajax.post($('uiPostForm'));
}

function ajax_SetProc(_text){
	//Success, then jump to page
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1'
		return false;
	}
	
	if (G_Error == '0'){
		$G('/cgi-bin/webproc',{
			'var:menu'			: 'setup',
			'var:page'			: 'wancfg',
			'getpage'			: 'html/index.html',
			'errorpage'			: 'html/index.html'
		});	
	} else if(G_Error == '1'){
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg';		
	}
}
function uiAdd3g(){
	var _url;	
	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3g';
	
	document.location.href = _url;
}

function uiEdit3g(){
	if(_array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	} else
	if(_array_id.length == 0){
		alert(SEcode[9888]);
		return false;
	}
	
	var wan_conn = G_WanConns3g[_array_id[0]][0];
	
	$H({
		'var:conn_type' : wan_conn.indexOf('PPP') > -1 ? 'PPP' : 'IP', //IP/PPP
		'var:secIdx'    : wan_conn.split('.')[4],
		'var:thdIdx'    : wan_conn.split('.')[6],
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wan3g',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function uiDelete3g(){
	if(_array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	} else
	if(_array_id.length == 0){
		alert(SEcode[9888]);
		return false;
	}

	var Idx = _array_id[0];
	$H({
		'mid'           : '0430',
		'del-obj'       : G_WanConns3g[Idx][0],
		'obj-action'    : 'del',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>'
	},true);
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
