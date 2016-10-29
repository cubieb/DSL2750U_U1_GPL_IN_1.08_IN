// qos business
var G_Qos_Enable = G_Qos_AppName = G_Qos_AppQueue = G_Qos_DSCPMark = G_Qos_COSMark = '';
<?if neq '-' `$var:pathIndex`
`	<?mget :InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>. "Enable AppName AppQueue DSCPMark COSMark"
	`	G_Qos_Enable   = "$01"; //Enable
		G_Qos_AppName  = "$02"; //AppName
		G_Qos_AppQueue = "$03"; //AppQueue
		G_Qos_DSCPMark = "$04"; //DSCPMark
		G_Qos_COSMark  = "$05"; //COSMark
	`?>
`?>
// qos queue
var G_Qos_Queue = [];
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Queue. ""
`	G_Qos_Queue[n] = [];
	G_Qos_Queue[n][0] = "$00"; //Index
	n++;
`?>

//调度策略
var G_QueueSchedulerAlgorithm = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Interface.1.SchedulerType?>";

function uiOnload(){
	//动态生成下拉框：Wan接口
	createInfOption();
	//动态生成802.1P标记
	createCosMarkOption();
	//动态生成业务名称
	createbusinessNameOption();
	//动态生成DSCP标记
	createDSCPMarkOpions();
	//统一赋值
	setJSONValue({
		INPUT_Enable         : G_Qos_Enable || '0',
		SELECT_Interface     : G_Qos_AppName,
		SELECT_DSCPMark      : G_Qos_DSCPMark || '-1',
		SELECT_COSMark       : G_Qos_COSMark || '-1',
		SELECT_ClassifyQueue : G_Qos_AppQueue || '-1'
	});
}

function createInfOption(){	
	var _text = [], _value = [];
	var _len = 4;
	//分类队列
	switch(G_QueueSchedulerAlgorithm){
		case 'SP'   : break;
		case 'DWRR' : break;
		case 'CAR'  : _len = 6;
	} 
	
	_text.push(data_language.dynamic.lang_unbound);
	_value.push('-1');
	//
	for(var i = 0; i < _len; i++){
		_text.push('Up_Q' + G_Qos_Queue[i][0]);
		_value.push(G_Qos_Queue[i][0]);
	}
	$S('SELECT_ClassifyQueue',_text,_value);
}

function createDSCPMarkOpions(){
	var value_array   = [data_language.dynamic.lang_notMark, 'AF11','AF12','AF13','AF21','AF22','AF23','AF31','AF32','AF33','AF41','AF42','AF43','CS1','CS2','CS3','CS4','CS5','CS6','CS7','EF', 'Default'];
	var value_options = ['-1', '10','12','14','18','20','22','26','28','30','34','36','38','8','16','24','32','40','48','56','46','0']; 
	$S('SELECT_DSCPMark',value_array,value_options);
}

function createCosMarkOption(){
	var value_array = [data_language.dynamic.lang_notMark, '0', '1', '2', '3', '4', '5', '6', '7'];
	var value_options = ['-1', '0', '1', '2', '3', '4', '5', '6', '7'];
	$S('SELECT_COSMark',value_array,value_options);
}

function createbusinessNameOption(){
	var value_array	= ['TR069', 'VOIP', data_language.dynamic.lang_other];
	var value_options = ['TR069', 'VOIP', ''];
	$S('SELECT_Interface',value_array,value_options);
}

function uiSubmit(){
	var node_array = $('SELECT_Interface','SELECT_ClassifyQueue','SELECT_DSCPMark','SELECT_COSMark');
	
	$H({
		'getpage'  :'html/index.html',
		'errorpage':'html/index.html',
		'var:menu' :'advanced',
		'var:page' :'qos_config',
		'var:subpage':'qos_flux',
		'var:errorpage': 'qos_queue_edit',
		'var:pathIndex': '<?echo $var:pathIndex?>',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	if('<?echo $var:pathIndex?>' == '-'){ //添加
		$H({
			'add-obj'   : 'InternetGatewayDevice.X_TWSZ-COM_QoS.App.',
			':Enable'   : Form.Checkbox('INPUT_Enable'),
			':AppName'  : node_array[0].value,
			':AppQueue' : node_array[1].value,
			':DSCPMark' : node_array[2].value,
			':COSMark'  : node_array[3].value,
			'obj-action': 'add-set'
		});		
	} else { //编辑
		$H({
			':InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>.Enable'   : Form.Checkbox('INPUT_Enable'),
			':InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>.AppName'  : node_array[0].value,
			':InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>.AppQueue' : node_array[1].value,
			':InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>.DSCPMark' : node_array[2].value,
			':InternetGatewayDevice.X_TWSZ-COM_QoS.App.<?echo $var:pathIndex?>.COSMark'  : node_array[3].value,
			'obj-action' : 'set'
		});
	}
	$('uiPostForm').submit();
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=qos_config&var:subpage=qos_queue_edit&var:pathIndex=<?echo $var:pathIndex?>';
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	dealErrorMsg({
		'AppName' : 'SELECT_Interface',
		'AppQueue': 'SELECT_ClassifyQueue'
	}, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
