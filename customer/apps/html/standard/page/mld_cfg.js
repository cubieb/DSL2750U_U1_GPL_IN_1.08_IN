//MLD
<?mget :InternetGatewayDevice.X_TWSZ-COM_MLD.Proxy. "Enable AssociatedWANConnection"
`	var G_MLDProxyEnable  = "$01"; // Enable
	var G_WANPathName = "$02"; // AssociatedWANConnection
`?>

var G_Wanconns = [];
var n = 0;
//WAN Device
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Enable Name ConnectionType X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [
						 "$02",
						 "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00.X_TWSZ-COM_IPv6Config",
						 "$03",
						 "$04".replace('_','&'),
						 "$01"//enable
						 ];
		        n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Enable Name ConnectionType X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [
							 "$02",
							 "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.X_TWSZ-COM_IPv6Config",
							 "$03",
							 "$04".replace('_','&'),
							 "$01"//enable
							 ];
			n++;
		`?>
	`?>
`?>

<?mget :InternetGatewayDevice.X_TWSZ-COM_MLD.Snooping. "Enable"
`	var G_MLD_Snooping = "$01";
`?>

function uiOnload(){
	Form.Checkbox('INPUT_MLDProxyEnable',G_MLDProxyEnable);
	GetwanPath();

	Form.Checkbox('INPUT_MLDSnoopingEnable',G_MLD_Snooping);
	dealWithError();	
}

function GetwanPath(){
	var text_wan = [],value_wan = [];
	
	for(var i = 0; i < G_Wanconns.length; i++){
		
		/* dhcp,static ipv6 */
		
		/*pppoe_v6*/
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0 && G_Wanconns[i][2] == 'IP_Routed' && G_Wanconns[i][4] == '1' 
			&& (G_Wanconns[i][3] == '1')){
			text_wan.push(G_Wanconns[i][0]);
			value_wan.push(G_Wanconns[i][1]);
			if (G_WANPathName == G_Wanconns[i][1])
				$('SELECT_WanInterface').value = G_WANPathName;
		}
	}
	
	$S('SELECT_WanInterface',text_wan,value_wan);
}

function uiSubmit(){
	$F(':InternetGatewayDevice.X_TWSZ-COM_MLD.Proxy.Enable', Form.Checkbox('INPUT_MLDProxyEnable'));
	if (Form.Checkbox('INPUT_MLDProxyEnable')){
		if ($('SELECT_WanInterface').value == ""){
			alert("Please add a wan connection!");
			return false;
		}
		$F(':InternetGatewayDevice.X_TWSZ-COM_MLD.Proxy.AssociatedWANConnection', $('SELECT_WanInterface').value);
	}
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_MLD.Snooping.Enable':Form.Checkbox('INPUT_MLDSnoopingEnable'),
		'var:menu'     : 'advanced',
		'var:page'     : 'mld_cfg',
		'obj-action'   : 'set',
		'getpage'      : 'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:errorpage': 'mld_cfg',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError(){
	if(G_Error != 1){
		return false;
	}
		
	dealErrorMsg({}, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

