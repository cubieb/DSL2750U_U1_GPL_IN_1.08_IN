/*
AntiAttack
*/

//总开关
var G_AntiDosEnable = "<?get :InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDosEnable ?>";
var G_AntiLogEnable = "<?get :InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiLogEnable ?>";

//单项防护开关
<?mget :InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos. " SynCookieEnable SynMaxConnections IcmpEchoIgnrBcastEnable AntiFraggleEnable AntiEchoCargenEnable AntiIPLandEnable IgnrPortScanEnable"
`	var G_SynCookieEnable = "$01";
    var G_SynMaxConnections = "$02";
	var G_IcmpEchoIgnrBcastEnable = "$03";
	var G_AntiFraggleEnable = "$04";
	var G_AntiEchoCargenEnable = "$05";
	var G_AntiIPLandEnable = "$06";
	var G_IgnrPortScanEnable = "$07";
`?>

//防端口扫描
<?mget :InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan. "AntiScanSetSynFinEnable AntiScanSetSynRstEnable AntiScanSetFinRstEnable AntiScanUnAckSetFinEnable AntiScanUnAckSetPshEnable AntiScanUnAckSetUrgEnable AntiScanUnsetAllEnable AntiScanSetAllEnable AntiScanForAllSetSynRstAckFinUrgEnable AntiScanForAllSetFinEnable AntiScanForAllSetFinUrgPshEnable AntiScanUnAckSetFinEnable AntiScanUnAckSetPshEnable AntiScanUnAckSetUrgEnable"
`	var G_AntiScanSetSynFinEnable = "$01";
	var G_AntiScanSetSynRstEnable = "$02";
	var G_AntiScanSetFinRstEnable = "$03";
	var G_AntiScanUnAckSetFinEnable = "$04";
	var G_AntiScanUnAckSetPshEnable = "$05";
	var G_AntiScanUnAckSetUrgEnable = "$06";
	var G_AntiScanUnsetAllEnable = "$07";
	var G_AntiScanSetAllEnable = "$08";
	var G_AntiScanForAllSetSynRstAckFinUrgEnable = "$09";
	var G_AntiScanForAllSetFinEnable = "$0a";
	var G_AntiScanForAllSetFinUrgPshEnable = "$0b";
    var G_AntiScanUnAckSetFinEnable = "$0c";
    var G_AntiScanUnAckSetPshEnable = "$0d";
    var G_AntiScanUnAckSetUrgEnable = "$0e";
`?>

function OnSynCookieEnableClick(){
    var bSynCookieEnable = Form.Checkbox('INPUT_SynCookieEnable');
	if(bSynCookieEnable){
		$("INPUT_SynMaxConnections").disabled = false;
	} else {
		$("INPUT_SynMaxConnections").disabled = true;
	}
}


function uiOnload(){
	//总开关

	Form.Checkbox('INPUT_AntiDosEnable',G_AntiDosEnable);

	//单项防护开关
	Form.Checkbox('INPUT_SynCookieEnable',G_SynCookieEnable);
	$('INPUT_SynMaxConnections').value = G_SynMaxConnections;
	OnSynCookieEnableClick();

	Form.Checkbox('INPUT_IcmpEchoIgnrBcastEnable',G_IcmpEchoIgnrBcastEnable);

	Form.Checkbox('INPUT_AntiFraggleEnable',G_AntiFraggleEnable);

	Form.Checkbox('INPUT_AntiEchoCargenEnable',G_AntiEchoCargenEnable);

	Form.Checkbox('INPUT_AntiIPLandEnable',G_AntiIPLandEnable);

	Form.Checkbox('INPUT_IgnrPortScanEnable',G_IgnrPortScanEnable);

	//防端口扫描
	Form.Checkbox('INPUT_AntiScanSetSynFinEnable',G_AntiScanSetSynFinEnable);
	Form.Checkbox('INPUT_AntiScanSetSynRstEnable',G_AntiScanSetSynRstEnable);
	Form.Checkbox('INPUT_AntiScanSetFinRstEnable',G_AntiScanSetFinRstEnable);
    Form.Checkbox('INPUT_AntiScanSetAckFinEnable',G_AntiScanUnAckSetFinEnable);
    Form.Checkbox('INPUT_AntiScanSetAckPshEnable',G_AntiScanUnAckSetPshEnable);
    Form.Checkbox('INPUT_AntiScanSetAckUrgEnable',G_AntiScanUnAckSetUrgEnable);

	dealWithError();
}

function uiSubmit(){
	var path = ':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.';
	$H({
		//总开关
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDosEnable' :Form.Checkbox('INPUT_AntiDosEnable'),		
		//单项防护开关
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.SynCookieEnable':Form.Checkbox('INPUT_SynCookieEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.SynMaxConnections':Form.Checkbox('INPUT_SynCookieEnable') ? $('INPUT_SynMaxConnections').value : undefined,
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.IcmpEchoIgnrBcastEnable':Form.Checkbox('INPUT_IcmpEchoIgnrBcastEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.AntiFraggleEnable':Form.Checkbox('INPUT_AntiFraggleEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.AntiEchoCargenEnable':Form.Checkbox('INPUT_AntiEchoCargenEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.AntiIPLandEnable':Form.Checkbox('INPUT_AntiIPLandEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos.IgnrPortScanEnable':Form.Checkbox('INPUT_IgnrPortScanEnable'),
		
		//防端口扫描
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanSetSynFinEnable':Form.Checkbox('INPUT_AntiScanSetSynFinEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanSetSynRstEnable':Form.Checkbox('INPUT_AntiScanSetSynRstEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanSetFinRstEnable':Form.Checkbox('INPUT_AntiScanSetFinRstEnable'),
       // ':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanUnAckSetFinEnable':Form.Checkbox('INPUT_AntiScanSetAckFinEnable'),
       // ':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanUnAckSetPshEnable':Form.Checkbox('INPUT_AntiScanSetAckPshEnable'),
      //  ':InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan.AntiScanUnAckSetUrgEnable':Form.Checkbox('INPUT_AntiScanSetAckUrgEnable'),
		
		'obj-action':'set',
		'var:menu'  :'advanced',
		'var:page'  :'dos',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:errorpage' : 'dos',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	arrayHint['AntiDosEnable'] = 'INPUT_AntiDosEnable';
	arrayHint['AntiLogEnable'] = 'INPUT_AntiLogEnable';
	
	arrayHint['SynCookieEnable'] = 'INPUT_SynCookieEnable';
	arrayHint['SynMaxConnections'] = 'INPUT_SynMaxConnections';
	arrayHint['IcmpEchoIgnrBcastEnable'] = 'INPUT_IcmpEchoIgnrBcastEnable';
	arrayHint['AntiFraggleEnable'] = 'INPUT_AntiFraggleEnable';
	arrayHint['AntiEchoCargenEnable'] = 'INPUT_AntiEchoCargenEnable';
	arrayHint['AntiIPLandEnable'] = 'INPUT_AntiIPLandEnable';
	arrayHint['IgnrPortScanEnable'] = 'INPUT_IgnrPortScanEnable';

	arrayHint['AntiScanSetSynFinEnable'] = 'INPUT_AntiScanSetSynFinEnable';
	arrayHint['AntiScanSetSynRstEnable'] = 'INPUT_AntiScanSetSynRstEnable';
	arrayHint['AntiScanSetFinRstEnable'] = 'INPUT_AntiScanSetFinRstEnable';

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
