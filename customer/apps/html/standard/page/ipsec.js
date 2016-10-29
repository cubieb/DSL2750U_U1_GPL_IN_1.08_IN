var wanConnection = new Array();
var n = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00";//path
			++n;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name"
		`	wanConnection[n] = new Array();
			wanConnection[n][0] = "$01";//name
			wanConnection[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00";//path
			++n;
		`?>
	`?>
`?>

var IPSECEnable = "<?get :InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSECEnable?>";

var IPSECList = new Array();
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSEC. "ConnectionName TunnelMode RemoteGW LocalIPAddr LocalIPSubnetMask RemoteIPAddr RemoteIPSubnetMask KeyExchangeMethod PreSharedKey PerfectForwardSecrecyEnable Mode EncryptionAlgorithm_Auto1 IntegrityAlgorithm_Auto1 DHGroup KeyLifeTime1"
`	IPSECList[n] = new Array();
	IPSECList[n][0]  = "InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSEC.$00.";	//path
	IPSECList[n][1]  = "$01"; //ConnectionName
	IPSECList[n][2]  = "$02"; //TunnelMode
	IPSECList[n][3]  = "$03"; //RemoteGW
	IPSECList[n][4]  = "$04"; //LocalIPAddr
	IPSECList[n][5]  = "$05"; //LocalIPSubnetMask
	IPSECList[n][6]  = "$06"; //RemoteIPAddr
	IPSECList[n][7]  = "$07"; //RemoteIPSubnetMask
	IPSECList[n][8]  = "$08"; //KeyExchangeMethod
	IPSECList[n][9]  = "$09"; //PreSharedKey
	IPSECList[n][10] = "$0a"; //PerfectForwardSecrecyEnable
	IPSECList[n][11] = "$0b"; //Mode
	IPSECList[n][12] = "$0c"; //EncryptionAlgorithm_Auto1
	IPSECList[n][13] = "$0d"; //IntegrityAlgorithm_Auto1
	IPSECList[n][14] = "$0e"; //DHGroup
	IPSECList[n][15] = "$0f"; //KeyLifeTime1
	n++;
`?>

n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSEC. "EncryptionAlgorithm_Auto2 IntegrityAlgorithm_Auto2 PFSGroup KeyLifeTime2 EncryptionAlgorithm_Manual EncryptionKey IntegrityAlgorithm_Manual IntegrityKey SPI Interface"
`	IPSECList[n][16] = "$01"; //EncryptionAlgorithm_Auto2
	IPSECList[n][17] = "$02"; //IntegrityAlgorithm_Auto2
	IPSECList[n][18] = "$03"; //PFSGroup
	IPSECList[n][19] = "$04"; //KeyLifeTime2
	IPSECList[n][20] = "$05"; //EncryptionAlgorithm_Manual
	IPSECList[n][21] = "$06"; //EncryptionKey
	IPSECList[n][22] = "$07"; //IntegrityAlgorithm_Manual
	IPSECList[n][23] = "$08"; //IntegrityKey
	IPSECList[n][24] = "$09"; //SPI
	IPSECList[n][25] = "$0a"; //Interface
	n++;
`?>

var option = '-';
var path_index = -1;

function uiOnload()
{
	if (IPSECEnable == 1)
		$('IPSECEnabled').checked = true;
	else
		$('IPSECEnabled').checked = false;

	createTable();
	initParameter();
	doKeyMethod();
	dealWithError();
}

function createTable()
{
	Table.Clear('IPSecList');

	var array_value = [];
	for (var i = 0; i < IPSECList.length; i++)
	{
		{
			array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="edit'+i+'" onclick="$id('+i+')">');
			array_value[i].push(IPSECList[i][1]); //ConnectionName
			array_value[i].push(IPSECList[i][3]); //RemoteGW
			array_value[i].push(IPSECList[i][4]); //LocalIPAddr
			array_value[i].push(IPSECList[i][6]); //RemoteIPAddr

			for (var x = 0; x < wanConnection.length; x++)
			{
				if (IPSECList[i][25] == wanConnection[x][1])
					array_value[i].push(wanConnection[x][0]); //interface
			}
		}
	}
	$T('IPSecList', array_value);
}

