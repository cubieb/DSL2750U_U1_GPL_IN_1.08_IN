//wlan status
var G_Wlan = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "Enable SSID X_TWSZ-COM_DeviceName BeaconType BasicAuthenticationMode BasicEncryptionModes WPAAuthenticationMode IEEE11iAuthenticationMode"
`	G_Wlan[m] = ['InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.', //path
				 '$01', //Enable
				 '$02', //SSID
				 '$03', //X_TWSZ-COM_DeviceName
				 '$04', //BeaconType
				 '$05', //BasicAuthenticationMode
				 '$06', //BasicEncryptionModes
				 '$07', //WPAAuthenticationMode
				 '$08'  //IEEE11iAuthenticationMode
				 ];
	m++;
`?>

var G_W_Status = [];
var n = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "TotalAssociations"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$20.AssociatedDevice. "AssociatedDeviceMACAddress X_TWSZ-COM_AssociatedDeviceState"
		`	G_W_Status[n] = ["InternetGatewayDevice.LANDevice.1.WLANConfiguration.$20.",
							 "$01",
							 "$02"
							 ];
			n++;
		`?>
	`?>
`?>

function createTable(){
	var array_wlan = [];
	
	for(var i = 0, _len = G_W_Status.length; i < _len; i++){
		if(G_W_Status[i][2] == '1'){
			array_wlan[i] = [G_W_Status[i][1], //Mac
						  	 'Connected', //Associated
						  	 displayStatus(G_W_Status[i][0],'Authorized'), //Authorized
						  	 displayStatus(G_W_Status[i][0],'SSID'), //SSID
						  	 displayStatus(G_W_Status[i][0],'Interface')  //Interface
						  	 ];
		}
	}
	
	$T('td_wlan',array_wlan);
}

function displayStatus(_path,_type){
	var _i = 0;
	
	for(var i = 0, _len = G_Wlan.length; i < _len; i++){
		if(G_Wlan[i][0] == _path){
			_i = i;
			break;
		}
	}	
	
	switch(_type){
		case 'Authorized' : {
			//BeaconType:None/Basic/WPA/11i/WPAand11i
			var type_wlan = G_Wlan[_i][4], auth_basic = G_Wlan[_i][5], encry_basic = G_Wlan[_i][6];
			
			return type_wlan == 'Basic' && auth_basic == 'None' && encry_basic == 'None' ? 'None' : 
				    type_wlan == 'Basic' && auth_basic == 'None' && encry_basic == 'WEPEncryption' ? 'Open' :
				    type_wlan == 'Basic' && auth_basic == 'SharedAuthentication' && encry_basic == 'WEPEncryption' ? 'Share' :
					type_wlan == 'WPA' ? 'WPA only' :
					type_wlan == 'WPAand11i' ? 'Auto(WPA or WPA2)' :
					type_wlan == '11i' ? 'WPA2 only' : 'None';
		}
		case 'SSID' : {
			return G_Wlan[_i][2];
		}
		case 'Interface' : {
			return G_Wlan[_i][3];
		}		
	}
}

addListeners(createTable);
