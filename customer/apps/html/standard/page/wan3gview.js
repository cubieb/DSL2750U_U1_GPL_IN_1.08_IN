/*NEW3G Created by scb at 2011-7-22*/
//WANAccessType
var G_WANAccessType = "<?get :InternetGatewayDevice.WANDevice.2.WANCommonInterfaceConfig.WANAccessType?>";

<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "Status Enable Inform"
`	var G_3gInform	= "$01";
	var G_Enable		= "$02";
	var G_3gStatus		= "$03";
`?>

//Wan Conns List
var G_WanConns = [];
var m = 0;
var G_WanConns3g = [];
var n = 0;

//WanPPPConns List
var temp = [];
var t =0;

var dial_act;

var szNoUsbCard = 'NO USB CARD';

var szNoSimCard = 'NO SIM CARD';

var MID_NEW3G = '0x0499';

<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger ConnectionType X_TWSZ-COM_ProtocolClass X_TWSZ-COM_IPv6Config.ConnectionStatus X_TWSZ-COM_ServiceList"
			`	temp[t] =[];
				temp[t][0] = "InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection.$00.";//Path
				temp[t][1]=		 "$01";//ConnectionStatus
				temp[t][2]=		 "$02";//name;
				temp[t][3]=		 "$03";//Enable;
				temp[t][4]=		 "$04";//pppoe
				temp[t][5]=		 "$05";//ConnectionType
				temp[t][6]=		 "PPPoE";
				temp[t][7]=      "$06";
				temp[t][8]=      "$07";
				temp[t][9]=      "$08";      //WAN service Type
				++t;
			`?>
	`?>
`?>


var G_DefaultRouter = "<?get :InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW?>";  //默认路由;

function initWanData(){

		for(var i = 0; i < temp.length; i++)
		{
			if("pppo3g" == temp[i][2])
			{
				G_WanConns3g[n] = [];
				G_WanConns3g[n][0] = temp[i][0];
				G_WanConns3g[n][1] = temp[i][1];
				G_WanConns3g[n][2] = temp[i][2];
				G_WanConns3g[n][3] = temp[i][3];
				G_WanConns3g[n][4] = temp[i][4];
				G_WanConns3g[n][5] = temp[i][5];
				G_WanConns3g[n][6] = temp[i][7];
				G_WanConns3g[n][7] = temp[i][8];

				++n;
				break;
			}
		}
}
function uiOnload(){
	// reboot only both these two conditions met:
	//1.want to reboot,this means tag had been set value as 'reboot'
	//2.wan type changed successfully,this means error code is 0
		initWanData();
	if(('reboot' == '<?echo $var:tag?>')
		&&("<?error found?>" == "0")){
		$H({
			'obj-action'     : 'reboot',
			'var:page'       : 'system',
			'var:noredirect' : '1',
			'getpage'        : 'html/page/restarting.html'
		});
		$('uiPostForm').submit();
	}
	Form.Radio('INPUT_WanAcsType',G_WANAccessType == 1);
	
	createDSLTable();
	checkShowBtn();
}

function checkShowBtn()
{
	$('info_3g').disabled = 0;
	//$('pin_3g').style.display = 'none';
	$('driver_3g').style.display = 'none';
	if (n > 0) {
		$('add_3g').disabled = 1;
		$('del_3g').disabled = 0;
		$('edit1_3g').disabled = 0;
	
		//if (G_3gStatus == '' || G_3gStatus.indexOf(szNoUsbCard) >= 0 )
		//	$('info_3g').disabled = 1;

		if (G_3gStatus.indexOf('CONNECTTED') >= 0 || 
				G_3gStatus.indexOf('Connected') >= 0 ||
				G_3gStatus.indexOf('dialing') >= 0 || 
				G_3gStatus.indexOf('undialing') >= 0)
		{
			$('driver_3g').disabled = 1;
			//$('del_3g').disabled = 1;
		}
		else
		{
			$('driver_3g').disabled = 0;
			$('edit1_3g').disabled = 0;
		}


		if (!(G_3gStatus.indexOf('DISCONNECT') >= 0 || 
				G_3gStatus.indexOf('Disconnect') >= 0 ||
				G_3gStatus.indexOf('Unconfigured') >= 0 || 
				(G_3gStatus.indexOf('NO') >= 0 && G_3gStatus.indexOf('CARD') >= 0 )))
		{
	            $('edit1_3g').disabled = 1;
		}

		
		if ((G_3gStatus.indexOf('DISCONNECT') >= 0 || G_3gStatus.indexOf('Disconnect') >= 0 || 
				G_3gStatus.indexOf('CODE') >= 0) && G_Enable == '1' ) {
			$('pin_3g').disabled = 0;
		} else
			$('pin_3g').disabled = 1;
		

		if ((G_3gStatus.indexOf('CONNECTTED') >= 0 || 
				G_3gStatus.indexOf('Connected') >= 0 || 
				G_3gStatus.indexOf('DISCONNECT')  >= 0 ||
				G_3gStatus.indexOf('Disconnect')  >= 0) &&  G_Enable == '1' ) {
			$('info_3g').disabled = 0;
		}
		else {
			$('info_3g').disabled = 1;
		}

		if (G_3gStatus.indexOf('CONNECTTED') >= 0 || 
				G_3gStatus.indexOf('Connected') >= 0 || 
				G_3gStatus.indexOf('Connecting')  >= 0 ||
				G_3gStatus.indexOf('...') >= 0 ||
				G_3gStatus.indexOf('SEARCHING') >= 0) 
			$('del_3g').disabled = 1;
			
		if (!(G_3gStatus.indexOf('DISCONNECT') >= 0 || 
				G_3gStatus.indexOf('Disconnect') >= 0 ||
				G_3gStatus.indexOf('Unconfigured') >= 0 || 
				(G_3gStatus.indexOf('NO') >= 0 && G_3gStatus.indexOf('CARD') >= 0 )))
		{
	            $('edit1_3g').disabled = 1;
		}
	} else {
		$('add_3g').disabled = 0;
		$('del_3g').disabled = 1;
		$('edit1_3g').disabled = 1;
		$('info_3g').disabled = 1;
		$('pin_3g').disabled = 1;		
	}
	
}

