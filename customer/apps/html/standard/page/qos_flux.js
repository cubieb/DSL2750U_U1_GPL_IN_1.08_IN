//Classifition
var G_Qos_Classifition = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification. "Enable Direction ClassInterface EgressInterface SourceMACAddress SourceMACMask DestMACAddress DestMACMask Ethertype EthernetPriorityCheck EthernetPriorityMark VLANIDCheck SourceIP SourceIPMax"
`	G_Qos_Classifition[m] = [];
	G_Qos_Classifition[m][0] = "<?echo $11?>"; //Enable
	G_Qos_Classifition[m][1] = "<?echo $12?>"; //Direction
	G_Qos_Classifition[m][2] = "<?echo $13?>"; //ClassInterface
	G_Qos_Classifition[m][3] = "<?echo $14?>"; //EgressInterface
	G_Qos_Classifition[m][4] = "<?echo $15?>"; //SourceMACAddress
	G_Qos_Classifition[m][5] = "<?echo $16?>"; //SourceMACMask
	G_Qos_Classifition[m][6] = "<?echo $17?>"; //DestMACAddress
	G_Qos_Classifition[m][7] = "<?echo $18?>"; //DestMACMask
	G_Qos_Classifition[m][8] = "<?echo $19?>"; //Ethertype
	G_Qos_Classifition[m][9] = "<?echo $1a?>"; //EthernetPriorityCheck
	G_Qos_Classifition[m][10] = "<?echo $1b?>"; //EthernetPriorityMark
	G_Qos_Classifition[m][11] = "<?echo $1c?>"; //VLANIDCheck
	G_Qos_Classifition[m][12] = "<?echo $1d?>"; //SourceIP
	G_Qos_Classifition[m][13] = "<?echo $1e?>"; //SourceIPMax
	G_Qos_Classifition[m][14] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.SourceMask?>"; //SourceMask
	G_Qos_Classifition[m][15] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DestIP?>"; //DestIP
	G_Qos_Classifition[m][16] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DestIPMax?>"; //DestIPMax
	G_Qos_Classifition[m][17] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DestMask?>"; //DestMask
	G_Qos_Classifition[m][18] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.IPLengthMin?>"; //IPLengthMin
	G_Qos_Classifition[m][19] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.IPLengthMax?>"; //IPLengthMax
	G_Qos_Classifition[m][20] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DSCPCheck?>"; //DSCPCheck
	G_Qos_Classifition[m][21] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.TOSCheck?>";//TOSCheck
	G_Qos_Classifition[m][22] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DSCPMark?>"; //DSCPMark
	G_Qos_Classifition[m][23] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.Protocol?>";//Protocol
	G_Qos_Classifition[m][24] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.SourcePort?>";//SourcePort
	G_Qos_Classifition[m][25] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.SourcePortRangeMax?>";//SourcePortRangeMax
	G_Qos_Classifition[m][26] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DestPort?>";//DestPort
	G_Qos_Classifition[m][27] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.DestPortRangeMax?>";//DestPortRangeMax
	G_Qos_Classifition[m][28] = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $20?>.ClassQueue?>";//ClassQueue
	G_Qos_Classifition[m][29] = "InternetGatewayDevice.X_TWSZ-COM_QoS.Classification.<?echo $10?>.";//Path
	m++;
`?>

//Business
var G_Qos_Business = [];
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.App. "AppName AppQueue Enable DSCPMark COSMark"
`	G_Qos_Business[n] = [];
	G_Qos_Business[n][0] = "$01"; //AppName
	G_Qos_Business[n][1] = "$02"; //AppQueue
	G_Qos_Business[n][2] = "InternetGatewayDevice.X_TWSZ-COM_QoS.App.$00."; //index
	G_Qos_Business[n][3] = "$03"; //Enable
	G_Qos_Business[n][4] = "$04"; //DSCPMark
	G_Qos_Business[n][5] = "$05"; //COSMark
	n++;
`?>

function uiOnload(){
	if('<?echo $var:config?>' != '-'){
		$('SELECT_ClassifyModel').value = '<?echo $var:config?>';
	}
	
	var Model = $('SELECT_ClassifyModel').value; //分类模式 : 0 = 业务分类; 1 = 流量分类
	var nodes = $('td_business','td_flux');
	
	if(Model == '0'){
		nodes[0].style.display = 'block';
		nodes[1].style.display = 'none';
		Table.Clear('td_business');
		BusinessTable();
	} else if(Model == '1'){
		nodes[0].style.display = 'none';
		nodes[1].style.display = 'block';
		Table.Clear('td_flux');
		fluxTable();
	}
}

//动态生成分类模式
function createClassifyModeOption(){
	var value_array = [data_language.dynamic.lang_flowClassify, data_language.dynamic.lang_businessClassify];
	var value_options = ['1', '0'];
	$S('SELECT_ClassifyModel', value_array, value_options);
}


