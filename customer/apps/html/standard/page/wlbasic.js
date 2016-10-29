//Wireless basic
var G_Wireless = [];
//var wireless_name="Speedy-a13906";
var m = 0;
var flag=0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "Enable SSID SSIDAdvertisementEnabled X_TWSZ-COM_CurrentCountry Channel Standard X_TWSZ-COM_Bandwidth X_TWSZ-COM_ControlSideband"
`	G_Wireless[m] = ["InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.", //path
					 "$01", //Enable
					 "$02", //SSID
					 "$03", //SSIDAdvertisementEnabled
					 "$04", //X_TWSZ-COM_CurrentCountry
					 "$05", //Channel
					 "$06",  //Standard
					 "$07",  //band width
					 "$08"   //Control Sideband
					 ];
	m++;
`?>


//WPS
var G_WPS = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "WPS.Enable  WPS.Version"
`	G_WPS[t] = [':InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.',//path
				'$01', //Enable
				'$02' //Version
				];
	t++;
`?>

var G_WirelessEnable   = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable?>";
var G_WirelessMultiAPIsolationEnable   = "<?get :InternetGatewayDevice.X_TWSZ-COM_WLAN.WirelessMultiAPIsolationEnable?>";

function uiOnload(){
	var Idx = 0;
	flag=0;
	//生成国家码
	createCurrentCtryOpt();
	$('SELECT_Country').value = G_Wireless[Idx][4];
	//生成信道
	ctrlChannelOpt();
	setJSONValue({
		'INPUT_Enable'   : G_Wireless[Idx][1],
		'INPUT_EnableMultiAPIsolation'   : G_WirelessMultiAPIsolationEnable,
		'INPUT_SSID'     : G_Wireless[Idx][2],
		'vb_status'      : G_Wireless[Idx][3],
//		'SELECT_Country' : G_Wireless[Idx][4],
		'SELECT_Channel' : G_Wireless[Idx][5],
		'SELECT_Mode'    : G_Wireless[Idx][6],
		'SELECT_BandWidth'    : G_Wireless[Idx][7],
		'SELECT_ControlSideband'    : G_Wireless[Idx][8]
	});
   OnBandWidthEnable();
   OnChangeBandWidth();
}

function OnBandWidthEnable()
{	
	//ctrlChannelOpt();
	//n mode, or b/g/n
	if ($("SELECT_Mode").value.indexOf('n') > -1)
	{
		$("SELECT_BandWidth").disabled = false;
	}
	else
	{
		$("SELECT_BandWidth").disabled = true;
		$("SELECT_BandWidth").value = 20;
	}
	if(flag==0)
	{
		flag=1;
	}
	else
	{
		ctrlChannelOpt();
	}
}

function OnChangeBandWidth()
{
	if($("SELECT_BandWidth").value == 20)
	{
	     $("SELECT_ControlSideband").disabled = true;
	}
	else
	{
		 $("SELECT_ControlSideband").disabled = false;
	}
}

function ctrlChannelOpt(){
	var country = $('SELECT_Country').value;
	var _s = 0, _e = 11; //最小信道

	if('Anguilla,Argentina,Bermuda,Brazil,Canada,Chile,Colombia,Guam,Mexico,Micronesia,Peru,Taiwan,USA,Yugoslavia'.indexOf(country) > -1){
		_s = 0, _e = 11;
	}else if( 'Cayman Islands' == country || 'Puerto Rico' == country || 'United States' == country
			 || 'United States Minor Outlying Islands' == country || 'Virgin Islands\, U.S' == country){ //中间有空格的国家或地区的名称不能用indexOf这个函数进行查找，否则查找到china之后就会返回，而找不到China Taiwan，导致错误
		_s = 0, _e = 11;
	}else if('France,Jordan'.indexOf(country) > -1){
		_s = 9, _e = 13;
	} else if('Israel' == country){
		_s = 2, _e = 9;
	} else if('Japan1' == country){
		if ($('SELECT_Mode').value == "g")
		{
			_s = 0, _e = 13;
		}
		else
		{
			_s = 0, _e = 14;
		}
	} else if('Japan2' == country){
		_s = 13, _e = 14;
	} else if('Spain' == country || 'Peru' == country){//该处和底层有区别 lhj 100419
		_s = 0, _e = 13;
	//} else if('Peru' == country){//该处和底层有区别 lhj 100419
	//	_s = 0, _e = 11;
	} 
	else { //最常用
		_s = 0, _e = 13;
	}
	createChannelOpt(_s, _e);
}

function createChannelOpt(_start, _end){
	var _text = [
				 'CH1',
				 'CH2',
				 'CH3',
				 'CH4',
				 'CH5',
				 'CH6',
				 'CH7',
				 'CH8',
				 'CH9',
				 'CH10',
				 'CH11',
				 'CH12',
				 'CH13',
				 'CH14'
				 ].slice(_start,_end);
	var _value = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14'].slice(_start,_end);
	
	$S('SELECT_Channel', [data_language.dynamic.auto_select].concat(_text), ['0'].concat(_value));	
}

