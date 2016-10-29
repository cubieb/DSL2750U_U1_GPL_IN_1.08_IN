
/*********************       get lan statistics *************************/
var G_LanStatus = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "Status Stats.BytesSent Stats.BytesReceived Stats.PacketsSent Stats.PacketsReceived MACAddress X_TWSZ-COM_Description"
`	G_LanStatus[m] = [];
	G_LanStatus[m][0] = "$01"; //status
	G_LanStatus[m][1] = "$02"; //bytesSent
	G_LanStatus[m][2] = "$03"; //BytesReceived
	G_LanStatus[m][3] = "$04"; //PacketsSent
	G_LanStatus[m][4] = "$05"; //PacketsReceived
	G_LanStatus[m][5] = "0"; //RxError
	G_LanStatus[m][6] = "0"; //TxError
	G_LanStatus[m][7] = "0"; //RxDrop
	G_LanStatus[m][8] = "0"; //TxDrop
	G_LanStatus[m][9] = "$06";
	G_LanStatus[m][10] = "$07";
	m++;
`?>
/****************************************************************************/


/**********************        get wireless statistics    ***********************/
var G_WirelessStatus = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "Enable TotalBytesSent TotalBytesReceived TotalPacketsSent TotalPacketsReceived X_TWSZ-COM_TotalRxError X_TWSZ-COM_TotalTxError X_TWSZ-COM_TotalRxDrop X_TWSZ-COM_TotalTxDrop SSID"
`	G_WirelessStatus[m] = [];
	G_WirelessStatus[m][0] = "$01"; //Enable
	G_WirelessStatus[m][1] = "$02"; //byteSent
	G_WirelessStatus[m][2] = "$03"; //BytesReceive
	G_WirelessStatus[m][3] = "$04"; //PacketsSent
	G_WirelessStatus[m][4] = "$05"; //PacketsReceived
	G_WirelessStatus[m][5] = "$06"; //RxError
	G_WirelessStatus[m][6] = "$07"; //TxError
	G_WirelessStatus[m][7] = "$08"; //RxDrop
	G_WirelessStatus[m][8] = "$09"; //TxDrop
	G_WirelessStatus[m][9] = "$0a"; //SSID
	m++;
`?>

/**********************************************************************/


/***********************      get adsl statistics       ******************************/
var G_adsl_mode;
var G_adsl_type;
var G_line_code;
var G_status;
var G_UpTime;
//---------------------------------------
var G_up_margin
var G_down_margin;
var G_up_attenuation;
var G_down_attenuation;
var G_up_power;
var G_down_power;
var G_up_attain;
var G_down_attain;
var G_up_rate;
var G_down_rate;
var G_up_interleave;
var G_down_interleave;
var G_up_delay;
var G_down_delay;
//---------------------------------------
var G_up_hec_errors;
var G_down_hec_errors;
var G_up_ocd_errors;
var G_down_ocd_errors;
var G_up_lcd_errors;
var G_down_lcd_errors;
var G_up_total;
var G_down_total;
var G_down_crc_errors;
var G_up_crc_errors;
var G_down_frames;
var G_up_frames;
var G_up_fec_errors;
var G_down_fec_errors;
var G_up_blocks;
var G_down_blocks;
var G_flag_up;
var G_flag_down;
var G_sum_up=0;
var G_sum_down=0;

