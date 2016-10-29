//Wireless Advanced
var G_Wireless = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "Enable SSID SSIDAdvertisementEnabled X_TWSZ-COM_APIsolate X_TWSZ-COM_WMMEnabled X_TWSZ-COM_MAXSTATION MaxBitRate X_TWSZ-COM_MulticastRate X_TWSZ-COM_Power X_TWSZ-COM_BeaconInterval X_TWSZ-COM_RCSCTSThreshlod X_TWSZ-COM_FRAGLEN X_TWSZ-COM_DTIM X_TWSZ-COM_PreambleType Standard X_TWSZ-COM_Bandwidth"
`	G_Wireless[m] = [":InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.", //Path
					 "$01", //Enable
					 "$02", //SSID
					 "$03", //SSIDAdvertisementEnabled
					 "$04", //X_TWSZ-COM_APIsolate
					 "$05", //X_TWSZ-COM_WMMEnabled
					 "$06", //X_TWSZ-COM_MAXSTATION

					 "$07", //MaxBitRate 
					 "$08", //X_TWSZ-COM_MulticastRate
					 "$09", //X_TWSZ-COM_Power
					 "$0a", //X_TWSZ-COM_BeaconInterval
					 "$0b", //X_TWSZ-COM_RCSCTSThreshlod
					 "$0c", //X_TWSZ-COM_FRAGLEN
					 "$0d", //X_TWSZ-COM_DTIM
					 "$0e", //X_TWSZ-COM_PreambleType
					 "$0f", //Standard
					 "$0g"//X_TWSZ-COM_Bandwidth
					 ];
	m++;
`?>


//WPS
var G_WPS = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "WPS.Enable  WPS.Version"
`	G_WPS[t] = [':InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.',//path
				'$01', //Enable
				'$02' //Version
				];
	t++;
`?>
	
var G_WirelessType   = "<?get :InternetGatewayDevice.X_TWSZ-COM_WLAN.WirelessType?>";

function uiOnload(){
	createMaxRateOpt();
	//赋�?
	setJSONValue({
		//全局配置
		'SELECT_MaxBitRate'    : G_Wireless[0][7],
		'SELECT_MulRate'       : G_Wireless[0][8],
		'SELECT_Power'         : G_Wireless[0][9],
		'INPUT_BeaconPeriod'   : G_Wireless[0][10],
		'INPUT_RTSThreshold'   : G_Wireless[0][11],
		'INPUT_FragThres'      : G_Wireless[0][12],
		'INPUT_DTIM'           : G_Wireless[0][13],
		'SELECT_PreType'       : G_Wireless[0][14],
		//SSID
		'INPUT_Enable'         : G_Wireless[0][1],
		'INPUT_SSID'           : G_Wireless[0][2],
		'vb_status'            : G_Wireless[0][3],
		'SELECT_UserIsolation' : G_Wireless[0][4],
		'SELECT_WMMAdv'        : G_Wireless[0][5],
		'INPUT_MaxClients'     : G_Wireless[0][6],
		//Guest_1
		'INPUT_Enable_1'       : G_Wireless[1][1],
		'INPUT_GuestSSID_1'    : G_Wireless[1][2],
		'vs_status_1'          : G_Wireless[1][3],
		'SELECT_UserIso_1'     : G_Wireless[1][4],
		'SELECT_WMMAdv_1'      : G_Wireless[1][5],
		'INPUT_MaxClients_1'   : G_Wireless[1][6],
		//Guest_2
		'INPUT_Enable_2'       : G_Wireless[2][1],
		'INPUT_GuestSSID_2'    : G_Wireless[2][2],
		'vs_status_2'          : G_Wireless[2][3],
		'SELECT_UserIso_2'     : G_Wireless[2][4],
		'SELECT_WMMAdv_2'      : G_Wireless[2][5],
		'INPUT_MaxClients_2'   : G_Wireless[2][6],
		//Guest_3
		'INPUT_Enable_3'       : G_Wireless[3][1],
		'INPUT_GuestSSID_3'    : G_Wireless[3][2],
		'vs_status_3'          : G_Wireless[3][3],
		'SELECT_UserIso_3'     : G_Wireless[3][4],
		'SELECT_WMMAdv_3'      : G_Wireless[3][5],
		'INPUT_MaxClients_3'   : G_Wireless[3][6]
	});
	$('INPUT_Enable').disabled = true;
	
    if($('INPUT_Enable').checked == false)
    	{
    	$('access_point1').style.display = 'none';
		$('access_point2').style.display = 'none';
		$('access_point3').style.display = 'none';
    	}
}

