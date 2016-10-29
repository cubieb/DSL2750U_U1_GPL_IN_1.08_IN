//Queue
var G_QosQueue = [];
var n = 0;
var tmp_G_dsl_uprate = 0;

G_dsl_uprate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate ?>";
tmp_G_dsl_uprate = Math.floor(G_dsl_uprate *0.85);

<?objget :InternetGatewayDevice.QueueManagement.Queue. "QueueEnable X_TWSZ-COM_EnableForceWeight QueueWeight X_TWSZ-COM_QueueCAR ShapingRate SchedulerAlgorithm X_TWSZ-COM_EnableDSCPMark X_TWSZ-COM_EnableCOSMark QueueInterface QueuePrecedence"
`	G_QosQueue[n] = ["$00",//Index
					 "$01",//QueueEnable
					 "$02",// X_TWSZ-COM_EnableForceWeight
					 "$03",//QueueWeight
					 "$04", //X_TWSZ-COM_QueueCAR
					 "$05", // ShapingRate
					 "$06", // SchedulerAlgorithm
					 "$07", // X_TWSZ-COM_EnableDSCPMark
					 "$08", // X_TWSZ-COM_EnableCOSMark
					 "$09", // QueueInterface
					 "$0a", // QueuePrecedence
					 "InternetGatewayDevice.QueueManagement.Queue.<? echo $10 ?>."  // QueuePrecedence
					 ];
	n++;
`?>

if (G_QosQueue.length < 2){
	alert("WARNNING: At least 2 default queues required, current default queue number: " + G_QosQueue.length);
}

var G_Connection = [];
var n = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. ""
`	<?objget :InternetGatewayDevice.WANDevice.$20.WANConnectionDevice.<?echo $20?>.WANIPConnection. "Name X_TWSZ-COM_ServiceList" 
	`	<?if neq $12 `TR069`
		`	G_Connection[n] = [ "<?echo $21?>", //Name
						  "InternetGatewayDevice.WANDevice.<?echo $40?>.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $20?>" //path
						  ];
			n++;
		`?>
	`?>
	<?objget :InternetGatewayDevice.WANDevice.$20.WANConnectionDevice.<?echo $20?>.WANPPPConnection. "Name X_TWSZ-COM_ServiceList"
	`	<?if neq $12 `TR069`
		`	G_Connection[n] = ["<?echo $21?>", //Name
						  "InternetGatewayDevice.WANDevice.<?echo $40?>.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $20?>" //path
						  ];
			n++;
		`?>
	`?>
`?>
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

//Qos模板
//var G_QosMode = "< ? get :InternetGatewayDevice.X_TWSZ-COM_UplinkQoS.Mode ? >";
//计数器
var Counter = 0;

function checkQosConfig(){
	//检查判断QOS配置是上行，还是下行
	var qostype = '<?echo $var:qostype?>';
	
	if(qostype != '-' && Counter == 0) {
		Counter++;
		return Form.Radio('INPUT_QosConfig', qostype);
	} else {
		return Form.Radio('INPUT_QosConfig');
	}
}

function uiOnload(){
	var num  = checkQosConfig(); //根据QOS配置：上行 = 0；下行 = 1
	//生成调度策略
	createOption();
	//给全局节点赋值
	setJSONValue({
	//	'SELECT_QosMode'       : G_QosMode,
		'INPUT_QosEnable'      : G_QosQueue[num][1] || 0,
		'INPUT_QosShapingRate'   : G_QosQueue[num][5],
		'SELECT_QosScheduling' : G_QosQueue[num][6],
		'INPUT_EnableDSCPMark' : G_QosQueue[num][7],
		'INPUT_EnableCOSMark'  : G_QosQueue[num][8]
	});
	CreateTable();
}

