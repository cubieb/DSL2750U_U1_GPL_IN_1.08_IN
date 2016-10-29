
var G_InternetWanType = "<?get :InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANAccessType?>";

var G_Class = new Array();
var G_Description = new Array();
var G_Enable = new Array();
var G_ClassificationType = new Array();
var G_ClassInterface = new Array();
var G_SourceMACAddress = new Array();
var G_SourceMACMask = new Array();
var G_DestMACAddress = new Array();
var G_DestMACMask = new Array();
var G_Ethertype = new Array();
var G_EthernetPriorityCheck = new Array();
var G_SourceIP = new Array();
var G_SourceMask = new Array();
var G_DestIP = new Array();
var G_DestMask = new Array();
var G_DSCPCheck = new Array();
var G_Protocol = new Array();
var G_SourcePort = new Array();
var G_SourcePortRangeMax = new Array();
var G_DestPort = new Array();
var G_DestPortRangeMax = new Array();
var G_ClassQueue = new Array();
var G_DSCPMark = new Array();
var G_EthernetPriorityMark = new Array();
var G_WanConnectionName = new Array();
var G_Status = new Array();
var G_DSCPCheckEnable = new Array();
var G_TosCheck = new Array();
var G_WANPathName = new Array();

var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown. "Description Enable ClassificationType ClassInterface SourceMACAddress SourceMACMask DestMACAddress DestMACMask Ethertype EthernetPriorityCheck "
`
	G_Class[n] = "$00."; 
	G_Description[n] = "$01";
	G_Enable[n] = "$02";
	G_ClassificationType[n] ="$03";
	G_ClassInterface[n] = "$04";
	G_SourceMACAddress[n] = "$05";
	G_SourceMACMask[n] = "$06";
	G_DestMACAddress[n] = "$07";
	G_DestMACMask[n] = "$08";
	G_Ethertype[n] = "$09";
	G_EthernetPriorityCheck[n] = "$0a";
	++n;
`?>

n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown. "SourceIP  SourceMask DestIP DestMask DSCPCheck  Protocol SourcePort SourcePortRangeMax DestPort DestPortRangeMax ClassQueue DSCPMark  EthernetPriorityMark "
`
	G_SourceIP[n] = "$01";
	G_SourceMask[n] = "$02";
	G_DestIP[n] = "$03";
	G_DestMask[n] = "$04";
	G_DSCPCheck[n] = "$05";
	G_Protocol[n] = "$06";
	G_SourcePort[n] = "$07";
	G_SourcePortRangeMax[n] = "$08";
	G_DestPort[n] = "$09";
	G_DestPortRangeMax[n] = "$0a";
	G_ClassQueue[n] = "$0b";
	G_DSCPMark[n] = "$0c";
	G_EthernetPriorityMark[n] = "$0d";	
	++n;
`?>

n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown. "WanConnectionName  Status DSCPCheckEnable TosCheck WANPathName"
`
	G_WanConnectionName[n] = "$01";
	G_Status[n] = "$02";
	G_DSCPCheckEnable[n] = "$03";
	G_TosCheck[n] = "$04";
	G_WANPathName[n]= "$05";
	++n;
`?>

// get queue
var G_queue = new Array();
n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Queue. "QueuePrecedence QueueInterface Enable QueueWeight"
`	G_queue[n] = new Array();
	G_queue[n][0] = "$00"; 
	G_queue[n][1] = "$01";
	G_queue[n][2] = "$02";
	G_queue[n][3] = "$03";
	G_queue[n][4] = "$04";
	++n;
`?>

var G_Wanconns = [];
var n = 0;
//WAN Device
<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries X_TWSZ-COM_DeviceName"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$13";
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$13";
			n++;
		`?>
		`?>
	`?>
`?>

function wanPath(){
	var text_wan = [],value_wan = [];
       text_wan.push("any");
	 value_wan.push("any");
	for(var i = 0; i < G_Wanconns.length; i++)
	{
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0)
		{
			
				text_wan.push(G_Wanconns[i][0]);
			       value_wan.push(G_Wanconns[i][0]);
		}		
	}
	
	$S('selectConnectionName',text_wan,value_wan);
}


function changeScheduleMode()
{
    if($('RADIO_SelectDSCPCheck').checked)
	{
		$('select_c_dscp_check').disabled = false;
		$('input_Tos').disabled = true;
	}
   else
	{
		$('select_c_dscp_check').disabled = true;
		$('input_Tos').disabled = false;
	}
  
}

