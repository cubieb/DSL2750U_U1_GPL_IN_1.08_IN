//qos flux
var G_Qos_Enable=G_Qos_Direction=G_Qos_ClassInterface=G_Qos_EgressInterface=G_Qos_SourceMACAddress=G_Qos_SourceMACMask=G_Qos_DestMACAddress=G_Qos_DestMACMask=G_Qos_Ethertype=G_Qos_EthernetPriorityCheck=G_Qos_EthernetPriorityMark=G_Qos_VLANIDCheck=G_Qos_SourceIP=G_Qos_SourceIPMax='';
var G_Qos_SourceMask=G_Qos_DestIP=G_Qos_DestIPMax=G_Qos_DestMask=G_Qos_DSCPCheck=G_Qos_TOSCheck=G_Qos_DSCPMark=G_Qos_Protocol=G_Qos_SourcePort=G_Qos_SourcePortRangeMax=G_Qos_DestPort=G_Qos_DestPortRangeMax=G_Qos_ClassQueue=G_Qos_Path='';
var G_Qos_IpProtocolType=G_Qos_SourcePrefixLength=G_Qos_DestPrefixLength=G_Qos_FlowLabelCheck=G_Qos_TrafficClassCheck=G_Qos_TrafficClassMark='';
//全局属性
<?if neq `-` `$var:pathIndex`
`	<?mget :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $var:pathIndex?>. "Enable Direction ClassInterface EgressInterface SourceMACAddress SourceMACMask DestMACAddress DestMACMask Ethertype EthernetPriorityCheck EthernetPriorityMark VLANIDCheck SourceIP SourceIPMax"
	`	G_Qos_Enable                = "$01"; //Enable
		G_Qos_Direction             = "$02"; //Direction
		G_Qos_ClassInterface        = "$03"; //ClassInterface
		G_Qos_EgressInterface       = "$04"; //EgressInterface
		G_Qos_SourceMACAddress      = "$05"; //SourceMACAddress
		G_Qos_SourceMACMask         = "$06"; //SourceMACMask
		G_Qos_DestMACAddress        = "$07"; //DestMACAddress
		G_Qos_DestMACMask           = "$08"; //DestMACMask
		G_Qos_Ethertype             = "$09"; //Ethertype
		G_Qos_EthernetPriorityCheck = "$0a"; //EthernetPriorityCheck
		G_Qos_EthernetPriorityMark  = "$0b"; //EthernetPriorityMark
		G_Qos_VLANIDCheck           = "$0c"; //VLANIDCheck
		G_Qos_SourceIP              = "$0d"; //SourceIP
		G_Qos_SourceIPMax           = "$0e"; //SourceIPMax
	`?>
	<?mget :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $var:pathIndex?>. "SourceMask DestIP DestIPMax DestMask DSCPCheck TOSCheck DSCPMark Protocol SourcePort"
	`	G_Qos_SourceMask         = "$01"; //SourceMask
		G_Qos_DestIP             = "$02"; //DestIP
		G_Qos_DestIPMax          = "$03"; //DestIPMax
		G_Qos_DestMask           = "$04"; //DestMask
		G_Qos_DSCPCheck          = "$05"; //DSCPCheck
		G_Qos_TOSCheck           = "$06"; //TOSCheck
		G_Qos_DSCPMark           = "$07"; //DSCPMark
		G_Qos_Protocol           = "$08"; //Protocol
		G_Qos_SourcePort         = "$09"; //SourcePort
	`?>
		//new add
	<?mget :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $var:pathIndex?>. "SourcePortRangeMax DestPort DestPortRangeMax ClassQueue IpProtocolType SourcePrefixLength DestPrefixLength FlowLabelCheck TrafficClassCheck TrafficClassMark"
	`	G_Qos_SourcePortRangeMax = "$01"; //SourcePortRangeMax
		G_Qos_DestPort           = "$02"; //DestPort
		G_Qos_DestPortRangeMax   = "$03"; //DestPortRangeMax
		G_Qos_ClassQueue         = "$04"; //ClassQueue
		G_Qos_IpProtocolType     = "$05"; //IpProtocolType
		G_Qos_SourcePrefixLength = "$06"; //SourcePrefixLength
		G_Qos_DestPrefixLength   = "$07"; //DestPrefixLength
		G_Qos_FlowLabelCheck     = "$08"; //FlowLabelCheck
		G_Qos_TrafficClassCheck  = "$09"; //TrafficClassCheck
		G_Qos_TrafficClassMark   = "$0a"; //TrafficClassMark
	`?>	
		G_Qos_Path = "InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $var:pathIndex?>."; //Path
	
`?>
//获取桥连接名
var G_Connection = [];
var n = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. ""
`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $20?>.WANIPConnection. "Name X_TWSZ-COM_ServiceList" 
	`	<?if neq $12 `TR069`
		`	G_Connection[n] = [ "<?echo $21?>", //Name
						  "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $20?>" //path
						  ];
			n++;
		`?>
	`?>
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $20?>.WANPPPConnection. "Name X_TWSZ-COM_ServiceList"
	`	<?if neq $12 `TR069`
		`	G_Connection[n] = ["<?echo $21?>", //Name
						  "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $20?>" //path
						  ];
			n++;
		`?>
	`?>
`?>
/*获取VAP的SSID
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID"
`	<?if neq `1` `<?echo $20?>`
	`	G_Connection[n] = new Array();
		G_Connection[n][0] = "<?echo $21?>"; // SSID
		G_Connection[n][1] = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.<?echo $20?>";
		n++;
	`?>
`?>
*/

