// ip filter。$var:IPfilterType表示过滤类型<?echo $var:IPfilterType?>:<?echo $var:nodePath?>
var G_Description=G_Enable=G_SrcIP=G_DestIP=G_SrcPort=G_DestPort=G_Protocol=G_SrcMask=G_DstMask=G_DevPath='';
<? if neq `-` `$var:nodePath`
`	<?mget :InternetGatewayDevice.X_TWSZ-COM_Firewall.<?echo $var:IPfilterType?>Filter.<?echo $var:nodePath?>. "Description Enable SrcIP DestIP SrcPort DestPort Protocol SrcMask DstMask DevPath"
	`	G_Description = "$01";
		G_Enable     = "$02";
		G_SrcIP       = "$03";
		G_DestIP      = "$04";
		G_SrcPort     = "$05";
		G_DestPort    = "$06";
		G_Protocol    = "$07";
		G_SrcMask     = "$08";
		G_DstMask     = "$09";
		G_DevPath     = "$0a";
	`?>	
`?>

var G_wanConnction = [];
var m = 0;
//Wan Device
<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00"; // about Path
			m++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // wan's name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00"; // about Path;
			m++;
            `?>
		`?>
	`?>
`?>
//Lan Device
var n = 0;
<?objget :InternetGatewayDevice.Layer2Bridging.Bridge. "BridgeName X_TWSZ-COM_AssociatedLANDevice"
`	G_wanConnction[m] = [];
	G_wanConnction[m][0] = "$01" ; // lan's name
	G_wanConnction[m][1] = "$02";
	n++;
	m++;
`?>

/*
</objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID"
`	G_wanConnction[m] = [];
	G_wanConnction[m][0] = "$01"; // lan's name
	G_wanConnction[m][1] = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00";
	++m;
`/>
*/

function uiOnload(){
	//给DevPath赋值
	var _text = [], _value = [];
	for(var i = 0; i < G_wanConnction.length; i++){
		//过滤条件：wan->lan，只能选wan端的连接；lan->wan，只能选lan端的连接
		if(('<?echo $var:IPfilterType?>'.indexOf('Lan') > -1 && G_wanConnction[i][1].indexOf('LANDevice') > -1) || 
			('<?echo $var:IPfilterType?>'.indexOf('Wan') > -1 && G_wanConnction[i][1].indexOf('WANDevice') > -1)
		){
			_text.push(G_wanConnction[i][0]);
			_value.push(G_wanConnction[i][1]);
		}
	}
	$S('SELECT_ConnsName',_text,_value);
	//当用户编辑的时候，禁止用户再修改DevPath
	$('SELECT_ConnsName').disabled = G_DevPath == '' ? false : true; 
	
	//给其他的节点赋值
	setJSONValue({
		'INPUT_Enable'     :G_Enable || 1,
		'INPUT_Description':G_Description,
		'INPUT_SrcIP'      :G_SrcIP,
		'INPUT_SrcMask'    :G_SrcMask,
		'INPUT_DestIP'     :G_DestIP,
		'INPUT_DstMask'    :G_DstMask,
		'SELECT_Protocol'  :(G_Protocol == '' ? 'TCP' : G_Protocol.toUpperCase()),
		'SELECT_ConnsName' :G_DevPath || G_wanConnction[0][1]
	});
	
	changeProtocol();
}

