//Interface
var G_QosInterface = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Interface. "Enable CommittedRate SchedulerType EnableDSCPMark EnableCOSMark"
`	G_QosInterface[m] = ["$01",//Enable
						 "$02",//CommittedRate
						 "$03",//SchedulerType
						 "$04",//EnableDSCPMark
						 "$05" //EnableCOSMark
						 ];
	m++;
`?>
//Queue
var G_QosQueue = [];
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Queue. "Enable EnableForceWeight QueueWeight QueueCAR"
`	G_QosQueue[n] = ["$00",//Index
					 "$01",//Enable
					 "$02",//EnableForceWeight
					 "$03",//QueueWeight
					 "$04" //QueueCAR
					 ];
	n++;
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
		'INPUT_QosEnable'      : G_QosInterface[num][0] || 0,
		'INPUT_QosBandwidth'   : G_QosInterface[num][1],
		'SELECT_QosScheduling' : G_QosInterface[num][2],
		'INPUT_EnableDSCPMark' : G_QosInterface[num][3],
		'INPUT_EnableCOSMark'  : G_QosInterface[num][4]
	});
	CreateTable();
}

function createOption(){
	var _text, _value;
//	alert(typeof checkQosConfig());
	if(checkQosConfig() == '0'){
		_text = ['SP','DWRR','CAR'];
		_value = ['SP','DWRR','CAR'];
	} else {
		_text = ['SP'];
		_value = ['SP'];
	}
	$S('SELECT_QosScheduling',_text,_value);
}

function CreateTable(){
	var value_array = [];
	var qos_config  = checkQosConfig(); //根据QOS配置：上行 = 0；下行 = 1
	var qos_SchedulerAlgorithm = $('SELECT_QosScheduling').value; //根据调度策略来选择队列配置
	//整理标题栏
	DealWithTitle(qos_config,qos_SchedulerAlgorithm);
	//清空Table
	Table.Clear('td_config');

	if (qos_config == 0){ //上行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var i = 0, Precedence = [data_language.dynamic.lang_highest,data_language.dynamic.lang_high,data_language.dynamic.lang_medium,data_language.dynamic.lang_low]; i < 4; i++){
				value_array[i] = [];
				value_array[i].push(i+1); //index
				value_array[i].push($CheckBox('SP_Enable' + i,G_QosQueue[i][1])); //enable
				value_array[i].push(Precedence[i]); //Precedence
			}
		} else if (qos_SchedulerAlgorithm == 'DWRR'){ //DWRR
			for(var j = 0; j < 4; j++){
				value_array[j] = [];
				value_array[j].push(j+1); //inex
				value_array[j].push($CheckBox('DWRR_Enable' + j,G_QosQueue[j][1])); //enable
				value_array[j].push($CheckBox('DWRR_EnableForceWeight' + j,G_QosQueue[j][2])); //EnableForceWeight
				value_array[j].push($Text('DWRR_QueueWeight' + j,G_QosQueue[j][3])); //QueueWeight
			}
		} else if (qos_SchedulerAlgorithm == 'CAR'){ //CAR
			for(var k = 0; k < 6; k++){
				value_array[k] = [];
				value_array[k].push(k+1); //index
				value_array[k].push($CheckBox('CAR_Enable' + k,G_QosQueue[k][1])); //enable
				value_array[k].push($Text('CAR_ShapingRate' + k,G_QosQueue[k][4])); //QueueCAR
			}
		}
	} else if (qos_config == 1){ //下行
		if (qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var l = 6, Precedence = [data_language.dynamic.lang_highest,data_language.dynamic.lang_high,data_language.dynamic.lang_medium,data_language.dynamic.lang_low]; l < 10; l++){
				value_array[l] = [];
				value_array[l].push(l - 5); //index
				value_array[l].push($CheckBox('Down_Enable' + l,G_QosQueue[l][1])); //enable
				value_array[l].push(Precedence[l - 6]); //Precedence			
			}
		}
	//	for(var l = 0; l < 6; l++){
	//		value_array[l] = [];
	//		value_array[l].push(l+1); //index
	//		value_array[l].push($CheckBox('Down_Enable' + l,G_QosQueue[l+6][1])); //enable
	//		value_array[l].push($Text('Down_ShapingRate' + l,G_QosQueue[l+6][4])); //QueueCAR
	//	}
	}
	
	$T('td_config',value_array);
}

function DealWithTitle(_type,_option){ //整理标题栏和语言显示
	var nodes = $('lang_bandWidth','lang_type','lang_qosBandwidth','lang_queueConfig');
	nodes[1].style.display = 'none';
	
	if(_type == 1){
		if(_option == 'SP'){
			nodes[0].innerHTML = data_language.dynamic.priority;
		}
	} else if(_type == 0){
		if(_option == 'SP'){
			nodes[0].innerHTML = data_language.dynamic.priority;
		} else if(_option == 'DWRR'){
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
	var qos_config  = checkQosConfig(); //根据QOS配置：上行 = 0；下行 = 1
	var qos_SchedulerAlgorithm = $('SELECT_QosScheduling').value; //根据调度策略来选择队列配置
	var nodeIndex = 1;
	
	$H({
		'var:qostype': qos_config,
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_interface',
		'obj-action' : 'set',
		'var:errorpage': 'qos_interface',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	if(qos_config == 0){
		if(qos_SchedulerAlgorithm == 'SP'){ //SP
			for(var i = 0; i < 4; i++){
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (i+1) + '.Enable',Form.Checkbox('SP_Enable' + i));
			}
		} else if(qos_SchedulerAlgorithm == 'DWRR'){ //DWRR
			for(var j = 0; j < 4; j++){
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (j+1) + '.Enable',Form.Checkbox('DWRR_Enable' + j));
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (j+1) + '.EnableForceWeight',Form.Checkbox('DWRR_EnableForceWeight' + j));
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (j+1) + '.QueueWeight',$('DWRR_QueueWeight' + j).value || '0');
			}
		} else if(qos_SchedulerAlgorithm == 'CAR'){ //CAR
			for(var k = 0; k < 6; k++){
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (k+1) + '.Enable',Form.Checkbox('CAR_Enable' + k));
				$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (k+1) + '.QueueCAR',$('CAR_ShapingRate' + k).value || '0');
			}
		}
	} else if(qos_config == 1){
		nodeIndex = 2;
		for(var l = 6; l < 10; l++){
	//		$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (l+1) + '.Enable',Form.Checkbox('Down_Enable' + (l - 6)));
	//		$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (l+1) + '.QueueCAR',$('Down_ShapingRate' + (l - 6)).value || '0');
			$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + (l+1) + '.Enable',Form.Checkbox('Down_Enable' + l));
		}
	}
	//全局属性
	$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.' + nodeIndex + '.Enable', Form.Checkbox('INPUT_QosEnable'));
	$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.' + nodeIndex + '.CommittedRate',$('INPUT_QosBandwidth').value);
	$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.' + nodeIndex + '.SchedulerType',$('SELECT_QosScheduling').value);
	$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.' + nodeIndex + '.EnableDSCPMark',Form.Checkbox('INPUT_EnableDSCPMark'));
	$F(':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.' + nodeIndex + '.EnableCOSMark',Form.Checkbox('INPUT_EnableCOSMark'));
	
	$('uiPostForm').submit();
}

//切换调度策略
function uiOnchgQosSchedul(){
	/*
	if(!confirm('你确认变换调度策略吗?')){//data_language.dynamic.tips
		return false;
	}*/
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.1.SchedulerType' : $('SELECT_QosScheduling').value,
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_interface',
		'obj-action' : 'set',
		'var:errorpage': 'qos_interface',
		'var:CacheLastData': ViewState.Save()		
	},true);
	$('uiPostForm').submit();
}

//变换Mode
function onChgMode(){
	if(!confirm('你确认变换QOS模板？')){
		uiPageRefresh();
		return false;
	}
	
	$H({
	//	':InternetGatewayDevice.X_TWSZ-COM_UplinkQoS.Mode' : $('SELECT_QosMode').value,//QOS模板
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_interface',
		'obj-action' : 'set',
		'var:errorpage' : 'qos_interface',
		'var:CacheLastData' : ViewState.Save()		
	},true);
	
	$('uiPostForm').submit();	
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	//arrayHint['CommittedRate'] = 'INPUT_QosBandwidth';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);