function initParameter()
{
	/* Init Connection Name */
	$('ipsecname').value = "name";

	/* Init Tunnel Mode */
	var _text = [], _value = [];
	_text[0] = "ESP";
	_text[1] = "AH";
	_value[0] = "esp";
	_value[1] = "ah";
	$S('tunnelmode', _text, _value);

	/* Init Remote IPSEC GW */
	$('gw').value = "0.0.0.0";

	/* Init Tunnel local/remote IP addr */
	var _text = [], _value = [];
	_text[0] = "Subnet";
	_text[1] = "Single Address";
	_value[0] = "subnet";
	_value[1] = "single";
	$S('TunnelLocalAddr', _text, _value);
	$S('TunnelRemoteAddr', _text, _value);

	/* Init Local/remote IP Address */
	$('LocalIP').value = "0.0.0.0";
	$('LocalMask').value = "255.255.255.0";
	$('RemoteIP').value = "0.0.0.0";
	$('RemoteMask').value = "255.255.255.0";

	/* Init Key Exchange Method */
	var _text = [], _value = [];
	_text[0] = "Auto(IKE)";
	_text[1] = "Manual";
	_value[0] = "auto";
	_value[1] = "manual";
	$S('KeyMethod', _text, _value);

	/* Init Pre-Shard Key */
	$('ShardKey').value = "key";

	/* Init Mode */
	var _text = [], _value = [];
	_text[0] = "Main";
	_text[1] = "Aggressive";
	_value[0] = "main";
	_value[1] = "aggressive";
	$S('Mode', _text, _value);

	/* Init Encryption Algorithm 1/2 */
	var _text = [], _value = [];
	_text[0] = "DES";
	_text[1] = "3DES";
	_text[2] = "AES-128";
	_text[3] = "AES-192";
	_text[4] = "AES-256";
	_value[0] = "des";
	_value[1] = "3des";
	_value[2] = "aes128";
	_value[3] = "aes192";
	_value[4] = "aes256";
	$S('EncryAlg1', _text, _value);

	var _text = [], _value = [];
	_text[0] = "DES";
	_text[1] = "3DES";
	_value[0] = "des";
	_value[1] = "3des";
	$S('EncryAlg2', _text, _value);

	/* Init Encryption Algorithm 1/2 */
	var _text = [], _value = [];
	_text[0] = "MD5";
	_text[1] = "SHA1";
	_value[0] = "md5";
	_value[1] = "sha1";
	$S('IntegrAlg1', _text, _value);

	var _text = [], _value = [];
	_text[0] = "MD5";
	_text[1] = "SHA1";
	_value[0] = "hmac_md5";
	_value[1] = "hmac_sha1";
	$S('IntegrAlg2', _text, _value);

	/* Init Encryption Algorithm 1/2 */
	var _text = [], _value = [];
	_text[0] = "768bit";
	_text[1] = "1024bit";
	_text[2] = "1536bit";
	_text[3] = "2048bit";
	_text[4] = "3072bit";
	_text[5] = "4096bit";
	_value[0] = "768";
	_value[1] = "1024";
	_value[2] = "1536";
	_value[3] = "2048";
	_value[4] = "3072";
	_value[5] = "4096";
	$S('GroupKey1', _text, _value);

	/* Init Key Life Time */
	$('KeyLifeTime1').value = 3600;
	$('KeyLifeTime2').value = 3600;

	/* Init Encryption Algorithm in manual */
	var _text = [], _value = [];
	_text[0] = "DES";
	_text[1] = "3DES";
	_value[0] = "des-cbc";
	_value[1] = "3des-cbc";
	$S('EncryAlg3', _text, _value);

	/* Init Auth Algorithm 1/2 */
	var _text = [], _value = [];
	_text[0] = "MD5";
	_text[1] = "SHA1";
	_value[0] = "hmac-md5";
	_value[1] = "hmac-sha1";
	$S('AuthAlg', _text, _value);

	/* Init SPI */
	$('SPI').value = 101;

	/* Init WAN Interface */
	var _text = [], _value = [];
	for (var i = 0; i < wanConnection.length; i++)
	{
		_text[i]  = wanConnection[i][0];
		_value[i] = i;
	}
	$S('selectConnectionName', _text, _value);
}

