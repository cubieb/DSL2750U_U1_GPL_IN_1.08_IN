//wizard -> wlan
var Wlan;

var wireless_name = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID?>";
var randomWEPKey = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.1.WEPKey?>";
var randomWPAKEY = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.KeyPassphrase?>";
var keymode="<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BeaconType?>";
var visibleenable = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSIDAdvertisementEnabled?>";
var wirlessEnable = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable?>";

function createRandomSSID()
{
	var tmp='';
    for(var i=0; i<6; i++){
		var random=parseInt(Math.random()*10).toString();
		//alert(random);
		tmp = tmp + random;
	}
	wireless_name="Speedy-"+tmp;
}

function createRandomWEPKey()
{
	var tmp='';
    for(var i=0; i<5; i++){
		var random=parseInt(Math.random()*10).toString();
		//alert(random);
		tmp = tmp + random;
	}
	randomWEPKey=tmp;
}

function uiOnload(){

	//createRandomSSID();
	
	//createRandomWEPKey();
	
	chgHTML();
	
	//èŽ·å–æš‚å­˜æ•°æ®
	Wlan = getJsonObj('sec');
	if(Wlan == null){
		alert('Wlan is null!');
		return false;
	}
	
	//èµ‹å€?
	setJSONValue({
		'INPUT_WlanEnable' : Wlan && Wlan['INPUT_WlanEnable'] || wirlessEnable,
		'INPUT_SSID'       : Wlan && Wlan['INPUT_SSID'] || wireless_name,
		//'vb_status'        : Wlan && Wlan['vb_status'] || '0',
		'vb_status'        : Wlan && Wlan['vb_status'] || visibleenable||'1',
		'auth'             : Wlan && Wlan['auth'] || keymode,
		'INPUT_WepKey'     : Wlan && Wlan['INPUT_WepKey'] || randomWEPKey,
		'INPUT_WPA_Key'    : Wlan && Wlan['INPUT_WPA_Key'] || randomWPAKEY,
		'INPUT_WPA2_Key'   : Wlan && Wlan['INPUT_WPA2_Key'] || randomWPAKEY
	});
	
	ctrlSecType();
}

function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//éšè—ä¸»èœå?
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//éšè—äºŒçº§èœå•
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}

//ç®¡ç†åŠ å¯†å®‰å…¨é¡µé¢
function ctrlSecType(){
	var nodes   = $('none','wep','wpa_psk','wpa2_psk');
	var secType = Form.Radio('auth');
	var _none = 'none', _basic = 'none', _wap = 'none', _11i = 'none';
	
	switch(secType){
		case 'None' : {
			_none = 'block';
			break;
		}
		case 'Basic' : {
			_basic = 'block';
			break;
		}
		case 'WPA' : {
			_wap = 'block';
			break;
		}
		case '11i' : {
			_11i = 'block';
			break;
		}
	}
	
	nodes[0].style.display = _none;
	nodes[1].style.display = _basic;
	nodes[2].style.display = _wap;
	nodes[3].style.display = _11i;
	$('Menu').style.height=566 + 'px';
	$('Content').style.height=566 + 'px';
}

function uiNextPage(){
	if(!checkValue())
	{
		return false;
	}
	
	if(!createJsonData()){
		return false;
	}
	
	
	$H({
	    'var:menu'	: 'setup',
		'var:subpage'  : 'wizaccountpsd',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	});
	
	$('uiPostForm').submit();	
}

function checkValue()
{
	//check ssid
	if(!checkSSID($('INPUT_SSID').value))
	{
		return false;
	}

	//check password
	var secType = Form.Radio('auth');
	var nodes_isp = $('INPUT_WepKey','INPUT_WPA_Key','INPUT_WPA2_Key');

	switch(secType)
	{
		case 'None' :
		{
			break;
		}
		case 'Basic' :
		{
			if(!checkBasic(nodes_isp[0].value))
			{
				alert('Please enter the Basic password with right format !');
				return false;
			}
			break;
		}
		case 'WPA' :
		{
			if(!checkWPA(nodes_isp[1].value))
			{
				alert('Please enter the WPA password with right format !');
				return false;
			}
			break;
		}
		case '11i' :
		{
			if(!checkWPA(nodes_isp[2].value))
			{
				alert('Please enter the 11i password with right format !');
				return false;
			}
			break;
		}
	}

	return true;
}