//生成业务分类列表
function BusinessTable(){
	var value_array = [];
	for(var i = 0, _len = G_Qos_Business.length; i < _len; i++){
		value_array[i] = [i+1,//序号
						  G_Qos_Business[i][3],//启用
						  G_Qos_Business[i][0],//业务名称
						  data_language.dynamic.lang_DSCPCheck + G_Qos_Business[i][4] + data_language.dynamic.lang_8021pCheck + G_Qos_Business[i][5],//标记
						  G_Qos_Business[i][1] == '-1' ? '' : 'Up_Q' + G_Qos_Business[i][1],//分类队列
						  '<input type="button" value="'+ data_language.dynamic.edit +'" onclick="uiEdit(G_Qos_Business['+ i +'][2])"> <input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_Qos_Business['+ i +'][2])">'
						  ];
	}
	$T('td_business',value_array);
}

//生成流量分类列表
function fluxTable(){
	var value_array = [];
	var classQueueNum = 0;//上/下行节点序号
	for(var i = 0, _len = G_Qos_Classifition.length; i < _len; i++){
		if(G_Qos_Classifition[i][1] == 'Up'){
			classQueueNum = 0;	
		} else if(G_Qos_Classifition[i][1] == 'Down'){
			classQueueNum = 6;
		}
		value_array[i] = [i+1,//序号
						  G_Qos_Classifition[i][0],//启用
						  G_Qos_Classifition[i][1],//分类方向
						  ClassifyContent(i),//分类条件
						  displayTag(i),//标记
						  G_Qos_Classifition[i][28] == '-1' ? '' : G_Qos_Classifition[i][1] + '_Q' + (G_Qos_Classifition[i][28] - classQueueNum),//分类队列
						  '<input type="button" value="'+ data_language.dynamic.edit +'" onclick="uiEdit(G_Qos_Classifition['+ i +'][29])"> <input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_Qos_Classifition['+ i +'][29])">'//操作
						  ];
	}
	$T('td_flux',value_array);
}

//标记
function displayTag(Id){
	if(G_Qos_Classifition[Id][1] == 'Up'){
		return '802.1P: ' + G_Qos_Classifition[Id][10] + '<br>' + 'DSCP: ' + getDSCPMark(G_Qos_Classifition[Id][22]);
	} else {
		return 'WMM: ' + G_Qos_Classifition[Id][22];
	}
}

//分类条件
function ClassifyContent(Id){
	var _str = '';
	_str = data_language.dynamic.lang_srcDstMacAddress + G_Qos_Classifition[Id][4] + '/' + G_Qos_Classifition[Id][6] + '<br>' +
		   data_language.dynamic.lang_ethernetType + checkEthnetType(G_Qos_Classifition[Id][8]) + '<br>' +
		   'VLANID : ' + G_Qos_Classifition[Id][11] + '<br>' + 
		   '802.1P : ' + G_Qos_Classifition[Id][9] + '<br>' + 
		   data_language.dynamic.lang_srcDstIPAddress + G_Qos_Classifition[Id][12] + '/' + G_Qos_Classifition[Id][15] + '<br>' +
		   data_language.dynamic.lang_srcDstMask + G_Qos_Classifition[Id][14] + '/' + G_Qos_Classifition[Id][17] + '<br>' + 
		   data_language.dynamic.lang_DSCPValue + checkDSCP(G_Qos_Classifition[Id][20]) + '<br>' +
		   data_language.dynamic.lang_protocol + checkProtocol(G_Qos_Classifition[Id][23]) + '<br>' + 
		   data_language.dynamic.lang_srcPortRange + G_Qos_Classifition[Id][24] + '-' + G_Qos_Classifition[Id][25] + '<br>' +
		   data_language.dynamic.lang_dstPortRange + G_Qos_Classifition[Id][26] + '-' + G_Qos_Classifition[Id][27];
	return _str;
}

function getDSCPMark(typeValue){
	if (typeValue == "-1")
		return "";
		
	var returnStr = '';//'<br>' + data_language.dynamic.lang_DSCPCheck + ":"
	var dscpMark  = '';
	
	var value_array   = [data_language.dynamic.lang_notCheck, 'AF11','AF12','AF13','AF21','AF22','AF23','AF31','AF32','AF33','AF41','AF42','AF43','CS1','CS2','CS3','CS4','CS5','CS6','CS7','EF', 'default'];
	var value_options = ['-1', '40','48','56','72','80','88','104','112','120','136','144','152','32','64','96','128','160','192','224','184', '0']; 
		
	for (var i = 0; i < value_options.length; i++){
		if (Number(typeValue)*4 == value_options[i]){
			dscpMark = value_array[i];
			break;
		}
	}
	
	//如果是不存在的DSCP值，也显示
	if(typeValue != '' && value_options.indexOf(typeValue) < 0){
		dscpMark = typeValue;
	}
	
	returnStr += dscpMark;
	return returnStr;
}

