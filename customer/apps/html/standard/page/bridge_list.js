///
var AllowEnable = "<?get :InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeRule ?>";
var ajax;
//DMZ模块

var G_ATM = [];
var m = 0;
var G_InternetWanType = "Ethernet";
<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?if eq `DSL` `<?echo $21?>`
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS"
		`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02",//DestinationAddress
				"$03",//ATMEncapsulation
				"$04" //ATMQoS
				];
			m++;
		`?>
		G_InternetWanType = "DSL";
	`?>
?>

var G_Dmz = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ConnectedDeviceName"
		`	G_Dmz[m] = [];
			G_Dmz[m][0] = "$01";   // Name
			G_Dmz[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.";   // Path
			G_Dmz[m][2] = "$02"
			G_Dmz[m][3] = "$03"
			m++;
		`?>
	`?>
`?>
`?>

var G_time_schedule = [];

//用来记录当前的Wan连接
var wanConIndex = '<?echo $var:wanConIndex?>';

var bridge_filter = new Array();
var n = 0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList. "BridgeRuleWan ProtocolType DstMac SrcMac LanDirection TimeSchedule UsrPriority VlanId"
`	bridge_filter[n] = new Array();
	bridge_filter[n][0] = "$01"; //BridgeRuleWan
	bridge_filter[n][1] = "$02"; //ProtocolType
	bridge_filter[n][2] = "$03"; //DstMac
	bridge_filter[n][3] = "$04"; //SrcMac
	bridge_filter[n][4] = "$05"; //LanDirection
	bridge_filter[n][5] = "$00."; //instance
	bridge_filter[n][6] = "$06"; //TimeSchedule
    bridge_filter[n][7] = "$07"; //UserPriority
    bridge_filter[n][8] = "$08"; // VlanId
	n++;
`?>	

/**********************           for time schedule *****************************/
var EntryNum=0;
var G_SchedEntry=[];
<?if lt 0 `<?get :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedNumberOfEntries ?>`
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry. "SchedName"
		`	G_SchedEntry[EntryNum] = [];
			G_SchedEntry[EntryNum][0] = "InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry.$00.";    //path
			G_SchedEntry[EntryNum][1] = "$01";   	//ScheduleName
			++EntryNum;
	`?>
`?>


function DeleteClick()
{
	disCtrl("select_mode", false);
	disCtrl("apply_rule", false);
    var flag=0;
	if (bridge_filter.length == 0)
	{
		disCtrl("select_mode", true);
		disCtrl("apply_rule", true);
		alert(SEcode[9888]);
		return;
	}
	if(!confirm(SEcode[1008])){
		disCtrl("select_mode", true);
		disCtrl("apply_rule", true);
		return false;
	}
	for (var i = 0; i < bridge_filter.length; i++) {
			var tmpid = "choose" + i;
			if (Form.Checkbox(tmpid) == 1) {
				    flag=1;
					var path = 'InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.' + bridge_filter[i][5];
					$H({
					'del-obj'    : path ,
					'obj-action' : 'del',
					'var:menu'   : 'advanced',
					'var:page'   : 'filtering_options',
					'getpage'    : 'html/index.html',
					'var:subpage'   :'bridge_list',
					'errorpage'  : 'html/index.html',
					'var:errorpage' : 'bridge_list',
					'var:CacheLastData' : ViewState.Save()
			});
			$('uiPostForm').submit();
		 }
	}
	if(flag==0){
	    alert(SEcode[1009]);
		}
}

var edit_apply = -1;
function EditClick()
{
	if (bridge_filter.length == 0)
	{
		alert(SEcode[9888]);
		return;
	}
	$('apply_rule').style.display = 'none';
    var MContent = $('Content').offsetHeight;
	$('aa').style.display = 'block';
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('aa').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
	for (var i = 0; i < bridge_filter.length; i++) {
        var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) {
			Form.Select("select_wan",bridge_filter[i][0]);
			Form.Select("protocol", bridge_filter[i][1]);
			$("destMac").value = bridge_filter[i][2];
			$("srcMac").value = bridge_filter[i][3];
			Form.Select("direction",bridge_filter[i][4]);
			Form.Select("time_schedule", bridge_filter[i][6]);
            $("userPriority").value = bridge_filter[i][7];
            $("vlanID").value = bridge_filter[i][8];
			edit_apply = i;
			protocolChange(bridge_filter[i][1]);
			return;
		}
	}
}

//将Path转换为Schedule的Name
function transform_time(Path){
	if(Path =='Always')
		return 'Always';
	if(Path =='Never')
		return 'Never';
	for(var i = 0; i < G_SchedEntry.length; i++){
		if(G_SchedEntry[i][0] == Path){
			return G_SchedEntry[i][1];
		}
	}
   return 'Always';
}
/*****************************************************************************/

function load_time()
{
	var _text = [], _value = [];
	_text.push(data_language.dynamic.always);
	_value.push("Always");
	_text.push(data_language.dynamic.never);
	_value.push("Never");
	for(var i = 0; i < G_SchedEntry.length; i++){
		_text.push(G_SchedEntry[i][1]);
		_value.push(G_SchedEntry[i][0]);
	}
	$S('time_schedule',_text,_value);

}

function load_wan()
{
	var _text = [], _value = [];
	_text.push(data_language.dynamic.select_all_wan);
	_value.push("select_all");
	for(var i = 0; i < G_Dmz.length; i++){
		if (G_Dmz[i][2] == 'IP_Bridged') {
			_text.push(G_Dmz[i][0]);
			_value.push(G_Dmz[i][1]);
		}
	}
	$S('select_wan',_text,_value);
	$('select_wan').value = 'select_all';
}

function protocolChange(proc)
{
    if (proc == '8') //802.1Q
    {
       $('vlanID').disabled = false;
       $('userPriority').disabled = false;
    }
    else
    {
       $('vlanID').disabled = true;
       $('userPriority').disabled = true;
    }   
}

function uiOnload(){
	Form.Radio('macFltGP', AllowEnable);
	$('aa').style.display = 'none';
	load_wan();
	load_time();
	if (G_InternetWanType == "DSL")
		createTable();
	else
		createTable_Ethnet();

    protocolChange($('protocol').value);
}
	
function transform_protocol()
{
	var proto = arguments[0];
	switch (proto) {
		case "1" :
			return "PPPoE";
		case "2" :
			return "IPv4";
		case "3" :
			return "IPv6" ;
		case "4" :
			return "AppleTalk" ;
		case "5" :
			return "IPX" ;
		case "6" :
			return "NetBEUI" ;
		case "7" :
			return "IGMP"
        case "8" :
            return "802.1Q"
	}
	return "NONE";
}

function transform_dir()
{
	var dir = arguments[0];
	switch(dir) {
		case "2" :
			return "LAN=&gt;WAN";
		case "1" :
			return "WAN=&gt;LAN";
	}
	return "WAN=&gt;LAN";
}




function transform_vpi()
{
	var path_wan = arguments[0];
	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return G_ATM[i][2];
		}
	}
	return "ALL";
}

function createTable(){
		Table.Clear('BridgeList');
		var array_value = [];
		for(var i = 0; i < bridge_filter.length; i++){
			array_value[i] = [];
			
			if(i==0){
				array_value[i].push('<input type="radio" id="choose'+i+'" name="choose" value="'+i+'" checked>');
			}
			else
			{
				array_value[i].push('<input type="radio" id="choose'+i+'" name="choose" value="'+i+'">');
			}
			
			array_value[i].push(transform_vpi(bridge_filter[i][0]));
			array_value[i].push(transform_protocol(bridge_filter[i][1]));
			array_value[i].push(bridge_filter[i][2]);
			array_value[i].push(bridge_filter[i][3]);
            array_value[i].push(bridge_filter[i][7]);
			array_value[i].push(bridge_filter[i][8]);
			array_value[i].push(transform_dir(bridge_filter[i][4]));
			array_value[i].push(transform_time(bridge_filter[i][6]));
		}
		$T('BridgeList',array_value);
	}
	
function createTable_Ethnet(){
		Table.Clear('BridgeList');
		var array_value = [];
		for(var i = 0; i < bridge_filter.length; i++){
			array_value[i] = [];
			
			if(i==0){
				array_value[i].push('<input type="radio" id="choose'+i+'" name="choose" value="'+i+'" checked>');
			}
			else
			{
				array_value[i].push('<input type="radio" id="choose'+i+'" name="choose" value="'+i+'">');
			}
			
			array_value[i].push("N/A");
			array_value[i].push(transform_protocol(bridge_filter[i][1]));
			array_value[i].push(bridge_filter[i][2]);
			array_value[i].push(bridge_filter[i][3]);
            array_value[i].push(bridge_filter[i][7]);
			array_value[i].push(bridge_filter[i][8]);
			array_value[i].push(transform_dir(bridge_filter[i][4]));
			array_value[i].push(transform_time(bridge_filter[i][6]));
		}
		$T('BridgeList',array_value);
}

function addClick(){
	$('apply_rule').style.display = 'none';
	
	var MContent = $('Content').offsetHeight;
	$('aa').style.display = 'block';
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('aa').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}	
}

function AddApply(){
	
	disCtrl("select_mode",false);
	disCtrl("set_value",false);
	
	var wanConn = $('select_wan').value;
	var path = '';
	var _text = [];	
	
	if(wanConn == 'select_all'){
		for(var i = 0; i < G_Dmz.length; i++){
			if (G_Dmz[i][2] == 'IP_Bridged') {
				_text.push(G_Dmz[i][0]);
			}
		}
		if(_text == ''){
			alert(SEcode[9886]);
			disCtrl("select_mode",true);
			$('apply_rule').style.display = 'block';
			return false;
		}
	}

    if (edit_apply == -1) {
        $H({
	        'add-obj'                        : 'InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.',
            'obj-action'                     : 'add-set',
            'var:menu'                       : 'advanced',
      	    'var:page'                       : 'filtering_options',
            'var:subpage'                    : 'bridge_list',	   
            'var:errorpage'                  : 'bridge_list',
            'getpage'                        : 'html/index.html',
            'errorpage'                      : 'html/index.html',
            'var:wanConIndex'                : wanConn,
            'var:CacheLastData'              : ViewState.Save()
        })
        path = ':';
	}
	else {
        $H({
	        'add-obj'                        : 'InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.' + bridge_filter[edit_apply][5],
            'obj-action'                     : 'set',
            'var:menu'                       : 'advanced',
      	    'var:page'                       : 'filtering_options',
            'var:subpage'                    : 'bridge_list',	   
            'var:errorpage'                  : 'bridge_list',
            'getpage'                        : 'html/index.html',
            'errorpage'                      : 'html/index.html',
            'var:wanConIndex'                : wanConn,
            'var:CacheLastData'              : ViewState.Save()
        })
        path = ':InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.' + bridge_filter[edit_apply][5];
    }
    $F(path+'ProtocolType'      , $('protocol').value );
    $F(path+'DstMac'            , $('destMac').value );
    $F(path+'SrcMac'            , $('srcMac').value );
    $F(path+'LanDirection'      , $('direction').value );
    $F(path+'BridgeRuleWan'     , $('select_wan').value );
    $F(path+'TimeSchedule'      , $('time_schedule').value );
    $F(path+'UsrPriority'       , $('userPriority').value );
    $F(path+'VlanId'            , $('vlanID').value );

	$('uiPostForm').submit();
	
/*
	if (edit_apply == -1) {
		$H({
	   'add-obj' : 'InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.',
	   ':ProtocolType'    : $('protocol').value,
	   ':DstMac'          : $('destMac').value,
	   ':SrcMac'          : $('srcMac').value,
	   ':LanDirection'    : $('direction').value,
	   ':BridgeRuleWan'   : $('select_wan').value,
	   ':TimeSchedule'    : $('time_schedule').value,
	   'var:menu'         :'advanced',
	   'var:page'         :'filtering_options',
	   'var:subpage'      :'bridge_list',
	   'obj-action'       : 'add-set',
	   'var:errorpage'    :'bridge_list',
	   'getpage'          : 'html/index.html',
	   'errorpage'        : 'html/index.html',
	   'var:wanConIndex'  : wanConn,
	   'var:CacheLastData': ViewState.Save()
	   });
		$('uiPostForm').submit();
	}
	else {
		$H({
	   'var:menu'         :'advanced',
	   'var:page'         :'filtering_options',
	   'obj-action'       : 'set',
	   'var:errorpage'    :'bridge_list',
	   'var:subpage'      :'bridge_list', 
	   'getpage'          : 'html/index.html',
	   'errorpage'        : 'html/index.html',
	   'var:wanConIndex'  : wanConn,
	   'var:CacheLastData': ViewState.Save()
		},true);
		path = ':InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.' + bridge_filter[edit_apply][5];
		$F(path + 'ProtocolType',$('protocol').value);
		$F(path + 'DstMac',$('destMac').value);
		$F(path + 'SrcMac',$('srcMac').value);
		$F(path + 'LanDirection',$('direction').value);
		$F(path + 'BridgeRuleWan',$('select_wan').value);
		$F(path + 'TimeSchedule', $('time_schedule').value);
		$F(path + 'BridgeRuleWan', $('select_wan').value);
		$('uiPostForm').submit();
	}
*/
}

function MacModeApply() {
	
	disCtrl("select_mode",false);
	disCtrl("apply_rule",false);
	disCtrl("set_value",false);
	
	var MacMode = $('macFltGP1', 'macFltGP2');
	var MacStr;
	
	for (var i = 0; i < 2; i++) {
		if (MacMode[i].checked == true) {
			MacStr = MacMode[i].value;	
		}
	}

	$H({
		':InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeRule' : MacStr,
		'getpage'       : 'html/page/wiz_ajax.js',
		'var:page'      : 'bridge_list',
		'obj-action'    : 'set'
	}, true);

	if(MacStr != AllowEnable){
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, uiAjax_del);
		ajax.post($('uiPostForm'));	
	}
	else{
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, ajax_SetProc);
		ajax.post($('uiPostForm'));
	}
}

function ajax_SetProc(_text){
	//成功, 则set
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		disCtrl("select_mode",true);
		disCtrl("apply_rule",true);
		disCtrl("set_value",true);
		return false;
	}
	
	if (G_Error == '0'){
		$G('/cgi-bin/webproc',{
			'var:menu'			: 'advanced',
			'var:page'			: 'bridge_list',
			'getpage'			: 'html/index.html',
			'errorpage'			: 'html/index.html'
		});	
	} else if (G_Error == '1'){
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=bridge_list';
	}
}

function uiAjax_del(){
	if (bridge_filter.length == 0) 
	{
		disCtrl("select_mode",true);
		disCtrl("apply_rule",true);
		disCtrl("set_value",true);
		return;
	}
		for (var i = 0; i < bridge_filter.length; i++) {
			var pathex = 'InternetGatewayDevice.X_TWSZ-COM_BridgeFilter.BridgeFilterList.' + bridge_filter[i][5];
			$H({
			'del-obj'    : pathex,
			'obj-action' : 'del',
			'var:page'   : 'bridge_list'
			});
			
			var _url = "/cgi-bin/webproc";
			ajax = Ajax.getInstance(_url, "", 0, ajax_SetProc);
			ajax.post($('uiPostForm'));	
		 }		
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
  }
  
  
addListeners(uiOnload, dealWithError);
