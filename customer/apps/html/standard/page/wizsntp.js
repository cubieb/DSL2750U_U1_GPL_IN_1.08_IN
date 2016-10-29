//wizard -> sntp


var G_timepar = [];

<?mget :InternetGatewayDevice.Time. "Enable NTPServer1 NTPServer2 LocalTimeZone DaylightSavingsUsed DaylightSavingsStart DaylightSavingsEnd CurrentLocalTime country_num"
	`
		G_timepar[0] = "$01"; //Enable
		G_timepar[1] = "$02"; //NTPServer1
		G_timepar[2] = "$03"; //NTPServer2
		G_timepar[3] = "$04"; //LocalTimeZone
		G_timepar[4] = "$05"; //DaylightSavingsUsed
		G_timepar[5] = "$06"; //DaylightSavingsStart
		G_timepar[6] = "$07"; //DaylightSavingsEnd
		G_timepar[7] = "$08"; //CurrentLocalTime
		G_timepar[8] = "$09"; //country_num
`?>

var aDaylightSavingsStartDay1 = G_timepar[5].split('T');
var aDaylightSavingsStartDay2 = aDaylightSavingsStartDay1[0].split('-');
var aDaylightSavingsStartTime = aDaylightSavingsStartDay1[1].split(':');

var aDaylightSavingsEndDay1 = G_timepar[6].split('T');
var aDaylightSavingsEndDay2 = aDaylightSavingsEndDay1[0].split('-');
var aDaylightSavingsEndTime = aDaylightSavingsEndDay1[1].split(':');

var CurrentTimeDay1  = G_timepar[7].split('T');
var CurrentDateDay2  = CurrentTimeDay1[0].split('-');	
var CurrentTime      = CurrentTimeDay1[1].split(':');

function uiOnload(){
	
	chgHTML();
	
	//获取暂存数据
	var Sntp = getJsonObj('stp');

	if(Sntp == null){
		alert('Stp is null!');
		return false;
	}
    var CurrentLocalTime 		= (Sntp['CurrentLocalTime'] && Sntp['CurrentLocalTime'].split('T')) || '';
	
	if(CurrentLocalTime != ''){
		var CurrentLocalTime_Savings = [].concat(CurrentLocalTime[0].split('-'),CurrentLocalTime[1].split(':'));	
	} else {
		var CurrentLocalTime_Savings = [];
	}
	
	var DaylightSavingsStart	= (Sntp['DaylightSavingsStart'] && Sntp['DaylightSavingsStart'].split('T')) || '';
	var DaylightSavingsEnd		= (Sntp['DaylightSavingsEnd'] && Sntp['DaylightSavingsEnd'].split('T')) || '';
	if(DaylightSavingsEnd != ''){
		var DaylightSavings = [].concat(DaylightSavingsStart[0].split('-'),DaylightSavingsStart[1].split(':'),DaylightSavingsEnd[0].split('-'),DaylightSavingsEnd[1].split(':'));	
	} else {
		var DaylightSavings = [];
	}
	
	var TimeZone_tmp;
	
	if(Sntp && Sntp['INPUT_TimeZone'])
	{
		 TimeZone_tmp = Sntp['INPUT_TimeZone'] + '_' + G_timepar[8];
	}
	else if(G_timepar[3] && G_timepar[8])
	{
		TimeZone_tmp = G_timepar[3] + '_' + G_timepar[8];
	}
	else
	{
		TimeZone_tmp = '-08:00_1';
	}
	//赋�?
	setJSONValue({
		'INPUT_Enable'      		: (Sntp && Sntp['INPUT_Enable']) || G_timepar[0] || '0',
		'INPUT_NTPServer1'  		: (Sntp && Sntp['INPUT_NTPServer1']) || G_timepar[1] || '',
		'INPUT_NTPServer2'  		: (Sntp && Sntp['INPUT_NTPServer2']) || G_timepar[2] || '',
		
		'INPUT_TimeZone'    		: TimeZone_tmp,
		'INPUT_DaylightSavingsUsed' : (Sntp && Sntp['INPUT_DaylightSavingsUsed']) || G_timepar[4]|| '0',
		
		'CurrentYear' 				: CurrentLocalTime_Savings[0] || CurrentDateDay2[0] ||  '',
		'CurrentMonth'				: CurrentLocalTime_Savings[1] || CurrentDateDay2[1] ||  '',
		'CurrentDay'				: CurrentLocalTime_Savings[2] || CurrentDateDay2[2] ||  '',
		'CurrentHour'				: CurrentLocalTime_Savings[3] || CurrentTime[0] ||  '',
		'CurrentMinute'				: CurrentLocalTime_Savings[4] || CurrentTime[1] ||  '',
		'CurrentSecond'				: CurrentLocalTime_Savings[5] || CurrentTime[2] ||  '',
		
		'INPUT_StartYear'   		: DaylightSavings[0] || aDaylightSavingsStartDay2[0] ||  '',
		'INPUT_StartMonth'  		: DaylightSavings[1] || aDaylightSavingsStartDay2[1] || '',
		'INPUT_StartDay'    		: DaylightSavings[2] || aDaylightSavingsStartDay2[2] || '',
		'INPUT_StartHour'   		: DaylightSavings[3] || aDaylightSavingsStartTime[0] ||  '',
		'INPUT_StartMinute' 		: DaylightSavings[4] || aDaylightSavingsStartTime[1] || '',
		'INPUT_StartSecond' 		: DaylightSavings[5] || aDaylightSavingsStartTime[2] || '',
			
		'INPUT_EndYear'     		: DaylightSavings[6] || aDaylightSavingsEndDay2[0] || '',
		'INPUT_EndMonth'    		: DaylightSavings[7] || aDaylightSavingsEndDay2[1] || '',
		'INPUT_EndDay'      		: DaylightSavings[8] || aDaylightSavingsEndDay2[2] || '',
		'INPUT_EndHour'     		: DaylightSavings[9] || aDaylightSavingsEndTime[0] || '',
		'INPUT_EndMinute'   		: DaylightSavings[10] || aDaylightSavingsEndTime[1] || '',
		'INPUT_EndSecond'   		: DaylightSavings[11] || aDaylightSavingsEndTime[2] || ''
	});
	
	ctrlSntpEnable();
}

