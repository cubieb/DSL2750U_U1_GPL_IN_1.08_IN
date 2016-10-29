//FTPD Setting
<?mget :InternetGatewayDevice.X_TWSZ-COM_FTPD. "Enable WANAccessEnable Status Port"
`var G_FtpdServerEnable = "$01";
var G_FtpdServerWANAccessEnable = "$02";
var G_FtpdServerStatus = "$03";
var G_FtpdMonitorPort = "$04";
`?>
var G_LgAccessCtrl = [];
var nn = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.LACL.1.Service. "InternalPort ExternalPort"
`	G_LgAccessCtrl[nn] = [];
	G_LgAccessCtrl[nn][0] = "$01"; //InternalPort
	G_LgAccessCtrl[nn][1] = "$02"; //ExternalPort
	nn++;
`?>
var G_RgAccessCtrl = [];
var t = 0, n = 0;
<?setvar var:num <?get :InternetGatewayDevice.X_TWSZ-COM_ACL.RACLNumberOfEntries?>?>
<?if gt $var:num 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL. "DevPath ServiceNumberOfEntries"
	`	
		<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL.$10.Service. "InternalPort ExternalPort"
		`	G_RgAccessCtrl[n] = [];
			G_RgAccessCtrl[n][0] = "$01"; //InternalPort
			G_RgAccessCtrl[n][1] = "$02"; //ExternalPort
			n++;
		`?>
		t++;
	`?>
`?>

function uiOnload(){
	setJSONValue({
		'SELECT_ftpd_server_status' : G_FtpdServerStatus,
		'INPUT_FtpdServerEnable'  : G_FtpdServerEnable,
		'INPUT_FtpdMonitorPort' : G_FtpdMonitorPort
	});

	display_wan();
	dealWithError();
}

function display_wan(){
	if(1 == Form.Checkbox('INPUT_FtpdServerEnable')){
		$("ftp_for_wan").style.display = "block";
		Form.Checkbox('INPUT_FtpdWANAccessEnable', G_FtpdServerWANAccessEnable);
	}
	else{
		$("ftp_for_wan").style.display = "none";
		Form.Checkbox('INPUT_FtpdWANAccessEnable', '0');
	}
}

function uiSubmit(){
	if (checkValue() === false)
	{
		return false;
	}
	
	var port = $("INPUT_FtpdMonitorPort").value;
	
	if(Form.Checkbox('INPUT_FtpdWANAccessEnable') == 0){
		for(var i = 0; i < G_LgAccessCtrl.length ;i++){
			if( port ==  G_LgAccessCtrl[i][0] || port ==  G_LgAccessCtrl[i][1] ){
				 alert("The port is conflict with Access Control Service FTP, Please change the port of Access Control Service FTP!");
				 return false;
			}
		}
	}
	else{
		for(var i = 0; i < G_LgAccessCtrl.length ;i++){
			if( port ==  G_LgAccessCtrl[i][0] || port ==  G_LgAccessCtrl[i][1] ){
				 alert("The port is conflict with Access Control Service FTP, Please change the port of Access Control Service FTP!");
				 return false;
			}
		}
		
		for(var i = 0; i < G_RgAccessCtrl.length ;i++){
			if(port ==  G_RgAccessCtrl[i][0] || port ==  G_RgAccessCtrl[i][1] ){
				 alert("The port is conflict with Access Control Service FTP, Please change the port of Access Control Service FTP!");
				 return false;
			}
		}
	}
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_FTPD.Enable':Form.Checkbox('INPUT_FtpdServerEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_FTPD.WANAccessEnable':Form.Checkbox('INPUT_FtpdWANAccessEnable'),		
		':InternetGatewayDevice.X_TWSZ-COM_FTPD.Port':$("INPUT_FtpdMonitorPort").value,
		'var:menu'         :'advanced',
		'var:page'  :'ftpd_setting',
		'obj-action':'set',
		'var:menu'  :'advanced',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function checkValue()
{
	var port = $("INPUT_FtpdMonitorPort").value;
	if (CheckValidity.Port("INPUT_FtpdMonitorPort", "FTP监控端口") === false)
	{
		return false;
	}

	return true;
}

function uiPageRefresh(){
	document.location.href = "/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=ftpd_setting";
}

function dealWithError(){
	if (G_Error != 1) return false;

	var arrayHint = [];
	//arrayHint['X_TWSZ-COM_DNSRelayEnable'] = '开启';

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