function checkEthnetType(varType){
	var ethnetTypes = [];
	
	ethnetTypes['2048'] = "IPv4";
	ethnetTypes['2053'] = "X25";
	ethnetTypes['2054'] = "ARP";
	ethnetTypes['2056'] = "FR_ARP";
	ethnetTypes['2303'] = "BPQ";
	ethnetTypes['24576'] = "DEC";
	ethnetTypes['24577'] = "DNA_DL";
	ethnetTypes['24578'] = "DNA_RC";
	ethnetTypes['24579'] = "DNA_RT";
	ethnetTypes['24580'] = "LAT";
	ethnetTypes['24581'] = "DIAG";
	ethnetTypes['24582'] = "CUST";
	ethnetTypes['24583'] = "SCA";
	ethnetTypes['25944'] = "TEB";
	ethnetTypes['25945'] = "RAW_FR";
	ethnetTypes['33011'] = "AARP";
	ethnetTypes['32923'] = "ATALK";
	ethnetTypes['33024'] = "802_1Q";
	ethnetTypes['33079'] = "IPX";
	ethnetTypes['33169'] = "NetBEUI";
	ethnetTypes['34525'] = "IPv6";
	ethnetTypes['34827'] = "PPP";
	ethnetTypes['34892'] = "ATMMPOA";
	ethnetTypes['34915'] = "PPP_DISC";
	ethnetTypes['34916'] = "PPP_SES";
	ethnetTypes['34948'] = "ATMFATE";
	ethnetTypes['36864'] = "LOOP";
	
	if (ethnetTypes[varType] == undefined)
		return data_language.dynamic.unknown;
	else
		return ethnetTypes[varType];
}

function checkProtocol(){ //G_Qos_Flux[i][11]
	var PROTOCOL;
	switch(arguments[0]){
		case '-1':{
			PROTOCOL = data_language.dynamic.lang_notMatch;
			break;
		}
		case '0':{
			PROTOCOL = 'TCP/UDP';
			break;
		}
		case '1':{
			PROTOCOL = 'ICMP';
			break;
		}
		case '6':{
			PROTOCOL = 'TCP';
			break;
		}
		case '17':{
			PROTOCOL = 'UDP';
			break;
		}
		case '1000':{
			PROTOCOL = 'RTP';
			break;
		}
	}
	return PROTOCOL;
}

function checkDSCP(){
	var DSCP;
	var _value = arguments[0] == '' ? '' :
				 arguments[0] == '-1' ? '-1' : Number(arguments[0])*4;
	
	switch(String(_value)){
		case '-1':{
			DSCP = data_language.dynamic.lang_notCheck;
			break;
		}
		case '0':{
			DSCP = 'default';
			break;
		}
		case '40':{
			DSCP = 'AF11';
			break;
		}
		case '48':{
			DSCP = 'AF12';
			break;
		}
		case '56':{
			DSCP = 'AF13';
			break;
		}
		case '72':{
			DSCP = 'AF21';
			break;
		}
		case '80':{
			DSCP = 'AF22';
			break;
		}
		case '88':{
			DSCP = 'AF23';
			break;
		}
		case '104':{
			DSCP = 'AF31';
			break;
		}
		case '112':{
			DSCP = 'AF32';
			break;
		}
		case '120':{
			DSCP = 'AF33';
			break;
		}
		case '136':{
			DSCP = 'AF41';
			break;
		}
		case '144':{
			DSCP = 'AF42';
			break;
		}
		case '152':{
			DSCP = 'AF43';
			break;
		}
		case '32':{
			DSCP = 'CS1';
			break;
		}
		case '64':{
			DSCP = 'CS2';
			break;
		}
		case '96':{
			DSCP = 'CS3';
			break;
		}
		case '128':{
			DSCP = 'CS4';
			break;
		}
		case '160':{
			DSCP = 'CS5';
			break;
		}
		case '192':{
			DSCP = 'CS6';
			break;
		}
		case '224':{
			DSCP = 'CS7';
			break;
		}
		case '184':{
			DSCP = 'EF';
			break;
		}
	}
	return DSCP;
}

function uiEdit(){
	var Index = arguments[0].split('.');
	$H({
		'var:pathIndex':Index[3],
		'getpage'      :'html/index.html',
		'errorpage'    :'html/index.html',
		'var:menu'     :'advanced',
		'var:page'     :'qos_config',
		'var:subpage'  :Index[2] == 'App' ? 'qos_queue_edit' : 'qos_flux_edit'  // 2009-03-05 modify
	},true);
	$('uiPostForm').submit();
}

function uiAddConfig(){
	$H({
		'getpage'    :'html/index.html',
		'errorpage'  :'html/index.html',
		'var:config' : $('SELECT_ClassifyModel').value,
		'var:menu'   :'advanced',
		'var:page'   :'qos_config',
		'var:subpage':$('SELECT_ClassifyModel').value == 0 ? 'qos_queue_edit' : 'qos_flux_edit'
	},true);
	$('uiPostForm').submit();
}

function uiDelete(){
	if(!confirm(SEcode[1001])){ return false;}
	$H({
		'del-obj'   : arguments[0],
		'obj-action':'del',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:menu'  :'advanced',
		'var:page'  :'qos_config',
		'var:subpage':'qos_flux',
		'var:config' : $('SELECT_ClassifyModel').value,		
		'var:errorpage' :'qos_flux' 
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

addListeners(createClassifyModeOption);
addListeners(uiOnload, dealWithError);

