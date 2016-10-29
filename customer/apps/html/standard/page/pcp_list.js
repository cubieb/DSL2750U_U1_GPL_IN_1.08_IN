var G_WanConns = [];
var G_PcpArr = [];
var G_PcpMapArr = [];
var G_PcpPeerArr = [];
var n_pcp = 0;
var n_pcp_map = 0;
var n_pcp_peer = 0;
var n_wanconn = 0;

// Get all PCP/Map/Peer instances
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList"
		`	G_WanConns[n_wanconn] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00",
			                         "$01", //Name
			                         "$02", //ConnectionType
			                         "$03", //X_TWSZ-COM_ServiceList
			                        ];
			<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANIPConnection.$10.X_TWSZ-COM_PCP. "ServerName PcpPid MapNumberOfEntries PeerNumberOfEntries"
			`	G_PcpArr[n_pcp] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANIPConnection.$10.X_TWSZ-COM_PCP.$00.",
				                   "$01", // ServerName
				                   "$02", // PcpPid
				                  ];
				<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANIPConnection.$20.X_TWSZ-COM_PCP.$10.Map. "Protocol ExternalAddr6 InternalPort ExternalPort Lifetime State ThirdPartyAddr6 PreferFailure"
				`	G_PcpMapArr[n_pcp_map] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANIPConnection.$20.X_TWSZ-COM_PCP.$10.Map.$00.",
					                          "$01", // Protocol
					                          "$02", // InternalPort
					                          "$03", // ExternalAddr6
					                          "$04", // ExternalPort
					                          "$05", // Lifetime
					                          "$06", // State
					                          "$07", // ThirdPartyAddr6
					                          "$08", // PreferFailure
					                         ];
					n_pcp_map++;
				`?>
				<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANIPConnection.$20.X_TWSZ-COM_PCP.$10.Peer. "Protocol InternalPort ExternalAddr6 ExternalPort RemoteAddr6 RemotePort Lifetime State ThirdPartyAddr6"
				`	G_PcpPeerArr[n_pcp_peer] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANIPConnection.$20.X_TWSZ-COM_PCP.$10.Peer.$00.",
					                            "$01", // Protocol
					                            "$02", // InternalPort
					                            "$03", // ExternalAddr6
					                            "$04", // ExternalPort
					                            "$05", // RemoteAddr6
					                            "$06", // RemotePort
					                            "$07", // Lifetime
					                            "$08", // State
					                            "$09", // ThirdPartyAddr6
					                           ];
					n_pcp_peer++;
				`?>
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
			<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANPPPConnection.$10.X_TWSZ-COM_PCP. "ServerName PcpPid MapNumberOfEntries PeerNumberOfEntries"
			`	G_PcpArr[n_pcp] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANPPPConnection.$10.X_TWSZ-COM_PCP.$00.",
				                   "$01", // ServerName
				                   "$02", // PcpPid
				                  ];
				<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANPPPConnection.$20.X_TWSZ-COM_PCP.$10.Map. "Protocol InternalPort ExternalAddr6 ExternalPort Lifetime State ThirdPartyAddr6 PreferFailure"
				`	G_PcpMapArr[n_pcp_map] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANPPPConnection.$20.X_TWSZ-COM_PCP.$10.Map.$00.",
					                          "$01", // Protocol
					                          "$02", // InternalPort
					                          "$03", // ExternalAddr6
					                          "$04", // ExternalPort
					                          "$05", // Lifetime
					                          "$06", // State
					                          "$07", // ThirdPartyAddr6
					                          "$08", // PreferFailure
					                         ];
					n_pcp_map++;
				`?>
				<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANPPPConnection.$20.X_TWSZ-COM_PCP.$10.Peer. "Protocol InternalPort ExternalAddr6 ExternalPort RemoteAddr6 RemotePort Lifetime State ThirdPartyAddr6"
				`	G_PcpPeerArr[n_pcp_peer] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$40.WANPPPConnection.$20.X_TWSZ-COM_PCP.$10.Peer.$00.",
					                            "$01", // Protocol
					                            "$02", // InternalPort
					                            "$03", // ExternalAddr6
					                            "$04", // ExternalPort
					                            "$05", // RemoteAddr6
					                            "$06", // RemotePort
					                            "$07", // Lifetime
					                            "$08", // State
					                            "$09", // ThirdPartyAddr6
					                           ];
					n_pcp_peer++;
				`?>
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
	// $('SELECT_WanConns').value = (wanEditable == '-' ? (G_WanConns[0] ? G_WanConns[0][0] : '') : wanEditable);
	if (wanEditable == '-')
		$('SELECT_WanConns').selectedIndex = 0;
	else
		$('SELECT_WanConns').value = wanEditable;

	//生成配置列表
	createTables();
}

function createTables() {
	createPcpServerTables();
	createPcpMapTables();
	createPcpPeerTables();

	if ($('SELECT_PcpServerList').value == '')
		$('addMap').disabled = true;

	if ($('SELECT_PcpServerList').value == '')
		$('addPeer').disabled = true;
}

var pcpServerIndex = '<?echo $var:ThdIndex?>';

function createPcpServerTables() {
	var wanConns    = $('SELECT_WanConns').value;
	var _text = [], _value = [];
	for(var k = 0; k < G_PcpArr.length; k++){
		if(G_PcpArr[k][0].indexOf(wanConns) > -1){
			_text.push(G_PcpArr[k][1]);
			_value.push(G_PcpArr[k][0]);
		}
	}
	$S('SELECT_PcpServerList', _text, _value);

	if (pcpServerIndex == '-') {
		$('SELECT_PcpServerList').selectedIndex = 0;
	}
    else
	{
		for(var i = 0; i < G_PcpArr.length; i++) {
			var _split = G_PcpArr[i][0].split('.');
			if (_split[4] == '<?echo $var:FstIndex?>' &&
			    _split[6] == '<?echo $var:SecIndex?>' &&
			    _split[8] == '<?echo $var:ThdIndex?>') {
				$('SELECT_PcpServerList').value = G_PcpArr[i][0];
			}
		}
	}
}

function createPcpMapTables() {
	var pcpServer    = $('SELECT_PcpServerList').value;
	var array_value = [];
	
	for(var k = 0,i = 0; k < G_PcpMapArr.length; k++){
		if(pcpServer != '' && G_PcpMapArr[k][0].indexOf(pcpServer) > -1){
			array_value[k] = [];
			array_value[k].push(i+1); //序号
			for(var j = 0; j < G_PcpArr.length; j++){
				if(G_PcpMapArr[k][0].indexOf(G_PcpArr[j][0]) > -1){
					array_value[k].push(G_PcpArr[j][1]); // 服务器IP
					break;
				}
			}
			array_value[k].push(G_PcpMapArr[k][1]); // Protocol
			array_value[k].push(G_PcpMapArr[k][2]); // InternalPort
			array_value[k].push(G_PcpMapArr[k][3]); // ExternalAddr6
			array_value[k].push(G_PcpMapArr[k][4]); // ExternalPort
			array_value[k].push(G_PcpMapArr[k][5]); // Lifetime
			array_value[k].push(G_PcpMapArr[k][6]); // State
			array_value[k].push('<input type="button" id="edit" value="'+ data_language.dynamic.edit +'" onclick=uiEditPcpMap("' + G_PcpMapArr[k][0] + '")> <input type="button" id="delete" value="'+ data_language.dynamic.del +'" onclick=uiDelete("' + G_PcpMapArr[k][0] + '")>');
			i++;
		}
	}
	$T('PcpMapList',array_value);
	var node_tr = $('PcpMapList').getElementsByTagName('tr');
	
	for(var j = 0, new_node; j < node_tr.length - 1; j++){
		new_node = $(j + '2');
		if(new_node){
			new_node.title     = new_node.innerHTML;
			new_node.innerHTML = new_node.innerHTML.ellipsis();
		}
	}
}

function createPcpPeerTables() {
	var pcpServer    = $('SELECT_PcpServerList').value;
	var array_value = [];
	
	for(var k = 0,i = 0; k < G_PcpPeerArr.length; k++){
		if(G_PcpPeerArr[k][0].indexOf(pcpServer) > -1){
			array_value[k] = [];
			array_value[k].push(i+1); //序号
			for(var j = 0; j < G_PcpArr.length; j++){
				if(G_PcpPeerArr[k][0].indexOf(G_PcpArr[j][0]) > -1){
					array_value[k].push(G_PcpArr[j][1]); // 服务器IP
					break;
				}
			}
			array_value[k].push(G_PcpPeerArr[k][1]); // Protocol
			array_value[k].push(G_PcpPeerArr[k][2]); // InternalPort
			array_value[k].push(G_PcpPeerArr[k][3]); // ExternalAddr6
			array_value[k].push(G_PcpPeerArr[k][4]); // ExternalPort
			array_value[k].push(G_PcpPeerArr[k][5]); // RemoteAddr6
			array_value[k].push(G_PcpPeerArr[k][6]); // RemotePort
			array_value[k].push(G_PcpPeerArr[k][7]); // Lifetime
			array_value[k].push(G_PcpPeerArr[k][8]); // State
			array_value[k].push('<input type="button" id="edit" value="'+ data_language.dynamic.edit +'" onclick=uiEditPcpPeer("' + G_PcpPeerArr[k][0] + '")> <input type="button" id="delete" value="'+ data_language.dynamic.del +'" onclick=uiDelete("' + G_PcpPeerArr[k][0] + '")>');
			i++;
		}
	}
	$T('PcpPeerList',array_value);
	var node_tr = $('PcpPeerList').getElementsByTagName('tr');
	
	for(var j = 0, new_node; j < node_tr.length - 1; j++){
		new_node = $(j + '2');
		if(new_node){
			new_node.title     = new_node.innerHTML;
			new_node.innerHTML = new_node.innerHTML.ellipsis();
		}
	}
}

function uiEditPcpMap(Path){
	var _split = Path.split('.');
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_map_config',
		'var:WanType' : (Path.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'var:ThdIndex': _split[8],
		'var:FourthIndex': _split[10],
		'var:wanEditable' : $('SELECT_WanConns').value,
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_map_config',
	},true);
	
	$('uiPostForm').submit();
}

function uiEditPcpPeer(Path){
	var _split = Path.split('.');
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_peer_config',
		'var:WanType' : (Path.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'var:ThdIndex': _split[8],
		'var:FourthIndex': _split[10],
		'var:wanEditable' : $('SELECT_WanConns').value,
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_peer_config',
	},true);
	
	$('uiPostForm').submit();
}

function uiDelete(){
	if(!confirm(SEcode[1001])){
		return false;
	}
	
	$H({
		'del-obj'    : arguments[0],
		'var:menu'   : 'advanced',
		'var:page'   : 'pcp_list',
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'obj-action' : 'del'
	},true);
	
	$('uiPostForm').submit();
}

function uiConfigPcpServer(){
	var wanConns = $('SELECT_WanConns').value;
	var _split   = wanConns.split('.');
	if(wanConns == ''){
		DealWith.Wrong('SELECT_WanConns',SEcode[1002]);
		return false;
	}
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_server',
		'var:WanType' : (wanConns.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_server',
		'var:wanEditable' : wanConns,
	},true);
	
	$('uiPostForm').submit();
}

function uiAddMap() {
	var wanConns = $('SELECT_WanConns').value;
	var pcpServer = $('SELECT_PcpServerList').value;
	var _split   = pcpServer.split('.');

	if(wanConns == ''){
		DealWith.Wrong('SELECT_WanConns',SEcode[1002]);
		return false;
	}

	if(pcpServer == ''){
		DealWith.Wrong('SELECT_PcpServerList',SEcode[1009]);
		return false;
	}
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_map_config',
		'var:WanType' : (wanConns.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'var:ThdIndex': _split[8],
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_map_config',
		'var:wanEditable' : wanConns,
	},true);
	
	$('uiPostForm').submit();
}

function uiAddPeer() {
	var wanConns = $('SELECT_WanConns').value;
	var pcpServer = $('SELECT_PcpServerList').value;
	var _split   = pcpServer.split('.');

	if(wanConns == ''){
		DealWith.Wrong('SELECT_WanConns',SEcode[1002]);
		return false;
	}
	
	if(pcpServer == ''){
		DealWith.Wrong('SELECT_PcpServerList',SEcode[1009]);
		return false;
	}
	
	$H({
		'var:menu'    : 'advanced',
		'var:page'    : 'pcp_list',
		'var:subpage' : 'pcp_peer_config',
		'var:WanType' : (wanConns.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
		'var:FstIndex': _split[4],
		'var:SecIndex': _split[6],
		'var:ThdIndex': _split[8],
		'getpage'     : 'html/index.html',
		'errorpage'   : 'html/index.html',
		'var:errorpage': 'pcp_peer_config',
		'var:wanEditable' : wanConns,
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