function uiSubmit(){	
	var Value_Nodes = $('INPUT_Description','INPUT_SrcIP','INPUT_SrcMask','INPUT_SrcPortStart','INPUT_SrcPortEnd','INPUT_DestIP','INPUT_DstMask','INPUT_DestPortStart','INPUT_DestPortEnd','SELECT_Protocol','SELECT_ConnsName');
	//如果连接为空，则不能提交
	if(Value_Nodes[10].value == ''){
		DealWith.Wrong('SELECT_ConnsName', SEcode[1002]);
		return false;
	}
	<?setvar var:Path <?if eq `-` `$var:nodePath` `` `InternetGatewayDevice.X_TWSZ-COM_Firewall.<?echo $var:IPfilterType?>Filter.$var:nodePath.`?>?>
	$H({
		':<?echo $var:Path?>Enable'     : Form.Checkbox('INPUT_Enable'),
		':<?echo $var:Path?>Description': Value_Nodes[0].value,
		':<?echo $var:Path?>SrcIP'      : Value_Nodes[1].value,
		':<?echo $var:Path?>SrcMask'    : Value_Nodes[2].value,
		':<?echo $var:Path?>DestIP'     : Value_Nodes[5].value,
		':<?echo $var:Path?>DstMask'    : Value_Nodes[6].value,
		':<?echo $var:Path?>Protocol'   : Value_Nodes[9].value,
		':<?echo $var:Path?>SrcPort'    : Value_Nodes[9].value != 'ICMP' ? (Value_Nodes[3].value + ':' + Value_Nodes[4].value).replace(/(^:*)|(:*$)/g,'') : undefined,
		':<?echo $var:Path?>DestPort'   : Value_Nodes[9].value != 'ICMP' ? (Value_Nodes[7].value + ':' + Value_Nodes[8].value).replace(/(^:*)|(:*$)/g,'') : undefined,
		':<?echo $var:Path?>DevPath'    : Value_Nodes[10].value,
		'var:nodePath':'<?echo $var:nodePath?>',
		'getpage'  : 'html/index.html',
		'errorpage': 'html/index.html',
		'var:menu' : 'advanced',
		'var:page' : 'ip_list',
		'var:IPfilterType' : '<?echo $var:IPfilterType?>',
		'var:errorpage'    : 'ip_filter',
		'var:CacheLastData': ViewState.Save()
	},true);
	//提交分两种情况：1、添加实例；2、编辑实例
	if('<?echo $var:nodePath?>' == '-'){
		$F('add-obj','InternetGatewayDevice.X_TWSZ-COM_Firewall.<?echo $var:IPfilterType?>Filter.');
		$F('obj-action','add-set');
	} else {
		$F('obj-action','set');
	}
	$('uiPostForm').submit();
}

function changeProtocol(){
	var node_value = $('SELECT_Protocol','INPUT_SrcPortStart','INPUT_SrcPortEnd','INPUT_DestPortStart','INPUT_DestPortEnd','div_src_port','div_des_port');
	if(node_value[0].value == 'ICMP'){
		node_value[1].disabled = true;
		node_value[2].disabled = true;
		node_value[3].disabled = true;
		node_value[4].disabled = true;
		node_value[5].style.color = '#aaa';
		node_value[6].style.color = '#aaa';
		supplyValue('INPUT_SrcPortStart','');
		supplyValue('INPUT_SrcPortEnd','');
		supplyValue('INPUT_DestPortStart','');
		supplyValue('INPUT_DestPortEnd','');
	} else {
		node_value[1].disabled = false;
		node_value[2].disabled = false;
		node_value[3].disabled = false;
		node_value[4].disabled = false;
		node_value[5].style.color = '#000';
		node_value[6].style.color = '#000';
		supplyValue('INPUT_SrcPortStart',G_SrcPort.split(':')[0]);
		supplyValue('INPUT_SrcPortEnd',G_SrcPort.split(':')[1] || '');
		supplyValue('INPUT_DestPortStart',G_DestPort.split(':')[0]);
		supplyValue('INPUT_DestPortEnd',G_DestPort.split(':')[1] || '');
	}
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=ip_list&var:subpage=ip_filter&var:IPfilterType=<?echo $var:IPfilterType?>&var:nodePath=<?echo $var:nodePath?>';
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	arrayHint['SrcIP']      = 'INPUT_SrcIP';
	arrayHint['SrcMask']    = 'INPUT_SrcMask';
	arrayHint['SrcPort']    = 'INPUT_SrcPortStart';
	arrayHint['DestIP']     = 'INPUT_DestIP';
	arrayHint['DstMask']    = 'INPUT_DstMask';
	arrayHint['DestPort']   = 'INPUT_DestPortStart';
	arrayHint['Enable']     = 'INPUT_Enable';
	arrayHint['Protocol']   = 'SELECT_Protocol';
	arrayHint['Description']= 'INPUT_Description';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
