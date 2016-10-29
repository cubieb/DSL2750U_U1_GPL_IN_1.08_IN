//MAC Filter

var G_MacBlackList = [];
var G_BlackListNum = 0;
var G_FilterMode = "<?get :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.FilterMode ?>";

<?objget :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.BlackList. "MACAddress SchedulePath Description Enable"
`	G_MacBlackList[G_BlackListNum] = [];
	G_MacBlackList[G_BlackListNum][0] = "InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.BlackList.$00.";  				//index
	G_MacBlackList[G_BlackListNum][1] = "$01";		//MAC
	G_MacBlackList[G_BlackListNum][2] = "$02";		//SchedulePath
	G_MacBlackList[G_BlackListNum][3] = "$03";		//UserName
	G_MacBlackList[G_BlackListNum][4] = "$04";		//UserName
	G_BlackListNum++;
`?>

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

var LanHosts = [];               
var m = 0;
<?objget :InternetGatewayDevice.LANDevice. "Hosts.HostNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.LANDevice.$20.Hosts.Host. "IPAddress MACAddress HostName"
		`	LanHosts[m] = [];
			LanHosts[m][0] = "$01";
			LanHosts[m][1] = "$02";
			LanHosts[m][2] = "$03";
			++m;
		`?>
	`?>
`?>

var G_CurrentIP = "<?echo $var:sys_RemoteAddr ?>";
var G_CurrentMAC = GetMACByIP(G_CurrentIP);
/* Get Mac By IP */
function GetMACByIP(ip)
{
	for (var i=0; i<LanHosts.length; i++)
	{
		if (LanHosts[i][0] == ip)
		{
			return LanHosts[i][1];
		}
	}
	return "";
}

function enableRule(index){
	if($("rule_index" + index).checked == true){
		$("rule_mac" + index).disabled = false;
		$("copy_mac" + index).disabled = false;
		$("SELECT_Mac" + index).disabled = false;
		$("SELECT_Schedules" + index).disabled = false;
	}
	else{
		$("rule_mac" + index).disabled = true;
		$("copy_mac" + index).disabled = true;
		$("SELECT_Mac" + index).disabled = true;
		$("SELECT_Schedules" + index).disabled = true;
	}
}

function copyMac(index){
	if($("SELECT_Mac" + index).value == "computers"){
		alert(data_language.dynamic.cpmacerror);
		return false;
	}
	$("rule_mac" + index).value = $("SELECT_Mac" + index).value;
}

function createTable(){
	//创建MAC Filter表格
	var array_MACAddrs = [];
	for(var i = 0; i < G_MacBlackList.length; i++){
		array_MACAddrs[i] = [];
		array_MACAddrs[i].push(i + 1);
		array_MACAddrs[i].push('<input type="checkbox" id="rule_index' +i+ '" name="rule_index' +i+ '" ' + (G_MacBlackList[i][4]=='1' ? "checked":"") + ' onclick="enableRule(' +i+ ')">'); //CheckBox
		array_MACAddrs[i].push('<input type="text" size="18" maxlength="17" id="rule_mac' +i+ '" name="rule_mac' +i+ '" value="' + G_MacBlackList[i][1] + '">');	//MACAddress
		array_MACAddrs[i].push('<input type="button" id="copy_mac' +i+ '" name="copy_mac' +i+ '" value="<<" onclick="copyMac(' +i+ ')">');			// << button
		array_MACAddrs[i].push('<select id="SELECT_Mac' +i+ '" name="SELECT_Mac' +i+ '"></select>');    //Dhcp client list
		array_MACAddrs[i].push('<select id="SELECT_Schedules' +i+ '" name="SELECT_Schedules' +i+ '"></select>');    //Schedules
	}
	$T('td_macAddrs',array_MACAddrs);

	var mac_text = [], mac_value = [], schedule_text = [], schedule_value = [];
	mac_text.push(data_language.dynamic.pc_list);
	mac_value.push('computers');
	for(var i = 0; i < LanHosts.length; i++){
		var tmpText = LanHosts[i][2] + '(' + LanHosts[i][1] + ')';
		if (tmpText.length > 25){
			tmpText = tmpText.substr(0, 22) + '...';    //The web page will not show properly when this string is too long
		}
		mac_text.push(tmpText);
		mac_value.push(LanHosts[i][1]);
	}
	schedule_text.push(data_language.dynamic.always);
	schedule_value.push('Always');
	for(var i = 0; i < G_SchedEntry.length; i++){
		schedule_text.push(G_SchedEntry[i][1]);
		schedule_value.push(G_SchedEntry[i][0]);
	}
	schedule_text.push(data_language.dynamic.never);
	schedule_value.push('Never');

	//添加MAC和时间表下拉列表框
	var macId, scheduleId;
	for(var i = 0; i < G_MacBlackList.length; i++){
		macId = "SELECT_Mac" + i;
		scheduleId = "SELECT_Schedules" + i;
		$S(macId, mac_text, mac_value);
		$S(scheduleId, schedule_text, schedule_value);
	}

	//初始化MAC Filter表格控件属性
	for(var i = 0; i < G_MacBlackList.length; i++){
		enableRule(i);	
		if (G_MacBlackList[i][2] != "")
			$("SELECT_Schedules" + i).value = G_MacBlackList[i][2];
	}
}


