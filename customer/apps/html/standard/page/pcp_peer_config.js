//外来参数：var:WanType/var:FstIndex/var:SecIndex/var:ThdIndex/var:FourthIndex
var G_WanConnection    = ''; // WAN连接
var G_ServerName       = ''; // 服务器IP
var G_Protocol         = ''; // 协议
var G_InternalPort     = ''; // LAN端主机端口
var G_ExternalAddr6    = ''; // 外部公共IP
var G_ExternalPort     = ''; // 外部公共端口
var G_Lifetime         = ''; // 有效期(秒)
var G_ThirdPartyEnable = ''; // 代理第三方主机
var G_ThirdPartyAddr6  = ''; // 第三方主机IP

G_WanConnection = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:FstIndex.WAN<?echo $var:WanType?>Connection.$var:SecIndex.Name?>";

G_ServerName = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:FstIndex.WAN<?echo $var:WanType?>Connection.$var:SecIndex.X_TWSZ-COM_PCP.$var:ThdIndex.ServerName?>";

//PPPOE AND IP
<?if neq '-' $var:FourthIndex
`
	<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>. "Protocol InternalPort ExternalAddr6 ExternalPort Lifetime ThirdPartyAddr6"
	`
		G_Protocol        = "$01";
		G_InternalPort    = "$02";
		G_ExternalAddr6   = "$03";
		G_ExternalPort    = "$04";
		G_Lifetime        = "$05";
		G_ThirdPartyAddr6 = "$06";
	`?>
`?>

var G_ThirdPartyEnable = G_Protocol ? 1 : 0; // 代理第三方主机

function uiOnload(){
	//统一赋值
	setJSONValue({
		'INPUT_WanConnection'    : G_WanConnection, // WAN连接
		'INPUT_ServerName'       : G_ServerName, // 服务器IP
		'SELECT_Protocol'        : G_Protocol || 'TCP', // 协议
		'INPUT_InternalPort'     : G_InternalPort, // LAN端主机端口
		'INPUT_ExternalAddr6'    : G_ExternalAddr6, // 外部公共IP
		'INPUT_ExternalPort'     : G_ExternalPort, // 外部公共端口
		'INPUT_Lifetime'         : G_Lifetime, // 有效期(秒)
		'INPUT_ThirdPartyEnable' : G_ThirdPartyEnable, // 代理第三方主机
		'INPUT_ThirdPartyAddr6'  : G_ThirdPartyAddr6, // 第三方主机IP
	});
    $('INPUT_WanConnection').disabled = true;
    $('INPUT_ServerName').disabled = true;

	ctrlThirdPartyEnable();
}

function ctrlThirdPartyEnable(){
    G_ThirdPartyEnable = $('INPUT_ThirdPartyEnable').checked;
    $('INPUT_ThirdPartyAddr6').disabled = !G_ThirdPartyEnable;
}

function uiSubmit() {
	if(!checkIllegalValue()){
		return false;
	}

	//提交分两种情况：添加和编辑
	if('-' == '<?echo $var:FourthIndex?>'){
		$H({
			'add-obj' : '<?echo $var:wanEditable?>' + '.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.',
			':Protocol'     : $('SELECT_Protocol').value,
			':InternalPort'     : $('INPUT_InternalPort').value,
			':ExternalAddr6'     : $('INPUT_ExternalAddr6').value,
			':ExternalPort'     : $('INPUT_ExternalPort').value,
			':Lifetime'     : $('INPUT_Lifetime').value,
			':ThirdPartyAddr6'     : $('INPUT_ThirdPartyAddr6').value,
			'obj-action' : 'add-set'
		},true);
	} else {
		$H({
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.Protocol'     : $('SELECT_Protocol').value,
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.InternalPort'     : $('INPUT_InternalPort').value,
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.ExternalAddr6'     : $('INPUT_ExternalAddr6').value,
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.ExternalPort'     : $('INPUT_ExternalPort').value,
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.Lifetime'     : $('INPUT_Lifetime').value,
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $var:FstIndex?>.WAN<?echo $var:WanType?>Connection.<?echo $var:SecIndex?>.X_TWSZ-COM_PCP.<?echo $var:ThdIndex?>.Peer.<?echo $var:FourthIndex?>.ThirdPartyAddr6'     : $('INPUT_ThirdPartyAddr6').value,
			'obj-action':'set'
		},true);		
	}
	$H({
		'var:menu'     : 'advanced',
		'var:page'     : 'pcp_list',
		'var:subpage'  : 'pcp_list',
		'var:WanType'  : '<?echo $var:WanType?>',
		'var:FstIndex' : '<?echo $var:FstIndex?>',
		'var:SecIndex' : '<?echo $var:SecIndex?>',
		'var:ThdIndex' : '<?echo $var:ThdIndex?>',
		'var:FourthIndex' : '<?echo $var:FourthIndex?>',
		'getpage'      : 'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:errorpage': 'pcp_peer_config',
		'var:wanEditable' : '<?echo $var:wanEditable?>',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}


//判断值是否存在为0的函数
function checkIllegalValue(){
	var nodes = $('INPUT_ServerName','INPUT_InternalPort','INPUT_ExternalAddr6','INPUT_ExternalPort');

	if(nodes[0].value == '') {
		DealWith.Wrong('INPUT_ServerName',UEcode[1037]);
		return false;
	} else if(nodes[1].value == '') {
		DealWith.Wrong('INPUT_InternalPort',UEcode[1037]);
		return false;
	} else if(nodes[2].value == '') {
		DealWith.Wrong('INPUT_ExternalAddr6',UEcode[1037]);
		return false;
	} else if(nodes[3].value == '') {
		DealWith.Wrong('INPUT_ExternalPort',UEcode[1037]);
		return false;
	} else if(nodes[1].value == '0') {
		DealWith.Wrong('INPUT_InternalPort',SEcode[1003]);
		return false;
	} else if(nodes[3].value == '0') {
		DealWith.Wrong('INPUT_ExternalPort',SEcode[1003]);
		return false;
	}
	
	return true;
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=pcp_list&var:subpage=pcp_peer_config&var:WanType=<?echo $var:WanType?>&var:FstIndex=<?echo $var:FstIndex?>&var:SecIndex=<?echo $var:SecIndex?>&var:ThdIndex=<?echo $var:ThdIndex?>&var:FourthIndex=<?echo $var:FourthIndex?>';
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	arrayHint['ServerName']      = 'INPUT_ServerName';
	arrayHint['Protocol']        = 'SELECT_Protocol';
	arrayHint['InternalPort']    = 'INPUT_InternalPort';
	arrayHint['ExternalAddr6']   = 'INPUT_ExternalAddr6';
	arrayHint['ExternalPort']    = 'INPUT_ExternalPort';
	arrayHint['Lifetime']        = 'INPUT_Lifetime';
	arrayHint['ThirdPartyAddr6'] = 'INPUT_thirdPartyAddr6';

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