//---------------------------------------
var G_ATM = [];
var m = 0;
var G_buttonClearUp = data_language.value.clear_up;
var G_buttonClearDown = data_language.value.clear_down;
//---------------------------------------
<?if eq 1 $var:mod_ETHWAN
`
	var enable_ethwan = "<?get :InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable?>";  //默认路由;
`?>
var G_InternetWanType = "Ethernet";
<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?if eq `DSL` `<?echo $21?>`
	`	G_InternetWanType = "<?echo $21?>";
		<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig. "StandardUsed Latency TrellisCoding Status UpTime"
		`	G_adsl_mode = "$01";
			G_adsl_type = "$02";
			G_line_code = "$03"=="Enable" ? data_language.dynamic.Enable :data_language.dynamic.DISABLE;
			G_status    = "$04";
			G_UpTime    = "$05";
		` ?>


		<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig. "UpstreamNoiseMargin DownstreamNoiseMargin UpstreamAttenuation DownstreamAttenuation UpstreamPower DownstreamPower UpstreamAttainableRate  DownstreamAttainableRate UpstreamCurrRate DownstreamCurrRate UpstreamInterleaveDepth DownstreamInterleaveDepth UpstreamDelay DownstreamDelay"
		`	G_up_margin = "$01";
			G_down_margin = "$02";
			G_up_attenuation = "$03";
			G_down_attenuation = "$04";
			G_up_power = "$05";
			G_down_power = "$06";
			G_up_attain = "$07";
			G_down_attain = "$08";
			G_up_rate = "$09";
			G_down_rate = "$0a";
			G_up_interleave = "$0b";
			G_down_interleave = "$0c";
			G_up_delay = "$0d";
			G_down_delay = "$0e";
	` ?>

		<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Total. "UpstreamHECErrors DownstreamHECErrors UpstreamOCDErrors DownstreamOCDErrors UpstreamLCDErrors DownstreamLCDErrors UpstreamTotalES DownstreamTotalES UpstreamCRCErrors DownstreamCRCErrors UpstreamTotalFrames DownstreamTotalFrames"
		`	G_up_hec_errors = "$01";
			G_down_hec_errors = "$02";
			G_up_ocd_errors = "$03";
			G_down_ocd_errors = "$04";
			G_up_lcd_errors = "$05";
			G_down_lcd_errors = "$06";
			G_up_total = "$07";
			G_down_total = "$08";
			G_up_crc_errors = "$09";
                     G_down_crc_errors = "$0a";
			G_up_frames = "$0b";
			G_down_frames = "$0c";
		` ?>

		<?objget :InternetGatewayDevice.WANDevice.$20.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS"
		`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.$20.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02",//DestinationAddress
				"$03",//ATMEncapsulation
				"$04" //ATMQoS
				];
			m++;
		`?>

	`?>
`?>

<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?if eq `DSL` `<?echo $21?>`
	`	G_InternetWanType = "<?echo $21?>";
		<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig. "UpstreamBlocks DownstreamBlocks UpstreamFlag DownstreamFlag"
		`	G_up_blocks = "$01";
			G_down_blocks = "$02";
			G_flag_up = "$03";
			G_flag_down = "$04";
		` ?>

		<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Total. "UpstreamFECErrors DownstreamFECErrors"
		`	G_up_fec_errors = "$01";
			G_down_fec_errors = "$02";
		` ?>
	`?>
`?>

//Wan Conns List
var G_WanConns = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType AddressingType Stats.EthernetBytesReceived Stats.EthernetPacketsReceived Stats.EthernetDiscardPacketsReceived Stats.EthernetErrorsReceived Stats.EthernetBytesSent Stats.EthernetPacketsSent Stats.EthernetDiscardPacketsSent Stats.EthernetErrorsSent"
			`	G_WanConns[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
								 "$01",   //name
								 "$02" ,    // connection type
								 "$03",   //address type
								  "$04",	 //r_byte
								 "$05",     //r_pkts
								 "$06",		//r_drop
								 "$07",		//r_error
								 "$08",		//t_type
								 "$09",		//t_pkts
								 "$0a"	,		//t_drop
								 "$0b"			//t_error
								];			
				++m;
			`?>
		`?>
		<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name ConnectionType Stats.EthernetBytesReceived Stats.EthernetPacketsReceived Stats.EthernetDiscardPacketsReceived Stats.EthernetErrorsReceived Stats.EthernetBytesSent Stats.EthernetPacketsSent Stats.EthernetDiscardPacketsSent Stats.EthernetErrorsSent"
			`	G_WanConns[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
								 "$01",   //name
								 "$02" ,      //connection type
								 "pppoe",   //address type
								  "$03",	 //r_byte
								 "$04",     //r_pkts
								 "$05",		//r_drop
								 "$06",		//r_error
								 "$07",		//t_type
								 "$08",		//t_pkts
								 "$09"	,		//t_drop
								 "$0a"			//t_error
								 ];
				++m;
			`?>
		`?>
	`?>
`?>


/****************************************************************************************/