function createCurrentCtryOpt(){
	var _text = ['Afghanistan','Albania','Angola','Anguilla','Antigua And Barbuda','Argentina','Armenia','Aruba','Australia','Austria',
					'Azerbaijan','Bahamas','Bahrain','Bangladesh','Barbados','Belarus','Belgium','Belize','Benin','Bermuda','Bhutan',
					'Bolivia','Bosnia And Herzegovina','Botswana','Brazil','British Indian Ocean Territory','Brunei Darussalam',
					'Bulgaria','Burkina Faso','Burundi','Cambodia','Cameroon','Canada','Cape Verde','Cayman Islands',
					'Central African Republic','Chad','Chile','China','Christmas Island','Colombia','Comoros','Congo','Costa Rica',
					'Cote Divoire','Croatia','Cuba','Cyprus','Czech Republic','Denmark','Djibouti','Dominica','Dominican Republic','Ecuador',
					'Egypt','El Salvador','Equatorial Guinea','Eritrea','Estonia','Ethiopia','Falkland Islands (malvinas)','Faroe Islands',
					'Fiji','Finland','France','French Guiana','French Polynesia','French Southern Territories','Gabon','Gambia','Georgia',
					'Germany','Ghana','Gibraltar','Greece','Grenada','Guadeloupe','Guam','Guatemala','Guernsey','Guinea','Guinea-bissau',
					'Guyana','Haiti','Holy See (vatican city state)','Honduras','Hong Kong','Hungary','Iceland','India','Indonesia','Iran','Iraq',
					'Ireland','Israel','Italy','Jamaica','Japan1','Japan2','Jersey','Jordan','Kazakhstan','Kenya','Kiribati','Korea','Kuwait',
					'Kyrgyzstan','Lao People\'s Democratic Republic','Latvia','Lebanon','Lesotho','Liberia','Libyan Arab Jamahiriya',
					'Liechtenstein','Lithuania','Luxembourg','Macao','Macedonia','Madagascar','Malawi','Malaysia','Maldives','Mali','Malta',
					'Man, Isle Of','Martinique','Mauritania','Mauritius','Mayotte','Mexico','Micronesia','Moldova','Monaco','Mongolia',
					'Montenegro','Montserrat','Morocco','Mozambique','Myanmar','Namibia','Nauru','Nepal','Netherlands','Netherlands Antilles',
					'New Caledonia','New Zealand','Nicaragua','Niger','Nigeria','Niue','Norfolk Island','Northern Mariana Islands',
					'Norway','Oman','Pakistan','Palau','Panama','Papua New Guinea','Paraguay','Peru','Philippines','Poland','Portugal',
					'Puerto Rico','Qatar','Reunion','Romania','Russian Federation','Rwanda','Saint Kitts And Nevis','Saint Lucia',
					'Saint Pierre And Miquelon','Saint Vincent And The Grenadines','Samoa','San Marino','Sao Tome And Principe',
					'Saudi Arabia','Senegal','Serbia','Seychelles','Sierra Leone','Singapore','Slovakia','Slovenia','Solomon Islands',
					'Somalia','South Africa','Spain','Sri Lanka','Suriname','Swaziland','Sweden','Switzerland','Taiwan','Tajikistan',
					'Tanzania','Thailand','Togo','Tonga','Trinidad And Tobago','Tunisia','Turkey','Turkmenistan','Turks And Caicos Islands',
					'Tuvalu','Uganda','Ukraine','United Arab Emirates','United Kingdom','United States','United States Minor Outlying Islands',
					'Uruguay','Uzbekistan','Vanuatu','Venezuela','Viet Nam','Virgin Islands, British','Virgin Islands\, U.S',
					'Wallis And Futuna','Western Sahara','Yemen','Zambia','Zimbabwe','other'];
	
	$S('SELECT_Country', _text);
}



function uiSubmit(){
	var node_wl = $('INPUT_SSID','SELECT_Country','SELECT_Channel','SELECT_Mode', 'SELECT_BandWidth');

	document.getElementById('uiPostForm').acceptCharset = "UTF-8";
	document.charset='UTF-8';
	if($('INPUT_SSID').value.charAt(0)==' ')
	{
	   alert("Error: The first character of SSID can not be null");
       return false;
	}
	if($('INPUT_SSID').value.charAt($('INPUT_SSID').value.length  -1)==' ')
	{
	   alert("Error: The last character of SSID can not be null ");
       return false;
	}

	if($('INPUT_SSID').value.indexOf('<\?') != -1 )
	{
	   alert("Error: The string of SSID not support contain <\? ");
       return false;
	}
	if($('INPUT_SSID').value == '')
	{
	   alert("Error: SSID can not be null");
       return false;
	}

	//WPS2.x
	if((G_WPS[0][1] ==1) && (G_WPS[0][2]==2))	
	{	
		if(Form.Radio('vb_status') == 0)
		{
			if(confirm("If you hide the SSID, the WPS would be disabled. Do you sure?"))
			{
				$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WPS.Enable','0');
			}
			else
			{
				return;
			}
		}
	}
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_WLAN.WirelessMultiAPIsolationEnable'            : Form.Checkbox('INPUT_EnableMultiAPIsolation'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable'                    : Form.Checkbox('INPUT_Enable'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID'                      : node_wl[0].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSIDAdvertisementEnabled'  : Form.Radio('vb_status'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_CurrentCountry' : node_wl[1].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Channel'                   : node_wl[2].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Standard'                  : node_wl[3].value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.MaxBitRate'                : (node_wl[3].value == G_Wireless[0][6]  &&  node_wl[4].value == G_Wireless[0][7]) ? undefined:"Auto", //rate
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_Bandwidth'      : node_wl[4].value, //band width
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_ControlSideband' : $('SELECT_ControlSideband').value, //control sideband
		'var:menu'      : 'setup',
		'var:page'      : 'wireless',
		'var:subpage'   : 'wlbasic',
		'var:errorpage' : 'wlbasic',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
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

addListeners(uiOnload, dealWithError);