function doKeyMethod()
{
	var KeyMethod = $('KeyMethod').value;

	if (KeyMethod == "auto")
	{
		$('KeyManual').style.display = 'none';
		$('KeyAuto').style.display = 'block';
	}
	else
	{
		$('KeyAuto').style.display = 'none';
		$('KeyManual').style.display = 'block';
	}
}

function doLocalIP()
{
	var IPType = $('TunnelLocalAddr').value;

	if (IPType == "single")
		$('LocalVPN').style.display = 'none';
	else
		$('LocalVPN').style.display = 'block';
}

function doRemoteIP()
{
	var IPType = $('TunnelRemoteAddr').value;

	if (IPType == "single")
		$('RemoteVPN').style.display = 'none';
	else
		$('RemoteVPN').style.display = 'block';
}

function uiIPSECEnabled()
{
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSECEnable' : Form.Checkbox('IPSECEnabled'),
		'obj-action'        : 'set',
		'getpage'           : 'html/index.html',
		'errorpage'         : 'html/index.html',
		'var:menu'          : 'advanced',
		'var:page'          : 'ipsec',
		'var:errorpage'     : 'ipsec'
	});

	$('uiPostForm').submit();
}

function uiCheckRepeatedAccount()
{
	var _nodes = $('ipsecname');

	for (var i = 0; i < IPSECList.length; i++)
	{
		if (IPSECList[i][1] == _nodes.value)
			return false;
	}

	return true;
}

