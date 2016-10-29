//MAC Filter


var array_MACAddrs = [];

var G_MacBlackList = [];
var G_BlackListNum = 0;
var array_MACAddrs ;
var G_ListNumber;
var addAction=0;
var editIdx=0;

var array_PassMACAddrs = [];
var G_MacWhiteList = [];
var G_WhiteListNum = 0;

var G_FilterMode = "<?get :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.FilterMode?>";

//增加自己的checkbox id 判断因为全局的_array_id不方便用在两组checkbox中
var white_array_id = [];

function WhiteGetOrDelArrayIdx(_value){
	if(!white_array_id.remove(_value)){
		white_array_id.push(_value);
	}
}



<?objget :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.BlackList. "MACAddress ScheduleMode SchedulePath EnableAllDays Days EnableAllTimes StartTime EndTime Description"
`	G_MacBlackList[G_BlackListNum] = [];
	G_MacBlackList[G_BlackListNum][0] = "InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.BlackList.$00.";  				//index
	G_MacBlackList[G_BlackListNum][1] = "$01";		//MAC
	G_MacBlackList[G_BlackListNum][2] = "$02";		//ScheduleMode	
	G_MacBlackList[G_BlackListNum][3] = "$03";		//SchedulePath
	G_MacBlackList[G_BlackListNum][4] = "$04";		//EnableAllDays
	G_MacBlackList[G_BlackListNum][5] = "$05";		//Days
	G_MacBlackList[G_BlackListNum][6] = "$06";		//EnableAllTimes
	G_MacBlackList[G_BlackListNum][7] = "$07";		//StartTime
	G_MacBlackList[G_BlackListNum][8] = "$08";		//EndTime
	G_MacBlackList[G_BlackListNum][9] = "$09";		//UserName
	G_BlackListNum++;
`?>

