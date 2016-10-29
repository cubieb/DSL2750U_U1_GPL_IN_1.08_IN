
var editIdx=0;
var G_wanConnction = [];
var m = 0;

var G_ATM = [];
var G_InternetWanType = "Ethernet";
<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?if eq `DSL` `<?echo $21?>`
	`	<?objget :InternetGatewayDevice.WANDevice.$20.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS"
		`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.$20.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02"//DestinationAddress
				];
			m++;
		`?>
		G_InternetWanType = "DSL";
	`?>
`?>

//Wan Device
m=0;

<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00"; // about Path
			G_wanConnction[m][2] = "$02"; 
			m++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // wan's name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00"; // about Path;
			G_wanConnction[m][2] = "$02";
			m++;
		`?>
	`?>
`?>
`?>

var n = 0;
var G_Wan_White = [];
<? if gt `<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilterNumberOfEntries?>` 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilter. "SrcIP SrcMask SrcPort DestIP DstMask DestPort Protocol Description SchedulePath SrcIPType DestIPType SrcStartIP SrcEndIP DestStartIP DestEndIP"
	`	G_Wan_White[n] = [];
		G_Wan_White[n][0] = "$01"; //SrcIP
		G_Wan_White[n][1] = "$02"; //SrcMask
		G_Wan_White[n][2] = "$03"; //SrcPort
		G_Wan_White[n][3] = "$04"; //DestIP
		G_Wan_White[n][4] = "$05"; //DstMask
		G_Wan_White[n][5] = "$06"; //DestPort
		G_Wan_White[n][6] = "$07"; //Protocol
		G_Wan_White[n][7] = "$08"; //Description
		G_Wan_White[n][8] = "$09"; //Schedule
		G_Wan_White[n][9] = "InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilter.$00.";
		G_Wan_White[n][10] = "$0a";  	//SrcIPType
		G_Wan_White[n][11] = "$0b";	   	//DestIPType
		G_Wan_White[n][12] = "$0c";		//SrcStartIP
		G_Wan_White[n][13] = "$0d";		//SrcEndIP
		G_Wan_White[n][14] = "$0e";		//DestStartIP
		G_Wan_White[n][15] = "$0f";		//DestEndIP
		n++;
	`?>
`?>

