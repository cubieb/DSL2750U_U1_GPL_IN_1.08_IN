//SNTP
<?mget :InternetGatewayDevice.Time. "Enable X_TWSZ-COM_Status NTPServer1 NTPServer2 X_TWSZ-COM_UpdateInterval X_TWSZ-COM_RetryInterval LocalTimeZoneName LocalTimeZone DaylightSavingsUsed DaylightSavingsStart DaylightSavingsEnd CurrentLocalTime country_num"
`	var G_Enable 			  = "$01";
	var G_Status 			  = "$02";
	var G_NTPServer1 		  = "$03";
	var G_NTPServer2 	      = "$04";
	var G_UpdateInterval	  = "$05";
	var G_RetryInterval		  = "$06";
	var G_LocalTimeZoneName   = "$07";
	var G_LocalTimeZone       = "$08";
	var G_DaylightSavingsUsed = "$09";
	var G_DaylightSavingsStart= "$0a";
	var G_DaylightSavingsEnd  = "$0b";
	var G_CurrentLocalTime    = "$0C";
	var G_country_num    	  = "$0d";
`?>


function uiOnload(){
	//处理时间的格式
	var DaylightSavingsStart = G_DaylightSavingsStart.split('T');
	var DaylightSavingsEnd   = G_DaylightSavingsEnd.split('T');
	var DaylightSavings = [].concat(DaylightSavingsStart[0].split('-'),DaylightSavingsStart[1].split(':'),DaylightSavingsEnd[0].split('-'),DaylightSavingsEnd[1].split(':'));
	var CurrentTimeSplit=[];
	CurrentTimeSplit=G_CurrentLocalTime.split('T');
	//alert(CurrentTimeSplit[0]);
	// alert(CurrentTimeSplit[1]);
	//检查TimeZone值是否在范围内
	var CurrentDateSplit=[];
		CurrentDateSplit=CurrentTimeSplit[0].split('-');
	var CurrentTimeSpliet=[];
		CurrentTimeSpliet=CurrentTimeSplit[1].split(':');
	
	checkTimeZone();     //检查时区是否正确
	//统一赋值
	setJSONValue({
		'INPUT_Enable'             : G_Enable, //是否自动同步网络时间
		//'DIV_Status'                : DisplayStatusLang(G_Status),
		'INPUT_NTPServer1'         : G_NTPServer1, //自动同步时间服务地址
		'INPUT_NTPServer2'          : G_NTPServer2,
		'CurrentYear'               :  CurrentDateSplit[0], //当前CPE的时间
		'CurrentMonth'              :  CurrentDateSplit[1],
		'CurrentDay'                :  CurrentDateSplit[2],
		'CurrentHour'               : CurrentTimeSpliet[0],
		'CurrentMinute'             : CurrentTimeSpliet[1],
		'CurrentSecond'             : CurrentTimeSpliet[2],
		//'INPUT_LocalTime'           :'<font color="#33CC66" >' + G_CurrentLocalTime.replace('T',' ') + '</font>',
		//'INPUT_UpdateInterval'      : G_UpdateInterval,
		//'INPUT_RetryInterval'       : G_RetryInterval,
		'INPUT_TimeZone'            : G_LocalTimeZone + '_' + G_country_num,  //时区
		'INPUT_DaylightSavingsUsed' : G_DaylightSavingsUsed, //是否启动下令时
		'INPUT_StartYear'          : DaylightSavings[0], //下令时开始时间
		'INPUT_StartMonth'          : DaylightSavings[1],
		'INPUT_StartDay'            : DaylightSavings[2],
		'INPUT_StartHour'            : DaylightSavings[3],
		'INPUT_StartMinute'         : DaylightSavings[4],
		'INPUT_StartSecond'         : DaylightSavings[5],
		'INPUT_EndYear'             : DaylightSavings[6],  //夏令时结束时间
		'INPUT_EndMonth'            : DaylightSavings[7],
		'INPUT_EndDay'              : DaylightSavings[8],
		'INPUT_EndHour'             : DaylightSavings[9],
		'INPUT_EndMinute'           : DaylightSavings[10],
		'INPUT_EndSecond'           : DaylightSavings[11]
	});
	//根据夏令时的状态，判断开始和结束时间的disalbed。
	ctrAutoDisplay();
}