<?objget :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.WhiteList. "MACAddress ScheduleMode SchedulePath EnableAllDays Days EnableAllTimes StartTime EndTime Description"
`	G_MacWhiteList[G_WhiteListNum] = [];
	G_MacWhiteList[G_WhiteListNum][0] = "InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.WhiteList.$00.";  				//index
	G_MacWhiteList[G_WhiteListNum][1] = "$01";		//MAC
	G_MacWhiteList[G_WhiteListNum][2] = "$02";		//ScheduleMode	
	G_MacWhiteList[G_WhiteListNum][3] = "$03";		//SchedulePath
	G_MacWhiteList[G_WhiteListNum][4] = "$04";		//EnableAllDays
	G_MacWhiteList[G_WhiteListNum][5] = "$05";		//Days
	G_MacWhiteList[G_WhiteListNum][6] = "$06";		//EnableAllTimes
	G_MacWhiteList[G_WhiteListNum][7] = "$07";		//StartTime
	G_MacWhiteList[G_WhiteListNum][8] = "$08";		//EndTime
	G_MacWhiteList[G_WhiteListNum][9] = "$09";		//UserName
	G_WhiteListNum++;
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
	`	<?objget :InternetGatewayDevice.LANDevice.$20.Hosts.Host. "IPAddress MACAddress"
		`	LanHosts[m] = [];
			LanHosts[m][0] = "$01";
			LanHosts[m][1] = "$02";
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

//灏哖ath鐨勮矾寰勮浆鎹负Schedule鐨凬ame
function searchPath(Path){
	if(Path =='Always')
		return 'Always';
	if(Path =='Never')
		return 'Never';
	for(var i = 0; i < G_SchedEntry.length; i++){
		if(G_SchedEntry[i][0] == Path){
			return G_SchedEntry[i][1];
		}
	}
}

function uiRadioSubmit()
{
	//disCtrl('button_group',false);
	
	$H({
		'obj-action'        : 'set',
		'var:menu'			: G_Menu,
		'var:page'			: G_Page,
		'var:subpage'		: G_SubPage,
		'var:errorpage'		: G_SubPage,
		'getpage'    		: 'html/index.html',
		'errorpage'  		: 'html/index.html',
		'var:url_belong'	: '0',
		'var:CacheLastData' : ViewState.Save()
	},true);

	var path=':InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.FilterMode';		
	$F(path,G_FilterMode);

    if ((G_FilterMode == 'Allow') && (G_MacWhiteList.length == 0))
    {
        alert(SEcode[9887]);
        return 0;
    }
        		
	$('uiPostForm').submit();

}


function onRadioClick()
{	
	if($('mac_black_radio').checked)
	{
		$('white_block').style.display = 'none';
		$('black_block').style.display = 'block';
		$('button_group').style.display = 'block';

		G_FilterMode = 'Deny';
	}	
	if($('mac_white_radio').checked)
	{
		$('black_block').style.display = 'none';
		$('white_block').style.display = 'block';
		$('button_group').style.display = 'block';

		G_FilterMode = 'Allow';
	}
}

function createTable(){	
	var array_MACAddrs = [];
	for(var i = 0; i < G_MacBlackList.length; i++){
		array_MACAddrs[i] = [];
		array_MACAddrs[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">'); 
		array_MACAddrs[i].push(G_MacBlackList[i][9].ellipsis(8));						//UserName
		array_MACAddrs[i].push(G_MacBlackList[i][1]);						//MACAddress
		if(G_MacBlackList[i][2]=='Select')
			array_MACAddrs[i].push(searchPath(G_MacBlackList[i][3]));		//Schedule
		else
        {
			var shed_list=0;
			shed_list=G_MacBlackList[i][5]+' time '+G_MacBlackList[i][7]+' '+G_MacBlackList[i][8];
			array_MACAddrs[i].push(shed_list);	
		}	
	}
	$T('td_macAddrs',array_MACAddrs);	

	var array_PassMACAddrs = [];
	for(var i = 0; i < G_MacWhiteList.length; i++){
		array_PassMACAddrs[i] = [];
		array_PassMACAddrs[i].push('<input type="checkbox" id="rule_index_white'+i+'" name="rule_index'+i+'" onclick="WhiteGetOrDelArrayIdx(' + i + ')">'); 
		array_PassMACAddrs[i].push(G_MacWhiteList[i][9].ellipsis(8));						//UserName
		array_PassMACAddrs[i].push(G_MacWhiteList[i][1]);						//MACAddress
		if(G_MacWhiteList[i][2]=='Select')
			array_PassMACAddrs[i].push(searchPath(G_MacWhiteList[i][3]));		//Schedule
		else{
			var shed_list=0;
			shed_list=G_MacWhiteList[i][5]+' time '+G_MacWhiteList[i][7]+' '+G_MacWhiteList[i][8];
			array_PassMACAddrs[i].push(shed_list);	
		}	
	}
	$T('td_passmacAddrs',array_PassMACAddrs);	
	for(var b = 0; b < G_MacBlackList.length; b++){
	$('td_macAddrs_' + b + '1').title = G_MacBlackList[b][9];
	
	}

	for(var i = 0; i < G_MacWhiteList.length; i++){
	$('td_passmacAddrs_' + i + '1').title = G_MacWhiteList[i][9];
	
	}
	
}

function uiOnload(){
	//鐢熸垚URL鍦板潃鍒楄〃
	Table.Clear('td_macAddrs');
	
	disCtrl('button_group',true);

	if(G_FilterMode  == "Deny")
	{
		$('mac_black_radio').checked = true;
		
		$('white_block').style.display = 'none';
		
	}
	else if(G_FilterMode == "Allow")
	{
		
		$('mac_white_radio').checked = true;
		$('black_block').style.display = 'none';
	}
	
	createTable();
// wang start 	
	//Schedule list
	var Sched_text = [], Sched_value = [];
	Sched_text.push(data_language.dynamic.always);
	Sched_value.push('Always');

	for(var j = 0; j < G_SchedEntry.length; j++){
		//Schedule
		Sched_text.push(G_SchedEntry[j][1]);
		Sched_value.push(G_SchedEntry[j][0]);
	}
	Sched_text.push(data_language.dynamic.never);
	Sched_value.push('Never');
	$S('SELECT_Schedules',Sched_text,Sched_value);
	$('RADIO_OtherMacAddress').checked=true;

	
// wang end	
    CancelEdit();
	
}

function setWholeDay(){
	if($('EnableAllTimes').checked){
		$('startTimeH').value="00";
		$('endTimeH').value="23";
		$('startTimeM').value="00";
		$('endTimeM').value="59";
		$('startTimeH').disabled=true;
		$('endTimeH').disabled=true;
		$('startTimeM').disabled=true;
		$('endTimeM').disabled=true;
	}else{
		$('startTimeH').value="";
		$('endTimeH').value="";
		$('startTimeM').value="";
		$('endTimeM').value="";
		$('startTimeH').disabled=false;
		$('endTimeH').disabled=false;
		$('startTimeM').disabled=false;
		$('endTimeM').disabled=false;
	}
	return true;	
}


function uiSubmit(){	
	var _nodes = $('startTimeH','startTimeM','endTimeH','endTimeM');
	var starttime=_nodes[0].value+':'+_nodes[1].value;
	var endtime=_nodes[2].value+':'+_nodes[3].value;
	var selectdays='';
	if(Form.Checkbox('sunday')=='1')
	     selectdays='Sun,';
	if(Form.Checkbox('monday')=='1')
	     selectdays+='Mon,';
	if(Form.Checkbox('tuesday')=='1')
	     selectdays+='Tue,';
	if(Form.Checkbox('wednesday')=='1')
	     selectdays+='Wed,';
	if(Form.Checkbox('thursday')=='1')
	     selectdays+='Thu,';
	if(Form.Checkbox('friday')=='1')
	     selectdays+='Fri,';
	if(Form.Checkbox('saturday')=='1')
	     selectdays+='Sat,';

	var MACAddress =0;

    if($('RADIO_CurrentPcAddress').checked){
	    var MACAddress=$('INPUT_CurrentPcAddress').value;
	}else{
		var MACAddress=$('INPUT_OtherMacAddress').value;
	}

	var ScheduleMode=0;
	ScheduleMode=Form.Radio('RADIO_ScheduleMode');
	if(addAction=='1')
    {
		$H({
			'add-obj' 	 	: G_FilterMode=='Deny'? 'InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.BlackList.' : 'InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.WhiteList.',
			':Description'       	: $('INPUT_UserName').value,
			':MACAddress'       : MACAddress,
			':ScheduleMode' 	: Form.Radio('RADIO_ScheduleMode')=='Select'?'Select':'Manual',
			':SchedulePath'		: Form.Radio('RADIO_ScheduleMode')=='Select'? $('SELECT_Schedules').value : undefined,
			':EnableAllDays'   	: ScheduleMode=='Manual' ? Form.Radio('EnableAllDays')=='1'?1:0 :undefined ,
			':Days'      		: ScheduleMode=='Manual' ? selectdays : undefined,
			':EnableAllTimes'   : ScheduleMode=='Manual' ? Form.Checkbox('EnableAllTimes'): undefined,
			':StartTime' 		: ScheduleMode=='Manual' ? starttime : undefined,
			':EndTime'   		: ScheduleMode=='Manual' ? endtime : undefined,
			'obj-action' 		: 'add-set',
			'var:menu'			: G_Menu,
			'var:page'			: G_Page,
			'var:subpage'		: G_SubPage,
			'var:errorpage'		: G_SubPage,
			'getpage'    		: 'html/index.html',
			'errorpage'  		: 'html/index.html',
			'var:url_belong'	: '0',
			'var:CacheLastData' : ViewState.Save()
		},true);
	}
    else if(addAction=='2')
	{		
		$H({
			'obj-action'        : 'set',
			'var:menu'			: G_Menu,
			'var:page'			: G_Page,
			'var:subpage'		: G_SubPage,
			'var:errorpage'		: G_SubPage,
			'getpage'    		: 'html/index.html',
			'errorpage'  		: 'html/index.html',
			'var:url_belong'	: '0',
			'var:CacheLastData' : ViewState.Save()
		},true);
        
		var pathTemp = G_FilterMode=='Deny' ? G_MacBlackList[editIdx][0] : G_MacWhiteList[editIdx][0];
		var path=':' + pathTemp;
		$F(path+'Description',$('INPUT_UserName').value);
		$F(path+'MACAddress',MACAddress);
		if(Form.Radio('RADIO_ScheduleMode')=='Select')
        {
			$F(path+'ScheduleMode','Select');
			$F(path+'SchedulePath',$('SELECT_Schedules').value);
		}
        else
        {
			$F(path+'ScheduleMode','Manual');
			$F(path+'EnableAllDays',Form.Radio('EnableAllDays')=='1'?1:0);
			$F(path+'Days',selectdays);
			$F(path+'EnableAllTimes',Form.Checkbox('EnableAllTimes'));
			$F(path+'StartTime',starttime);
			$F(path+'EndTime',endtime);
		}
	}
    
	$('uiPostForm').submit();
}

function AddEntry(){
	var MContent = $('Content').offsetHeight;

	$('mac_white_radio').disabled = true;
	$('mac_black_radio').disabled = true;
	addAction = 1;
    $('addEntry').disabled=true;
	$('editEntry').disabled=true;
	$('removeEntry').disabled=true;
	$('EntryInfo').style.display='block';
	$('INPUT_CurrentPcAddress').disabled=true;
	$('INPUT_CurrentPcAddress').value=G_CurrentMAC;
	changeScheduleMode();
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
}


function EditEntry(){
	var MContent = $('Content').offsetHeight;

	
	$('mac_white_radio').disabled = true;
	$('mac_black_radio').disabled = true;

     	if(white_array_id.length == 0 && _array_id.length == 0){
		alert(SEcode[9888]);
		return false;
	}	
	
     	if(white_array_id.length + _array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	}
	Heigtduiqi();	
	EditEntryWhitelist();
	for(var j = 0; j < G_MacBlackList.length; j++){
		var EditIndexID="rule_index"+j;
		var EditIndex=Form.Checkbox(EditIndexID);
		if(EditIndex=='1'){
			G_editListMode = 0;
			editIdx=j;
			$('EntryInfo').style.display = 'block';
			var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
			addAction = 2;
			//alert(AddEntry);
			$('addEntry').disabled=true;
			$('editEntry').disabled=true;
			$('removeEntry').disabled=true;		
			
			Form.Radio('RADIO_ScheduleMode', G_MacBlackList[j][2]); 		
			$('INPUT_UserName').value = G_MacBlackList[j][9];
			
			if(LanHosts.length >0 ){
				if(G_MacBlackList[j][1]==G_CurrentMAC){
					$('INPUT_CurrentPcAddress').value=G_CurrentMAC;
					$('INPUT_OtherMacAddress').value='';
					$('RADIO_CurrentPcAddress').checked=true;
					$('INPUT_OtherMacAddress').disabled=true;
					$('INPUT_CurrentPcAddress').disabled=true;
				}else{
					$('INPUT_CurrentPcAddress').value=G_CurrentMAC;
					$('INPUT_OtherMacAddress').value=G_MacBlackList[j][1];	
					$('RADIO_OtherMacAddress').checked=true;
					$('INPUT_CurrentPcAddress').disabled=true;
				}
			}else{
				$('INPUT_OtherMacAddress').value=G_MacBlackList[j][1];	
			}
			Form.Radio('RADIO_ScheduleMode', G_MacBlackList[j][2]); 	
			
			if( G_MacBlackList[j][2]=='Select'){
				$('SELECT_Schedules').value = G_MacBlackList[j][3];
				disCtrl('manualSchedule', false);	
			}else{
				$('SELECT_Schedules').value = G_MacBlackList[j][3];
				$('SELECT_Schedules').disabled=true;
				
				var starttime= G_MacBlackList[j][7];
				var num=starttime.indexOf(':');
				var starttimeh=starttime.substring(0,num);
				var starttimem=starttime.substring(num+1,starttime.length+1);
				
				var endtime= G_MacBlackList[j][8];
				var num=endtime.indexOf(':');
				var endtimeh=endtime.substring(0,num);
				var endtimem=endtime.substring(num+1,endtime.length+1);
				
				
				var day=G_MacBlackList[j][5];
				
				if(day.indexOf('Sun')!=-1){
					var SunEnable=1; //Sun
				}else{
					var SunEnable=0; //Sun
				}
				if(day.indexOf('Mon')!=-1)
					var MonEnable=1; //Mon
				else
					var MonEnable=0; //Mon
				if(day.indexOf('Tue')!=-1)
					var TueEnable=1; //Tue
				else
					var TueEnable=0; //Tue
				if(day.indexOf('Wed')!=-1)
					var WedEnable=1; //Wed
				else
					var WedEnable=0; //Wed
				if(day.indexOf('Thu')!=-1)
					var ThuEnable=1; //Thu
				else
					var ThuEnable=0; //Thu
				if(day.indexOf('Fri')!=-1)
					var FriEnable=1; //Fri
				else
					var FriEnable=0; //Fri
				if(day.indexOf('Sat')!=-1)
					var SatEnable=1; //Sat
				else
					var SatEnable=0; //Sat
				Form.Radio('EnableAllDays', G_MacBlackList[j][4]);
				
				setJSONValue({	
					sunday          : SunEnable,
					monday          : MonEnable,
					tuesday         : TueEnable,
					wednesday       : WedEnable,
					thursday        : ThuEnable,
					friday          : FriEnable,
					saturday        : SatEnable,
					EnableAllTimes 	: G_MacBlackList[j][6],
					startTimeH 		: starttimeh,
					startTimeM 		: starttimem,
					endTimeH		: endtimeh,
					endTimeM		: endtimem
				});
				if($('EnableAllTimes').checked){
					$('startTimeH').disabled=true;
					$('endTimeH').disabled=true;
					$('startTimeM').disabled=true;
					$('endTimeM').disabled=true;
				}
				if($('AllWeek').checked){
					var _input=$('daysArea').getElementsByTagName('INPUT');
					for(var k = 0, _len = _input.length; k < _len; k++){
						_input[k].disabled = true;
					}
				}
			}
		}
	}
	Heigtduiqi();
}

function EditEntryWhitelist()
{
var MContent = $('Content').offsetHeight;
	for(var j = 0; j < G_MacWhiteList.length; j++){
		var EditIndexID="rule_index_white"+j;
		var EditIndex=Form.Checkbox(EditIndexID);
		if(EditIndex=='1'){
			G_editListMode = 1;
			editIdx=j;
			$('EntryInfo').style.display = 'block';
			var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
			addAction = 2;
			//alert(AddEntry);
			$('addEntry').disabled=true;
			$('editEntry').disabled=true;
			$('removeEntry').disabled=true;		
			
			Form.Radio('RADIO_ScheduleMode', G_MacWhiteList[j][2]); 		
			$('INPUT_UserName').value = G_MacWhiteList[j][9];
			
			if(LanHosts.length >0 ){
				if(G_MacWhiteList[j][1]==G_CurrentMAC){
					$('INPUT_CurrentPcAddress').value=G_CurrentMAC;
					$('INPUT_OtherMacAddress').value='';
					$('RADIO_CurrentPcAddress').checked=true;
					$('INPUT_OtherMacAddress').disabled=true;
					$('INPUT_CurrentPcAddress').disabled=true;
				}else{
					$('INPUT_CurrentPcAddress').value=G_CurrentMAC;
					$('INPUT_OtherMacAddress').value=G_MacWhiteList[j][1];	
					$('RADIO_OtherMacAddress').checked=true;
					$('INPUT_CurrentPcAddress').disabled=true;
				}
			}else{
				$('INPUT_OtherMacAddress').value=G_MacWhiteList[j][1];	
			}
			Form.Radio('RADIO_ScheduleMode', G_MacWhiteList[j][2]); 	
			
			if( G_MacWhiteList[j][2]=='Select'){
				$('SELECT_Schedules').value = G_MacWhiteList[j][3];
				disCtrl('manualSchedule', false);	
			}else{
				$('SELECT_Schedules').value = G_MacWhiteList[j][3];
				$('SELECT_Schedules').disabled=true;
				
				var starttime= G_MacWhiteList[j][7];
				var num=starttime.indexOf(':');
				var starttimeh=starttime.substring(0,num);
				var starttimem=starttime.substring(num+1,starttime.length+1);
				
				var endtime= G_MacWhiteList[j][8];
				var num=endtime.indexOf(':');
				var endtimeh=endtime.substring(0,num);
				var endtimem=endtime.substring(num+1,endtime.length+1);
				
				
				var day=G_MacWhiteList[j][5];
				
				if(day.indexOf('Sun')!=-1){
					var SunEnable=1; //Sun
				}else{
					var SunEnable=0; //Sun
				}
				if(day.indexOf('Mon')!=-1)
					var MonEnable=1; //Mon
				else
					var MonEnable=0; //Mon
				if(day.indexOf('Tue')!=-1)
					var TueEnable=1; //Tue
				else
					var TueEnable=0; //Tue
				if(day.indexOf('Wed')!=-1)
					var WedEnable=1; //Wed
				else
					var WedEnable=0; //Wed
				if(day.indexOf('Thu')!=-1)
					var ThuEnable=1; //Thu
				else
					var ThuEnable=0; //Thu
				if(day.indexOf('Fri')!=-1)
					var FriEnable=1; //Fri
				else
					var FriEnable=0; //Fri
				if(day.indexOf('Sat')!=-1)
					var SatEnable=1; //Sat
				else
					var SatEnable=0; //Sat
				Form.Radio('EnableAllDays', G_MacWhiteList[j][4]);
				
				setJSONValue({	
					sunday          : SunEnable,
					monday          : MonEnable,
					tuesday         : TueEnable,
					wednesday       : WedEnable,
					thursday        : ThuEnable,
					friday          : FriEnable,
					saturday        : SatEnable,
					EnableAllTimes 	: G_MacWhiteList[j][6],
					startTimeH 		: starttimeh,
					startTimeM 		: starttimem,
					endTimeH		: endtimeh,
					endTimeM		: endtimem
				});
				if($('EnableAllTimes').checked){
					$('startTimeH').disabled=true;
					$('endTimeH').disabled=true;
					$('startTimeM').disabled=true;
					$('endTimeM').disabled=true;
				}
				if($('AllWeek').checked){
					var _input=$('daysArea').getElementsByTagName('INPUT');
					for(var k = 0, _len = _input.length; k < _len; k++){
						_input[k].disabled = true;
					}
				}
			}
		}
	}
}

function RemoveEntry(){

     	if(white_array_id.length == 0 && _array_id.length == 0){
		alert(SEcode[9888]);
		return false;
	}	
	
     	if(white_array_id.length + _array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	}
    
	RemoveEntryWhilte();
	for(var j = 0; j < G_MacBlackList.length; j++){
		var DelIndexID="rule_index"+j;
		var DelIndex=Form.Checkbox(DelIndexID);
		if(DelIndex=='1'){           			
			if(!confirm(SEcode[1008])){return false;}
			
			$H({
				'del-obj'		: G_MacBlackList[j][0],
				'obj-action'	: 'del',
				'var:menu'		: G_Menu,
				'var:page'		: G_Page,
				'var:subpage'	: G_SubPage,
				'var:errorpage'	: G_SubPage,
				'getpage'		: 'html/index.html',
				'errorpage'		: 'html/index.html'
			});
			$('uiPostForm').submit();
		}
	}	
}

function RemoveEntryWhilte()
{
    var FilterMode;
	for(var j = 0; j < G_MacWhiteList.length; j++){
	var DelIndexID="rule_index_white"+j;
	var DelIndex=Form.Checkbox(DelIndexID);
	if(DelIndex=='1'){
        FilterMode = "<?get :InternetGatewayDevice.X_TWSZ-COM_MAC_Filter.FilterMode?>";
        if ((FilterMode == 'Allow') && (G_MacWhiteList.length == 1))
        {
            alert(SEcode[9887]);
            return 0;
        }             
		
		if(!confirm(SEcode[1008])){return false;}
		
		$H({
			'del-obj'		: G_MacWhiteList[j][0],
			'obj-action'	: 'del',
			'var:menu'		: G_Menu,
			'var:page'		: G_Page,
			'var:subpage'	: G_SubPage,
			'var:errorpage'	: G_SubPage,
			'getpage'		: 'html/index.html',
			'errorpage'		: 'html/index.html'
		});
		$('uiPostForm').submit();
	}
}	
}

function changeScheduleMode(){
	if($('RADIO_SelectSchedule').checked)	{
		disCtrl('manualSchedule', false);
		$('SELECT_Schedules').disabled=false;
	}else{
		disCtrl('manualSchedule', true);
		$('SELECT_Schedules').disabled=true;
	}
}

function changeMACSelect(value){
	if(value=='1'){
		$('INPUT_OtherMacAddress').disabled=true;
	}else{
		$('INPUT_OtherMacAddress').disabled=false;
	}
	
}

function cleanInputArea(){
	
	var _input=$('EntryInfo').getElementsByTagName('INPUT');
	if(value=='1'){
		for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].value = '';
		}	
	}
}

function changeDaysSelect(){
	
	var _input=$('daysArea').getElementsByTagName('INPUT');
	if($('AllWeek').checked){
		for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].checked = true;
			_input[k].disabled = true;
		}
	}else{
		for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].checked = false;
			_input[k].disabled = false;	
		}
	}

}


function CancelEdit(){
	addAction=0;
	$('EntryInfo').style.display = 'none';
    $('addEntry').disabled=false;
	$('editEntry').disabled=false;
	$('removeEntry').disabled=false;	
}
/*
function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	arrayHint['Description'] = 'INPUT_UserName';
	arrayHint['MACAddress'] = 'INPUT_URLAddress';
	arrayHint['StartTime'] = 'INPUT_STARTTIME';
	arrayHint['EndTime'] = 'INPUT_ENDTIME';
	arrayHint['Days'] = 'wednesday';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}*/
function dealWithError(){
         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