function uiSubmit()
{
	var connection = $('ipsecname', 'tunnelmode', 'gw');
	var localIPRang = $('TunnelLocalAddr', 'LocalIP', 'LocalMask');
	var remoteIPRang = $('TunnelRemoteAddr', 'RemoteIP', 'RemoteMask');
	var keyMethod = $('KeyMethod', 'ShardKey');
	var phase1 = $('Mode', 'EncryAlg1', 'IntegrAlg1', 'GroupKey1', 'KeyLifeTime1');
	var phase2 = $('EncryAlg2', 'IntegrAlg2', 'KeyLifeTime2');
	var manual = $('EncryAlg3', 'EncryKey', 'AuthAlg', 'AuthKey', 'SPI');
	var wanIndex = $('selectConnectionName');
	var wanPath = wanConnection[wanIndex.value][1];

	if (option == 'add')
	{
		if (!uiCheckRepeatedAccount())
		{
			alert(SEcode[1014]);
			$('ipsecname').focus();
			return false;
		}
		$H({
			'add-obj'          : 'InternetGatewayDevice.X_TWSZ-COM_IPSEC.IPSEC.',
			':ConnectionName'  : connection[0].value,
			':TunnelMode'      : connection[1].value,
			':RemoteGW'        : connection[2].value,

			':LocalIPAddr'        : localIPRang[1].value,
			':LocalIPSubnetMask'  : localIPRang[0].value == "subnet" ? localIPRang[2].value : "255.255.255.255",

			':RemoteIPAddr'        : remoteIPRang[1].value,
			':RemoteIPSubnetMask'  : remoteIPRang[0].value == "subnet" ? remoteIPRang[2].value : "255.255.255.255",

			':KeyExchangeMethod'   : keyMethod[0].value,
			':PreSharedKey'        : keyMethod[1].value,

			':Mode'                        : keyMethod[0].value == "auto" ? phase1[0].value : "",
			':EncryptionAlgorithm_Auto1'   : keyMethod[0].value == "auto" ? phase1[1].value : "",
			':IntegrityAlgorithm_Auto1'    : keyMethod[0].value == "auto" ? phase1[2].value : "",
			':DHGroup'                     : phase1[3].value,
			':KeyLifeTime1'                : phase1[4].value,

			':EncryptionAlgorithm_Auto2'   : keyMethod[0].value == "auto" ? phase2[0].value : "",
			':IntegrityAlgorithm_Auto2'    : keyMethod[0].value == "auto" ? phase2[1].value : "",
			':KeyLifeTime2'                : phase2[2].value,

			':EncryptionAlgorithm_Manual'  : keyMethod[0].value == "manual" ? manual[0].value : "",
			':EncryptionKey'               : keyMethod[0].value == "manual" ? manual[1].value : "",
			':IntegrityAlgorithm_Manual'   : keyMethod[0].value == "manual" ? manual[2].value : "",
			':IntegrityKey'                : keyMethod[0].value == "manual" ? manual[3].value : "",
			':SPI'                         : keyMethod[0].value == "manual" ? manual[4].value : "",
			':Interface'       : wanPath,
			'obj-action'       : 'add-set'
		});
	}
	else
	{
		$F(':' + IPSECList[path_index][0] + 'ConnectionName', connection[0].value);
		$F(':' + IPSECList[path_index][0] + 'TunnelMode', connection[1].value);
		$F(':' + IPSECList[path_index][0] + 'RemoteGW', connection[2].value);

		$F(':' + IPSECList[path_index][0] + 'LocalIPAddr', localIPRang[1].value);
		$F(':' + IPSECList[path_index][0] + 'LocalIPSubnetMask', localIPRang[0].value == "subnet" ? localIPRang[2].value : "255.255.255.255");

		$F(':' + IPSECList[path_index][0] + 'RemoteIPAddr', remoteIPRang[1].value);
		$F(':' + IPSECList[path_index][0] + 'RemoteIPSubnetMask', remoteIPRang[0].value == "subnet" ? remoteIPRang[2].value : "255.255.255.255");

		$F(':' + IPSECList[path_index][0] + 'KeyExchangeMethod', keyMethod[0].value);
		if (keyMethod[0].value == "auto")
		{
			$F(':' + IPSECList[path_index][0] + 'PreSharedKey', keyMethod[1].value);

			$F(':' + IPSECList[path_index][0] + 'Mode', keyMethod[0].value == "auto" ? phase1[0].value : "");
			$F(':' + IPSECList[path_index][0] + 'EncryptionAlgorithm_Auto1', keyMethod[0].value == "auto" ? phase1[1].value : "");
			$F(':' + IPSECList[path_index][0] + 'IntegrityAlgorithm_Auto1', keyMethod[0].value == "auto" ? phase1[2].value : "");
			$F(':' + IPSECList[path_index][0] + 'DHGroup', phase1[3].value);
			$F(':' + IPSECList[path_index][0] + 'KeyLifeTime1', phase1[4].value);

			$F(':' + IPSECList[path_index][0] + 'EncryptionAlgorithm_Auto2', keyMethod[0].value == "auto" ? phase2[0].value : "");
			$F(':' + IPSECList[path_index][0] + 'IntegrityAlgorithm_Auto2', keyMethod[0].value == "auto" ? phase2[1].value : "");
			$F(':' + IPSECList[path_index][0] + 'KeyLifeTime2', phase2[2].value);
		}
		else
		{
			$F(':' + IPSECList[path_index][0] + 'EncryptionAlgorithm_Manual', keyMethod[0].value == "manual" ? manual[0].value : "");
			$F(':' + IPSECList[path_index][0] + 'EncryptionKey', keyMethod[0].value == "manual" ? manual[1].value : "");
			$F(':' + IPSECList[path_index][0] + 'IntegrityAlgorithm_Manual', keyMethod[0].value == "manual" ? manual[2].value : "");
			$F(':' + IPSECList[path_index][0] + 'IntegrityKey', keyMethod[0].value == "manual" ? manual[3].value : "");
			$F(':' + IPSECList[path_index][0] + 'SPI', keyMethod[0].value == "manual" ? manual[4].value : "");
		}
		$F(':' + IPSECList[path_index][0] + 'Interface', wanPath);
		$F('obj-action', 'set');
	}

	$H({
		'getpage'           : 'html/index.html',
		'errorpage'         : 'html/index.html',
		'var:menu'          : 'advanced',
		'var:page'          : 'ipsec',
		'var:errorpage'     : 'ipsec',
		'var:CacheLastData' : ViewState.Save()
	});

	$('uiPostForm').submit();
}

function uiCancel()
{
	option = "-";
	$('IPSecSet').style.display = 'none';
	$('add').disabled = false;
	$('edit').disabled = false;
	$('del').disabled = false;
}

function uiAdd()
{
	var MContent = $('Content').offsetHeight;
	option = "add";
	$('IPSecSet').style.display = 'block';
	$('add').disabled = true;
	$('edit').disabled = true;
	$('del').disabled = true;

	var ContentHeigh;
	if (MContent == $('Content').offsetHeight)
	{
		ContentHeigh = FirthContentHeigh + $('IPSecSet').offsetHeight;
		$('Menu').style.height = ContentHeigh + 'px';
		$('Content').style.height = ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}
}