//获取queue的i值
var G_classQueues = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Queue. ""
`	G_classQueues[m] = "$00";
	m++;
`?>

//获取LAN
var G_Wlan = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_Description"
`	G_Wlan[t] = ["$01","InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.$00"];
	t++;
`?>
//获取WLAN
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID"
`	G_Wlan[t] = ["$01","InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00"];
	t++;
`?>

//这个语句很重要，用来判断是set还是add
<?setvar var:Path <?if eq `-` `$var:pathIndex` `` `InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.$var:pathIndex.`?>?>
//调度策略
var G_QueueSchedulerAlgorithm = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.1.SchedulerType?>";

function changIcmpVal(_val){
	var n_Protocol  = $('SELECT_Protocol');
	var _IP_ProType = G_Qos_IpProtocolType || 'IPv4';
	
	if(_val){
		_IP_ProType = _val; //IP Protocol Type
	}
	
	n_Protocol.options[5].value = _IP_ProType == 'IPv4' ? '1' : '58';
	//Flow Label值
	$('flow_label').style.display = _IP_ProType == 'IPv4' ? 'none' : 'block';
	//IPv4的DSCP值/DSCP标记
	$('IPv4_DSCPCheck').style.display = _IP_ProType == 'IPv4' ? 'block' : 'none';
	$('IPv4_DSCPMark').style.display = _IP_ProType == 'IPv4' ? 'block' : 'none';
	//IPv6的TC值/TC标记
	$('IPv6_TrafficClassCheck').style.display = _IP_ProType == 'IPv4' ? 'none' : 'block';
	$('IPv6_TrafficClassMark').style.display = _IP_ProType == 'IPv4' ? 'none' : 'block';
	
	$('INPUT_TrafficClassCheck').value = G_Qos_TrafficClassCheck == '-1' ? '' : G_Qos_TrafficClassCheck;
	$('INPUT_TrafficClassMark').value  = G_Qos_TrafficClassMark == '-1' ? '' : G_Qos_TrafficClassMark;
}

