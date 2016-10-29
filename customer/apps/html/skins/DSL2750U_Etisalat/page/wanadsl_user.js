/* ------------Wan Config-----------*/
//ATM
var G_ATM = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS WANDSLLinkConfig.ATMPeakCellRate WANDSLLinkConfig.ATMSustainableCellRate WANDSLLinkConfig.ATMMaximumBurstSize X_TWSZ-COM_VLANID X_TWSZ-COM_ProxyArp X_TWSZ-COM_QinQEnable X_TWSZ-COM_VLANPriority X_TWSZ-COM_FirewallEnable"
`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
				];
	m++;
`?>

var G_WanConns = [];

<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Username Password"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx."; //Path
		G_WanConns['Username']						= "$01";
		G_WanConns['Password']					= "**********";
`?>

function uiOnload(){
	setJSONValue({
		'INPUT_UserName'      : G_WanConns['Username'] || '',
		'INPUT_Psd'           : (G_WanConns['Username'] == "")?"":G_WanConns['Password'],
		'INPUT_ConfirmPsd'    : (G_WanConns['Username'] == "")?"":G_WanConns['Password']
	});

}

function checkValue(){
	if($('INPUT_Psd').value != $('INPUT_ConfirmPsd').value){
		alert(SEcode[1010]);
		display_ctl();
		return false;
	}
	
	return true;
}

function uiSubmit(){
	$('Apply').disabled = 1;
	$('Cancel').disabled = 1;
	var _node; //瀛樻斁鑺傜偣鐨勬暟缁?
	var _path = ':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.';
	
	if(!checkValue()){
		return false;
	}
	
	//缁熶竴鐨凢orm鍐呭?
	$H({
		'obj-action'    : 'set',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : 'wanadsl_user',
		'var:conn_type' : '<?echo $var:conn_type?>',
		'var:secIdx'    : '<?echo $var:secIdx?>',
		'var:thdIdx'    : '<?echo $var:thdIdx?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);
	
	_path = ':' + G_ATM[0][0];

	if(G_WanConns['Path'] && G_WanConns['Path'].indexOf("PPP") > -1){ // -> not to webproxy
		_path = ':' + G_WanConns['Path'];
	} 
	
	_node = $('INPUT_UserName','INPUT_Psd');
	$F(_path + 'Username'                  , _node[0].value.strip());
	if(_node[1].value != '**********'){
		$F(_path + 'Password'              , _node[1].value);
	}

	$('uiPostForm').submit();
}

function backtowancfg(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg';
}


function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanadsl_user&var:conn_type=<?echo $var:conn_type?>&var:secIdx=<?echo $var:secIdx?>&var:thdIdx=<?echo $var:thdIdx?>';
}

function display_ctl(){
	$('Apply').disabled = 0;
	$('Cancel').disabled = 0;
}
function dealWithError(){
	if (G_Error != 1){
		return false; 
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