var m = 0;
var G_Wan_White_port = [];
<? if gt `<?get :InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilterNumberOfEntries?>` 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilter. "SrcPortType DestPortType DevPath"
	`	G_Wan_White_port[m] = [];
		G_Wan_White_port[m][0] = "$01"; //SrcPortType
		G_Wan_White_port[m][1] = "$02"; //DestPortType
		G_Wan_White_port[m][2] = "$03";  //DevPath
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

//��Pathת��ΪSchedule��Name
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

function getAtmParam(_i){
	var path_wan = G_Wan_White_port[_i][2];

	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}
function createTable(){
	//初始化table,清空
	Table.Clear('Table_ip_list');
	var srcShow=0;
	var destShow=0;
	//遍历指定的FILTER
	var array_value = [];
	for(var i = 0; i < G_Wan_White.length; i++){
		switch(G_Wan_White[i][10]){
			case 'Any' :
			case 'SingleIP' :
				srcShow=G_Wan_White[i][0];
				break;
			case 'NetworkIP' :
				srcShow=G_Wan_White[i][0]+'/'+G_Wan_White[i][1];
				break;
			case 'iprange' :	
				srcShow=G_Wan_White[i][12]+'/'+G_Wan_White[i][13];
				break;
		}
		
		
		switch(G_Wan_White[i][11]){
			case 'Any' :
			case 'SingleIP' :
				destShow=G_Wan_White[i][3];
				break;
			case 'NetworkIP' :
				destShow=G_Wan_White[i][3]+'/'+G_Wan_White[i][4];
				break;
			case 'iprange' :	
				destShow=G_Wan_White[i][14]+'/'+G_Wan_White[i][15];
				break;
		}
		var j = getAtmParam(i);
		array_value[i] = [];
		array_value[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">');
		array_value[i].push(G_Wan_White[i][7]+'&nbsp;'); //Description
		array_value[i].push(j == -1 ? 'N/A' : G_ATM[j][2]); //PVC
		array_value[i].push(G_Wan_White[i][6]); //protocol
		array_value[i].push(srcShow.ellipsis(8)+'&nbsp;'); //Source IP
		array_value[i].push(G_Wan_White[i][2]+'&nbsp;'); //Src. Port
		array_value[i].push(destShow.ellipsis(8)+'&nbsp;'); //Dest IP 
		array_value[i].push(G_Wan_White[i][5]+'&nbsp;'); //Dest. Port
		array_value[i].push(searchPath(G_Wan_White[i][8])); //Schedule
	}
	$T('Table_ip_list',array_value);
	for(var b = 0; b < G_Wan_White.length; b++){
	$('Table_ip_list_' + b + '4').title = srcShow;
	
	}
	for(var k = 0; k < G_Wan_White.length; k++){
	$('Table_ip_list_' + k + '6').title = destShow;
	
	}
}

//这个函数有两个用处：1、加载函数；2、变换安全级别时作为onchange的事件处理函�?
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
	
	var _text = [], _value = [];
	for(var i = 0; i < G_wanConnction.length; i++){
		    if(G_wanConnction[i][2]=="TR069"){
				continue;
		    }
			_text.push(G_wanConnction[i][0]);
			_value.push(G_wanConnction[i][1]);
	}
	$S('SELECT_ConnsName',_text,_value);
	
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
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight -200;
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

		for(var j = 0; j < G_Wan_White.length; j++){
			
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
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight - 200;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
				editIdx=j;
				$('INPUT_Description').value=G_Wan_White[j][7];
				$('SELECT_protocol').value=G_Wan_White[j][6];
				
				$('SELECT_SrcIP').value=G_Wan_White[j][10];
				changeSrcIPType();
				if(G_Wan_White[j][10]=='SingleIP'){
					$('SrcIPAddress').value=G_Wan_White[j][0];
				}else if(G_Wan_White[j][10]=='NetworkIP'){
					$('SrcIPAddress').value=G_Wan_White[j][0];
					$('SrcMask').value=G_Wan_White[j][1];
				}else if(G_Wan_White[j][10]=='iprange'){
					$('SrcStartIPAddress').value=G_Wan_White[j][12];
					$('SrcEndIPAddress').value=G_Wan_White[j][13];		
				}
				
				$('SELECT_SrcPortType').value=G_Wan_White_port[j][0];
				changeSrcPortType();
				$('SrcPort').value=G_Wan_White[j][2];
				
				$('SELECT_DestIP').value=G_Wan_White[j][11];
				changeDestIPType();
				if(G_Wan_White[j][11]=='SingleIP'){
					$('DestIPAddress').value=G_Wan_White[j][3];
				}else if(G_Wan_White[j][11]=='NetworkIP'){
					$('DestIPAddress').value=G_Wan_White[j][3];
					$('DestMask').value=G_Wan_White[j][4];
				}else if(G_Wan_White[j][11]=='iprange'){
					$('DestStartIPAddress').value=G_Wan_White[j][14];
					$('DestEndIPAddress').value=G_Wan_White[j][15];		
				}
				
				$('SELECT_DestPortType').value=G_Wan_White_port[j][1];
				changeDestPortType();
				$('DestPort').value=G_Wan_White[j][5];
				
				$('SELECT_Schedules').value=G_Wan_White[j][8];
			}
		}
}

function RemoveEntry(){
	
	if(_array_id.length ==0){
		alert(SEcode[1009]);
		return false;
	}

	if(!confirm(SEcode[1008])){return false;}
	
	for(var j = 0; j < G_Wan_White.length; j++){
		var DelIndexID="rule_index"+j;
		if(Form.Checkbox(DelIndexID) == 1){
			
			
			$H({
				'del-obj'   :G_Wan_White[j][9],
				'obj-action':'del',
				'var:menu'  :'advanced',
				'var:page'  :'inbound_ipfiltering',
				'getpage'   :'html/index.html',
				'errorpage' :'html/index.html',
				'var:errorpage':'inbound_ipfiltering'
			});
			$('uiPostForm').submit();
		}
	}
}

function CancelEdit(){
	addAction=0;
	$('EntryInfo').style.display = 'none';
	Heigtduiqi();
    $('addEntry').disabled=false;
	$('editEntry').disabled=false;
	$('removeEntry').disabled=false;	
}

function IpToInt(_Ip) { 

    var Array=_Ip.split('.');
	
    var Int=Array[0] * 256*256*256 + Array[1]*256*256 + Array[2]*256+ Array[3];

    return Int;
}

function uiSumbit(){
		var Value_Nodes = $('INPUT_Description','SELECT_protocol','SELECT_SrcIP','SrcIPAddress','SrcMask','SrcStartIPAddress','SrcEndIPAddress','SELECT_SrcPortType','SrcPort','SELECT_DestIP','DestIPAddress','DestMask','DestStartIPAddress','DestEndIPAddress','SELECT_DestPortType','DestPort','SELECT_Schedules','SELECT_ConnsName');
	
	var srcipadress=0;
	var srcnetmask=0;
	if($('SELECT_Schedules').value=='Never')
		enable_Entry=0;
	else
		enable_Entry=1;

	if(Value_Nodes[0].value.length > 32)
	{
	      alert("the length of the Inbound Filter name must be in 32!");
	      return;
	}

	if(Value_Nodes[2].value == 'iprange')
	{
		if(IpToInt(Value_Nodes[5].value) > IpToInt(Value_Nodes[6].value) )
		{
			    alert("the StartIPAddress is larger than the EndIPAddress!");
			    return ;
		}
			
	}
	
	if(Value_Nodes[9].value == 'iprange')
	{
		if(IpToInt(Value_Nodes[12].value) > IpToInt(Value_Nodes[13].value) )
		{
			    alert("the StartIPAddress is larger than the EndIPAddress!");
			    return ;
		}
			
	}
	
	if(addAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TWSZ-COM_Firewall.WanWhiteFilter.',
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
			':DevPath'		: Value_Nodes[17].value,
			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:page' : 'filtering_options',
			'var:subpage'   :'inbound_ipfiltering',
			'var:errorpage'    : 'inbound_ipfiltering',
			'var:CacheLastData': ViewState.Save()
		},true);
		
	}else if(addAction=='2'){     //edit
	
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:page' : 'filtering_options',
			'var:subpage'   :'inbound_ipfiltering',
			'var:errorpage'    : 'inbound_ipfiltering',
			'var:CacheLastData': ViewState.Save()
		},true);		
		
		var path=':'+G_Wan_White[editIdx][9];
		
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
			$F(path+'SrcIP','');
			$F(path+'SrcMask','');
			$F(path+'SrcStartIP',Value_Nodes[5].value );
			$F(path+'SrcEndIP',Value_Nodes[6].value);
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
		$F(path+'DevPath',Value_Nodes[17].value);
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