function load_adsl()
{
	/*clear相关_____start*/
	//G_up_blocks == '' ? '0' : G_up_blocks;
	//G_down_blocks == '' ? '0' : G_down_blocks;
	G_flag_up == '' ? '0' : G_flag_up;
	G_flag_down == '' ? '0' : G_flag_down;
	if(G_up_blocks=='')
		G_up_blocks=0;
	if(G_down_blocks=='')
		G_down_blocks=0;
	if(((G_flag_up>G_up_blocks) || (G_flag_down>G_down_blocks)) && G_flag_down!=0)
	{
		Clear_Blocks();
	}
	G_sum_up=G_up_blocks-G_flag_up;
	G_sum_down=G_down_blocks-G_flag_down;
	/*clear相关_____end*/
		
	setJSONValue({
		'adsl_mode'					: G_adsl_mode == '' ? '0' : G_adsl_mode,
		'adsl_type'					: G_adsl_type == '' ? '0' : G_adsl_type,
		'adsl_line_coding'			: G_line_code,
		'adsl_status'				: G_status,
		'up_snr_margin'				: G_up_margin,
		'down_snr_margin'			: G_down_margin,
		'up_Attenuation'			: G_up_attenuation,
		'down_Attenuation'			: G_down_attenuation,
		'up_output_power'			: G_up_power,
		'down_output_power'			: G_down_power,
		'up_attainable_rate'		: G_up_attain,
		'down_attainable_rate'		: G_down_attain,
		'up_rate'					: G_up_rate,
		'down_rate'					: G_down_rate,
		'up_interleaver_depth'		: G_up_interleave == '' ? '0' : G_up_interleave,
		'down_interleaver_depth'	: G_down_interleave == '' ? '0' : G_down_interleave,
		'up_delay'					: G_up_delay == '' ? '0' : G_up_delay,
		'down_delay'				: G_down_delay == '' ? '0' : G_down_delay,
		'up_blocks'					: G_sum_up,
		'clear_up'					: G_buttonClearUp,
		'down_blocks'				: G_sum_down,
		'clear_down'				: G_buttonClearDown,
		'up_hhc_errors'				: G_up_hec_errors =='' ? '0' : G_up_hec_errors,
		'down_hhc_errors'			: G_down_hec_errors =='' ? '0' : G_down_hec_errors,
		'up_ocd_errors'				: G_up_ocd_errors =='' ? '0' : G_up_ocd_errors,
		'down_ocd_errors'			: G_down_ocd_errors =='' ? '0' : G_down_ocd_errors,
		'up_lcd_errors'				: G_up_lcd_errors =='' ? '0' : G_up_lcd_errors,
		'down_lcd_errors'			: G_down_lcd_errors =='' ? '0' : G_down_lcd_errors,
		'up_total_es'				: G_up_total == '' ? '0' : G_up_total,
		'down_total_es'				: G_down_total == '' ? '0' : G_down_total,
		'up_crc_errors'				: G_up_crc_errors == '' ? '0' : G_up_crc_errors,
		'down_crc_errors'			: G_down_crc_errors == '' ? '0' : G_down_crc_errors,
		'up_total_frames'			: G_up_frames == '' ? '0' : G_up_frames,
		'down_total_frames'			: G_down_frames == '' ? '0' : G_down_frames,
		'up_fec_errors'				: G_up_fec_errors == '' ? '0' : G_up_fec_errors,
		'down_fec_errors'			: G_down_fec_errors == '' ? '0' : G_down_fec_errors,
		'adsl_uptime'				: G_UpTime
	});
	
}

function Clear_Up_Blocks()
{
	$H({
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamFlag' : G_up_blocks,
		'var:menu'			: G_Menu,
		'var:page'			: G_Page,
		'var:subpage'		: G_SubPage,
		'var:errorpage'		: G_SubPage,
		'obj-action'		: 'set',
		'getpage'			: 'html/index.html',
		'errorpage'			: 'html/index.html',
		'var:CacheLastData'	: ViewState.Save()
		});
		$('uiPostForm').submit();
}

function Clear_Down_Blocks()
{
	$H({
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamFlag' : G_down_blocks,
		'var:menu'			: G_Menu,
		'var:page'			: G_Page,
		'var:subpage'		: G_SubPage,
		'var:errorpage'		: G_SubPage,
		'obj-action'		: 'set',
		'getpage'			: 'html/index.html',
		'errorpage'			: 'html/index.html',
		'var:CacheLastData'	: ViewState.Save()
		});
		$('uiPostForm').submit();
}

function Clear_Blocks()
{
	$H({
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamFlag' : 0,
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamFlag' : 0,
		'var:menu'				: G_Menu,
		'var:page'				: G_Page,
		'var:subpage'			: G_SubPage,
		'var:errorpage'			: G_SubPage,
		'obj-action'			: 'set',
		'getpage'				: 'html/index.html',
		'errorpage'				: 'html/index.html',
		'var:CacheLastData'		: ViewState.Save()
		});
		$('uiPostForm').submit();
}