//动态生成传输速率
function createMaxRateOpt(){
	var _text = ['Auto','54','48','36','24','18','12','11','9','6','5.5','2','1'], _value = ['Auto','54','48','36','24','18','12','11','9','6','5.5','2','1'];
	var rate_auto = ['Auto'];
	var rate_b = ['1', '2', '5.5', '11'];
	var rate_g = ['6', '9', '12', '18', '24', '36', '48', '54'];
	//var rate_n = ['6.5', '13', '19.5', '26', '39', '52', '58.5', '65', '78', '104', '117', '130'];
	//var rate_n_20m_1T2R = ['6.5','13','19.5','26','39','52','58.5','65'];
	//var rate_n_40m_1T2R = ['13.5','27','40.5','54','81','108','121.5','135'];
	var rate_n_20m_1T2R = ['7.2','14.4','21.7','28.9','43.3','57.8','65','72.2'];
	var rate_n_40m_1T2R = ['15','30','45','60','90','120','135','150'];
	//var rate_n_20m_2T2R = ['6.5','13','19.5','26','39','52','58.5','65','78','104','117','130'];
	//var rate_n_40m_2T2R = ['13.5','27','40.5','54','81','108','121.5','135','162','216','243','270'];
	var rate_n_20m_2T2R = ['14.4','28.9','43.3','57.8','86.7','115.6','130','144.4'];
	var rate_n_40m_2T2R = ['30','60','90','120','180','240','270','300'];

	if(G_Wireless[0][15] == 'b'){ // -> for b
		_text  = [].concat(rate_auto , rate_b);
		_value = [].concat(rate_auto , rate_b);
	} else if (G_Wireless[0][15] == 'g') { // -> for g
		_text  = [].concat(rate_auto , rate_g);
		_value = [].concat(rate_auto , rate_g);
	} else if (G_Wireless[0][15] == 'bg') { // -> for bg
		_text  = [].concat(rate_auto , rate_b, rate_g); 
		_value = [].concat(rate_auto , rate_b, rate_g); 
	} else if (G_Wireless[0][15] == 'n'){ // -> for n only
		if(G_WirelessType == '0'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_n_20m_1T2R);
				_value = [].concat(rate_auto , rate_n_20m_1T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_n_40m_1T2R);
				_value = [].concat(rate_auto , rate_n_40m_1T2R);
			}	
		}
		else if(G_WirelessType == '1'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_n_20m_2T2R);
				_value = [].concat(rate_auto , rate_n_20m_2T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_n_40m_2T2R);
				_value = [].concat(rate_auto , rate_n_40m_2T2R);
			}	
		}	
	}else if (G_Wireless[0][15] == 'ng'){ // -> for n/g
		if(G_WirelessType == '0'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_g, rate_n_20m_1T2R);
				_value = [].concat(rate_auto , rate_g,  rate_n_20m_1T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_g, rate_n_40m_1T2R);
				_value = [].concat(rate_auto , rate_g, rate_n_40m_1T2R);
			}	
		}
		else if(G_WirelessType == '1'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_g, rate_n_20m_2T2R);
				_value = [].concat(rate_auto , rate_g, rate_n_20m_2T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_g, rate_n_40m_2T2R);
				_value = [].concat(rate_auto , rate_g, rate_n_40m_2T2R);
			}	
		}
	}
	else if (G_Wireless[0][15] == 'bgn'){ // -> for b/g/n
		if(G_WirelessType == '0'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_b, rate_g, rate_n_20m_1T2R);
				_value = [].concat(rate_auto , rate_b,rate_g,  rate_n_20m_1T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_b, rate_g, rate_n_40m_1T2R);
				_value = [].concat(rate_auto , rate_b, rate_g, rate_n_40m_1T2R);
			}	
		}
		else if(G_WirelessType == '1'){
			if(G_Wireless[0][16] == '20'){
				_text  = [].concat(rate_auto , rate_b, rate_g, rate_n_20m_2T2R);
				_value = [].concat(rate_auto , rate_b, rate_g, rate_n_20m_2T2R);
			}
			else if(G_Wireless[0][16] == '40'){
				_text  = [].concat(rate_auto , rate_b, rate_g, rate_n_40m_2T2R);
				_value = [].concat(rate_auto , rate_b, rate_g, rate_n_40m_2T2R);
			}	
		}
	}
	
	$S('SELECT_MaxBitRate', _text, _value);
}

