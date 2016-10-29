
var EntryNum=0;
var G_Entry=[];
<?if lt 0 `<?get :InternetGatewayDevice.X_TWSZ-COM_TraditionalNAT.TraditionalNATNumberOfEntries ?>`
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_TraditionalNAT.TraditionalNATEntry. "X_TWSZ-COM_EntryName X_TWSZ-COM_InternalIPType X_TWSZ-COM_StartInternalIP X_TWSZ-COM_EndInternalIP X_TWSZ-COM_ExternalIPType X_TWSZ-COM_StartExternalIP X_TWSZ-COM_EndExternalIP"
		`	G_Entry[EntryNum] = [];
			G_Entry[EntryNum][0] = "InternetGatewayDevice.X_TWSZ-COM_TraditionalNAT.TraditionalNATEntry.$00.";    //path
			G_Entry[EntryNum][1] = "$01";   	//X_TWSZ-COM_EntryName
			G_Entry[EntryNum][2] = "$02";   	//X_TWSZ-COM_InternalIPType
			G_Entry[EntryNum][3] = "$03";   	//X_TWSZ-COM_StartInternalIP
			G_Entry[EntryNum][4] = "$04";   	//X_TWSZ-COM_EndInternalIP
			
			G_Entry[EntryNum][5] = "$05";   	//X_TWSZ-COM_ExternalIPType
			G_Entry[EntryNum][6] = "$06";   	//X_TWSZ-COM_StartExternalIP 
			G_Entry[EntryNum][7] = "$07";   	//X_TWSZ-COM_EndExternalIP
			++EntryNum;
	`?>
`?>

var multinatmode = '<?get :InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TELEFONICA-ES_MultiNATMode?>';
var netmask = '<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask ?>';

<?if eq 1 `<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPServerEnable ?>`
	` netmask = '<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.SubnetMask?>';
