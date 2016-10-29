
<?mget :InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration. "Enable"
	`var WDS_Enable="$01";
`?>


<?mget :InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Repeater. "MACAddress1 MACAddress2 MACAddress3 MACAddress4"
	`var WDS_MACAddress1="$01";
	 var WDS_MACAddress2="$02";
	 var WDS_MACAddress3="$03";
	 var WDS_MACAddress4="$04";
`?>

var WirelessMAC ="<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BSSID?>";
//var DeviceIp="<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress?>" ;

function uiOnload()
{
	//alert(DeviceIp);
	//Form.Radio('INPUT_radiomode',WDS_WDSMode);
	supplyValue('INPUT_WirelessMAC',WirelessMAC);
	setJSONValue({
	'INPUT_EnableRepeater'	: WDS_Enable,
	//'ReapterIP'	: WDS_ReIPAddress,
	//'BaseMAC'	: WDS_BaseMACAddress,
	'INPUT_MAC1' : WDS_MACAddress1,
	'INPUT_MAC2' : WDS_MACAddress2,
	'INPUT_MAC3' : WDS_MACAddress3,
	'INPUT_MAC4' : WDS_MACAddress4
	});	
	//setIPValue();
	//ClickEnableRepeater();
	dealWithError();
}
/*
function setIPValue()
{
	var tmpIP;
	if(WDS_ReIPAddress=='')
	{
		tmpIP=setDefaultIPValue();
	}
	else
	{
		tmpIP=WDS_ReIPAddress || '';
	}
	//OnloadIP4hops(tmpIP,'INPUTRepeaterIpAddr_1','INPUTRepeaterIpAddr_2','INPUTRepeaterIpAddr_3','INPUTRepeaterIpAddr_4');
}

function setDefaultIPValue()
{
	var tmpDMZServer;
	var tmpLanIP;
	var tmpLanMask;
	var tmpIP=new Array(4);
	tmpLanIP=G_lan_ip_addr.split(".");
	tmpLanMask=G_lan_mask.split(".");
	for(var j=0; j<4; j++)
	{
		tmpIP[j]=tmpLanIP[j]&tmpLanMask[j];
		if(tmpIP[j]==0)
			tmpIP[j]='';
	}
	tmpDMZServer=tmpIP[0]+'.'+tmpIP[1]+'.'+tmpIP[2]+'.'+tmpIP[3];
	return tmpDMZServer;
}

function SelectMode()
{
	if($('INPUT_EnableRepeater').checked)
		{
			if($('Radio_repeater').checked)
				{	
					//$('ReapterIP').disabled=false;
					disCtrl('ReapterIP',1);
					$('BaseMAC').disabled=false;
					$('INPUT_MAC1').disabled=true;
					$('INPUT_MAC2').disabled=true;
					$('INPUT_MAC3').disabled=true;
					$('INPUT_MAC4').disabled=true;	
				}
			else if($('Radio_basestation').checked)
				{	
					//$('ReapterIP').disabled=true;
					disCtrl('ReapterIP',0);
					$('BaseMAC').disabled=true;
					$('INPUT_MAC1').disabled=false;
					$('INPUT_MAC2').disabled=false;
					$('INPUT_MAC3').disabled=false;
					$('INPUT_MAC4').disabled=false;
				}
			else
				{
				;
				}
		}
}

function ClickEnableRepeater()
{
	if($('INPUT_EnableRepeater').checked)
		{
			$('Radio_repeater').disabled = false;
			$('Radio_basestation').disabled = false;
			//$('INPUT_EnableClient').disabled = false;
//			$('ReapterIP').disabled=false;
//			$('BaseMAC').disabled=false;
//			$('INPUT_MAC1').disabled=false;
//			$('INPUT_MAC2').disabled=false;
//			$('INPUT_MAC3').disabled=false;
//			$('INPUT_MAC4').disabled=false;
			SelectMode();
		}
	else
		{
			$('Radio_repeater').disabled=true;
			$('Radio_basestation').disabled=true;
			//$('INPUT_EnableClient').disabled=true;
			//$('ReapterIP').disabled=true;
			disCtrl('ReapterIP',0);
			$('BaseMAC').disabled=true;
			$('INPUT_MAC1').disabled=true;
			$('INPUT_MAC2').disabled=true;
			$('INPUT_MAC3').disabled=true;
			$('INPUT_MAC4').disabled=true;
			

		}
}
*/
function uiSubmit()
{
	//alert(Form.Radio('INPUT_radiomode'));
	var nodes  = $('INPUT_EnableRepeater','INPUT_MAC1','INPUT_MAC2','INPUT_MAC3','INPUT_MAC4');

	$H({
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Enable': nodes[0].checked==true?'1':'0',
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Repeater.MACAddress1': nodes[1].value,
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Repeater.MACAddress2': nodes[2].value,
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Repeater.MACAddress3': nodes[3].value,
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_WDSConfiguration.Repeater.MACAddress4': nodes[4].value
		});
		
	$H({
	    
		'obj-action'	: 'set',
		'var:menu'		: 'advanced',
		'var:page'		: 'wadvance',
		'var:subpage'	: 'wireless_repeater',
		'var:errorpage'	: 'wireless_repeater',
		'getpage'		: 'html/index.html',
		'errorpage'		: 'html/index.html',
		'var:CacheLastData': ViewState.Save()
		});

	$('uiPostForm').submit();

}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}

	var arrayHint = [];

	dealErrorMsg(arrayHint, G_Error_Msg);

}

addListeners(uiOnload);