function uiOnload(){
	//动态生成输入/输出接口
	CreateIOoption();
	//动态生成DSCP值
	createDSCPValueOption();
	//动态生成DSCP标记
	createDSCPMarkOpions();
	//动态生成COS标记
	createCOSMarkOption();
	//动态生成802.1P
	createCOSOption();
	//动态生成WMM标记
	createWmmMarkOption();
	//动态生成分类队列
	DynamicCreateOption();
	//修改ICMP的值(IPv4 = 1; IPv6 = 58)
	changIcmpVal();	
	//统一赋值
	setJSONValue({
		'INPUT_Enable'           : G_Qos_Enable,
		'SELECT_IpProtocolType'  : G_Qos_IpProtocolType || 'IPv4', //IP Protocol Type
		'INPUT_Classifition'     : G_Qos_Direction, // UP or Down
		'SELECT_InputInterface'  : G_Qos_ClassInterface,
		'SELECT_OutputInterface' : G_Qos_EgressInterface ,
		'INPUT_sourceMAC'        : G_Qos_SourceMACAddress,
		'INPUT_sourceMask'       : G_Qos_SourceMACMask,
//		'INPUT_destinationMAC'   : G_Qos_DestMACAddress,
//		'INPUT_destiMask'        : G_Qos_DestMACMask,
//		'SELECT_EthnetType'      : G_Qos_Ethertype || -1,
//		'INPUT_VLANID'           : G_Qos_VLANIDCheck == '-1' ? '' : G_Qos_VLANIDCheck,
		'INPUT_802_1p'           : G_Qos_EthernetPriorityCheck || -1,
		'INPUT_IPAddress'        : G_Qos_SourceIP,
		'INPUT_Mask'             : G_Qos_SourceMask,
		'INPUT_DestinationIPAddress': G_Qos_DestIP,
		'INPUT_DestinationMask'  : G_Qos_DestMask,
		'SELECT_DSCPCheck'       : G_Qos_DSCPCheck || -1,
		'INPUT_FlowLabel'        : G_Qos_FlowLabelCheck == '-1' ? '' : G_Qos_FlowLabelCheck,
		'SELECT_Protocol'        : G_Qos_Protocol || -1,
		'INPUT_SourcePort'       : (G_Qos_SourcePort == '-1' ? '': G_Qos_SourcePort),
		'INPUT_SourcePortRangeMax':(G_Qos_SourcePortRangeMax == '-1' ? '': G_Qos_SourcePortRangeMax),
		'INPUT_DestPort'         : (G_Qos_DestPort == '-1' ? '' : G_Qos_DestPort),
		'INPUT_DestPortRangeMax' : (G_Qos_DestPortRangeMax == '-1' ? '' : G_Qos_DestPortRangeMax),
		//result
		'SELECT_WmmMark'    : G_Qos_DSCPMark || '-1',
		'SELECT_DSCPMark'   : G_Qos_DSCPMark || -1,
		'SELECT_COSMark'    : G_Qos_EthernetPriorityMark || -1,
		'SELECT_classQueue' : G_Qos_ClassQueue || -1
	});
	dealWithError();
	baseOnClassifyType();
	baseOnProtocol();
	//IP协议类型
	onChgIpProType();
	//已有的实例不能修改分类类型
	unEditor();
}

function CreateIOoption(){
	var _text = [data_language.dynamic.lang_notMatch], _value = [''];
	var Input, Output, Classifition = document.getElementsByName('INPUT_Classifition');
	
	if(G_Qos_Direction == 'Up' || (G_Qos_Direction == '' && Classifition[0].checked)){
		Input  = G_Wlan;
		Output = G_Connection;
	} else {
		Input  = G_Connection;
		Output = G_Wlan;
	}
	
	//输入接口
	for(var j = 0; j < Input.length; j++){
		_text.push(Input[j][0]);
		_value.push(Input[j][1]);
	}
	
	$S('SELECT_InputInterface', _text, _value);
	//输出接口
	_text = [data_language.dynamic.lang_notMatch], _value = ['']; //清空数组;
	for(var i = 0; i < Output.length; i++){
		_text.push(Output[i][0]);
		_value.push(Output[i][1]);
	}
	
	$S('SELECT_OutputInterface',_text,_value);
}

function createDSCPValueOption(){
	var value_array   = [data_language.dynamic.lang_notMatch, 'AF11(001010)','AF12(001100)','AF13(001110)','AF21(010010)','AF22(010100)','AF23(010110)','AF31(011010)','AF32(011100)','AF33(011110)','AF41(100010)','AF42(100100)','AF43(100110) ','CS1(001000)','CS2(010000)','CS3(011000)','CS4(100000)','CS5(101000)','CS6(110000)','CS7(111000)','EF(101110)','Default'];
	var value_options = ['-1', '10','12','14','18','20','22','26','28','30','34','36','38','8','16','24','32','40','48','56','46','0']; 
	
	//如果是不存在DSCP值，就添加到下拉框的最后
	if(G_Qos_DSCPCheck != '' && value_options.indexOf(G_Qos_DSCPCheck) < 0){
		value_array.push(G_Qos_DSCPCheck);
		value_options.push(G_Qos_DSCPCheck);
	}
	
	$S('SELECT_DSCPCheck',value_array,value_options);
}