function uiOnload(){
	
	Table.Clear('td_macAddrs');
	
	setJSONValue({
		'lang_note_mac_value'  : G_CurrentMAC
	});
	
	createTable();
	
	$('SELECT_block_mode').value = G_FilterMode;
	
	ChangeFilterMode();
}

function uiAddSchedule(){
    top.location = "/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=schedules";
}
function display_table(){
	for(var i = 0; i < G_MacBlackList.length; i++){
		if(G_MacBlackList[i][4]=='1'){
			$("rule_mac" + i).disabled = false;
			$("copy_mac" + i).disabled = false;
			$("SELECT_Mac" + i).disabled = false;
			$("SELECT_Schedules" + i).disabled = false;		
		}
		else{
			$("rule_mac" + i).disabled = true;
			$("copy_mac" + i).disabled = true;
			$("SELECT_Mac" + i).disabled = true;
			$("SELECT_Schedules" + i).disabled = true;
		}
	}
}
function ChangeFilterMode(){
	var disable_tmp;
	G_FilterMode = $('SELECT_block_mode').value;
	if(G_FilterMode == 'Disable'){
		disCtrl('display_control',false);
	}
	else{
		disCtrl('display_control',true);
		display_table();
	}
}

function uiApply(){
	var pc_mac = 0;
	var FilterMode = $('SELECT_block_mode').value;
	$H({
		'obj-action'		: 'set',
		'var:menu'			: 'advanced',
		'var:page'			: 'parent_control',
		'var:subpage'		: 'block_macaddress_dl',
		'var:errorpage'		: 'block_macaddress_dl',
		'getpage'			: 'html/index.html',
		'errorpage'			: 'html/index.html',
		'var:url_belong'	: '0',
		'var:CacheLastData'	: ViewState.Save()
	},true);
	$F(':InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.FilterMode', G_FilterMode);
	if(FilterMode == 'Allow')
	{
		for(var i = 0; i < G_MacBlackList.length; i++){
			if(G_CurrentMAC == $("rule_mac" + i).value)
			{
				var pc_mac = 1;
			}
		}
		if(0 == pc_mac)
		{
			var truthBeTold = window.confirm(data_language.dynamic.confirmMac);
			if (truthBeTold) { 
			} else{
				return;
			}
		}
	}
	for(var i = 0; i < G_MacBlackList.length; i++){
		var num = i + 1;
		if($("rule_index" + i).checked == true){
			if($("rule_mac" + i).value == ""){
				alert(data_language.dynamic.selectRule1 + num + data_language.dynamic.selectRule2);
				//alert("Selected Mac field is empty!");
				
				$("rule_mac" + i).focus();
				return;
			}
			$F(':'+G_MacBlackList[i][0]+'Enable', "1");
			$F(':'+G_MacBlackList[i][0]+'MACAddress', $("rule_mac" + i).value);
			$F(':'+G_MacBlackList[i][0]+'SchedulePath', $("SELECT_Schedules" + i).value);
			var hostName;
			for(var j = 0; j < LanHosts.length; j++){
				if(LanHosts[j][1] == $("rule_mac" + i).value)
					hostName = LanHosts[j][2];
			}
			$F(':'+G_MacBlackList[i][0]+'Description', hostName);
		}
		else if($("rule_index" + i).checked == false  && G_MacBlackList[i][4] == 1){
			$F(':'+G_MacBlackList[i][0]+'Enable', "0");
		}
	}

	$('uiPostForm').submit();
}

function dealWithError(){
         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