function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//隐藏主菜�?
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//隐藏二级菜单
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}

//总开�?
function ctrlSntpEnable(){
	var _bool = Form.Checkbox('INPUT_Enable') == '1' ? true : false;

	disCtrl('sntp1',_bool);
	disCtrl('sntp2',_bool);
	disCtrl('manual_set',!_bool);
	ctrlDayLightEnable();
}

//夏令时开�?
function ctrlDayLightEnable(){
	var _bool = Form.Checkbox('INPUT_Enable') == '0' ? false : Form.Checkbox('INPUT_DaylightSavingsUsed') == '1' ? true : false;
	
	disCtrl('daylight',_bool);	
}

function createJsonData(){
	var _json = '{';
	//组装时间的格�?
	var _time = $('CurrentYear','CurrentMonth','CurrentDay','CurrentHour','CurrentMinute','CurrentSecond');
	var _node = $('INPUT_StartYear','INPUT_StartMonth','INPUT_StartDay','INPUT_StartHour','INPUT_StartMinute','INPUT_StartSecond',
						'INPUT_EndYear','INPUT_EndMonth','INPUT_EndDay','INPUT_EndHour','INPUT_EndMinute','INPUT_EndSecond');
	var CurrentLocalTime = _time[0].value + '-' + _time[1].value + '-' + _time[2].value + 'T' + _time[3].value + ':' + _time[4].value + ':' + _time[5].value;
	var DaylightSavingsStart = _node[0].value + '-' + _node[1].value + '-' + _node[2].value + 'T' + _node[3].value + ':' + _node[4].value + ':' + _node[5].value;
	var DaylightSavingsEnd   = _node[6].value + '-' + _node[7].value + '-' + _node[8].value + 'T' + _node[9].value + ':' + _node[10].value + ':' + _node[11].value;
	//获取其他节点的引�?
	var node_sntp = $('INPUT_Enable','INPUT_NTPServer1','INPUT_NTPServer2','INPUT_TimeZone','INPUT_DaylightSavingsUsed');
	var country_num = node_sntp[3].value.split('_');
	
	if(node_sntp[0].checked){
		_json += 'INPUT_Enable:"1",';  //Enable
		_json += 'INPUT_NTPServer1:"' + node_sntp[1].value + '",';  //NTPServer1
		_json += 'INPUT_NTPServer2:"' + node_sntp[2].value + '",';  //NTPServer2
		_json += 'INPUT_TimeZoneName:"' + searchTimeZoneName(country_num[0]) + '",';  //TimeZoneName
		_json += 'INPUT_TimeZone:"' + country_num[0] + '",';  //TimeZone
		_json += 'INPUT_country_num:"' + country_num[1] + '",';  //TimeZone
		if(node_sntp[4].checked){
			_json += 'INPUT_DaylightSavingsUsed:"' + (node_sntp[4].checked ? 1 : 0) + '",';  //DaylightSavingsUsed
			_json += 'DaylightSavingsStart:"' + DaylightSavingsStart + '",';  //DaylightSavingsStart
			_json += 'DaylightSavingsEnd:"' + DaylightSavingsEnd + '",';  //DaylightSavingsEnd
		} else {
			_json += 'INPUT_DaylightSavingsUsed:"' + (node_sntp[4].checked ? 1 : 0) + '"';  //DaylightSavingsUsed
		}		
	} else {
		_json += 'INPUT_Enable:"0",';  //Enable
		_json += 'CurrentLocalTime:"' + CurrentLocalTime + '"';  //DaylightSavingsEnd
	}
	
	_json = _json.delcomma();
	_json += '}';

	addJsonObj('stp', _json);
	return true;
}