//小数位数控制，可以四舍五入 
function Fractional(n) 
{ 
	//小数保留位数 
	var bit = 2; 
	bit++; 
	n = n.toString(); 
	//获取小数点位置 
	var point = n.indexOf('.'); 
	//n的长度大于保留位数长度 
	if (n.length > point + bit) 
	{ 
		//保留小数后一位是否大于4，大于4进位 
		if (parseInt(n.substring(point + bit, point + bit + 1)) > 4) 
		{ 
			return n.substring(0, point) + "." + (parseInt(n.substring(point + 1, point + bit)) + 1); 
		} 
		else 
		{ 
			return n.substring(0, point) + n.substring(point, point + bit); 
		} 
	} 
	return n; 
} 


function load_lan()
{
	var Eindex;
	var value_array = [];
	for (var i = 0; i <G_LanStatus.length; i++) {
		if (G_LanStatus[i][0] == 'Up') 
		{
			var index = 0;
			var Eth_idx = 1 + i;
			var eth_index = 0;
			
			if(G_LanStatus[i][10].indexOf(Eth_idx) > 0)
			{
				index = 1+i;
				eth_index = 1+i;
			}
			else
			{
				index = 4-i;
				eth_index = 3-i;
			}
			if('DSL' == G_InternetWanType)
			{
				<?if eq 1 $var:mod_ETHWAN
				`
					if(0 == enable_ethwan)
					{
						for(Eindex = 1; Eindex < 3; Eindex++)
						{
							if(G_LanStatus[i][Eindex] > 1024*1024*1024)
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024*1024);
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("GB");
							}
							else if(G_LanStatus[i][Eindex] > 1024*1024 && G_LanStatus[i][Eindex] < 1024*1024*1024)
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024);
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("MB");
							}
							else 
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/1024;
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("KB");
							}
						
						}
						value_array[i] = [
							'LAN' +index,
							G_LanStatus[i][2],
							G_LanStatus[i][4],
							G_LanStatus[i][5],
							G_LanStatus[i][7],
							G_LanStatus[i][1],
							G_LanStatus[i][3],
							G_LanStatus[i][6],
							G_LanStatus[i][8]
							];
					}
					else
					{
						for(Eindex = 1; Eindex < 3; Eindex++)
						{
							if(G_LanStatus[i][Eindex] > 1024*1024*1024)
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024*1024);
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("GB");
							}
							else if(G_LanStatus[i][Eindex] > 1024*1024 && G_LanStatus[i][Eindex] < 1024*1024*1024)
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024);
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("MB");
							}
							else 
							{
								G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/1024;
								G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
								G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("KB");
							}
						
						}
						value_array[i] = [
							'LAN' +eth_index,
							G_LanStatus[i][2],
							G_LanStatus[i][4],
							G_LanStatus[i][5],
							G_LanStatus[i][7],
							G_LanStatus[i][1],
							G_LanStatus[i][3],
							G_LanStatus[i][6],
							G_LanStatus[i][8]
							];
					}
				`?>
				<?if neq 1 $var:mod_ETHWAN
				`						
					for(Eindex = 1; Eindex < 3; Eindex++)
					{
						if(G_LanStatus[i][Eindex] > 1024*1024*1024)
						{
							G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024*1024);
							G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
							G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("GB");
						}
						else if(G_LanStatus[i][Eindex] > 1024*1024 && G_LanStatus[i][Eindex] < 1024*1024*1024)
						{
							G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024);
							G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
							G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("MB");
						}
						else 
						{
							G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/1024;
							G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
							G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("KB");
						}
					
					}
					value_array[i] = [
						'LAN' +index,
						G_LanStatus[i][2],
						G_LanStatus[i][4],
						G_LanStatus[i][5],
						G_LanStatus[i][7],
						G_LanStatus[i][1],
						G_LanStatus[i][3],
						G_LanStatus[i][6],
						G_LanStatus[i][8]
						];
				`?>
			}
			else
			{
				for(Eindex = 1; Eindex < 3; Eindex++)
				{
					if(G_LanStatus[i][Eindex] > 1024*1024*1024)
					{
						G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024*1024);
						G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
						G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("GB");
					}
					else if(G_LanStatus[i][Eindex] > 1024*1024 && G_LanStatus[i][Eindex] < 1024*1024*1024)
					{
						G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/(1024*1024);
						G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
						G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("MB");
					}
					else 
					{
						G_LanStatus[i][Eindex] = G_LanStatus[i][Eindex]/1024;
						G_LanStatus[i][Eindex] = Fractional(G_LanStatus[i][Eindex]); 
						G_LanStatus[i][Eindex] =  G_LanStatus[i][Eindex].concat("KB");
					}
				
				}
				value_array[i] = [
					'LAN' + 1,
					G_LanStatus[i][2],
					G_LanStatus[i][4],
					G_LanStatus[i][5],
					G_LanStatus[i][7],
					G_LanStatus[i][1],
					G_LanStatus[i][3],
					G_LanStatus[i][6],
					G_LanStatus[i][8]
					];
			}
		}
		if (i == 4)
			value_array[i][0] = 'USB0';
	}
			
	for(var j = 0; j< G_WirelessStatus.length;j++){
		if (G_WirelessStatus[j][0] == '1') {
				for(Eindex = 1; Eindex < 3; Eindex++)
				{
					if(G_WirelessStatus[j][Eindex] > 1024*1024*1024)
					{
						G_WirelessStatus[j][Eindex] = G_WirelessStatus[j][Eindex]/(1024*1024*1024);
						G_WirelessStatus[j][Eindex] = Fractional(G_WirelessStatus[j][Eindex]); 
						G_WirelessStatus[j][Eindex] =  G_WirelessStatus[j][Eindex].concat("GB");
					}
					else if(G_WirelessStatus[j][Eindex] > 1024*1024 && G_WirelessStatus[j][Eindex] < 1024*1024*1024)
					{
						G_WirelessStatus[j][Eindex] = G_WirelessStatus[j][Eindex]/(1024*1024);
						G_WirelessStatus[j][Eindex] = Fractional(G_WirelessStatus[j][Eindex]); 
						G_WirelessStatus[j][Eindex] =  G_WirelessStatus[j][Eindex].concat("MB");
					}
					else 
					{
						G_WirelessStatus[j][Eindex] = G_WirelessStatus[j][Eindex]/1024;
						G_WirelessStatus[j][Eindex] = Fractional(G_WirelessStatus[j][Eindex]); 
						G_WirelessStatus[j][Eindex] =  G_WirelessStatus[j][Eindex].concat("KB");
					}
				
				}
			value_array[j+i] = [			  
							G_WirelessStatus[j][9],
							G_WirelessStatus[j][2],
							G_WirelessStatus[j][4],
							G_WirelessStatus[j][5],
							G_WirelessStatus[j][7],
							G_WirelessStatus[j][1],
							G_WirelessStatus[j][3],
							G_WirelessStatus[j][6],
							G_WirelessStatus[j][8]
							];
		}
	}
	$T('interface_info_table', value_array);
	
}