`?>

function createTable(){
	
	Table.Clear('Table_ip_list');
	var InternalIPShow=0;
	var ExternalIPShow=0;
	
	var array_value = [];
	for(var i = 0; i < G_Entry.length; i++){

		switch(G_Entry[i][2]){
			case 'SingleIP' :
				InternalIPShow=G_Entry[i][3];
				break;
			case 'IPRange' :	
				InternalIPShow=G_Entry[i][3]+'-'+G_Entry[i][4];
				break;
		}

		switch(G_Entry[i][5]){
			case 'SingleIP' :
				ExternalIPShow=G_Entry[i][6];
				break;
			case 'IPRange' :	
				ExternalIPShow=G_Entry[i][6]+'-'+G_Entry[i][7];
				break;
		}
		
		array_value[i] = [];
		array_value[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">');
		array_value[i].push(G_Entry[i][1]+'&nbsp;'); //Description
		array_value[i].push(InternalIPShow+'&nbsp;'); //Internal IP
		array_value[i].push(ExternalIPShow+'&nbsp;'); //External IP 

	}
	$T('Table_ip_list',array_value);
}

function uiOnload(){

	createTable();
	
}

function changeInternalIPType(){
	
	var value_mode = $('SELECT_InternalIPType').value;
	
	switch(value_mode){
		case 'SingleIP' : {
			$('InternalSingleIPArea').style.display='block';
			$('InternalIPRangeArea').style.display='none';
			$('InternalIPAddress').value='';
			break;
		}
		case 'IPRange':{
			$('InternalSingleIPArea').style.display='none';
			$('InternalIPRangeArea').style.display='block';
			break;
		}
	}
	
}


function changeExternalIPType(){
	var value_mode = $('SELECT_ExternalIPType').value;
	
	switch(value_mode){
		case 'SingleIP' : {
			$('ExternalSingleIPArea').style.display='block';
			$('ExternalIPRangeArea').style.display='none';
			$('ExternalIPAddress').value='';
			break;
		}
		case 'IPRange':{
			$('ExternalSingleIPArea').style.display='none';
			$('ExternalIPRangeArea').style.display='block';
			break;
		}
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
	
	addAction = 1;
	
  	$('addEntry').disabled=true;
	$('editEntry').disabled=true;
	$('removeEntry').disabled=true;
	$('EntryInfo').style.display='block';
	switch(multinatmode)
	{
		case 'One-to-One' :
			 $('SELECT_InternalIPType').value = 'SingleIP' ;
		     $('SELECT_ExternalIPType').value = 'SingleIP' ;
			 break;
		case 'Many-to-One':
			 $('SELECT_InternalIPType').value = 'IPRange' ;
		     $('SELECT_ExternalIPType').value = 'SingleIP' ;
			 break;
		case 'Many-to-Many':
			 $('SELECT_InternalIPType').value = 'IPRange' ;
		     $('SELECT_ExternalIPType').value = 'IPRange' ;
			 break;
		case 'Server':
			 $('SELECT_InternalIPType').value = 'SingleIP' ;
		     $('SELECT_ExternalIPType').value = 'IPRange' ;
			 break;		
	}
	changeInternalIPType();
  	changeExternalIPType();

}

function EditEntry(){
	
	 if(_array_id.length != 1){
		alert(SEcode[1009]);
		return false;
	}
	addAction = 2;

	for(var j = 0; j < G_Entry.length; j++){
		var EditIndexID="rule_index"+j;
		
		var EditIndex=Form.Checkbox(EditIndexID);
		if(EditIndex=='1'){
			
			$('addEntry').disabled=true;
			$('editEntry').disabled=true;
			$('removeEntry').disabled=true;
			$('EntryInfo').style.display='block';	

			editIdx=j;
			$('INPUT_EntryName').value=G_Entry[j][1];

			$('SELECT_InternalIPType').value=G_Entry[j][2];
			
			changeInternalIPType();
			
			if(G_Entry[j][2]=='SingleIP'){
				$('InternalIPAddress').value=G_Entry[j][3];
			}else if(G_Entry[j][2]=='IPRange'){
				$('StartInternalIPAddress').value=G_Entry[j][3];
				$('EndInternalIPAddress').value=G_Entry[j][4];		
			}

			$('SELECT_ExternalIPType').value=G_Entry[j][5];
			
			changeExternalIPType();
			
			if(G_Entry[j][5]=='SingleIP'){
				$('ExternalIPAddress').value=G_Entry[j][6];
			}else if(G_Entry[j][5]=='IPRange'){
				$('StartExternalIPAddress').value=G_Entry[j][6];
				$('EndExternalIPAddress').value=G_Entry[j][7];		
			}			
			
		}
	}
}

function RemoveEntry(){
	
	//if(_array_id.length != 1){
	//	alert(SEcode[1009]);
	//	return false;
	//}
	if(!confirm(SEcode[1008])){return false;}
	for(var j = 0; j < G_Entry.length; j++){
		var DelIndexID="rule_index"+j;
		var DelIndex=Form.Checkbox(DelIndexID);
		if(DelIndex=='1'){
			
			
			$H({
				'del-obj'   :G_Entry[j][0],
				'obj-action':'del',
				'var:menu'  :'advanced',
				'var:page'  :'nat',
				'getpage'   :'html/index.html',
				'errorpage' :'html/index.html',
				'var:subpage'   :'nat',
				'var:errorpage':'nat'
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

function ipToNumber(ip) {  
	var numbers = ip.split(".");  
	return parseInt(numbers[0]) * 256 * 256 * 256 +   
	parseInt(numbers[1]) * 256 * 256 +   
	parseInt(numbers[2]) * 256 +   
	parseInt(numbers[3]);  
} 

function uiSumbit(){
	var Value_Nodes = $('INPUT_EntryName','SELECT_InternalIPType','InternalIPAddress','StartInternalIPAddress','EndInternalIPAddress','SELECT_ExternalIPType','ExternalIPAddress','StartExternalIPAddress','EndExternalIPAddress');
  var StartInternalIP = 0;
  var EndInternalIP = 0; 
  
  var StartExternalIP = 0;
  var EndExternalIP = 0;
  var InnerIP = 0;
  var ExterIP = 0;
  var NetMask = 0;

  if(Value_Nodes[1].value == 'SingleIP'){
  	StartInternalIP = Value_Nodes[2].value;
  	EndInternalIP = '';; 
  }else{
  	StartInternalIP = Value_Nodes[3].value;
  	EndInternalIP = Value_Nodes[4].value;
  }

  if(Value_Nodes[5].value == 'SingleIP'){
  	StartExternalIP = Value_Nodes[6].value;
  	EndExternalIP = '';; 
  }else{
  	StartExternalIP = Value_Nodes[7].value;
  	EndExternalIP = Value_Nodes[8].value;
  } 
  
	for(var i = 0; i < G_Entry.length; i++){
		//when edit, don't cmp current edit entry.
		if(addAction=='2' && editIdx==i)
			continue;
                 
		if ( StartInternalIP == G_Entry[i][3] && EndInternalIP == G_Entry[i][4] )
		{
			alert(SEcode[1014]);
			return false;
		}
	}

	NetMask = ipToNumber(netmask);
	InnerIP = ipToNumber(StartInternalIP);
	ExterIP = ipToNumber(StartExternalIP);
	if ( (InnerIP & NetMask) == (ExterIP & NetMask) )
	{
		alert(SEcode[1015]);
		return false;
	}
	
	if(addAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TWSZ-COM_TraditionalNAT.TraditionalNATEntry.',
			':X_TWSZ-COM_EntryName'	: 	Value_Nodes[0].value,
			':X_TWSZ-COM_InternalIPType'	:  Value_Nodes[1].value,
			':X_TWSZ-COM_StartInternalIP'   : StartInternalIP,
			':X_TWSZ-COM_EndInternalIP'    	: EndInternalIP,
	
			':X_TWSZ-COM_ExternalIPType'		: Value_Nodes[5].value,
			':X_TWSZ-COM_StartExternalIP'   :	StartExternalIP,
			':X_TWSZ-COM_EndExternalIP'    	: EndExternalIP,

			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'nat',
			'var:page' : 'nat',
			'var:errorpage'    : 'nat',
			'var:CacheLastData': ViewState.Save()
		},true);

	}else if(addAction=='2'){     //edit
	
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'nat',
			'var:page' : 'nat',
			'var:errorpage'    : 'nat',
			'var:CacheLastData': ViewState.Save()
		},true);		
		
		var path=':'+G_Entry[editIdx][0];
		$F(path+'X_TWSZ-COM_EntryName',Value_Nodes[0].value);
		$F(path+'X_TWSZ-COM_InternalIPType',Value_Nodes[1].value);
		
		if(Value_Nodes[1].value == 'SingleIP'){
			$F(path+'X_TWSZ-COM_StartInternalIP',Value_Nodes[2].value);
			$F(path+'X_TWSZ-COM_EndInternalIP','');
		}
		
		if(Value_Nodes[1].value == 'IPRange'){
			$F(path+'X_TWSZ-COM_StartInternalIP',Value_Nodes[3].value );
			$F(path+'X_TWSZ-COM_EndInternalIP',Value_Nodes[4].value);
		}
		
		$F(path+'X_TWSZ-COM_ExternalIPType',Value_Nodes[5].value);
		
		if(Value_Nodes[5].value == 'SingleIP'){
			$F(path+'X_TWSZ-COM_StartExternalIP',Value_Nodes[6].value);
			$F(path+'X_TWSZ-COM_EndExternalIP','');
		}
		
		if(Value_Nodes[5].value == 'IPRange'){
			$F(path+'X_TWSZ-COM_StartExternalIP',Value_Nodes[7].value );
			$F(path+'X_TWSZ-COM_EndExternalIP',Value_Nodes[8].value);

		}

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