//DSL
function createDSLTable(){ //生成WAN连接列表
	var value_array3g = [];
	var connect;
	var dial_disabled = '';
	var connect_status ='';		
		
	for(var i = 0, _len = G_WanConns3g.length; i < _len; i++) {		
   		if (G_3gStatus.indexOf('CONNECTTED') >= 0
			|| G_3gStatus.indexOf('Connected') >= 0
			|| G_3gStatus.indexOf("Connecting") >= 0 
			|| G_3gStatus.indexOf('dial waiting...') >= 0
			|| G_3gStatus.indexOf('dialing...') >= 0)
   		{
			dial_disabled = '';
			dial_act = 'undial';	
   		}
		else if ((G_3gStatus.indexOf('DISCONNECT') >= 0) || 
						(G_3gStatus.indexOf('Unconfigured') >= 0))
		{
			dial_act = 'dial';
			 dial_disabled = '';
		}
		else 
		{
			dial_disabled = 'disabled';
			dial_act = 'dial';
		}

		if (G_3gStatus.indexOf(szNoUsbCard) >= 0 || G_Enable != '1')
		{
			dial_disabled = 'disabled';
		}

		connect_status = G_WanConns3g[i][1] == 'Unconfigured'? 'Disconnected':
						G_WanConns3g[i][1];
		value_array3g[i] = [
						  G_WanConns3g[i][2], //Service Name
						  'PPPo3G',  //Protocol
						  G_WanConns3g[i][3], //Enable
						  connect_status, //Status
						  '<input type="checkbox" id="INPUT_DefRoutingBtn' + i + '" onclick="ctrlDefaultRouting3g(' + i + ')"' + ((G_DefaultRouter + '.') == G_WanConns3g[i][0] ? 'checked' : '') + ' disabled>', //Default Gateway
   					  	  '<input type="button" value=' + dial_act + ' onclick="ctrlConnEnable3g(' + i + ')" ' + dial_act + ' ' + dial_disabled + ' >'
						  ];
	}
	$T('wan_conns_list_3g', value_array3g);
}

function ctrlConnEnable3g(_i){
	var _path = ':InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.';
	
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	/*let webproc send the set msg to logic_3g module*/
	$F('mid', MID_NEW3G);
	
	/*let webproc known this page is belong to new3g module*/
	$F('isnew3g', '1');	

	$F(_path + 'action', 'dial');
	$F(_path + 'dialaction', dial_act);
	
	$('uiPostForm').submit();
}

function ctrlDefaultRouting3g(_id){
	var _path = G_WanConns3g[_id][0].replace(/(\.*$)/g,'');
	if(_path == G_DefaultRouter){
		_path = '';
	}
	
	$H({
		':InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService' : _path,
		'obj-action'    : 'set',
		'var:menu'      : 'setup',
		'var:page'      : 'wan3gcfg',
		'var:errorpage' : 'wancfg',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);
	$('uiPostForm').submit();
}

function uiAdd3g(){
	var _url;	
	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:WAN3GVIEW_act=add&var:subpage=wan3gcfg';
	
	document.location.href = _url;
}

function uiEdit3g(){
	var _url;	
	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:WAN3GVIEW_act=edit&var:subpage=wan3gcfg';
	
	document.location.href = _url;	
}

function uiDelete3g(){
	/*
	if(_array_id.length != 1){
		alert(SEcode[1009]);
		return false;
	}

	var Idx = _array_id[0];
	*/
	$H({
		'del-obj'       : G_WanConns3g[0][0],
		'obj-action'    : 'del',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : '<?echo $var:page?>'
	},true);

	/*let webproc send the set msg to logic_3g module*/
	$F('mid', MID_NEW3G);
	
	/*let webproc known this page is belong to new3g module*/
	$F('isnew3g', '1');		

	$('uiPostForm').submit();
}

function uiPin3g(){
	var _url;	
	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3gpin';
	
	document.location.href = _url;	
}
function uiInfo3g(){
	var _url;	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3ginfo';
	
	document.location.href = _url;	
}
function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);	
}

addListeners(uiOnload, dealWithError);

