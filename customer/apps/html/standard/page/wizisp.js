//wizard -> isp
var G_Conns = [];
var n = 0;
var wan_type = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`
		<?if gt $11 0
		`
			<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name"
			`
				G_Conns[n] = ["$01"];
				n++;
			`?>
		`?>
		<?if gt $12 0
		`
			<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name"
			`
				G_Conns[n] = ["$01"];
				n++;
			`?>
		`?>
	`?>
`?>
var Isp;
//PVC
var com_pvc  = '<?get :InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.DefaultPVCList?>';

function uiOnload(){
	//获取暂存数据
	Isp = getJsonObj('isp');
	if(Isp == null){
		alert('Isp is null!');
		return false;
	}
	$('display_access').style.display = 'none';
	<?if eq 1 $var:mod_ETHWAN
	`	
		get_wan_type();	 //先从页面缓存中读出选择的wan连接方式
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`	
		wan_type = 'DSL';  //Form.Radio('INPUT_WanAcsType') = 0, 选择的是dsl方式
	`?>
	chgHTML();
	chadisplay();
	createProtocolOpt();
	
	if(wan_type == 'DSL'){ // -> DSL
		createConnTypeOpt();
		setJSONValue({
			'SELECT_Protocol': (Isp && Isp['SELECT_Protocol']) || 'PPPoE',
			'SELECT_ConnType': (Isp && Isp['SELECT_ConnType']) || 'LLC',
			'INPUT_VPI'  : (Isp && Isp['INPUT_VPI']) || '0',
			'INPUT_VCI'  : (Isp && Isp['INPUT_VCI']) || '35',
			//ppp
			'INPUT_Username' : (Isp && Isp['INPUT_Username']) || '',
			'INPUT_Psd'  : (Isp && Isp['INPUT_Psd']) || '',
			'INPUT_CfmPsd'   : (Isp && Isp['INPUT_Psd']) || '',
			//ip
			'INPUT_IPAddress_static': (Isp && Isp['INPUT_IPAddress_static']) || '',
			'INPUT_SubMask_static'  : (Isp && Isp['INPUT_SubMask_static']) || '',
			'INPUT_DefaultGy_static': (Isp && Isp['INPUT_DefaultGy_static']) || '',
			'INPUT_PryDnsServer_static' : (Isp && Isp['INPUT_PryDnsServer_static']) || ''
		});
	} 
	else { // -> Internet
		$('isDsl').style.display = 'none';
		setJSONValue({
			'SELECT_Protocol': (Isp && Isp['SELECT_Protocol']) || 'PPPoE',
			//ppp
			'INPUT_Username' : (Isp && Isp['INPUT_Username']) || '',
			'INPUT_Psd'  : (Isp && Isp['INPUT_Psd']) || '',
			'INPUT_CfmPsd'   : (Isp && Isp['INPUT_Psd']) || '',
			//ip
			'INPUT_IPAddress_static': (Isp && Isp['INPUT_IPAddress_static']) || '',
			'INPUT_SubMask_static'  : (Isp && Isp['INPUT_SubMask_static']) || '',
			'INPUT_DefaultGy_static': (Isp && Isp['INPUT_DefaultGy_static']) || '',
			'INPUT_PryDnsServer_static' : (Isp && Isp['INPUT_PryDnsServer_static']) || ''
		});		
	}
	onchgProtocol();
}
function get_wan_type(){
	setJSONValue({
		'INPUT_WanAcsType' :  (Isp && Isp['INPUT_WanAcsType']) || '0'
	});
	if(0 == Form.Radio('INPUT_WanAcsType')){ 
		wan_type = 'DSL';  //Form.Radio('INPUT_WanAcsType') = 0, 选择的是dsl方式
	}
	else{
		wam_type = 'Ethernet' //Form.Radio('INPUT_WanAcsType') = 1, 选择的是LAN上行方式
	}
}
function changetypeSelect(value){
	if(0 == value){
		wan_type = 'DSL';
		$('isDsl').style.display = 'block';
		$('ppp').style.display = 'block';
		$('ip').style.display = 'none';
		createProtocolOpt();
		createConnTypeOpt();
		onchgProtocol();
	}
	else{
		wan_type = 'Ethernet';
		$('isDsl').style.display = 'none';
		$('ppp').style.display = 'none';
		$('ip').style.display = 'none';
		createProtocolOpt();
		onchgProtocol();
	}
}