function checkTimeZone(){
	var TimeZone = $('INPUT_TimeZone').options;
	
	for(var i = 0; i < TimeZone.length; i++){
		var TimeZone_tmp = TimeZone[i].value.split('_');
		if(G_LocalTimeZone == '' || TimeZone_tmp[0] == G_LocalTimeZone){
			return;
		}
	}
	//对'+'与'-'的符号进行交换处理
	TimeZone[26] = new Option('(GMT' + (G_LocalTimeZone.indexOf('-') > -1 ? G_LocalTimeZone.replace(/\-/,'+') : G_LocalTimeZone.replace(/\+/,'-')) + ')');
	TimeZone[26].value = G_LocalTimeZone;
}
/*********************************夏令时 时间设置显示控制**********************************************/
function ctrlDayLightDisplay(){
	var array_time = document.getElementsByClassName('timeLength');
	var _node      = $('INPUT_DaylightSavingsUsed','lang_DaylightSavingsStart','lang_DaylightSavingsEnd');
	
	if(_node[0].checked){
		for(var i = 0; i < array_time.length; i++){
			array_time[i].disabled = false;
		}	
		$('INPUT_StartYear').disabled = false;
		$('INPUT_EndYear').disabled = false;
		_node[1].style.color = '#000000';
		_node[2].style.color = '#000000';		
	} else {
		for(var i = 0; i < array_time.length; i++){
			array_time[i].disabled = true;
		}
		_node[1].style.color = '#AAAAAA';
		_node[2].style.color = '#AAAAAA';	
		$('INPUT_StartYear').disabled = true;
		$('INPUT_EndYear').disabled = true;
	}
}

function   ctrAutoDisplay(){
	
	var array_time = document.getElementsByClassName('timeLength');
	var _node  = $('INPUT_Enable','INPUT_NTPServer1','INPUT_NTPServer2','INPUT_TimeZone','CurrentYear','CurrentMonth','CurrentDay','CurrentHour','CurrentMinute','CurrentSecond','INPUT_DaylightSavingsUsed');
	if(!_node[0].checked){		//自动同步时间没开启，显示手动设置时间输入框使能，其他的关闭
		_node[1].disabled = true;
		_node[2].disabled = true;
		_node[3].disabled = true;
		
		
		_node[6].disabled = false;
		_node[4].disabled = false;
		_node[5].disabled = false;
		_node[7].disabled = false;
		_node[8].disabled = false;
		_node[9].disabled = false;
		_node[10].disabled = true;

		$('lang_DaylightSavingsStart').style.color = '#aaaaaa';
		$('lang_DaylightSavingsEnd').style.color = '#aaaaaa';
		
		for(var i = 0; i < array_time.length; i++){
			array_time[i].disabled = true;
			$('INPUT_StartYear').disabled = true;
		    $('INPUT_EndYear').disabled = true;
		}
			
	}
	else{          //开启自动同步网络时间
		_node[1].disabled = false;
		_node[2].disabled = false;
		_node[3].disabled = false;
		
		
		_node[6].disabled = true;
		_node[5].disabled = true;
		_node[4].disabled = true;
		_node[7].disabled = true;
		_node[8].disabled = true;
		_node[9].disabled = true;
		_node[10].disabled = false;
		
		if($('INPUT_DaylightSavingsUsed').checked)
		{
			for(var i = 0; i < array_time.length; i++){
			array_time[i].disabled = false;
			$('INPUT_StartYear').disabled = false;
			$('INPUT_EndYear').disabled = false;
			$('lang_DaylightSavingsStart').style.color = '#000000';
			$('lang_DaylightSavingsEnd').style.color = '#000000';
			}	
		}
		else
		{
			for(var i = 0; i < array_time.length; i++){
			array_time[i].disabled = true;
			$('INPUT_StartYear').disabled = true;
			$('INPUT_EndYear').disabled = true;
			$('lang_DaylightSavingsStart').style.color = '#AAAAAA';
			$('lang_DaylightSavingsEnd').style.color = '#AAAAAA';
			}
		}
	}	
}

function FullFillTimeBlank(_node){
	var num;
	num = parseInt(_node[1].value,10);
	if (num < 10)
		_node[1].value = "0" + num;
	num = parseInt(_node[2].value,10);
	if (num < 10)
		_node[2].value = "0" + num;
	num = parseInt(_node[3].value,10);
	if (num < 10)
		_node[3].value = "0" + num;
	num = parseInt(_node[4].value,10);
	if (num < 10)
		_node[4].value = "0" + num;
	num = parseInt(_node[5].value,10);
	if (num < 10)
		_node[5].value = "0" + num;
	num = parseInt(_node[7].value,10);
	if (num < 10)
		_node[7].value = "0" + num;
	num = parseInt(_node[8].value,10);
	if (num < 10)
		_node[8].value = "0" + num;
	num = parseInt(_node[9].value,10);
	if (num < 10)
		_node[9].value = "0" + num;
		num = parseInt(_node[10].value,10);
	if (num < 10)
		_node[10].value = "0" + num;
		num = parseInt(_node[11].value,10);
	if (num < 10)
		_node[11].value = "0" + num;
}