function getAtmParam(_i){
	var path_wan = G_WanConns[_i][0];
	
	if(path_wan.indexOf("WANDevice.3") > -1)
	{
		return 0;
	}
	else
	{
		for(var i = 0; i < G_ATM.length; i++){
			if(path_wan.indexOf(G_ATM[i][0]) > -1){
				return i;
			}
		}
	}
	return -1;
}


function get_conn_type()
{
		var i = arguments[0];
		var j = getAtmParam(i);
		if(G_WanConns[i][0].indexOf("WANDevice.3") > -1)
		{
			G_ATM[0] = [];
			G_ATM[0][0] = "N/A";
			G_ATM[0][1] = "EoA";
			G_ATM[0][2] = "N/A";
			G_ATM[0][3] = "N/A";
			G_ATM[0][4] = "N/A";
			if (G_WanConns[i][0].indexOf('IP') > -1) 
			{
				if (G_WanConns[i][2] == 'IP_Bridged')
					return 'BRIDGE';
				else 
					return G_WanConns[i][3].toUpperCase();
			}
			else
				return 'PPPOE';
		}

		else
		{
			if (G_ATM[j][1] == 'EoA') {
				if (G_WanConns[i][0].indexOf('IP') > -1) {
					if (G_WanConns[i][2] == 'IP_Bridged')
						return 'BRIDGE';
					else 
						return G_WanConns[i][3].toUpperCase();
				}
				else
					return 'PPPOE';
		}
			else
				return G_ATM[j][1];
		}
}