function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//隐藏主菜勿
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//隐藏二级菜单
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}
function chadisplay(){
	if(wan_type == 'DSL'){
		$('isDsl').style.display = 'block';
		$('ppp').style.display = 'block';
		$('ip').style.display = 'none';
	}
	else{
		$('isDsl').style.display = 'none';
		$('ppp').style.display = 'none';
		$('ip').style.display = 'none';
	}
}
//protocol
function createProtocolOpt(){
	var _text  = ['PPPoE','PPPoA','Dynamic IP','Static IP','Bridge','IPoA'];
	var _value = ['PPPoE','PPPoA','Dynamic IP','Static IP','Bridge','IPoA'];
	
	if(wan_type == 'Ethernet'){
		_text  = ['PPPoE','Dynamic IP','Static IP','Bridge'];
		_value = ['PPPoE','Dynamic IP','Static IP','Bridge'];
	}
	
	$S('SELECT_Protocol', _text, _value);
}

function createConnTypeOpt(){
	var _text  = ['VC-Mux','LLC'];
	var _value = ['VCMUX','LLC'];
	
	$S('SELECT_ConnType', _text, _value);	
}

function onchgProtocol(){
	var MContent = $('Content').offsetHeight;
	var nodes = $('ppp','ip');
	var value_protocol = $('SELECT_Protocol').value;
	var ppp = 'none', ip = 'none';
	
	switch(value_protocol){
		case 'PPPoE' :
		case 'PPPoA' :
			ppp = 'block';
		break;
			case 'Dynamic IP' : 
			break;
		case 'Static IP' : 
			ip = 'block';
			break;
		case 'IPoA' : 
			ip = 'block';
			break;
		case 'Bridge' :
	}
	
	nodes[0].style.display = ppp;
	nodes[1].style.display = ip;
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + nodes[0].offsetHeight + nodes[1].offsetHeight;
		//$('Menu').style.height=ContentHeigh + 'px';
		//$('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}
}

var ajax;

function autoSearchPVC(){
	var dslstatus = '<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Status?>';
	if(dslstatus != 'Up')
	{
		alert("DSL status is still disconnected! Please wait for a moment,after the DSL is OK, refresh the page and try again!");
	    return false;
	}
	$('Scan').disabled = true;
	$('display_access').style.display = 'block';
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.Enable': '1',
		':InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.SearchPVC' : com_pvc,
		'obj-action' : 'set',
		'var:page'   : G_Page,
		'getpage': 'html/page/autopvc_ajax.js',
		'errorpage'  : 'html/page/autopvc_ajax.js'
	});
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_getPvc);
	ajax.post($('uiPostForm'));
}

function ajax_getPvc(_text){
	//成功, 则处理PVC
	try{
		eval(_text);
	} catch (ex){
	}
	
	if (G_Error == '0' && G_FoundPvc != ''){
		$('lang_pvcresult').style.display = 'none',
		$('SPAN_PvcResult').innerHTML = 'Found Available PVC ' + G_FoundPvc + '.';
		//赋便
		$('INPUT_VPI').value = G_FoundPvc.split('/')[0];
		$('INPUT_VCI').value = G_FoundPvc.split('/')[1];
	} else if(G_FoundPvc == ''){
		$('lang_pvcresult').style.display = 'none';
		$('SPAN_PvcResult').innerHTML = data_language.dynamic.no_pvc;
		$('Scan').disabled = false;
		return false;
	}
}

function uiNextPage(){
	if(wan_type == 'DSL')
	{ // -> DSL
		if(($('INPUT_VPI').value < 0) ||($('INPUT_VPI').value > 255))
		{
			alert("Invalid VPI value (0~255)!");
			return false;
		}
		if(($('INPUT_VCI').value < 32) ||($('INPUT_VCI').value > 65535))
		{
			alert("Invalid VCI value (32~65535)!");
			return false;
		}
	}
	if($('SELECT_Protocol').value =="PPPoE" || $('SELECT_Protocol').value =="PPPoA")
	{
		if(!checkValue())
		{
			return false;
		}
	}
	
	var _bool = createJsonData();
	if(!_bool) 
		return false;
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage'  : 'wizwl',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();	
}