function createDSCPMarkOpions(){
	var value_array   = [data_language.dynamic.lang_notMark, 'AF11(001010)','AF12(001100)','AF13(001110)','AF21(010010)','AF22(010100)','AF23(010110)','AF31(011010)','AF32(011100)','AF33(011110)','AF41(100010)','AF42(100100)','AF43(100110) ','CS1(001000)','CS2(010000)','CS3(011000)','CS4(100000)','CS5(101000)','CS6(110000)','CS7(111000)','EF(101110)','Default'];
	var value_options = ['-1', '10','12','14','18','20','22','26','28','30','34','36','38','8','16','24','32','40','48','56','46','0']; 

	//如果是不存在DSCP值，就添加到下拉框的最后
	if(G_Qos_DSCPMark != '' && value_options.indexOf(G_Qos_DSCPMark) < 0){
		value_array.push(G_Qos_DSCPMark);
		value_options.push(G_Qos_DSCPMark);
	}
	
	$S('SELECT_DSCPMark',value_array,value_options);
}

function createCOSMarkOption(){
	var value_array = [data_language.dynamic.lang_notMark, '0', '1', '2', '3', '4', '5', '6', '7'];
	var value_options = ['-1', '0', '1', '2', '3', '4', '5', '6', '7'];
	$S('SELECT_COSMark',value_array,value_options);
}

function createCOSOption(){
	var value_array = [data_language.dynamic.lang_notMatch, '1', '2', '3', '4', '5', '6', '7'];
	var value_options = ['-1', '1', '2', '3', '4', '5', '6', '7'];
	$S('INPUT_802_1p',value_array,value_options);	
}

function createWmmMarkOption(){
	var value_array = [data_language.dynamic.lang_notMark, 'voice priority-7', 'voice priority-6', 'video priority-5', 'video priority-4', 'best effort-3', 'Background-2', 'Background-1', 'Best effort-0'];
	var value_options = ['-1', '56', '48', '40', '32', '24', '16', '8', '0'];
	$S('SELECT_WmmMark',value_array,value_options);
}

//绑定到队列
function DynamicCreateOption(){
	var _text = [data_language.dynamic.lang_NotBinding], _value = [-1]; //清空数组;
	var num = 0;
	var _len = 4;
	var _note = 'UP_Q';
	//下行流量
	if(G_Qos_Direction == 'Down' || (G_Qos_Direction == '' && $('INPUT_DownLink').checked)){
		num = 6;
		_len = 4;
		_note = 'DOWN_Q';
	} else {//上行流量
		switch(G_QueueSchedulerAlgorithm){//分类队列
			case 'SP'   : break;
			case 'DWRR' : break;
			case 'CAR'  : _len = 6;
		} 
	}
	
	for(var j = num; j < (_len + num); j++){
		_text.push(_note + (Number(G_classQueues[j]) - num));
		_value.push(G_classQueues[j]);
	}

	$S('SELECT_classQueue', _text, _value);
}
//另外，还管理上下流量不同的逻辑
function baseOnClassifyType(){
	var nodes = $('INPUT_UpLink','lang_downTips','div_downLink','div_upLink','InputIntf');
	
	if(nodes[0].checked){
		//nodes[1].style.display = 'none';
		nodes[2].style.display = 'none';
		nodes[3].style.display = 'block';
		nodes[4].style.display = 'block';
	} else {
		//nodes[1].style.display = 'block';
		nodes[2].style.display = 'block';
		nodes[3].style.display = 'none';
		nodes[4].style.display = 'block';//none
	}
	//动态选不同的分类方向
}

function baseOnProtocol(){
	var nodes = $('SELECT_Protocol','INPUT_SourcePort','INPUT_SourcePortRangeMax','INPUT_DestPort','INPUT_DestPortRangeMax');
	for(var i = 1; i < 5; i++){
		nodes[i].disabled = (nodes[0].value == '1' || nodes[0].value == '-1') ? true : false;
	}
}