//check ssid
function checkSSID(x)
{
	/*var patrn=/^[a-zA-Z0-9]{1}([a-zA-Z0-9]|[_]|[@]|[.]|[-]){0,31}$/;
	if (!patrn.exec(x))
		return false;
	else*/
	if(x.charAt(0)==' ')
	{
		alert("Error: The first character of SSID can not be null");
		return false;
	}
	else if(x.charAt(x.length  -1)==' ')
	{
		alert("Error: The last character of SSID can not be null ");
		return false;
	}
	else if(x.indexOf('<\?') != -1 )
	{
		alert("Error: The string of SSID not support contain <\? ");
		return false;
	}
	else if(x == '')
	{
		alert("Error: SSID can not be null");
		return false;
	}
	else
	{
		return true;
	}
}

//check basic
function checkBasic(x)
{
	var patrn;
	if(x.length == 10)
	{
		patrn=/^[a-fA-F0-9]{10}$/;
		if (!patrn.exec(x))
			return false;
		else
			return true;
	}
	else if(x.length == 26)
	{
		patrn=/^[a-fA-F0-9]{26}$/;
		if (!patrn.exec(x))
			return false;
		else
			return true;
	}
	else if((x.length == 5) || (x.length == 13))
		return true;
	else
		return false;
}

//check wpa or 11i
function checkWPA(x)
{
/*
var patrn=/^[a-zA-Z0-9]{8,63}$/;
	if (!patrn.exec(x))
		return false;
	else
		return true;
*/
//¶ÔWPA ºÍWPA2Ö»½øÐÐÎ»Êý¼ì²â£¬²»½øÐÐ×Ö·û¼ì²â
    if(x.length > 63 || x.length <8)
	    return false;
    else
		return true;
}

function createJsonData(){
	var _json = '{';
	var nodes_isp = $('INPUT_WlanEnable','INPUT_SSID',
					  //wep
					  'INPUT_WepKey',
					  //wpa
					  'INPUT_WPA_Key',
					  //wpa2
					  'INPUT_WPA2_Key');
	//åŠ å¯†ç±»åž‹
	var _auth = Form.Radio('auth');
	
	switch(_auth){
		case 'None' : 
			_json += 'auth:"' + _auth + '",';
			break;
		case 'Basic' : 
			_json += 'auth:"' + _auth + '",';
			_json += 'INPUT_WepKey:"' + nodes_isp[2].value + '",';
			break;
		case 'WPA' :
			_json += 'auth:"' + _auth + '",';
			_json += 'INPUT_WPA_Key:"' + nodes_isp[3].value + '",';
			break;
		case '11i' :
			_json += 'auth:"' + _auth + '",';
			_json += 'INPUT_WPA2_Key:"' + nodes_isp[4].value + '",';			
			break;
	}

	_json += 'INPUT_WlanEnable:"' + (nodes_isp[0].checked ? '1' : '0') + '",';
	_json += 'vb_status:"' + Form.Radio('vb_status') + '",';
	_json += 'INPUT_SSID:"' + nodes_isp[1].value + '",';
	
	//è°ƒæ•´JSONæ•°æ®
	_json = _json.delcomma();
	_json += '}';
	
	addJsonObj('sec', _json);
	return true;
}

function uiBack(){
	
	if(!createJsonData()){
		return false;
	}
	
	$H({
	    'var:menu'	: 'setup',
		'var:subpage' : 'wizisp',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function dealWithError(){
	
}

addListeners(uiOnload, dealWithError);
