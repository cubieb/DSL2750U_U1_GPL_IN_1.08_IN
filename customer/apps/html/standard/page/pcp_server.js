var G_WanConns = [];
var G_PcpArr = [];
var n_pcp = 0;
var n_wanconn = 0;

// Get all PCP instances
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList"
		`	G_WanConns[n_wanconn] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00",
			                         "$01", //Name
			                         "$02", //ConnectionType
			                         "$03", //X_TWSZ-COM_ServiceList
			                        ];
			<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANIPConnection.$10.X_TWSZ-COM_PCP. "ServerName"
			`	G_PcpArr[n_pcp] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANIPConnection.$10.X_TWSZ-COM_PCP.$00.",
				                   "$01", // ServerName
				                  ];
				n_pcp++;
			`?>
			n_wanconn++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList"
		`	G_WanConns[n_wanconn] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00",
			                         "$01", //Name
			                         "$02", //ConnectionType
			                         "$03", //X_TWSZ-COM_ServiceList
			                        ];
			<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANPPPConnection.$10.X_TWSZ-COM_PCP. "ServerName"
			`	G_PcpArr[n_pcp] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANPPPConnection.$10.X_TWSZ-COM_PCP.$00.",
				                   "$01", // ServerName
				                  ];
				n_pcp++;
			`?>
			n_wanconn++;
		`?>
	`?>
`?>

//记录当前的WAN连接
var wanEditable = '<?echo $var:wanEditable?>';

function uiOnload(){
	//生成WAN连接下拉框
	var _text = [], _value = [];
	for(var i = 0; i < G_WanConns.length; i++){
		if(G_WanConns[i][2] != 'PPPoE_Bridged' && G_WanConns[i][3] != 'TR069') {
			_text.push(G_WanConns[i][1]);
			_value.push(G_WanConns[i][0]);
		}
	}
	$S('SELECT_WanConns', _text, _value);
	$('SELECT_WanConns').value = (wanEditable == '-' ? (G_WanConns[0] ? G_WanConns[0][0] : '') : wanEditable);

	if ('-' != '<?echo $var:ThdIndex?>') {
		for(var i = 0; i < G_PcpArr.length; i++) {
			var _split = G_PcpArr[i][0].split('.');
			if (_split[4] == '<?echo $var:FstIndex?>' &&
			    _split[6] == '<?echo $var:SecIndex?>' &&
			    _split[8] == '<?echo $var:ThdIndex?>') {
				$('INPUT_ServerName').value = G_PcpArr[i][1];
			}
		}
	}

	//生成配置列表
	createTables();
}

function createTables() {
	createPcpServerTables();
}

function createPcpServerTables() {
	var wanConns    = $('SELECT_WanConns').value;
	var array_value = [];
	
	for(var k = 0,i = 0; k < G_PcpArr.length; k++){
		if(G_PcpArr[k][0].indexOf(wanConns) > -1){
			array_value[k] = [];
			array_value[k].push(i+1); //序号
			array_value[k].push(G_PcpArr[k][1]); // Protocol
			array_value[k].push('<input type="button" id="edit" value="'+ data_language.dynamic.edit +'" onclick=uiEdit("' + G_PcpArr[k][0] + '")> <input type="button" id="delete" value="'+ data_language.dynamic.del +'" onclick=uiDelete("' + G_PcpArr[k][0] + '")>');
			i++;
		}
	}
	$T('PcpServerList',array_value);

	var node_tr = $('PcpServerList').getElementsByTagName('tr');
	
	for(var j = 0, new_node; j < node_tr.length - 1; j++){
		new_node = $(j + '2');
		if(new_node){
			new_node.title     = new_node.innerHTML;
			new_node.innerHTML = new_node.innerHTML.ellipsis();
		}
	}
}

function uiEdit(Path){
	var _split = Path.split('.');
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_server',
		'var:WanType' : (Path.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'var:ThdIndex': _split[8],
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_server',
		'var:wanEditable' : Path.substring(0, Path.indexOf('.X_TWSZ-COM_PCP.')),
	},true);
	
	$('uiPostForm').submit();
}

function uiDelete(){
	var wanConns = $('SELECT_WanConns').value;
	var _split   = wanConns.split('.');

	if(!confirm(SEcode[1001])){
		return false;
	}
	
	$H({
		'del-obj'      : arguments[0],
		'var:menu'     : 'advanced',
		'var:page'     : 'pcp_list',
		'var:subpage'  : 'pcp_server',
		'var:WanType'  : (wanConns.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex' : _split[4],
		'var:SecIndex' : _split[6],
		'getpage'      : 'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:errorpage': 'pcp_server',
		'var:wanEditable' : wanConns,
		'obj-action'  : 'del'
	},true);
	
	$('uiPostForm').submit();
}

function checkIllegalValue(){
	var nodes = $('INPUT_ServerName');
	//端口不能为0
	if(nodes.value == ''){
		DealWith.Wrong('INPUT_ServerName', UEcode[1037]);
		return false;
	}

	return true;
}

function uiSubmit(){
	var wanConns = $('SELECT_WanConns').value;
	var _split   = wanConns.split('.');

	if(!checkIllegalValue()){
		return false;
	}

	//提交分两种情况：添加和编辑
	if('-' == '<?echo $var:ThdIndex?>'){
		$H({
			'add-obj' : $('SELECT_WanConns').value + '.X_TWSZ-COM_PCP.',
			':ServerName'     : $('INPUT_ServerName').value,
			'obj-action' : 'add-set'
		},true);
	} else {
		$H({
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.ServerName'     : $('INPUT_ServerName').value,
			'obj-action':'set'
		},true);		
	}
	$H({
		'var:menu'     : 'advanced',
		'var:page'     : 'pcp_list',
		'var:subpage'  : 'pcp_server',
		'var:WanType'  : (wanConns.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex' : _split[4],
		'var:SecIndex' : _split[6],
		'getpage'      : 'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:errorpage': 'pcp_server',
		'var:wanEditable' : wanConns,
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	arrayHint['MACAddress'] = 'INPUT_ServerName';

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