function onChgIpProType(){
	var _ProType = $('SELECT_IpProtocolType').value;
	
	if(_ProType == 'IPv4'){
		$('INPUT_Mask').value            = G_Qos_SourceMask;
		$('INPUT_DestinationMask').value = G_Qos_DestMask;
		$('lang_sourceIpAddress').innerHTML = data_language.dynamic.lang_srcIpv4address;
		$('lang_destiIpAddress').innerHTML = data_language.dynamic.lang_dstIpv4address;
		$('lang_sourceMask').innerHTML   = data_language.dynamic.lang_srcMask;
		$('lang_destiMask').innerHTML    = data_language.dynamic.lang_dstMask;
	}
	else if(_ProType == 'IPv6'){
		$('INPUT_Mask').value            = G_Qos_SourcePrefixLength == '-1' ? '' : G_Qos_SourcePrefixLength;
		$('INPUT_DestinationMask').value = G_Qos_DestPrefixLength == '-1' ? '' : G_Qos_DestPrefixLength;		
		$('lang_sourceIpAddress').innerHTML = data_language.dynamic.lang_srcIpv6address;
		$('lang_destiIpAddress').innerHTML = data_language.dynamic.lang_dstIpv6address;
		$('lang_sourceMask').innerHTML   = data_language.dynamic.lang_srcIpPrefixLength;
		$('lang_destiMask').innerHTML    = data_language.dynamic.lang_dstIpPrefixLength;		
	}
}

function unEditor(){
	var nodes;
	
	if('<?echo $var:pathIndex?>' != '-'){
		nodes = $('INPUT_UpLink','INPUT_DownLink');
		nodes[0].disabled = true;
		nodes[1].disabled = true;
	}
}

