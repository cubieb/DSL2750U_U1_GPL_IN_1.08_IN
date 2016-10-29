//ip filter
var G_IPFilterEnable = "<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.IPFilterEnable?>"; // filter enable
var G_SecurityLevel  = "<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.SecurityLevel?>";  // security level
var G_LanFilterTable = "<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.CurrentLANFilterTable?>";
var G_WanFilterTable = "<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.CurrentWANFilterTable?>";

//Lan Device
var n = 0;
var G_LanConnction = [];
<?objget :InternetGatewayDevice.Layer2Bridging.Bridge. "BridgeName X_TWSZ-COM_AssociatedLANDevice"
`	G_LanConnction[n] = [];
	G_LanConnction[n][0] = "$01" ; // lan's name
	G_LanConnction[n][1] = "$02";
	n++;
`?>

var n = 0;
var G_Lan_Black = [];

<? if gt `<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilterNumberOfEntries?>` 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilter. "SrcIP SrcMask SrcPort DestIP DstMask DestPort Protocol Description SchedulePath SrcIPType DestIPType SrcStartIP SrcEndIP DestStartIP DestEndIP"
	`	G_Lan_Black[n] = [];
		G_Lan_Black[n][0] = "$01"; //SrcIP
		G_Lan_Black[n][1] = "$02"; //SrcMask
		G_Lan_Black[n][2] = "$03"; //SrcPort
		G_Lan_Black[n][3] = "$04"; //DestIP
		G_Lan_Black[n][4] = "$05"; //DstMask
		G_Lan_Black[n][5] = "$06"; //DestPort
		G_Lan_Black[n][6] = "$07"; //Protocol
		G_Lan_Black[n][7] = "$08"; //Description
		G_Lan_Black[n][8] = "$09"; //SchedulePath
		G_Lan_Black[n][9] = "InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilter.$00.";
		G_Lan_Black[n][10] = "$0a";  	//SrcIPType
		G_Lan_Black[n][11] = "$0b";	   	//DestIPType
		G_Lan_Black[n][12] = "$0c";		//SrcStartIP
		G_Lan_Black[n][13] = "$0d";		//SrcEndIP
		G_Lan_Black[n][14] = "$0e";		//DestStartIP
		G_Lan_Black[n][15] = "$0f";		//DestEndIP
		n++;
	`?>