function select_c_protocol_change()
{
	if ($('select_c_protocol').value == 1 || $('select_c_protocol').value == -1) {
		$('input_c_src_port').disabled = true;
		$('input_c_dst_port').disabled = true;
	} else {
		$('input_c_src_port').disabled = false;
		$('input_c_dst_port').disabled = false;
	}
}
 
 
function get_port(port, range)
{
	if (range == 0) {
		if (port.indexOf(':') > -1)
			return port.replace(/(\d+):(\d+)/, "$1");
		else
			return port == "" ? -1 : port;
	}else {
		if (port.indexOf(':') > -1)
			return port.replace(/(\d+):(\d+)/, "$2");
		else
			return -1;
	}
}
 
 
function uiSubmit()
{
     var G_wanPath="any";
     for(var i = 0; i < G_Wanconns.length; i++)
	{
		if((G_Wanconns[i][1].indexOf('WANDevice')) > 0 &&( G_Wanconns[i][0]==$('selectConnectionName').value))
		{
			
				G_wanPath=G_Wanconns[i][1];
		}		
	}
	 alert(G_wanPath);
	if (edit_index == -1) {
		$H({
		 'add-obj'     : "InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown.",
		 ':Description' : $('input_c_name').value,
		 ':Enable'      : Form.Checkbox('input_c_enable'),
		 ':WanConnectionName'      : $('selectConnectionName').value,
		 ':WANPathName'      :G_wanPath,		 
		 ':Status'      : ($('INPUT_In').checked ? 1 : 0),
		 ':SourceIP'      : $('input_c_src_ip_addr').value,
		 ':SourceMask'      : $('input_c_src_ip_mask').value,
		 ':DestIP'      : $('input_c_dst_ip_addr').value,
		 ':DestMask'      : $('input_c_dst_ip_mask').value,
		 ':DSCPCheckEnable'      : ($('RADIO_SelectDSCPCheck').checked ? 1 : 0),
		 ':TosCheck'      : $('input_Tos').value,		 
		 ':DSCPCheck'      : $('select_c_dscp_check').value,
		 ':Protocol'      : $('select_c_protocol').value,
		 ':SourcePort'      : get_port($('input_c_src_port').value, 0),
		 ':SourcePortRangeMax'  : get_port($('input_c_src_port').value, 1),
		 ':DestPort'      : get_port($('input_c_dst_port').value, 0),
		 ':DestPortRangeMax' : get_port($('input_c_dst_port').value, 1),		 		 
		 ':ClassQueue'      : $('select_c_queue').value,
		
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action':'add-set',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
		},true);
	}
	else {
		$H({
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action':'set',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
		},true);
		path = ":InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown." + G_Class[edit_index];
		$F(path + 'Description', $('input_c_name').value);
		$F(path + 'Enable', Form.Checkbox('input_c_enable'));
		$F(path + 'WanConnectionName', $('selectConnectionName').value);
		$F(path + 'WANPathName', G_wanPath);
		$F(path + 'Status', ($('INPUT_In').checked ? 1 : 0));
		$F(path + 'SourceIP', $('input_c_src_ip_addr').value);
		$F(path + 'SourceMask', $('input_c_src_ip_mask').value);
		$F(path + 'DestIP', $('input_c_dst_ip_addr').value);
		$F(path + 'DestMask', $('input_c_dst_ip_mask').value);
		$F(path + 'DSCPCheckEnable', ($('RADIO_SelectDSCPCheck').checked ? 1 : 0));
		$F(path + 'TosCheck', $('input_Tos').value);
		$F(path + 'DSCPCheck', $('select_c_dscp_check').value);
		$F(path + 'Protocol', $('select_c_protocol').value);
		$F(path + 'SourcePort', get_port($('input_c_src_port').value, 0));
		$F(path + 'SourcePortRangeMax', get_port($('input_c_src_port').value, 1));
		$F(path + 'DestPort', get_port($('input_c_dst_port').value, 0));
		$F(path + 'DestPortRangeMax', get_port($('input_c_dst_port').value,1));	
		$F(path + 'ClassQueue', $('select_c_queue').value);

		}
		$('uiPostForm').submit();

}
 
 
 
 
 function uiDelete()
{
	if(!confirm(SEcode[1008])){
		 return false;
	}
	for (var i = 0; i < G_Class.length; i++) {
			var tmpid = "choose" + i;
			if (Form.Checkbox(tmpid) == 1) {
					path = "InternetGatewayDevice.X_TWSZ-COM_QoS.ClassificationDown." + G_Class[i];
					$H({
					'del-obj'    : path ,
					'obj-action' : 'del',
					'var:menu'     : G_Menu,
					'var:page'     : G_Page,
					'var:subpage'  : G_SubPage,
					'var:errorpage': G_SubPage,
					'getpage'   :'html/index.html',
					'errorpage' :'html/index.html',
					'var:CacheLastData': ViewState.Save()
			});
			$('uiPostForm').submit();
			return;
		 }
	}
	alert(SEcode[1009]);
}
 
 
 
function join_port(srcport, dstport)
{
	if (dstport == -1)
		return srcport == -1 ? "" : srcport ;
	else
		return srcport + ':' + dstport;
}
 
