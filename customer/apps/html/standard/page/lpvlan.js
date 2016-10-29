var G_LanWlanPort = [];
var m = 0;
//获取LAN端口的device name和路径
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_DeviceName"
`	G_LanWlanPort[m] = [];
    G_LanWlanPort[m][0] = "$01";//X_TWSZ-COM_DeviceName
    G_LanWlanPort[m][1] = "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.$00";
	m++;
`?>
//获取WLAN端口的device name和路径
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "X_TWSZ-COM_DeviceName"
`	G_LanWlanPort[m] = [];
    G_LanWlanPort[m][0] = "$01";//X_TWSZ-COM_DeviceName
    G_LanWlanPort[m][1] = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00";
	m++;
`?>

var G_VLAN = [];
var n =0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_LanPortVLAN.VLAN. "Enable PortPath LinkType PVID VLANIDList UntagVLANIDList"
`	G_VLAN[n] = [];
    G_VLAN[n][0] = "$01";//Enable
    G_VLAN[n][1] = "$02";//PortPath
    G_VLAN[n][2] = "$03";//LinkType
    G_VLAN[n][3] = "$04";//PVID
    G_VLAN[n][4] = "$05";//VLANIDList
    G_VLAN[n][5] = "$06";//UntagVLANIDList
    G_VLAN[n][6] = "InternetGatewayDevice.X_TWSZ-COM_LanPortVLAN.VLAN.$00.";//path
	n++;
`?>

var selectPort = [];
function uiOnload(_arc){

    if(_arc != 'false')
	{
	    lanAndWlanPath();
		selectPort = G_LanWlanPort[0][1];
	}
	else
	{
	    selectPort = $('SELECT_PORT').value;
	}
	selectLanWlanPort(selectPort);
	onChgLinkType();
}

function selectLanWlanPort(port)
{
	$('SELECT_PORT').value = port;
	for(var i = 0; i < G_VLAN.length; i++)
	{
	    if(G_VLAN[i][1] == $('SELECT_PORT').value)
    	{
    	    $('INPUT_VLANEnable').checked 	= G_VLAN[i][0] == '1' ? true : false;
			$('SELECT_LinkType').value 		= G_VLAN[i][2];
			$('INPUT_PVID').value 			= G_VLAN[i][3];
			$('INPUT_VIDList').value 		= G_VLAN[i][4];
			$('INPUT_UntagVIDList').value 	= G_VLAN[i][5];
			break;
    	}
	}
}

function lanAndWlanPath()
{
	var _text = [],_value = [];

	for(var i = 0; i < G_LanWlanPort.length; i++)
	{
		_text.push(G_LanWlanPort[i][0]);
		_value.push(G_LanWlanPort[i][1]);
	}
	$S('SELECT_PORT',_text,_value);
}

function onChgLinkType()
{
	var Type = $('SELECT_LinkType').value;

	switch(Type)
	{
		case "access": {
			$('vidlist').style.display = 'none';
			$('untagvidlist').style.display = 'none';
			break;		
		}
		case "trunk": {
			$('vidlist').style.display = 'block';
			$('untagvidlist').style.display = 'none';
			break;		
		}
		case "hybird": {
			$('vidlist').style.display = 'block';
			$('untagvidlist').style.display = 'block';			
			break;		
		}
	}
}


function uiSubmit()
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

	var path;
	for(var i = 0; i < G_VLAN.length; i++)
	{
	    if(G_VLAN[i][1] == $('SELECT_PORT').value)
    	{
    	    path = ':'+ G_VLAN[i][6];
			break;
    	}
	}
    $F(path+'Enable', $('INPUT_VLANEnable').checked == true ? '1' : '0');
    $F(path+'PortPath', $('SELECT_PORT').value);
    $F(path+'LinkType', $('SELECT_LinkType').value);
    $F(path+'PVID', $('INPUT_PVID').value);
    $F(path+'VLANIDList', $('INPUT_VIDList').value);
    $F(path+'UntagVLANIDList', $('INPUT_UntagVIDList').value);

	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
