<?setvaronce var:state 0?>
<?mget :InternetGatewayDevice.TracerouteDiagnostics. "Host DiagnosticsState MaxHopCount Timeout X_TWSZ-COM_TracertResult"
`	var G_Host                = "$01";
	var G_TracerouteState     = "$02";
	var G_MaxTTL              = "$03";
	var G_WaitTime            = "$04";
	var G_TracertResult       = "$05";
	
`?>
	
var G_wanIPAddress = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ExternalIPAddress"
		`	G_wanIPAddress[m] = ["$01"];
			++m;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ExternalIPAddress"
		`	G_wanIPAddress[m] = ["$01"];
		    ++m;
		`?>
	`?>
`?>
								 
var data_dynamic = {
	lang_tracert_status					: 'Tranceroute Status',
	lang_cannot_resolve_hostname		: 'The host name is unknown',
	lang_unreachable_dest_net			: 'The destination is unreachable ',
	lang_no_error						: ' No Error  ',
	lang_internal_error					: ' Internal error ',
	lang_running						: ' Traceroute is running... ',
	lang_finished						: ' Traceroute has finished'
};



function uiOnload(){
	var Node_Output = '';
	
	$('btn_Stop').disabled = true;
	$('INPUT_Host').value = G_Host || '192.168.1.1';
	$('INPUT_MAXTTL').value =  G_MaxTTL || '1';
	$('INPUT_WaitTime').value = G_WaitTime || '10';
	
	with(data_dynamic)
    {
		switch (G_TracerouteState){
			case 'Complete':{
				Node_Output += lang_tracert_status + ': '+ lang_finished + unescape("%0a");
				Node_Output +=  Base64.Decode(G_TracertResult)+ unescape("%0a");				
				break;
			}
			case 'No_Error':{
				Node_Output += lang_tracert_status + ': '+ lang_running + unescape("%0a");
				Node_Output +=  Base64.Decode(G_TracertResult)+ unescape("%0a");	
				setTimer();			
				break;
			}
			case 'Error_CannotResolveHostName':{
				Node_Output += lang_tracert_status +  ': ' + lang_cannot_resolve_hostname;
				break;
			}
			case 'Error_MaxHopCountExceeded':{
				Node_Output += lang_tracert_status +  ': ' + lang_unreachable_dest_net;
				break;
			}
			case 'Error_MaxHopCountExceeded':{
				Node_Output +=  lang_tracert_status + ': ' + lang_internal_error + unescape("%0a");
				Node_Output +=  Base64.Decode(G_TracertResult) + unescape("%0a");
				break;
			}
			case 'Requested':{
				Node_Output +=  lang_tracert_status + ': ' + lang_running;
				setTimer();
				break;
			}
			case 'None': break;
		}
	}
	$('TEXTAREA_Output').value = Node_Output;
	
	dealWithError();
}

function uiSubmit(){
	var value_array = $('INPUT_Host','INPUT_MAXTTL','INPUT_WaitTime');
	var LanIPaddress = '';
	var LanIPaddress2 = '';
	LanIPaddress = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress ?>";
	LanIPaddress2 = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.IPInterfaceIPAddress ?>";

	$H({
		':InternetGatewayDevice.TracerouteDiagnostics.Host'             :value_array[0].value,
		':InternetGatewayDevice.TracerouteDiagnostics.MaxHopCount'			:value_array[1].value,
		':InternetGatewayDevice.TracerouteDiagnostics.Timeout'         :value_array[2].value,
		':InternetGatewayDevice.TracerouteDiagnostics.DiagnosticsState'  :'Requested',
		'var:state' :'1',
		'var:menu'      : G_Menu,
		'var:page'      : G_Page,
		'var:subpage'   : G_SubPage,
		'var:errorpage' : G_SubPage,
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'var:CacheLastData':ViewState.Save(),
		'obj-action':'set'
	});

	    $('uiPostForm').submit();
}

function uiStop(){
	$H({
		':InternetGatewayDevice.TracerouteDiagnostics.Interface'   :'',
		'var:menu'      : G_Menu,
		'var:page'      : G_Page,
		'var:subpage'   : G_SubPage,
		'var:errorpage' : G_SubPage,
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action':'set'
	},true);
	$('uiPostForm').submit();
}


function setTimer(){
	var Timer = setTimeout('uiPageRefresh()',5000);
	if(G_TracerouteState != 'Requested' && G_TracerouteState != 'No_Error'){
		clearTimeout(Timer);
	} else {
		$('btn_Traceroute').disabled = true;
		$('btn_Stop').disabled = false;
	}
}

function uiPageRefresh(){
	document.location.href = uiGetPageUrl() + '&var:state=1';
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	arrayHint['Host']          	= 'INPUT_Host';
	arrayHint['MaxHopCount'] 		= 'INPUT_MAXTTL';
	arrayHint['Timeout']       = 'INPUT_WaitTime';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);