function createJsonData(){
	var _json = '{';
	var nodes_isp = $('SELECT_Protocol','SELECT_ConnType','INPUT_VPI','INPUT_VCI',
	//PPPoA/E
	'INPUT_Username','INPUT_Psd',
	//Static IP
	'INPUT_IPAddress_static','INPUT_SubMask_static','INPUT_DefaultGy_static','INPUT_PryDnsServer_static');
	switch(nodes_isp[0].value){
		case 'PPPoA' :
		case 'PPPoE' : 
			_json += 'INPUT_Username:"' + nodes_isp[4].value + '",';
			_json += 'INPUT_Psd:"' + nodes_isp[5].value + '",';
			break;
		case 'Static IP' :
			_json += 'INPUT_IPAddress_static:"' + nodes_isp[6].value + '",';
			_json += 'INPUT_SubMask_static:"' + nodes_isp[7].value + '",';
			_json += 'INPUT_DefaultGy_static:"' + nodes_isp[8].value + '",';
			_json += 'INPUT_PryDnsServer_static:"' + nodes_isp[9].value + '",';
			break;
		case 'IPoA' :
			_json += 'INPUT_IPAddress_static:"' + nodes_isp[6].value + '",';
			_json += 'INPUT_SubMask_static:"' + nodes_isp[7].value + '",';
			_json += 'INPUT_DefaultGy_static:"' + nodes_isp[8].value + '",';
			_json += 'INPUT_PryDnsServer_static:"' + nodes_isp[9].value + '",';
			break;
		case 'Dynamic IP' :
		case 'Bridge' :
	}

	if(wan_type == 'DSL'){
		_json += 'SELECT_ConnType:"' + nodes_isp[1].value + '",';
		_json += 'INPUT_VPI:"' + nodes_isp[2].value + '",';
		_json += 'INPUT_VCI:"' + nodes_isp[3].value + '",';
	}
	_json += 'SELECT_ServiceName:"' + getServiceName() + '",';
	_json += 'SELECT_Protocol:"' + nodes_isp[0].value + '",';
	<?if eq 1 $var:mod_ETHWAN
	`	
		_json += 'INPUT_WanAcsType:"' + Form.Radio('INPUT_WanAcsType') + '",';
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`	
		_json += 'INPUT_WanAcsType:"' + "0" + '",';
	`?>
	//调整JSON数据
	_json = _json.delcomma();
	_json += '}';
	addJsonObj('isp', _json);
	return true;
	}

function getServiceName(){
	var nodes = $('SELECT_Protocol','INPUT_VPI','INPUT_VCI');
	var name = '';
	
	switch(nodes[0].value){
		case 'PPPoE' :
			name = 'pppoe_';
			break;
		case 'PPPoA' :
			name = 'pppoa_';
			break;
		case 'Dynamic IP' : 
			name = 'mer_';
			break;
		case 'Static IP' : 
			name = 'mer_';
			break;
		case 'Bridge' :
			name = 'br_';
			break;
		case 'IPoA' :
			name = 'ipoa_';
			break;
	}
	
	if(wan_type == 'DSL'){ // -> DSL
		name += nodes[1].value + '_' + nodes[2].value + '_' + '0_'+ n+'_Internet';
		return name;
	} else { // -> Ethernet
		name += '0_' + n +'_Internet';
		return name;
	}
}

function uiBack(){
	var _bool = createJsonData();
	if(!_bool)
		return false;
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage' : 'wizsntp',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();
}
	
function checkValue(){
	//check vpi
	if((parseInt($('INPUT_VPI').value)<0) || (parseInt($('INPUT_VPI').value)>255))
	{
		alert('Please enter the VPI with right format !');
		return false;
	}
	
	//check vci
	if((parseInt($('INPUT_VCI').value)<32) || (parseInt($('INPUT_VCI').value)>65535))
	{
		alert('Please enter the VCI with right format !');
		return false;
	}
	
	var node_psd = $('INPUT_Psd','INPUT_CfmPsd');
	var node_type = getServiceName().split('_');
	
	/*for(var i = 0, _type, _len = G_Conns.length; i < _len; i++){
		_type = G_Conns[i][0].split('_');
		if( _type[1] == node_type[1] && _type[2] == node_type[2]){
			alert(SEcode[9999]);
			return false;
		}
	}*/
	
	if(node_psd[0].value != node_psd[1].value){
		alert(SEcode[1010]);
		return false;
	}
	//check username
	if(!checkPswd($('INPUT_Username').value))
	{
		alert('Please enter the username with right format !');
		return false;
	}
	//check password
	if(!checkPswd(node_psd[0].value))
	{
		alert('Please enter the password with right format !');
		return false;
	}
	
	return true;
}

//check password or username
function checkPswd(x)
{
	var patrn=/^[a-zA-Z0-9]{1}([a-zA-Z0-9]|[_]|[%]|[@]|[/]|[*]|[.]|[?]|[-]){0,62}$/;
	if (!patrn.exec(x))
		return false;
	else
		return true;
}

function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();
}

function dealWithError(){

}

addListeners(uiOnload, dealWithError);