function createOption(){
	var _text, _value;
//	alert(typeof checkQosConfig());
	if(checkQosConfig() == '0'){
		_text = ['SP','WRR','CAR'];
		_value = ['SP','WRR','CAR'];
	} else {
		_text = ['SP'];
		_value = ['SP'];
	}
	$S('SELECT_QosScheduling',_text,_value);
}

function CreateTable(){
	var value_array = [];
	var qos_direction  = checkQosConfig(); //根据QOS配置：上行 = 0；下行 = 1
	var qos_SchedulerAlgorithm = $('SELECT_QosScheduling').value; //根据调度策略来选择队列配置
	// var _text = [data_language.dynamic.lang_allInterfaces], _value = [''];
	var _text = [], _value = [];
	var Output;
	
	if (qos_direction == 0){ //上行
		_text.push('WAN');
		_value.push('WAN');
		Output = G_Connection;
	} else {
		_text.push('LAN');
		_value.push('LAN');
		Output = G_Wlan;
	}
	
	//输出接口
	for(var i = 0; i < Output.length; i++){
		_text.push(Output[i][0]);
		_value.push(Output[i][1]);
	}
	
	//整理标题栏
	DealWithTitle(qos_direction,qos_SchedulerAlgorithm);
	//清空Table
	Table.Clear('td_config');

	var viewIndex = 0;

	if (qos_direction == 0){ //上行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var i = 2; i < G_QosQueue.length; i++){
				if (G_QosQueue[i][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				value_array[i] = [];
				value_array[i].push(viewIndex); //index
				value_array[i].push(data_language.dynamic.lang_up + data_language.dynamic.lang_namePrefix + G_QosQueue[i][0]); // Queue Name
				value_array[i].push($CheckBox('SP_Enable' + viewIndex,G_QosQueue[i][1])); //enable
				value_array[i].push($Text('DWRR_QueuePrecedence' + viewIndex,G_QosQueue[i][10])); // QueuePrecedence
				value_array[i].push('<select id="SELECT_OutputInterface' + viewIndex + '"><option> </option></select></div>'); //Interface
				value_array[i].push('<input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_QosQueue[' + i + '][11])">'); // Operation
			}
		} else if (qos_SchedulerAlgorithm == 'WRR'){ //WRR
			for(var j = 2; j < G_QosQueue.length; j++){
				if (G_QosQueue[j][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				value_array[j] = [];
				value_array[j].push(viewIndex); //index
				value_array[j].push(data_language.dynamic.lang_up + data_language.dynamic.lang_namePrefix + G_QosQueue[j][0]); // Queue Name
				value_array[j].push($CheckBox('DWRR_Enable' + viewIndex,G_QosQueue[j][1])); //enable
				value_array[j].push($CheckBox('DWRR_EnableForceWeight' + viewIndex,G_QosQueue[j][2])); // X_TWSZ-COM_EnableForceWeight
				value_array[j].push($Text('DWRR_QueueWeight' + viewIndex,G_QosQueue[j][3])); //QueueWeight
				value_array[j].push('<select id="SELECT_OutputInterface' + viewIndex + '"><option> </option></select></div>'); //Interface
				value_array[j].push('<input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_QosQueue[' + j + '][11])">'); // Operation
			}
		} else if (qos_SchedulerAlgorithm == 'CAR'){ //CAR
			for(var k = 2; k < G_QosQueue.length; k++){
				if (G_QosQueue[k][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				value_array[k] = [];
				value_array[k].push(viewIndex); //index
				value_array[k].push(data_language.dynamic.lang_up + data_language.dynamic.lang_namePrefix + G_QosQueue[k][0]); // Queue Name
				value_array[k].push($CheckBox('CAR_Enable' + viewIndex,G_QosQueue[k][1])); //enable
				value_array[k].push($Text('CAR_ShapingRate' + viewIndex,G_QosQueue[k][4])); //X_TWSZ-COM_QueueCAR
				value_array[k].push('<select id="SELECT_OutputInterface' + viewIndex + '"><option> </option></select></div>'); //Interface
				value_array[k].push('<input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_QosQueue[' + k + '][11])">'); // Operation
			}
		}
	} else if (qos_direction == 1){ //下行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var l = 2; l < G_QosQueue.length; l++){
				if ( !(G_QosQueue[l][9].indexOf('LAN') >= 0) )   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				value_array[l] = [];
				value_array[l].push(viewIndex); //index
				value_array[l].push(data_language.dynamic.lang_down + data_language.dynamic.lang_namePrefix + G_QosQueue[l][0]); // Queue Name
				value_array[l].push($CheckBox('Down_Enable' + viewIndex,G_QosQueue[l][1])); //enable
				value_array[l].push($Text('DWRR_QueuePrecedence' + viewIndex,G_QosQueue[l][10])); // QueuePrecedence
				value_array[l].push('<select id="SELECT_OutputInterface' + viewIndex + '"><option> </option></select></div>'); //Interface
				value_array[l].push('<input type="button" value="'+ data_language.dynamic.del +'" onclick="uiDelete(G_QosQueue[' + l + '][11])">'); // Operation
			}
		}
	//	for(var l = 0; l < 6; l++){
	//		value_array[l] = [];
	//		value_array[l].push(l+1); //index
	//		value_array[l].push($CheckBox('Down_Enable' + l,G_QosQueue[l+6][1])); //enable
	//		value_array[l].push($Text('Down_ShapingRate' + l,G_QosQueue[l+6][4])); //X_TWSZ-COM_QueueCAR
	//	}
	}
	
	$T('td_config',value_array);

	viewIndex = 0;

	if (qos_direction == 0){ //上行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var i = 2; i < G_QosQueue.length; i++){
				if (G_QosQueue[i][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$S('SELECT_OutputInterface' + viewIndex,_text,_value);
				$('SELECT_OutputInterface' + viewIndex).value = G_QosQueue[i][9];
			}
		} else if (qos_SchedulerAlgorithm == 'WRR'){ //WRR
			for(var j = 2; j < G_QosQueue.length; j++){
				if (G_QosQueue[j][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$S('SELECT_OutputInterface' + viewIndex,_text,_value);
				$('SELECT_OutputInterface' + viewIndex).value = G_QosQueue[j][9];
			}
		} else if (qos_SchedulerAlgorithm == 'CAR'){ //CAR
			for(var k = 2; k < G_QosQueue.length; k++){
				if (G_QosQueue[k][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$S('SELECT_OutputInterface' + viewIndex,_text,_value);
				$('SELECT_OutputInterface' + viewIndex).value = G_QosQueue[k][9];
			}
		}
	} else if (qos_direction == 1){ //下行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var l = 2; l < G_QosQueue.length; l++){
				if ( !(G_QosQueue[l][9].indexOf('LAN') >= 0) )   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$S('SELECT_OutputInterface' + viewIndex,_text,_value);
				$('SELECT_OutputInterface' + viewIndex).value = G_QosQueue[l][9];
			}
		}
	}
}

function DealWithTitle(_type,_option){ //整理标题栏和语言显示
	var nodes = $('lang_bandWidth','lang_type','lang_qosBandwidth','lang_queueConfig');
	nodes[1].style.display = 'none';
	
	if(_type == 1){
		if(_option == 'SP'){
			nodes[0].innerHTML = data_language.dynamic.precedence;
		}
	} else if(_type == 0){
		if(_option == 'SP'){
			nodes[0].innerHTML = data_language.dynamic.precedence;
		} else if(_option == 'WRR'){
			nodes[0].innerHTML = data_language.dynamic.lang_bandwidthEnforcement;
			nodes[1].style.display = 'block';
			nodes[1].innerHTML = data_language.dynamic.lang_weight;
		} else if(_option == 'CAR'){
			//nodes[0].innerHTML = '权重(%)';
		}
	}	
	
	var type_intf = checkQosConfig();
	type_intf == 0 ? nodes[2].innerHTML = data_language.dynamic.lang_uplinkBandwidth : nodes[2].innerHTML = data_language.dynamic.lang_downlinkBandwidth;
	type_intf == 0 ? nodes[3].innerHTML = data_language.dynamic.lang_uplinkQueueConfiguration : nodes[3].innerHTML = data_language.dynamic.lang_downlinkQueueConfiguration;
}

function $CheckBox(_id,_value){ //生成checkbox
	return '<input type="checkbox" id="' + _id + '" ' + (_value == '1' ? 'checked' : '') + '>';
}

function $Text(_id,_value){ //生成text
	return '<input type="text" id="'+ _id +'" value="' + _value + '">';
}

function uiSubmit(){
	var qos_direction  = checkQosConfig(); //根据QOS配置：上行 = 0；下行 = 1
	var qos_SchedulerAlgorithm = $('SELECT_QosScheduling').value; //根据调度策略来选择队列配置
	var nodeIndex = 1;
	
	$H({
		'var:qostype': qos_direction,
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_interface_queue',
		'obj-action' : 'set',
		'var:errorpage': 'qos_interface_queue',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	var viewIndex = 0;

	if(qos_direction == 0){
		if(qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var i = 2; i < G_QosQueue.length; i++){
				if (G_QosQueue[i][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[i][0] + '.QueueEnable',Form.Checkbox('SP_Enable' + viewIndex));
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[i][0] + '.QueueInterface',$('SELECT_OutputInterface' + viewIndex).value);
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[i][0] + '.QueuePrecedence',$('DWRR_QueuePrecedence' + viewIndex).value);
			}
		} else if(qos_SchedulerAlgorithm == 'WRR'){ //WRR
			for(var j = 2; j < G_QosQueue.length; j++){
				if (G_QosQueue[j][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[j][0] + '.QueueEnable',Form.Checkbox('DWRR_Enable' + viewIndex));
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[j][0] + '.X_TWSZ-COM_EnableForceWeight',Form.Checkbox('DWRR_EnableForceWeight' + viewIndex));
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[j][0] + '.QueueWeight',$('DWRR_QueueWeight' + viewIndex).value || '0');
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[j][0] + '.QueueInterface',$('SELECT_OutputInterface' + viewIndex).value);
			}
		} else if(qos_SchedulerAlgorithm == 'CAR'){ //CAR
			for(var k = 2; k < G_QosQueue.length; k++){
				if (G_QosQueue[k][9].indexOf('LAN') >= 0)   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
					continue;
				++viewIndex;
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[k][0] + '.QueueEnable',Form.Checkbox('CAR_Enable' + viewIndex));
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[k][0] + '.X_TWSZ-COM_QueueCAR',$('CAR_ShapingRate' + viewIndex).value || '0');
				$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[k][0] + '.QueueInterface',$('SELECT_OutputInterface' + viewIndex).value);
			}
		}
	} else if(qos_direction == 1){
		for(var l = 2; l < G_QosQueue.length; l++){
			if ( !(G_QosQueue[l][9].indexOf('LAN') >= 0) )   // QueueInterface contains 'LAN' means Downstream, otherwise means Upstream, BLANK means Upstream also
				continue;
			++viewIndex;
	//		$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[l][0] + '.QueueEnable',Form.Checkbox('Down_Enable' + (l - 6)));
	//		$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[l][0] + '.X_TWSZ-COM_QueueCAR',$('Down_ShapingRate' + (l - 6)).value || '0');
			$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[l][0] + '.QueueEnable',Form.Checkbox('Down_Enable' + viewIndex));
			$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[l][0] + '.QueueInterface',$('SELECT_OutputInterface' + viewIndex).value);
			$F(':InternetGatewayDevice.QueueManagement.Queue.' + G_QosQueue[l][0] + '.QueuePrecedence',$('DWRR_QueuePrecedence' + viewIndex).value);
		}
	}

	if(qos_direction == 0){
		nodeIndex = 1;
	} else if(qos_direction == 1){
		nodeIndex = 2;
	}

	//全局属性
	$F(':InternetGatewayDevice.QueueManagement.Queue.' + nodeIndex + '.QueueEnable', Form.Checkbox('INPUT_QosEnable'));
	$F(':InternetGatewayDevice.QueueManagement.Queue.' + nodeIndex + '.ShapingRate',$('INPUT_QosShapingRate').value);
	$F(':InternetGatewayDevice.QueueManagement.Queue.' + nodeIndex + '.SchedulerAlgorithm',$('SELECT_QosScheduling').value);
	$F(':InternetGatewayDevice.QueueManagement.Queue.' + nodeIndex + '.X_TWSZ-COM_EnableDSCPMark',Form.Checkbox('INPUT_EnableDSCPMark'));
	$F(':InternetGatewayDevice.QueueManagement.Queue.' + nodeIndex + '.X_TWSZ-COM_EnableCOSMark',Form.Checkbox('INPUT_EnableCOSMark'));
	
	$('uiPostForm').submit();
}