function uiSubmit(){
	var nodes = $('SELECT_InputInterface','SELECT_OutputInterface','INPUT_sourceMAC','INPUT_sourceMask','INPUT_destinationMAC','INPUT_destiMask','SELECT_EthnetType',
				  'INPUT_VLANID','INPUT_802_1p','INPUT_IPAddress','INPUT_Mask','INPUT_DestinationIPAddress','INPUT_DestinationMask','SELECT_DSCPCheck','SELECT_Protocol',
				  'INPUT_SourcePort','INPUT_SourcePortRangeMax','INPUT_DestPort','INPUT_DestPortRangeMax','SELECT_WmmMark','SELECT_DSCPMark','SELECT_COSMark','SELECT_classQueue');
	var _ProType = $('SELECT_IpProtocolType').value;
	
	$H({
		':<?echo $var:Path?>Enable'            : Form.Checkbox('INPUT_Enable'),
		':<?echo $var:Path?>IpProtocolType'    : _ProType,
		':<?echo $var:Path?>Direction'         : Form.Radio('INPUT_Classifition'),
		':<?echo $var:Path?>ClassInterface'    : nodes[0].value ,
		':<?echo $var:Path?>EgressInterface'   : nodes[1].value,
		':<?echo $var:Path?>SourceMACAddress'  : nodes[2].value,
		':<?echo $var:Path?>SourceMACMask'     : nodes[3].value,
//		':<?echo $var:Path?>DestMACAddress'    : nodes[4].value,
//		':<?echo $var:Path?>DestMACMask'       : nodes[5].value,
//		':<?echo $var:Path?>Ethertype'         : nodes[6].value,
//		':<?echo $var:Path?>VLANIDCheck'       : nodes[7].value || '-1',
		':<?echo $var:Path?>EthernetPriorityCheck' : nodes[8].value,
		':<?echo $var:Path?>SourceIP'          : nodes[9].value,
		':<?echo $var:Path?>SourceMask'        : _ProType == 'IPv4' ? nodes[10].value : undefined, //IPv4
		':<?echo $var:Path?>SourcePrefixLength': _ProType == 'IPv6' ? (nodes[10].value == '' ? '-1' : nodes[10].value) : undefined, //IPv6
		':<?echo $var:Path?>DestIP'            : nodes[11].value,
		':<?echo $var:Path?>DestMask'          : _ProType == 'IPv4' ? nodes[12].value : undefined, //IPv4
		':<?echo $var:Path?>DestPrefixLength'  : _ProType == 'IPv6' ? (nodes[12].value == '' ? '-1' : nodes[12].value) : undefined, //IPv6
		':<?echo $var:Path?>DSCPCheck'         : _ProType == 'IPv4' ? nodes[13].value : undefined,
		':<?echo $var:Path?>TrafficClassCheck' : _ProType == 'IPv6' ? ($('INPUT_TrafficClassCheck').value || '-1') : undefined,
		':<?echo $var:Path?>FlowLabelCheck'    : $('INPUT_FlowLabel').value || '-1',
		':<?echo $var:Path?>Protocol'          : nodes[14].value,
		':<?echo $var:Path?>SourcePort'        : nodes[15].value || '-1',
		':<?echo $var:Path?>SourcePortRangeMax': nodes[16].value || '-1',
		':<?echo $var:Path?>DestPort'          : nodes[17].value || '-1',
		':<?echo $var:Path?>DestPortRangeMax'  : nodes[18].value || '-1',
		':<?echo $var:Path?>ClassQueue'        : nodes[22].value,
		//提交属性值
		'var:menu' : 'advanced',
		'var:page' : 'qos_config',
		'var:subpage' : 'qos_flux',
		'var:errorpage' : 'qos_flux_edit',
		'getpage' : 'html/index.html',
		'errorpage': 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	if(Form.Radio('INPUT_Classifition') == 'Down'){
		if(_ProType == 'IPv4')
			$F(':<?echo $var:Path?>DSCPMark',nodes[19].value);
		else
			$F(':<?echo $var:Path?>TrafficClassMark',$('INPUT_TrafficClassMark').value || '-1');
	} else {
		if(_ProType == 'IPv4')
			$F(':<?echo $var:Path?>DSCPMark',nodes[20].value);
		else
			$F(':<?echo $var:Path?>TrafficClassMark',$('INPUT_TrafficClassMark').value || '-1');
		$F(':<?echo $var:Path?>EthernetPriorityMark',nodes[21].value);
	}
	
	if('<?echo $var:pathIndex?>' == '-'){ //添加
		$F('add-obj','InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.');
		$F('obj-action','add-set');
	} else { //编辑
		$F('var:pathIndex','<?echo $var:pathIndex?>'); //保持实例的记录
		$F('obj-action','set');
	}
	
	$('uiPostForm').submit();
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=qos_config&var:subpage=qos_flux_edit&var:pathIndex=<?echo $var:pathIndex?>';
}

function dealWithError(){
	if (G_Error != 1){ return false;}

	var arrayHint = [];
	arrayHint['SourceIP'] = "INPUT_IPAddress";
	arrayHint['SourceMask'] = "INPUT_Mask";
	arrayHint['DestIP'] = "INPUT_DestinationIPAddress";
	arrayHint['DestMask'] = "INPUT_DestinationMask";
	arrayHint['DestPort'] = "INPUT_DestPort";
	arrayHint['IPLengthMin'] = "INPUT_IPLengthMin";
	arrayHint['IPLengthMax'] = "INPUT_IPLengthMax";
	arrayHint['SourcePort'] = "INPUT_SourcePort";
	arrayHint['SourcePortRangeMax'] = "INPUT_SourcePortRangeMax";
	arrayHint['DestPort'] = "INPUT_DestPort";
	arrayHint['DestPortRangeMax'] = "INPUT_DestPortRangeMax";
	
	arrayHint['SourcePort'] = "INPUT_SourcePort";
	arrayHint['Enable'] = "INPUT_Enable";
	arrayHint['ClassInterface'] = "SELECT_Interface";
	arrayHint['ClassificationType'] = "SELECT_ClassifyType";
	arrayHint['Protocol'] = "SELECT_Protocol";
	arrayHint['DSCPCheck'] = "SELECT_DSCPCheck";
	arrayHint['SourceMACAddress'] = "INPUT_sourceMAC";
	arrayHint['SourceMACMask'] = "INPUT_sourceMask";
	arrayHint['DestMACAddress'] = "INPUT_destinationMAC";
	arrayHint['DestMACMask'] = "INPUT_destiMask";
	arrayHint['VLANIDCheck'] = "INPUT_VLANID";
	arrayHint['EthernetPriorityCheck'] = "SELECT_EthnetPriority";
	arrayHint['Ethertype'] = "SELECT_EthnetType";
	arrayHint['EthernetPriorityMark'] = "SELECT_EthnetPriorityMark";
		
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