function load_wan()
{
	var value_array = [];
	var type;
	var Eindex;

	for(var i = 0, j, _len = G_WanConns.length; i < _len; i++){
		j = getAtmParam(i);
		type = get_conn_type(i);
		for(Eindex = 4; Eindex <= 8; Eindex = Eindex+4)
		{
			if(G_WanConns[i][Eindex] > 1024*1024*1024)
			{
				G_WanConns[i][Eindex] = G_WanConns[i][Eindex]/(1024*1024*1024);
				G_WanConns[i][Eindex] = Fractional(G_WanConns[i][Eindex]); 
				G_WanConns[i][Eindex] =  G_WanConns[i][Eindex].concat("GB");
			}
			else if(G_WanConns[i][Eindex] > 1024*1024 && G_WanConns[i][Eindex] < 1024*1024*1024)
			{
				G_WanConns[i][Eindex] = G_WanConns[i][Eindex]/(1024*1024);
				G_WanConns[i][Eindex] = Fractional(G_WanConns[i][Eindex]); 
				G_WanConns[i][Eindex] =  G_WanConns[i][Eindex].concat("MB");
			}
			else 
			{
				G_WanConns[i][Eindex] = G_WanConns[i][Eindex]/1024;
				G_WanConns[i][Eindex] = Fractional(G_WanConns[i][Eindex]); 
				G_WanConns[i][Eindex] =  G_WanConns[i][Eindex].concat("KB");
			}
				
		}		
		value_array[i] = [
							G_WanConns[i][1].ellipsis(8),
							G_ATM[j][2], //VPI/VCI
							//Service Name
							type, //Protocol
							//statistics
							G_WanConns[i][4], //IGMP
							G_WanConns[i][5], //QoS
							G_WanConns[i][6], //Enable
							G_WanConns[i][7], //Status
							G_WanConns[i][8], //Enable
							G_WanConns[i][9], //Status
							G_WanConns[i][10], //Enable
							G_WanConns[i][11] //Status
						];
	}
	$T('interface_statistics_table', value_array);
	for(var b = 0; b < G_WanConns.length; b++){
	$('interface_statistics_table_' + b + '0').title =  G_WanConns[b][1];
	
	}
}

function get_conn_type_Ethnet()
{
	var i = arguments[0];

	if (G_WanConns[i][0].indexOf('IP') > -1) {
		if (G_WanConns[i][2] == 'IP_Bridged')
			return 'BRIDGE';
		else 
			return G_WanConns[i][3].toUpperCase();
		}
	else
		return 'PPPOE';
}

function getAtmParam_Ethnet()
{
	return 0;
}

function load_wan_Ethnet()
{
	var value_array = [];
	var type;
	
	G_ATM[0] = [];
	G_ATM[0][0] = "N/A";
	G_ATM[0][1] = "EoA";
	G_ATM[0][2] = "N/A";
	G_ATM[0][3] = "N/A";
	G_ATM[0][4] = "N/A";
	
	G_dsl_uprate = "N/A";
	G_dsl_downrate = "N/A";
	
	for(var i = 0, j, _len = G_WanConns.length; i < _len; i++){
		j = getAtmParam_Ethnet(i);
		type = get_conn_type_Ethnet(i);
		value_array[i] = [
							G_WanConns[i][1].ellipsis(8),
							G_ATM[j][2], //VPI/VCI
							//Service Name
							type, //Protocol
							//statistics
							G_WanConns[i][4], //IGMP
							G_WanConns[i][5], //QoS
							G_WanConns[i][6], //Enable
							G_WanConns[i][7], //Status
							G_WanConns[i][8], //Enable
							G_WanConns[i][9], //Status
							G_WanConns[i][10], //Enable
							G_WanConns[i][11] //Status
						];
	}
	$T('interface_statistics_table', value_array);
	for(var b = 0; b < G_WanConns.length; b++){
	$('interface_statistics_table_' + b + '0').title =  G_WanConns[b][1];
	
	}
}

function uiOnload()
{
	load_lan();
	if (G_InternetWanType == "DSL")
	{
		load_wan();
		load_adsl();
	}
	else
	{
		load_wan_Ethnet();
		$('dsl_info').style.display = 'none'
	}
	window.setTimeout("refreshPage()", 6000);
}

function refreshPage()
{
	window.location.reload(); 
}

addListeners(uiOnload);

