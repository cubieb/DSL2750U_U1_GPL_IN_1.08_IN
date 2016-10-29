//SCHEDULES
var TimerEnable = "<?get :InternetGatewayDevice.Time.Enable ?>";
var G_SchedEntry = [];
var n = 0;
var addAction = 0;
var editIdx= -1;
<?if lt 0 `<?get :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedNumberOfEntries ?>`
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry. "SchedName SchedUsed EnableAllDays SelectDays EnableAllTimes StartTime EndTime"
		`	G_SchedEntry[n] = [];
			G_SchedEntry[n][0] = "$01";   	//ScheduleName
			G_SchedEntry[n][1] = "$02";		//ScheduleUsed	
			G_SchedEntry[n][2] = "$03";		//EnableAllDays
			G_SchedEntry[n][3] = "$04";		//SelectDays
			G_SchedEntry[n][4] = "$05";		//EnableAllTimes
			G_SchedEntry[n][5] = "$06";		//StartTime
			G_SchedEntry[n][6] = "$07";		//EndTime
			G_SchedEntry[n][7] = "InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry.$00.";    //path
			++n;
	`?>
`?>

function CreateTable(){
	var value_array = [];
	for(var i = 0; i < G_SchedEntry.length; i++){
		value_array[i] = [];
		value_array[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">'); //schedules index
		value_array[i].push(G_SchedEntry[i][0]); //schedules name
		var day=G_SchedEntry[i][3];
		if(day.indexOf('Sun')!=-1){
			value_array[i].push("X"); //Sun
		}else{
			value_array[i].push("&nbsp;"); //Sun
		}
		if(day.indexOf('Mon')!=-1)
			value_array[i].push("X"); //Mon
		else
			value_array[i].push("&nbsp;"); //Mon
		if(day.indexOf('Tue')!=-1)
			value_array[i].push("X"); //Tue
		else
			value_array[i].push("&nbsp;"); //Tue
		if(day.indexOf('Wed')!=-1)
			value_array[i].push("X"); //Wed
		else
			value_array[i].push("&nbsp;"); //Wed
		if(day.indexOf('Thu')!=-1)
			value_array[i].push("X"); //Thu
		else
			value_array[i].push("&nbsp;"); //Thu
		if(day.indexOf('Fri')!=-1)
			value_array[i].push("X"); //Fri
		else
			value_array[i].push("&nbsp;"); //Fri
		if(day.indexOf('Sat')!=-1)
			value_array[i].push("X"); //Sat
		else
			value_array[i].push("&nbsp;"); //Sat
			
		value_array[i].push(G_SchedEntry[i][5]); //StartTime
		value_array[i].push(G_SchedEntry[i][6]); //EndTime	
	}	
	$T('sched_table',value_array);

} 

function disablesUsedEntry(){
	for(var idx = 0; idx < G_SchedEntry.length; idx++){
		if(G_SchedEntry[idx][1] > 0){
			
		     var EditIndexID = "rule_index"+idx;
			 //$('EditIndexID').disabled=true;
			 $(EditIndexID).disabled=true;
		}
	}	
}


function uiOnload(){
	CreateTable();
	disablesUsedEntry();
	CancelEdit();
}

function FullFillTimeBlank(_nodes){
	var num;
	num = parseInt(_nodes[1].value,10);
	if (num < 10)
		_nodes[1].value = "0" + num;
	num = parseInt(_nodes[2].value,10);
	if (num < 10)
		_nodes[2].value = "0" + num;
	num = parseInt(_nodes[3].value,10);
	if (num < 10)
		_nodes[3].value = "0" + num;
	num = parseInt(_nodes[4].value,10);
	if (num < 10)
		_nodes[4].value = "0" + num;
}
function uiSubmit(){
	var confirmcontinue = 'NTP server is disabled,confirm to continue?';	
	var _nodes = $('schedName','startTimeH','startTimeM','endTimeH','endTimeM');

	if(TimerEnable == '0')
	{
		if(!confirm(confirmcontinue)){
			return false;
		 }
	}

    for(var i = 0; i < G_SchedEntry.length; i++)
	{	
		if(i == editIdx)
		{
			continue;
		}
		//alert(G_SchedEntry[i][0]);	
		//alert(G_SchedEntry[j][0]);
			if(_nodes[0].value==G_SchedEntry[i][0] & addAction=='1')
		{
			alert(SEcode[9889]);
			return false;
		}
    }
	FullFillTimeBlank(_nodes);
	var starttime=_nodes[1].value+':'+_nodes[2].value;
	var endtime=_nodes[3].value+':'+_nodes[4].value;
	var selectdays='';
	if(Form.Checkbox('sunday')=='1')
	     selectdays='Sun,';
	if(Form.Checkbox('monday')=='1')
	     selectdays+='Mon,';
	if(Form.Checkbox('tuesday')=='1')
	     selectdays+='Tue,';
	if(Form.Checkbox('wednesday')=='1')
	     selectdays+='Wed,';
	if(Form.Checkbox('thursday')=='1')
	     selectdays+='Thu,';
	if(Form.Checkbox('friday')=='1')
	     selectdays+='Fri,';
	if(Form.Checkbox('saturday')=='1')
	     selectdays+='Sat,';
	//alert(selectdays);
	if(addAction=='1'){
		$H({
		   	'add-obj'			:'InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry.',
			':SchedName'   		: _nodes[0].value,
			':EnableAllDays'   	: Form.Radio('EnableAllDays')=='1'?1:0,
			':SelectDays'   	: selectdays,
			':EnableAllTimes'   : Form.Checkbox('EnableAllTimes'),
			':StartTime'   		: starttime,
			':EndTime'   		: endtime,
			'obj-action'		:'add-set',
			'var:menu'     : 'advanced',
			'var:page'     : 'schedules',
			'var:errorpage': 'schedules',
			'getpage'      : 'html/index.html',
			'errorpage'    : 'html/index.html',
			'var:CacheLastData': ViewState.Save()
		});
	}else if(addAction=='2'){
		$H({
			'obj-action'		:'set',
			'var:menu'     : 'advanced',
			'var:page'     : 'schedules',
			'var:errorpage': 'schedules',
			'getpage'      : 'html/index.html',
			'errorpage'    : 'html/index.html',
			'var:CacheLastData': ViewState.Save()
		});
		
		var path=':'+G_SchedEntry[editIdx][7];
		$F(path+'SchedName',_nodes[0].value);

		var EnableValue=0;
		if($('AllWeek').checked){
			EnableValue=1;
		}
		$F(path+'EnableAllDays', EnableValue);

		$F(path+'SelectDays', selectdays);
		
		EnableValue=0;
		if($('EnableAllTimes').checked){
			EnableValue=1;
		}
		$F(path+'EnableAllTimes', EnableValue);

		$F(path+'StartTime', starttime);
		$F(path+'EndTime', endtime);
	
	}
	$('uiPostForm').submit();
}

function AddEntry(){
	var MContent;
	addAction = 1;
    $('addSched').disabled=true;
	$('editSched').disabled=true;
	$('removeSched').disabled=true;
	$('schedEntryInfo').style.display='block';
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('schedEntryInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}	
}

function EditEntry(){
	var MContent;	
	if(_array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	}
	if(0 == _array_id.length){
		alert(SEcode[9888]);
		return false;
	}
		
	for(var j = 0; j < G_SchedEntry.length; j++){
		var EditIndexID="rule_index"+j;
		var EditIndex=Form.Checkbox(EditIndexID);
		if(EditIndex=='1'){
			editIdx=j;
			$('schedEntryInfo').style.display = 'block';
			var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('schedEntryInfo').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}	
			addAction = 2;
			//alert(AddEntry);
			$('addSched').disabled=true;
			$('editSched').disabled=true;
			$('removeSched').disabled=true;
			
			var starttime= G_SchedEntry[j][5];
			var num=starttime.indexOf(':');
			var starttimeh=starttime.substring(0,num);
			var starttimem=starttime.substring(num+1,starttime.length+1);
			
			var endtime= G_SchedEntry[j][6];
			var num=endtime.indexOf(':');
			var endtimeh=endtime.substring(0,num);
			var endtimem=endtime.substring(num+1,endtime.length+1);
			
			
			var day=G_SchedEntry[j][3];
			
			if(day.indexOf('Sun')!=-1){
				var SunEnable=1; //Sun
			}else{
				var SunEnable=0; //Sun
			}
			if(day.indexOf('Mon')!=-1)
				var MonEnable=1; //Mon
			else
				var MonEnable=0; //Mon
			if(day.indexOf('Tue')!=-1)
				var TueEnable=1; //Tue
			else
				var TueEnable=0; //Tue
			if(day.indexOf('Wed')!=-1)
				var WedEnable=1; //Wed
			else
				var WedEnable=0; //Wed
			if(day.indexOf('Thu')!=-1)
				var ThuEnable=1; //Thu
			else
				var ThuEnable=0; //Thu
			if(day.indexOf('Fri')!=-1)
				var FriEnable=1; //Fri
			else
				var FriEnable=0; //Fri
			if(day.indexOf('Sat')!=-1)
				var SatEnable=1; //Sat
			else
				var SatEnable=0; //Sat
			
			setJSONValue({
				schedName       : G_SchedEntry[j][0],
				EnableAllDays  	: G_SchedEntry[j][2],
				sunday          : SunEnable,
				monday          : MonEnable,
				tuesday         : TueEnable,
				wednesday       : WedEnable,
				thursday        : ThuEnable,
				friday          : FriEnable,
				saturday        : SatEnable,
				EnableAllTimes 	: G_SchedEntry[j][4],
				startTimeH 		: starttimeh,
				startTimeM 		: starttimem,
				endTimeH		: endtimeh,
				endTimeM		: endtimem
			});
			if($('EnableAllTimes').checked){
					$('startTimeH').disabled=true;
					$('endTimeH').disabled=true;
					$('startTimeM').disabled=true;
					$('endTimeM').disabled=true;
			}
			if($('AllWeek').checked){
					var _input=$('daysArea').getElementsByTagName('INPUT');
					for(var k = 0, _len = _input.length; k < _len; k++){
						_input[k].disabled = true;
					}
			}
		}
	}
	Heigtduiqi();
}

function RemoveEntry(){
	
	if(_array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	}
	if(0 == _array_id.length){
		alert(SEcode[9888]);
		return false;
	}
	
	for(var j = 0; j < G_SchedEntry.length; j++){
		var DelIndexID="rule_index"+j;
		var DelIndex=Form.Checkbox(DelIndexID);
		if(DelIndex=='1'){
			if(!confirm(SEcode[1008])){ return false;}
			$H({
				'del-obj'   :G_SchedEntry[j][7],
				'obj-action':'del',
				'var:menu'  :'advanced',
				'var:page'  :'schedules',
				'getpage'   :'html/index.html',
				'errorpage' :'html/index.html',
				'var:errorpage':'schedules'
			});
			$('uiPostForm').submit();
		}
	}	
}

function changeDaysSelect(){
	
	var _input=$('daysArea').getElementsByTagName('INPUT');
	if($('AllWeek').checked){
		for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].checked = true;
			_input[k].disabled = true;
		}
	}else{
		for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].checked = false;
			_input[k].disabled = false;	
		}
	}

}

function SetWholeDay(){
	
	if($('EnableAllTimes').checked){
		$('startTimeH').value="00";
		$('endTimeH').value="23";
		$('startTimeM').value="00";
		$('endTimeM').value="59";
		$('startTimeH').disabled=true;
		$('endTimeH').disabled=true;
		$('startTimeM').disabled=true;
		$('endTimeM').disabled=true;
	}else{
		$('startTimeH').value="";
		$('endTimeH').value="";
		$('startTimeM').value="";
		$('endTimeM').value="";
		$('startTimeH').disabled=false;
		$('endTimeH').disabled=false;
		$('startTimeM').disabled=false;
		$('endTimeM').disabled=false;
	}
	return true;	
}

function CancelEdit(){
	addAction=0;
	$('schedEntryInfo').style.display = 'none';
    $('addSched').disabled=false;
	$('editSched').disabled=false;
	$('removeSched').disabled=false;	
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