`?>

var m = 0;
var G_Lan_Black_port = [];
<? if gt `<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilterNumberOfEntries?>` 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilter. "SrcPortType DestPortType "
	`	G_Lan_Black_port[m] = [];
		G_Lan_Black_port[m][0] = "$01"; //SrcPortType
		G_Lan_Black_port[m][1] = "$02"; //DestPortType
		m++;
	`?>
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

//将Path转换为Schedule的Name
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

function createTable(){
	//鍒濆鍖杢able,娓呯Ⓙ
	Table.Clear('Table_ip_list');
	var srcShow=0;
	var destShow=0;
	//閬嶅巻鎸囧畾鐨凢ILTER
	var array_value = [];
	for(var i = 0; i < G_Lan_Black.length; i++){
		/*
		if(G_Lan_Black[i][11]=='SingleIP'){
			srcShow=G_Lan_Black[i][0];
		}else if(G_Lan_Black[i][11]=='NetworkIP'){
			srcShow=G_Lan_Black[i][0]+'/'+G_Lan_Black[i][1];
		}else if()*/
		
		switch(G_Lan_Black[i][10]){
			case 'Any' :
			case 'SingleIP' :
				srcShow=G_Lan_Black[i][0];
				break;
			case 'NetworkIP' :
				srcShow=G_Lan_Black[i][0]+'/'+G_Lan_Black[i][1];
				break;
			case 'iprange' :	
				srcShow=G_Lan_Black[i][12]+'/'+G_Lan_Black[i][13];
				break;
		}
		
		
		switch(G_Lan_Black[i][11]){
			case 'Any' :
			case 'SingleIP' :
				destShow=G_Lan_Black[i][3];
				break;
			case 'NetworkIP' :
				destShow=G_Lan_Black[i][3]+'/'+G_Lan_Black[i][4];
				break;
			case 'iprange' :	
				destShow=G_Lan_Black[i][14]+'/'+G_Lan_Black[i][15];
				break;
		}
		
		array_value[i] = [];
		array_value[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">');
		array_value[i].push(G_Lan_Black[i][7]+'&nbsp;'); //Description
		array_value[i].push(G_Lan_Black[i][6]+'&nbsp;'); //protocol
		array_value[i].push(srcShow.ellipsis(8)+'&nbsp;'); //Source IP
		array_value[i].push(G_Lan_Black[i][2]+'&nbsp;'); //Src. Port
		array_value[i].push(destShow.ellipsis(8)+'&nbsp;'); //Dest IP 
		array_value[i].push(G_Lan_Black[i][5]+'&nbsp;'); //Dest. Port
		array_value[i].push(searchPath(G_Lan_Black[i][8])); //Schedule
	}
	$T('Table_ip_list',array_value);
	for(var b = 0; b < G_Lan_Black.length; b++){
	$('Table_ip_list_' + b + '3').title = srcShow;
	
	}
	for(var k = 0; k < G_Lan_Black.length; k++){
	$('Table_ip_list_' + k + '5').title = destShow;
	
	}
}

//杩欎釜鍑芥暟鏈変袱涓敤澶勶粿銆佸姞杞藉嚱鏁帮粿銆佸彉鎹㈠畨鍏ㄧ骇鍒椂浣滀负onchange鐨勪簨浠跺鐞嗗嚱鏁?
function uiOnload(){

	createTable();

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
	CancelEdit();
	
}

function changeSrcIPType(){
	var value_mode = $('SELECT_SrcIP').value;
	
	switch(value_mode){
		case 'Any' : {
			$('SrcSingleIPArea').style.display='block';
			$('SrcIPRangeArea').style.display='none';
			disCtrl('srcipaddr',false);
			disCtrl('srcipmask',false);
			$('SrcIPAddress').value='';
			$('SrcMask').value='';
			break;
		}
		case 'SingleIP' : {
			$('SrcSingleIPArea').style.display='block';
			$('SrcIPRangeArea').style.display='none';
			disCtrl('srcipaddr',true);
			disCtrl('srcipmask',false);
			$('SrcIPAddress').value='';
			$('SrcMask').value='255.255.255.255';
			break;
		}
		case 'NetworkIP' :{
			$('SrcSingleIPArea').style.display='block';
			$('SrcIPRangeArea').style.display='none';
			disCtrl('srcipaddr',true);
			disCtrl('srcipmask',true);
			$('SrcIPAddress').value='';
			$('SrcMask').value='255.255.255.0';
			break;
		}
		case 'iprange'	  :{
			$('SrcSingleIPArea').style.display='none';
			$('SrcIPRangeArea').style.display='block';
			break;
		}
	}
}

function changeSrcPortType(){
	
	var value_mode=$('SELECT_SrcPortType').value;
	switch(value_mode){
		case 'Any' :	{
			disCtrl('SrcPortArea',false);
			break;
		}
		case 'SinglePort' :
		case 'PortRange'  :
			disCtrl('SrcPortArea',true);
			break;
	}
}

function changeDestIPType(){
	var value_mode = $('SELECT_DestIP').value;
	
	switch(value_mode){
		case 'Any' : {
			$('DestSingleIPArea').style.display='block';
			$('DestIPRangeArea').style.display='none';
			disCtrl('destipaddr',false);
			disCtrl('destipmask',false);
			$('DestIPAddress').value='';
			$('DestMask').value='';
			break;
		}
		case 'SingleIP' : {
			$('DestSingleIPArea').style.display='block';
			$('DestIPRangeArea').style.display='none';
			disCtrl('destipaddr',true);
			disCtrl('destipmask',false);
			$('DestIPAddress').value='';
			$('DestMask').value='255.255.255.255';
			break;
		}
		case 'NetworkIP' :{
			$('DestSingleIPArea').style.display='block';
			$('DestIPRangeArea').style.display='none';
			disCtrl('destipaddr',true);
			disCtrl('destipmask',true);
			$('DestIPAddress').value='';
			$('DestMask').value='255.255.255.0';
			break;
		}
		case 'iprange'	  :{
			$('DestSingleIPArea').style.display='none';
			$('DestIPRangeArea').style.display='block';
			break;
		}
	}
}

function changeDestPortType(){
	var value_mode=$('SELECT_DestPortType').value;

	switch(value_mode){
		case 'Any' :{
			disCtrl('DestPortArea',false);
			break;
		}
		case 'SinglePort' :
		case 'PortRange'  :
			disCtrl('DestPortArea',true);
			break;
	}
}

function cleanInputArea(){
	var _input=$('EntryInfo').getElementsByTagName('INPUT');

	for(var k = 0, _len = _input.length; k < _len; k++){
		_input[k].value = '';
	}
}

var addAction=0;
function AddEntry(){
	var MContent = $('Content').offsetHeight;
	addAction = 1;
    $('addEntry').disabled=true;
	$('editEntry').disabled=true;
	$('removeEntry').disabled=true;
	$('EntryInfo').style.display='block';

	disCtrl('srcipaddr',false);
	disCtrl('srcipmask',false);
	disCtrl('destipaddr',false);
	disCtrl('destipmask',false);
	//disCtrl('DestSingleIPArea',false);
	disCtrl('SrcPortArea',false);
	disCtrl('DestPortArea',false);
	changeSrcIPType();
	changeSrcPortType();
	changeDestIPType();
	changeDestPortType();
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
	 if(_array_id.length != 1){
		alert(SEcode[1009]);
		return false;
	}
		addAction = 2;

		for(var j = 0; j < G_Lan_Black.length; j++){
			var EditIndexID="rule_index"+j;
			
			var EditIndex=Form.Checkbox(EditIndexID);
			if(EditIndex=='1'){
				
				$('addEntry').disabled=true;
				$('editEntry').disabled=true;
				$('removeEntry').disabled=true;
				$('EntryInfo').style.display='block';	
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
				editIdx=j;
				$('INPUT_Description').value=G_Lan_Black[j][7];
				$('SELECT_protocol').value=G_Lan_Black[j][6];
				
				$('SELECT_SrcIP').value=G_Lan_Black[j][10];
				changeSrcIPType();
				if(G_Lan_Black[j][10]=='SingleIP'){
					$('SrcIPAddress').value=G_Lan_Black[j][0];
				}else if(G_Lan_Black[j][10]=='NetworkIP'){
					$('SrcIPAddress').value=G_Lan_Black[j][0];
					$('SrcMask').value=G_Lan_Black[j][1];
				}else if(G_Lan_Black[j][10]=='iprange'){
					$('SrcStartIPAddress').value=G_Lan_Black[j][12];
					$('SrcEndIPAddress').value=G_Lan_Black[j][13];		
				}
				
				$('SELECT_SrcPortType').value=G_Lan_Black_port[j][0];
				changeSrcPortType();
				$('SrcPort').value=G_Lan_Black[j][2];
				
				$('SELECT_DestIP').value=G_Lan_Black[j][11];
				changeDestIPType();
				if(G_Lan_Black[j][11]=='SingleIP'){
					$('DestIPAddress').value=G_Lan_Black[j][3];
				}else if(G_Lan_Black[j][11]=='NetworkIP'){
					$('DestIPAddress').value=G_Lan_Black[j][3];
					$('DestMask').value=G_Lan_Black[j][4];
				}else if(G_Lan_Black[j][11]=='iprange'){
					$('DestStartIPAddress').value=G_Lan_Black[j][14];
					$('DestEndIPAddress').value=G_Lan_Black[j][15];		
				}
				
				$('SELECT_DestPortType').value=G_Lan_Black_port[j][1];
				changeDestPortType();
				$('DestPort').value=G_Lan_Black[j][5];
				
				$('SELECT_Schedules').value=G_Lan_Black[j][8];
			}
		}
}

function RemoveEntry(){
	if(_array_id.length ==0){
		alert(SEcode[1009]);
		return false;
	}
    if(!confirm(SEcode[1008])){return false;}
	
	for(var j = 0; j < G_Lan_Black.length; j++){
		var DelIndexID="rule_index"+j;
		if(Form.Checkbox(DelIndexID) == 1){
			
			$H({
				'del-obj'   :G_Lan_Black[j][9],
				'obj-action':'del',
				'var:menu'  :'advanced',
				'var:page'  :'filtering_options',
				'getpage'   :'html/index.html',
				'errorpage' :'html/index.html',
				'var:subpage'   :'outbound_ipfiltering',
				'var:errorpage':'outbound_ipfiltering'
			});
			$('uiPostForm').submit();
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

function IptoInt(str_IpAd) { 

    var arr_IpAd=str_IpAd.split('.');
    var Int_value=arr_IpAd[0] * 256^3 + arr_IpAd[1]*256^2 + arr_IpAd[2]*256^1+ arr_IpAd[3];
    return Int_value;
}

function uiSumbit(){
		var Value_Nodes = $('INPUT_Description','SELECT_protocol','SELECT_SrcIP','SrcIPAddress','SrcMask','SrcStartIPAddress','SrcEndIPAddress','SELECT_SrcPortType','SrcPort','SELECT_DestIP','DestIPAddress','DestMask','DestStartIPAddress','DestEndIPAddress','SELECT_DestPortType','DestPort','SELECT_Schedules');

	var srcipadress=0;
	var srcnetmask=0;
	if($('SELECT_Schedules').value=='Never')
		enable_Entry=0;
	else
		enable_Entry=1;
		
			if(Value_Nodes[0].value.length > 32)
	{
	      alert("the length of the Outbound Filter name must be in 32!");
	      return;
	}
		
	if(Value_Nodes[2].value == 'iprange')
	{
		if(IptoInt(Value_Nodes[5].value) > IptoInt(Value_Nodes[6].value) )
			{
			     alert("the StartIPAddress is larger than the EndIPAddress!");
			     return ;
			}
	}

	if(Value_Nodes[9].value == 'iprange')
	{
		if(IptoInt(Value_Nodes[12].value) > IptoInt(Value_Nodes[13].value) )
			{
			     alert("the StartIPAddress is larger than the EndIPAddress!");
			     return ;
			}
	}

	if(addAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TWSZ-COM_Firewall.LanBlackFilter.',
			':Enable'		: enable_Entry,
			':Description'	: Value_Nodes[0].value,
			':Protocol'		: Value_Nodes[1].value,
			':SrcIPType'	: Value_Nodes[2].value,
			':SrcIP'      	: (Value_Nodes[2].value != 'iprange')? Value_Nodes[3].value : undefined,
			':SrcMask'    	: (Value_Nodes[2].value != 'iprange')? Value_Nodes[4].value : undefined,
			':SrcStartIP'   : (Value_Nodes[2].value == 'iprange')? Value_Nodes[5].value : undefined,
			':SrcEndIP'    	: (Value_Nodes[2].value == 'iprange')? Value_Nodes[6].value : undefined,
			':SrcPortType'  : Value_Nodes[7].value,
			':SrcPort'    	: Value_Nodes[7].value != 'Any' ? Value_Nodes[8].value : '',
			
			':DestIPType'   :  Value_Nodes[9].value,
			':DestIP'     	: (Value_Nodes[9].value != 'iprange')? Value_Nodes[10].value : undefined,
			':DstMask'    	: (Value_Nodes[9].value != 'iprange')? Value_Nodes[11].value : undefined,
			':DestStartIP'  : (Value_Nodes[9].value == 'iprange')? Value_Nodes[12].value : undefined,
			':DestEndIP'    : (Value_Nodes[9].value == 'iprange')? Value_Nodes[13].value : undefined,
			
			':DestPortType' : Value_Nodes[14].value,
			':DestPort'   	: Value_Nodes[14].value != 'Any' ? Value_Nodes[15].value : '',
			':SchedulePath' : Value_Nodes[16].value,
			':DevPath'		: G_LanConnction[0][1],
			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'outbound_ipfiltering',
			'var:page' : 'filtering_options',
			'var:errorpage'    : 'outbound_ipfiltering',
			'var:CacheLastData': ViewState.Save()
		},true);
		
	}else if(addAction=='2'){     //edit
	
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'outbound_ipfiltering',
			'var:page' : 'filtering_options',
			'var:errorpage'    : 'outbound_ipfiltering',
			'var:CacheLastData': ViewState.Save()
		},true);		
		
		var path=':'+G_Lan_Black[editIdx][9];
		$F(path+'Enable',enable_Entry);
		$F(path+'Description',Value_Nodes[0].value);
		$F(path+'Protocol',Value_Nodes[1].value);
		$F(path+'SrcIPType',Value_Nodes[2].value);
		
		if(Value_Nodes[2].value != 'iprange'){
			$F(path+'SrcIP',Value_Nodes[3].value);
			$F(path+'SrcMask',Value_Nodes[4].value);
			$F(path+'SrcStartIP','');
			$F(path+'SrcEndIP','');
		}
		
		if(Value_Nodes[2].value == 'iprange'){
			$F(path+'SrcStartIP',Value_Nodes[5].value );
			$F(path+'SrcEndIP',Value_Nodes[6].value);
			$F(path+'SrcIP','');
			$F(path+'SrcMask','');
		}
		$F(path+'SrcPortType',Value_Nodes[7].value);
		
		if(Value_Nodes[7].value != 'Any'){         
			$F(path+'SrcPort',Value_Nodes[8].value);
		}else{
			$F(path+'SrcPort','');
		}
		
		$F(path+'DestIPType',Value_Nodes[9].value);
		if(Value_Nodes[9].value != 'iprange'){
			$F(path+'DestIP',Value_Nodes[10].value);
			$F(path+'DstMask',Value_Nodes[11].value);
			$F(path+'DestStartIP','');
			$F(path+'DestEndIP','');
		}
		
		if(Value_Nodes[9].value == 'iprange'){
			$F(path+'DestIP','');
			$F(path+'DstMask','');
			$F(path+'DestStartIP',Value_Nodes[12].value );
			$F(path+'DestEndIP',Value_Nodes[13].value);
		}
		$F(path+'DestPortType',Value_Nodes[14].value);
		
		if(Value_Nodes[14].value != 'Any'){         
			$F(path+'DestPort',Value_Nodes[15].value);
		}else{
			$F(path+'DestPort','');
		}
		
		$F(path+'SchedulePath',Value_Nodes[16].value);
		$F(path+'DevPath',G_LanConnction[0][1]);
	}
	$('uiPostForm').submit();
	addAction=0;
}



function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