function uiSubmit(){
	var nodes = $('SELECT_MaxBitRate','SELECT_MulRate','SELECT_Power','INPUT_BeaconPeriod','INPUT_RTSThreshold','INPUT_FragThres','INPUT_DTIM','SELECT_PreType',
				  'INPUT_SSID','SELECT_UserIsolation','SELECT_WMMAdv','INPUT_MaxClients',
				  'INPUT_GuestSSID_1','SELECT_UserIso_1','SELECT_WMMAdv_1','INPUT_MaxClients_1',
				  'INPUT_GuestSSID_2','SELECT_UserIso_2','SELECT_WMMAdv_2','INPUT_MaxClients_2',
				  'INPUT_GuestSSID_3','SELECT_UserIso_3','SELECT_WMMAdv_3','INPUT_MaxClients_3'
				  );
	var i = 7;

	if((G_WPS[0][1] ==1) && (G_WPS[0][2]==2))	
	{	
		if(Form.Radio('vb_status') == 0)
		{
			if(confirm("If you hide the SSID, the WPS would be disabled. Do you sure?"))
			{
				$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WPS.Enable','0');
			}
			else
			{
				return;
			}
		}
	}
	
	if((G_WPS[1][1] ==1) && (G_WPS[1][2]==2))	
	{	
		if(Form.Radio('vs_status_1') == 0)
		{
			if(confirm("If you hide the SSID, the WPS would be disabled. Do you sure?"))
			{
				$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.WPS.Enable','0');
			}
			else
			{
				return;
			}
		}
	}
	
	if((G_WPS[2][1] ==1) && (G_WPS[2][2]==2))	
	{	
		if(Form.Radio('vs_status_2') == 0)
		{
			if(confirm("If you hide the SSID, the WPS would be disabled. Do you sure?"))
			{
				$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.WPS.Enable','0');
			}
			else
			{
				return;
			}
		}
	}
	
	if((G_WPS[3][1] ==1) && (G_WPS[3][2]==2))	
	{	
		if(Form.Radio('vs_status_3') == 0)
		{
			if(confirm("If you hide the SSID, the WPS would be disabled. Do you sure?"))
			{
				$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.WPS.Enable','0');
			}
			else
			{
				return;
			}
		}
	}	
	
	if($('INPUT_SSID').value.indexOf('<\?') != -1 || 
		$('INPUT_GuestSSID_1').value.indexOf('<\?') != -1 || 
		$('INPUT_GuestSSID_2').value.indexOf('<\?') != -1 ||
		$('INPUT_GuestSSID_3').value.indexOf('<\?') != -1 )
	{
	   alert("Error: The string of SSID not support contain <\? ");
       return false;
	}
	if($('INPUT_SSID').value == '' || 
		$('INPUT_GuestSSID_1').value == '' || 
		$('INPUT_GuestSSID_2').value == '' ||
		$('INPUT_GuestSSID_3').value == '' )
	{
	   alert("Error: SSID can not be null");
       return false;
	}
	if($('INPUT_SSID').value.charAt(0)==' ' ||
		$('INPUT_GuestSSID_1').value.charAt(0)==' ' || 
		$('INPUT_GuestSSID_2').value.charAt(0)==' ' ||
		$('INPUT_GuestSSID_3').value.charAt(0)==' ' )
	{
	   alert("Error: The first character of SSID can not be null");
       return false;
	}
	if($('INPUT_SSID').value.charAt($('INPUT_SSID').value.length  -1)==' ' ||
		$('INPUT_GuestSSID_1').value.charAt($('INPUT_GuestSSID_1').value.length  -1)==' ' || 
		$('INPUT_GuestSSID_2').value.charAt($('INPUT_GuestSSID_2').value.length  -1)==' ' || 
		$('INPUT_GuestSSID_3').value.charAt($('INPUT_GuestSSID_3').value.length  -1)==' ')
	{
	   alert("Error: The last character of SSID can not be null ");
       return false;
	}
	
	//针对单独的SSID
	$H({
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable'                : 1, //Form.Checkbox('INPUT_Enable'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID'                  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSIDAdvertisementEnabled'   : Form.Radio('vb_status'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_APIsolate'  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_WMMEnabled' : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_MAXSTATION' : nodes[++i].value,
		//Guest-1
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.Enable'                : Form.Checkbox('INPUT_Enable_1'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.SSID'                  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.SSIDAdvertisementEnabled'   : Form.Radio('vs_status_1'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_TWSZ-COM_APIsolate'  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_TWSZ-COM_WMMEnabled' : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_TWSZ-COM_MAXSTATION' : nodes[++i].value,
		//Guest-2
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.Enable'                : Form.Checkbox('INPUT_Enable_2'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.SSID'                  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.SSIDAdvertisementEnabled'   : Form.Radio('vs_status_2'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_TWSZ-COM_APIsolate'  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_TWSZ-COM_WMMEnabled' : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_TWSZ-COM_MAXSTATION' : nodes[++i].value,
		//Guest-3
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.Enable'                : Form.Checkbox('INPUT_Enable_3'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.SSID'                  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.SSIDAdvertisementEnabled'   : Form.Radio('vs_status_3'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_TWSZ-COM_APIsolate'  : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_TWSZ-COM_WMMEnabled' : nodes[++i].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_TWSZ-COM_MAXSTATION' : nodes[++i].value,
		//常用参数
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : '<?echo $var:subpage?>',
		'var:errorpage' : '<?echo $var:subpage?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);
	//提交公共属�?
	for(var i = 0; i < 4; i++){
		$F(G_Wireless[i][0] + 'MaxBitRate'                , nodes[0].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_MulticastRate'  , nodes[1].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_Power'          , nodes[2].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_BeaconInterval' , nodes[3].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_RCSCTSThreshlod', nodes[4].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_FRAGLEN'        , nodes[5].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_DTIM'           , nodes[6].value);
		$F(G_Wireless[i][0] + 'X_TWSZ-COM_PreambleType'   , nodes[7].value);
	}
	
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