var edit_index = -1;
function uiEdit()
{   
  	for (var i = 0; i < G_Class.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) {
			edit_index = i;
			$('input_c_name').value = G_Description[i];
			Form.Checkbox('input_c_enable',G_Enable[i]);
			Form.Select('selectConnectionName', G_WanConnectionName[i]);
			if(G_Status[i]==1)
				{
				  $('INPUT_In').checked=true;
			         $('INPUT_Out').checked=false;
				}
			else
				{
				  $('INPUT_In').checked=false;
			         $('INPUT_Out').checked=true;
				}
					
			$('input_c_src_ip_addr').value = G_SourceIP[i];
			$('input_c_src_ip_mask').value = G_SourceMask[i];
			$('input_c_dst_ip_addr').value = G_DestIP[i];
			$('input_c_dst_ip_mask').value = G_DestMask[i];
			Form.Select('select_c_dscp_check', G_DSCPCheck[i]);
			
			if(G_DSCPCheckEnable[i]==1)
				{
				  $('RADIO_SelectDSCPCheck').checked=true;
			         $('RADIO_SelectTosCheck').checked=false;
			         $('select_c_dscp_check').disabled = false;
				  $('input_Tos').disabled = true;
				}
			else
				{
				  $('RADIO_SelectDSCPCheck').checked=false;
			         $('RADIO_SelectTosCheck').checked=true;
			         $('select_c_dscp_check').disabled = true;
				  $('input_Tos').disabled = false;
				}
			$('input_Tos').value = G_TosCheck[i];			
			Form.Select('select_c_protocol', G_Protocol[i]);
			$('input_c_src_port').value = join_port(G_SourcePort[i],G_SourcePortRangeMax[i]);
			$('input_c_dst_port').value = join_port(G_DestPort[i],G_DestPortRangeMax[i]) ;
			
			Form.Select('select_c_queue', G_ClassQueue[i]);
			
			$('classify_config').style.display = 'block';
			select_c_protocol_change();
			$('Menu').style.height=1110 + 'px';
            $('Content').style.height=$('Menu').style.height;
			return;
		}
		
	}
	if (i == G_Class.length) {
		if(!confirm(SEcode[1009]))
		 return false;
	}
}
 

function uiAdd()
{   
	$('classify_config').style.display = 'block';
	$('Menu').style.height=1110 + 'px';
    $('Content').style.height=$('Menu').style.height;
	
}

function get_dscp(str)
{
	for (var i = 0; i < $('select_c_dscp_check').length; i++) {
		if ($('select_c_dscp_check').options[i].value == str)
			return $('select_c_dscp_check').options[i].text.replace(/([^\(]+)(\S+)/, "$1");
	}
	return str;
}

function view(i)
{
			$('classify_config').style.display = 'block';
			$('apply').style.display = 'none';
			$('input_c_name').value = G_Description[i];
			Form.Checkbox('input_c_enable',G_Enable[i]);
			Form.Select('selectConnectionName', G_WanConnectionName[i]);
			if(G_Status[i]==1)
				{
				  $('INPUT_In').checked=true;
			         $('INPUT_Out').checked=false;
				}
			else
				{
				  $('INPUT_In').checked=false;
			         $('INPUT_Out').checked=true;
				}
			$('input_c_src_ip_addr').value = G_SourceIP[i];
			$('input_c_src_ip_mask').value = G_SourceMask[i];
			$('input_c_dst_ip_addr').value = G_DestIP[i];
			$('input_c_dst_ip_mask').value = G_DestMask[i];
			if(G_DSCPCheckEnable[i]==1)
				{
				  $('RADIO_SelectDSCPCheck').checked=true;
			         $('RADIO_SelectTosCheck').checked=false;
				  $('select_c_dscp_check').disabled = false;
				  $('input_Tos').disabled = true;
				}
			else
				{
				  $('RADIO_SelectDSCPCheck').checked=false;
			         $('RADIO_SelectTosCheck').checked=true;
				  $('select_c_dscp_check').disabled = true;
				  $('input_Tos').disabled = false;
				}
			$('input_Tos').value = G_TosCheck[i];	
			Form.Select('select_c_dscp_check', G_DSCPCheck[i]);
			Form.Select('select_c_protocol', G_Protocol[i]);
			$('input_c_src_port').value = join_port(G_SourcePort[i],G_SourcePortRangeMax[i]);
			$('input_c_dst_port').value = join_port(G_DestPort[i],G_DestPortRangeMax[i]) ;
			
			Form.Select('select_c_queue', G_ClassQueue[i]);
			select_c_protocol_change();
}

function uiOnload()
{
	var array_value = [];
		for(var i = 0; i < G_Class.length; i++){
			array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="choose'+i+'" name="choose'+i+'">'); 
			array_value[i].push(G_Description[i]);
			array_value[i].push(G_ClassQueue[i]== '-1'? data_language.dynamic.noassign : G_ClassQueue[i]);
			array_value[i].push(G_Enable[i]);
			array_value[i].push('<input type="button" id="button'+i+'" name="button'+i+'" onClick="view('+i+')" value=' +  data_language.dynamic.view + ' >');
                   
		}
		if($('RADIO_SelectDSCPCheck').checked)
				{
				  $('RADIO_SelectDSCPCheck').checked=true;
			         $('RADIO_SelectTosCheck').checked=false;
				  $('select_c_dscp_check').disabled = false;
				  $('input_Tos').disabled = true;
				}
			
		$T('class_table',array_value);
		wanPath();
		select_c_protocol_change();
}
function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = []; 
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