function checkValue()
{
	//check ip address
	
	/*
	if (checkIp($('INPUT_NTPServer1').value)==false)
	{
		alert(data_language.dynamic.ip1st);
		return false;
	}
	//check ip address
	if (checkIp($('INPUT_NTPServer2').value)==false)
	{
		alert(data_language.dynamic.ip2sec);
		return false;
	}
	*/
    if('INPUT_Enable'.checked == false){
		var _time = $('CurrentYear','CurrentMonth','CurrentDay','CurrentHour','CurrentMinute','CurrentSecond');
		var CurrentLocalTime = _time[0].value + '-' + _time[1].value + '-' + _time[2].value + 'T' + _time[3].value + ':' + _time[4].value + ':' + _time[5].value;
		if(checkDateTime(CurrentLocalTime)==false)
		{
			alert(data_language.dynamic.time1st);
			return false;
		}    
	}
	else if($('INPUT_DaylightSavingsUsed').checked==true) //check date and time
	{
		var _node = $('INPUT_StartYear','INPUT_StartMonth','INPUT_StartDay','INPUT_StartHour','INPUT_StartMinute','INPUT_StartSecond',
							'INPUT_EndYear','INPUT_EndMonth','INPUT_EndDay','INPUT_EndHour','INPUT_EndMinute','INPUT_EndSecond');
		var DaylightSavingsStart = _node[0].value + '-' + _node[1].value + '-' + _node[2].value + 'T' + _node[3].value + ':' 
									+ _node[4].value + ':' + _node[5].value;
		var DaylightSavingsEnd  = _node[6].value + '-' + _node[7].value + '-' + _node[8].value + 'T' + _node[9].value + ':' 
									+ _node[10].value + ':' + _node[11].value;
		if(checkDateTime(DaylightSavingsStart)==false)
		{
			alert(data_language.dynamic.time1st);
			return false;
		}
		if(checkDateTime(DaylightSavingsEnd)==false)
		{
			alert(data_language.dynamic.time2sec);
			return false;
		}
	}
	return true;
}

//check IP address
function checkIp(x)
{
	var patrn=/^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
	if (!patrn.exec(x))
	{
		return false;
	}
	else
		return true;
}

//DateTime format as XXXX-XX-XXTXX:XX:XX (y-m-dTh:m:s)
function checkDateTime (x)
{
	var patrn=/^\d{4}\-\d{2}\-\d{2}[T]{1}\d{2}[:]{1}\d{2}[:]{1}\d{2}$/;
	if (!patrn.exec(x))
	{
		return false;
	}

	var the1st = x.indexOf('-');
	var the2nd = x.lastIndexOf('-');
	var the3rd = x.lastIndexOf('T');
	var the4th = x.indexOf(':');
	var the5th = x.lastIndexOf(':');
	var the6th = x.length;
	
	var y = x.substring(0,the1st);
	var mo = x.substring(the1st+1,the2nd);
	var d = x.substring(the2nd+1,the3rd);
	var h = x.substring(the3rd+1,the4th);
	var mi = x.substring(the4th+1,the5th);
	var s = x.substring(the5th+1,the6th);
	var maxDays = 31;

	//check month
	if(mo<1 || mo>12)
	{
		return false;
	}
	//check day
	if(mo==4 || mo==6 || mo==9 || mo==11)
		maxDays   =   30;  
	else if(mo==2)
	{
		if((y%4 > 0) || ((y%100 == 0) && (y%400 > 0)))
			maxDays=28;
		else
			maxDays=29;
	}
	if(d < 1 || d > maxDays)
	{
		return false;
	}
	//check hour
	if(h<0 || h>24)
	{
		return false;
	}
	//check minute
	if(mi<0 || mi>59)
	{
		return false;
	}
	//check second
	if(s<0 || s>59)
	{
		return false;
	}

	return true;
}

function searchTimeZoneName(){
	//TimeZoneName和TimeZone的对应关系图
	var timeZoneNameMap = {
		'-12:00':'IDLW',
		'-11:00':'SST',
		'-10:00':'HST',
		'-09:00':'YST',
		'-08:00':'PST',
		'-07:00':'MST',
		'-06:00':'CST',
		'-05:00':'EST',
		'-04:00':'AST',
		'-03:00':'BST',
		'-02:00':'FST',
		'-01:00':'EGT',
		'+00:00':'GMT',
		'+01:00':'CET',
		'+02:00':'EET',
		'+03:00':'BST',
		'+04:00':'GST',
		'+05:00':'PKT',
		'+05:30':'IST',
		'+06:00':'BDT',
		'+07:00':'THA',
		'+08:00':'CCT',
		'+09:00':'JST',
		'+10:00':'AEST',
		'+11:00':'SBT',
		'+12:00':'NZT'
	};
	for(var i in timeZoneNameMap){
		if(i == arguments[0]){
			return timeZoneNameMap[i];
		}
	}
	return 'CCT';
}

function checkSntpValue(){
	//检查SNTP配置项合法�?
	
}


function uiNextPage(){

	if(!checkValue())
	{
		return false;
	}
	if(!createJsonData()){
		return false;
	}
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage' : 'wizisp',//'wizisp',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();	
}

function uiBack(){
	
	if(!createJsonData()){
		return false;
	}
	
	$H({
		'var:menu'	: 'setup',		
		'var:subpage' : 'wizentrance',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();	
}

function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function dealWithError(){
	
}

addListeners(uiOnload, dealWithError);