function uiEdit()
{
	var MContent = $('Content').offsetHeight;
	var j = -1;

	if (_array_id.length < 1)
	{
		alert(SEcode[9888]);
		return false;
	}
	else if (_array_id.length > 1)
	{
		alert(SEcode[9890]);
		return false;
	}

	for (var i = 0; i < IPSECList.length; i++)
	{
		var index = "edit";
		index = index + i;
		if (Form.Checkbox(index) == 1)
		{
			j = i;
			path_index = i;
		}
	}

	if (j == -1)
	{
		alert(SEcode[1009]);
		return false;
	}

	$('IPSecSet').style.display = 'block';
	var ContentHeigh;
	if (MContent == $('Content').offsetHeight)
	{
		ContentHeigh = FirthContentHeigh + $('IPSecSet').offsetHeight;
		$('Menu').style.height = ContentHeigh + 'px';
		$('Content').style.height = ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}
	$('add').disabled = true;
	$('edit').disabled = true;
	$('del').disabled = true;

	option = "edit";

	$('ipsecname').value = IPSECList[j][1];
	$('tunnelmode').value = IPSECList[j][2];
	$('gw').value = IPSECList[j][3];

	if (IPSECList[j][5] == "255.255.255.255")
		$('TunnelLocalAddr').value = "single";
	else
		$('TunnelLocalAddr').value = "subnet";
	doLocalIP();

	$('LocalIP').value = IPSECList[j][4];
	$('LocalMask').value = IPSECList[j][5];

	if (IPSECList[j][7] == "255.255.255.255")
		$('TunnelRemoteAddr').value = "single";
	else
		$('TunnelRemoteAddr').value = "subnet";
	doRemoteIP();

	$('RemoteIP').value = IPSECList[j][6];
	$('RemoteMask').value = IPSECList[j][7];
	$('KeyMethod').value = IPSECList[j][8];
	doKeyMethod();

	if (IPSECList[j][8] == "auto")
	{
		$('ShardKey').value = IPSECList[j][9];
		$('Mode').value = IPSECList[j][11];
		$('EncryAlg1').value = IPSECList[j][12];
		$('IntegrAlg1').value = IPSECList[j][13];
		$('GroupKey1').value = IPSECList[j][14];
		$('KeyLifeTime1').value = IPSECList[j][15];
		$('EncryAlg2').value = IPSECList[j][16];
		$('IntegrAlg2').value = IPSECList[j][17];
		$('KeyLifeTime2').value = IPSECList[j][19];
	}
	else
	{
		$('EncryAlg3').value = IPSECList[j][20];
		$('EncryKey').value = IPSECList[j][21];
		$('AuthAlg').value = IPSECList[j][22];
		$('AuthKey').value = IPSECList[j][23];
		$('SPI').value = IPSECList[j][24];
	}

	for (var i = 0; i < wanConnection.length; i++)
	{
		if (wanConnection[i][1] == IPSECList[j][25]);
			$('selectConnectionName').value = i;
	}
}

function uiDelete()
{
	var j = -1;

	if (_array_id.length < 1)
	{
		alert(SEcode[9888]);
		return false;
	}
	else if (_array_id.length > 1)
	{
		alert(SEcode[9890]);
		return false;
	}

	for (var i = 0; i < IPSECList.length; i++)
	{
		var index = "edit";
		index = index + i;

		if (Form.Checkbox(index) == 1)
		{
			j = i;
		}
	}

	if (j == -1)
	{
		alert(SEcode[1009]);
		return false;
	}

	if (!confirm(SEcode[1008]))
	{
		return false;
	}

	$H({
		'del-obj'          : IPSECList[j][0],
		'obj-action'       : 'del',
		'getpage'          : 'html/index.html',
		'errorpage'        : 'html/index.html',
		'var:menu'         : 'advanced',
		'var:page'         : 'ipsec',
		'var:errorpage'    : 'ipsec',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError()
{
	if (G_Error != 1)
	{
		return false;
	}

	var arrayHint = [];

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