//切换调度策略
function uiOnchgQosSchedul(){
	/*
	if(!confirm('你确认变换调度策略吗?')){//data_language.dynamic.tips
		return false;
	}*/
	
	
	if(!confirm('Sure to change the scheduling?')){//data_language.dynamic.tips
		uiPageRefresh();
		return false;
	}

	var qos_direction  = checkQosConfig(); // Upstream = 0, Downstream = 1
	
	$H({
		':InternetGatewayDevice.QueueManagement.Queue.1.SchedulerAlgorithm' : $('SELECT_QosScheduling').value,
		'var:qostype': qos_direction,
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_interface_queue',
		'obj-action' : 'set',
		'var:errorpage': 'qos_interface_queue',
		'var:CacheLastData': ViewState.Save()		
	},true);
	$('uiPostForm').submit();
}

function uiAddQueue(){
	var qos_direction  = checkQosConfig(); // Upstream = 0, Downstream = 1

	$H({
		':QueueInterface'  : (qos_direction == 0) ? 'WAN' : 'LAN',
		'var:qostype': qos_direction,
		'getpage'    :'html/index.html',
		'errorpage'  :'html/index.html',
		'var:menu'   :'advanced',
		'var:page'   :'qos_config',
		'var:subpage':'qos_interface_queue',
		'var:errorpage' :'qos_interface_queue'
	},true);

	$F('add-obj','InternetGatewayDevice.QueueManagement.Queue.');
	$F('obj-action','add-set');

	$('uiPostForm').submit();
}

function uiDelete(){
	// if(!confirm(SEcode[1001])){ return false; }
	var qos_direction  = checkQosConfig(); // Upstream = 0, Downstream = 1

	$H({
		'var:qostype': qos_direction,
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:menu'  :'advanced',
		'var:page'   :'qos_config',
		'var:subpage':'qos_interface_queue',
		'var:errorpage' :'qos_interface_queue',
		'del-obj'   : arguments[0],
		'obj-action':'del'
	},true);
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	//arrayHint['CommittedRate'] = 'INPUT_QosShapingRate';
	if(G_Error_Msg.indexOf('29289007') > -1)
    {
	     alert("Committed rate should be less than or equal to peak rate :" + tmp_G_dsl_uprate);
		 return false;
	}
	if(G_Error_Msg.indexOf('29439007') > -1)
    {
	     alert("The bandwidth sum of all enabled queues shouled be less than or equal to peak rate :" + tmp_G_dsl_uprate);
		 return false;
	}
	if(G_Error_Msg.indexOf('29359007') > -1)
    {
	     alert("The bandwidth sum of all enabled queues shouled be less than or equal to the global bandwidth value");
		 return false;
	}

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);
