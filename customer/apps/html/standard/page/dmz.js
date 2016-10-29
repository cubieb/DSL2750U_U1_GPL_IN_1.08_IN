//DMZ
var G_Dmz = [];
var m = 0;
//
<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_DMZEnabled X_TWSZ-COM_DMZHost X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Dmz[m] = [];
			G_Dmz[m][0] = "$01";   // Name
			G_Dmz[m][1] = "$02";   // Enable
			G_Dmz[m][2] = "$03";   // Host
			G_Dmz[m][3] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.";   // Path
			G_Dmz[m][4] = "$04";
			G_Dmz[m][5] = "$05";
			m++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_DMZEnabled X_TWSZ-COM_DMZHost X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Dmz[m] = [];
			G_Dmz[m][0] = "$01";   // Name
			G_Dmz[m][1] = "$02";   // Enable
			G_Dmz[m][2] = "$03";   // Host
			G_Dmz[m][3] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00.";   // Path
			G_Dmz[m][4] = "$04";
			G_Dmz[m][5] = "$05";
			m++;
		`?>
	`?>
`?>
`?>
<?setvaronce var:wanConIndex 0?>
var wanConIndex = '<?echo $var:wanConIndex?>';

function uiOnload(){
	var _text = [], _value = [];
	for(var i = 0; i < G_Dmz.length; i++){
		if (G_Dmz[i][5]=="IP_Bridged"||G_Dmz[i][4]=="TR069"||G_Dmz[i][0]=="pppo3g")
		 {
			continue;
		}
		_text.push(G_Dmz[i][0]);
		_value.push(i);
	}
	$S('SELECT_WanConnection',_text,_value);

	
    $('SELECT_WanConnection').value= wanConIndex;

    setDmzValue();
    
}

function setDmzValue(){
	wanConIndex = $('SELECT_WanConnection').value;
	
	for(var i = 0; i < G_Dmz.length; i++){
		if(wanConIndex == i){
			Form.Checkbox('INPUT_DMZEnable',G_Dmz[i][1]);
			$('INPUT_DMZHostIP').value = G_Dmz[i][2];
			break;
		}
	}
}

function uiSubmit(){
	if(m == 0){
		DealWith.Wrong('SELECT_WanConnection',SEcode[1002]);
		return false;
	}
	var wanConn = G_Dmz[wanConIndex][3];
	if(wanConn == ''){
		DealWith.Wrong('SELECT_WanConnection',SEcode[1002]);
		return false;
	}
	
	$H({
		'obj-action': 'set',
		'var:menu'  : 'advanced',
		'var:page'  : 'dmz',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:wanConIndex'  : wanConIndex,
		'var:errorpage'    : 'dmz',
		'var:CacheLastData': ViewState.Save()
	},true);
	$F(wanConn + 'X_TWSZ-COM_DMZEnabled',Form.Checkbox('INPUT_DMZEnable'));
	$F(wanConn + 'X_TWSZ-COM_DMZHost',$('INPUT_DMZHostIP').value);
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);