function uiSubmit(){
	//组装时间的格式
	var _node = $('INPUT_StartYear','INPUT_StartMonth','INPUT_StartDay','INPUT_StartHour','INPUT_StartMinute',
				'INPUT_StartSecond','INPUT_EndYear','INPUT_EndMonth',		'INPUT_EndDay','INPUT_EndHour','INPUT_EndMinute','INPUT_EndSecond');
	FullFillTimeBlank(_node);
	var DaylightSavingsStart = _node[0].value + '-' + _node[1].value + '-' + _node[2].value + 'T' + _node[3].value + ':' + _node[4].value + ':' + _node[5].value;
	var DaylightSavingsEnd   = _node[6].value + '-' + _node[7].value + '-' + _node[8].value + 'T' + _node[9].value + ':' + _node[10].value + ':' + _node[11].value;
	//获取其他节点的引用
	var node_sntp = $('INPUT_Enable','INPUT_NTPServer1','INPUT_NTPServer2','INPUT_TimeZone','INPUT_DaylightSavingsUsed');
	var _node2 = $('CurrentYear','CurrentMonth','CurrentDay','CurrentHour','CurrentMinute','CurrentSecond');
	var CurrentDateTime  = _node2[0].value + '-' + _node2[1].value + '-' + _node2[2].value + 'T' + _node2[3].value + ':' + _node2[4].value + ':' + _node2[5].value;
	var country_num = node_sntp[3].value.split('_');

	if(node_sntp[0].checked==true)//时间能同步时就不要修改时间值
	{
	$H({
		':InternetGatewayDevice.Time.Enable'                    : 1 ,
		':InternetGatewayDevice.Time.NTPServer1'                : node_sntp[1].value,
		':InternetGatewayDevice.Time.NTPServer2'                : node_sntp[2].value,
		//':InternetGatewayDevice.Time.X_TWSZ-COM_UpdateInterval' : node_sntp[3].value,
		//':InternetGatewayDevice.Time.X_TWSZ-COM_RetryInterval'  : node_sntp[3].value,
		':InternetGatewayDevice.Time.LocalTimeZoneName'         : searchTimeZoneName(country_num[0]),
		':InternetGatewayDevice.Time.LocalTimeZone'             : country_num[0],
		':InternetGatewayDevice.Time.country_num'               : country_num[1],
		':InternetGatewayDevice.Time.DaylightSavingsUsed'       : node_sntp[4].checked ? 1 : 0, //是否启动夏令时
		':InternetGatewayDevice.Time.DaylightSavingsStart'      : node_sntp[4].checked ? DaylightSavingsStart : undefined,
		':InternetGatewayDevice.Time.DaylightSavingsEnd'        : node_sntp[4].checked ? DaylightSavingsEnd : undefined,
		'obj-action': 'set',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:menu'  : 'setup',
		'var:page'  : 'date',
		'var:errorpage' : 'date',
		'var:CacheLastData' : ViewState.Save()
	});
	}
	else//时间未同步时就可以修改当前时候
	{
		//alert(CurrentDateTime);
		$H({
		':InternetGatewayDevice.Time.Enable'                    : 0,
		':InternetGatewayDevice.Time.NTPServer1'                : node_sntp[1].value,
		':InternetGatewayDevice.Time.NTPServer2'                : node_sntp[2].value,
		//':InternetGatewayDevice.Time.X_TWSZ-COM_UpdateInterval' : node_sntp[3].value,
		//':InternetGatewayDevice.Time.X_TWSZ-COM_RetryInterval'  : node_sntp[3].value,
		':InternetGatewayDevice.Time.LocalTimeZoneName'         : searchTimeZoneName(country_num[0]),
		':InternetGatewayDevice.Time.LocalTimeZone'             : country_num[0],
		':InternetGatewayDevice.Time.country_num'               : country_num[1],
		':InternetGatewayDevice.Time.DaylightSavingsUsed'       : node_sntp[4].checked ? 1 : 0,
		':InternetGatewayDevice.Time.DaylightSavingsStart'      : node_sntp[4].checked ? DaylightSavingsStart : undefined,
		':InternetGatewayDevice.Time.DaylightSavingsEnd'        : node_sntp[4].checked ? DaylightSavingsEnd : undefined,
		':InternetGatewayDevice.Time.CurrentLocalTime'          : CurrentDateTime,    //将手动设置的时间下传
		'obj-action': 'set',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:menu'  : 'setup',
		'var:page'  : 'date',
		'var:errorpage' : 'date',
		'var:CacheLastData' : ViewState.Save()
	});
	}
	$('uiPostForm').submit();
}

function searchTimeZoneName(){
	//TimeZoneName和TimeZone的对应关系图
	var timeZoneNameMap = {
		'-12:00':'IDLW',
		'-11:00':'NT',
		'-10:00':'HST',
		'-09:00':'AKST',
		'-08:00':'PST',
		'-07:00':'MST',
		'-06:00':'CST',
		'-05:00':'EST',
		'-04:00':'AST',
		'-03:00':'BRT',
		'-02:00':'FNT',
		'-01:00':'WAT',
		'+00:00':'GMT',
		'+01:00':'MET',
		'+02:00':'EET',
		'+03:00':'BT',
		'+04:00':'MUT',
		'+05:00':'TFT',
		'+05:30':'IST',
		'+06:00':'ALMT',
		'+07:00':'WAST',
		'+08:00':'CCT',
		'+09:00':'JST',
		'+10:00':'AEST',
		'+11:00':'AESST',
		'+12:00':'NZT'
	};
	for(var i in timeZoneNameMap){
		if(i == arguments[0]){
			return timeZoneNameMap[i];
		}
	}
	return G_LocalTimeZoneName;
}


function dealWithError(){

	if (G_Error != 1){ return false; }
		var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}


addListeners(uiOnload, dealWithError);
