var flow_enable  ="<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.Enable ?>";
var EnAverage    ="<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.EnAverage ?>";
var WanBand     ="<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.WanBand ?>";
var IdleWanBand = "<?get :InternetGatewayDevice.X_TWSZ-COM_QoS.OtherIntfsBand ?>";
var G_WANETHEnable = "<?get :InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable ?>";
var path = ':InternetGatewayDevice.X_TWSZ-COM_QoS.';
var path2 = ':InternetGatewayDevice.X_TWSZ-COM_QoS.IPFlow.';
var G_flow = new Array();
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.IPFlow. "Enable SrcStartIP SrcEndIP RateMin RateMax LanIntf ScheduleName"
`	G_flow[n] = new Array();
	G_flow[n][0] = "$00."; 
	G_flow[n][1] = "$01";//enable
	G_flow[n][2] = "$02";//strIP
	G_flow[n][3] = "$03";//endIP
	G_flow[n][4] = "$04";//rate min
	G_flow[n][5] = "$05";// rate max
	G_flow[n][6] = "$06"; //laninterface
	G_flow[n][7] = "$07"; //ScheduleName
	++n;
`?>

var EntryNum=0;
var G_SchedEntry=[];
<?if lt 0 `<?get :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedNumberOfEntries ?>`
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry. "SchedName"
		`	G_SchedEntry[EntryNum] = [];
			G_SchedEntry[EntryNum][0] = "InternetGatewayDevice.X_TWSZ-COM_SCHEDULES.SchedEntry.$00.";    //path
			G_SchedEntry[EntryNum][1] = "$01";   	//ScheduleName
			++EntryNum;
	`?>
`?>
function rule_hide()
{
		$('First_enable').disabled = true;
		$('First_enable').style.color = '#AAAAAA';
		$('First_IPstart').disabled = true;
		$('First_IPstart').style.color = '#AAAAAA';
		$('First_IPend').disabled = true;
		$('First_IPend').style.color = '#AAAAAA';
		$('First_run_interface').disabled= true;
		$('First_run_interface').style.color = '#AAAAAA';
		$('r111').disabled= true;
		$('r111').style.color = '#AAAAAA';
		$('First_Schedule').disabled= true;
		$('First_Schedule').style.color = '#AAAAAA';

		$('Second_enable').disabled = true;
		$('Second_enable').style.color = '#AAAAAA';
		$('Second_IPstart').disabled = true;
		$('Second_IPstart').style.color = '#AAAAAA';
		$('Second_IPend').disabled = true;
		$('Second_IPend').style.color = '#AAAAAA';
		$('Second_run_interface').disabled= true;
		$('Second_run_interface').style.color = '#AAAAAA';
		$('r222').disabled= true;
		$('r222').style.color = '#AAAAAA';
		$('Second_Schedule').disabled= true;
		$('Second_Schedule').style.color = '#AAAAAA';

		$('Three_enable').disabled = true;
		$('Three_enable').style.color = '#AAAAAA';
		$('Three_IPstart').disabled = true;
		$('Three_IPstart').style.color = '#AAAAAA';
		$('Three_IPend').disabled = true;
		$('Three_IPend').style.color = '#AAAAAA';
		$('Three_run_interface').disabled= true;
		$('Three_run_interface').style.color = '#AAAAAA';
		$('r333').disabled= true;
		$('r333').style.color = '#AAAAAA';
		$('Three_Schedule').disabled= true;
		$('Three_Schedule').style.color = '#AAAAAA';

		$('Four_enable').disabled = true;
		$('Four_enable').style.color = '#AAAAAA';
		$('Four_IPstart').disabled = true;
		$('Four_IPstart').style.color = '#AAAAAA';
		$('Four_IPend').disabled = true;
		$('Four_IPend').style.color = '#AAAAAA';
		$('Four_run_interface').disabled= true;
		$('Four_run_interface').style.color = '#AAAAAA';
		$('r444').disabled= true;
		$('r444').style.color = '#AAAAAA';
		$('Four_Schedule').disabled= true;
		$('Four_Schedule').style.color = '#AAAAAA';
}
function rule_display()
{
		$('First_enable').disabled = false;
		$('First_enable').style.color = '#000000';
		$('First_IPstart').disabled = false;
		$('First_IPstart').style.color = '#000000';
		$('First_IPend').disabled = false;
		$('First_IPend').style.color = '#000000';
		$('First_run_interface').disabled= false;
		$('First_run_interface').style.color = '#000000';
		$('r111').disabled= false;
		$('r111').style.color = '#000000';
		$('First_Schedule').disabled= false;
		$('First_Schedule').style.color = '#000000';

		$('Second_enable').disabled = false;
		$('Second_enable').style.color = '#000000';
		$('Second_IPstart').disabled = false;
		$('Second_IPstart').style.color = '#000000';
		$('Second_IPend').disabled = false;
		$('Second_IPend').style.color = '#000000';
		$('Second_run_interface').disabled= false;
		$('Second_run_interface').style.color = '#000000';
		$('r222').disabled= false;
		$('r222').style.color = '#000000';
		$('Second_Schedule').disabled= false;
		$('Second_Schedule').style.color = '#000000';

		$('Three_enable').disabled = false;
		$('Three_enable').style.color = '#000000';
		$('Three_IPstart').disabled = false;
		$('Three_IPstart').style.color = '#000000';
		$('Three_IPend').disabled = false;
		$('Three_IPend').style.color = '#000000';
		$('Three_run_interface').disabled= false;
		$('Three_run_interface').style.color = '#000000';
		$('r333').disabled= false;
		$('r333').style.color = '#000000';
		$('Three_Schedule').disabled= false;
		$('Three_Schedule').style.color = '#000000';

		$('Four_enable').disabled = false;
		$('Four_enable').style.color = '#000000';
		$('Four_IPstart').disabled = false;
		$('Four_IPstart').style.color = '#000000';
		$('Four_IPend').disabled = false;
		$('Four_IPend').style.color = '#000000';
		$('Four_run_interface').disabled= false;
		$('Four_run_interface').style.color = '#000000';
		$('r444').disabled= false;
		$('r444').style.color = '#000000';
		$('Four_Schedule').disabled= false;
		$('Four_Schedule').style.color = '#000000';	
}
function aclick()
{ 

	if($('auto_div_bandwidth').checked)
	{   
		rule_hide();
	}
	else
	{   
		rule_display();
	}
}

//将ip地址的低24位转换成整数
function IPtoInt(ip){
	var a = ip.split('.');

	var b = parseInt(a[1],10) << 16;
	var c = parseInt(a[2],10) << 8;
	var d = parseInt(a[3],10); 
	
	return (b + c + d);	
}

function s_to_int(s)
{
	return parseInt(s,10);
}

function uiSubmit()
{
	if($('First_enable').checked)
	{
		if($('First_IPstart').value == "")
		{
			alert("First IP start is null!");
			return false;
		}
		if ($('First_IPend').value == "")
		{
			alert("First IP end is null!");
			return false;
		}
		if(IPtoInt($('First_IPstart').value) > IPtoInt($('First_IPend').value))
		{
			alert("First IP end is less than First start end!");
			return false;
		}
		if($('First_Rate_min').value == "")
		{
			alert("First rate start is null!");
			return false;
		}
		if($('First_Rate_min').value == "")
		{
			alert("First rate end is null!");
			return false;
		}
		if(s_to_int($('First_Rate_max').value) < s_to_int($('First_Rate_min').value))
		{
			alert("First rate end is less than First rate start!");
			return false;
		}
	}
	if($('Second_enable').checked)
	{
		if($('Second_IPstart').value == "")
		{
			alert("Second IP start is null!");
			return false;
		}
		if($('Second_IPend').value == "")
		{
			alert("Second IP end is null!");
			return false;
		}
		if(IPtoInt($('Second_IPstart').value) > IPtoInt($('Second_IPend').value))
		{
			alert("Second IP end is less than Second IP start!");
			return false;
		}
		
		if($('Second_Rate_min').value == "")
		{
			alert("Second rate start is null!");
			return false;
		}
		if ($('Second_Rate_max').value == "")
		{
			alert("Second rate end is null!");
			return false;
		}
		if(s_to_int($('Second_Rate_max').value) < s_to_int($('Second_Rate_min').value))
		{
			alert("Second rate end is less than Second rate start!");
			return false;
		}
	}
	
	if($('Three_enable').checked)
	{
		if($('Three_IPstart').value == "")
		{
			alert("Three IP start is null!");
			return false;
		}
		if($('Three_IPend').value == "")
		{
			alert("Three IP end is null!");
			return false;
		}
		if(IPtoInt($('Three_IPstart').value) > IPtoInt($('Three_IPend').value))
		{
			alert("Three IP end is less than Three IP start!");
			return false;
		}
		
		if($('Three_Rate_min').value == "")
		{
			alert("Three rate start is null!");
			return false;
		}
		if ($('Three_Rate_max').value == "")
		{
			alert("Three rate end is null!");
			return false;
		}
		if(s_to_int($('Three_Rate_max').value) < s_to_int($('Three_Rate_min').value))
		{
			alert("Three rate end is less than Three rate start!");
			return false;
		}
	}
	if($('Four_enable').checked)
	{
		if($('Four_IPstart').value == "")
		{
			alert("Four IP start is null!");
			return false;
		}
		if($('Four_IPend').value == "")
		{
			alert("Four IP end is null!");
			return false;
		}
		if(IPtoInt($('Four_IPstart').value) > IPtoInt($('Four_IPend').value))
		{
			alert("Four IP end is less than Four IP start!");
			return false;
		}
		
		if($('Four_Rate_min').value == "")
		{
			alert("Four rate start is null!");
			return false;
		}
		if ($('Four_Rate_max').value == "")
		{
			alert("Four rate end is null!");
			return false;
		}
		if(s_to_int($('Four_Rate_max').value) < s_to_int($('Four_Rate_min').value))
		{
			alert("Four rate end is less than Four rate start!");
			return false;
		}
	}
		
	$H({
	'obj-action': 'set',
	'var:menu'  : 'advanced',
	'var:page'  : 'qos_flux_ctrl',
	'getpage'   :'html/index.html',
	'errorpage' :'html/index.html',
	'var:errorpage'    : 'qos_flux_ctrl',
	'var:CacheLastData': ViewState.Save()
	},true);
	
	if($('auto_div_bandwidth').checked)
	{
		$F(path + 'Enable',$('flux_control_enable').checked ? 1 : 0);
		$F(path + 'EnAverage',$('auto_div_bandwidth').checked ? 1 : 0);
		$F(path + 'WanBand',$('port_bandwidth').value);
		$('uiPostForm').submit();
		return ;
	}
		
	$F(path + 'Enable',$('flux_control_enable').checked ? 1 : 0);
	$F(path + 'EnAverage',$('auto_div_bandwidth').checked ? 1 : 0);
	$F(path + 'WanBand',$('port_bandwidth').value);
	$F(path + 'OtherIntfsBand',$('idle_port_bandwidth').value);
 
	$F(path2 + '1.Enable',$('First_enable').checked ? 1 : 0);
	$F(path2 + '1.SrcStartIP',$('First_IPstart').value);
	$F(path2 + '1.SrcEndIP',$('First_IPend').value);
	$F(path2 + '1.LanIntf',$('First_run_interface').value);
	$F(path2 + '1.RateMin',$('First_Rate_min').value );
	$F(path2 + '1.RateMax',$('First_Rate_max').value );
	$F(path2 + '1.ScheduleName',$('First_Schedule').value);
	
	$F(path2 + '2.Enable',$('Second_enable').checked ? 1 : 0);
	$F(path2 + '2.SrcStartIP',$('Second_IPstart').value);
	$F(path2 + '2.SrcEndIP',$('Second_IPend').value);								
	$F(path2 + '2.LanIntf',$('Second_run_interface').value);
	$F(path2 + '2.RateMin',$('Second_Rate_min').value );
	$F(path2 + '2.RateMax',$('Second_Rate_max').value );
	$F(path2 + '2.ScheduleName',$('Second_Schedule').value);

	$F(path2 + '3.Enable',$('Three_enable').checked ? 1 : 0);
	$F(path2 + '3.SrcStartIP',$('Three_IPstart').value);
	$F(path2 + '3.SrcEndIP',$('Three_IPend').value);
	$F(path2 + '3.LanIntf',$('Three_run_interface').value);
	$F(path2 + '3.RateMin',$('Three_Rate_min').value );
	$F(path2 + '3.RateMax',$('Three_Rate_max').value );
	$F(path2 + '3.ScheduleName',$('Three_Schedule').value);
	
	$F(path2 + '4.Enable',$('Four_enable').checked ? 1 : 0);
	$F(path2 + '4.SrcStartIP',$('Four_IPstart').value);
	$F(path2 + '4.SrcEndIP',$('Four_IPend').value);
	$F(path2 + '4.LanIntf',$('Four_run_interface').value);
	$F(path2 + '4.RateMin',$('Four_Rate_min').value );
	$F(path2 + '4.RateMax',$('Four_Rate_max').value );
	$F(path2 + '4.ScheduleName',$('Four_Schedule').value);	

	$('uiPostForm').submit();	

}
function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=qos_flux_ctrl';
}
function Select_shedule()
{
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=schedules';	 
}

function uiOnload()
{   

	var Sched_text = [], Sched_value = [];

	Sched_text.push("Always");
	Sched_value.push("Always");
	for(var j = 0; j < G_SchedEntry.length; j++){
		Sched_text.push(G_SchedEntry[j][1]);
		Sched_value.push(G_SchedEntry[j][0]);
	}
	if(EnAverage==1)
	{
		rule_hide();
	}
	else
	{
		rule_display();
	}
	$S('Three_Schedule',Sched_text,Sched_value);
	$S('Second_Schedule',Sched_text,Sched_value);
	$S('Four_Schedule',Sched_text,Sched_value);
 
	$('Second_Schedule').value=G_flow[1][7] ; 
	$('Three_Schedule').value=G_flow[2][7] ; 
	$('Four_Schedule').value=G_flow[3][7] ;

	var runMode_text = [], runMode_value = [];
		
	if(1 == G_WANETHEnable)
	{
		runMode_text = ['ethernet2','ethernet3','ethernet4','Dlink_1','Dlink_2','Dlink_3','Dlink_4'];
		runMode_value = ['vport2','vport3','vport4','wlan0','wlan1','wlan2','wlan3'];
	}
	else
	{
		runMode_text = ['ethernet1','ethernet2','ethernet3','ethernet4','Dlink_1','Dlink_2','Dlink_3','Dlink_4','Blank'];
		runMode_value = ['vport1','vport2','vport3','vport4','wlan0','wlan1','wlan2','wlan3',''];
	}
	
	$S('First_run_interface', runMode_text, runMode_value);
	$S('Second_run_interface', runMode_text, runMode_value);
	$S('Three_run_interface', runMode_text, runMode_value);
	$S('Four_run_interface', runMode_text, runMode_value);	
	
	setJSONValue({
			'flux_control_enable'       : flow_enable,
			'auto_div_bandwidth'        : EnAverage,
			'port_bandwidth'           : WanBand,
			'idle_port_bandwidth'    :  IdleWanBand
		});

		//aclick();
	if(1 == G_WANETHEnable)
	{
		if("vport1" == G_flow[0][6])
		{
			$S('First_Schedule',Sched_text,Sched_value);
			$('First_Schedule').value = "Always";
			setJSONValue({
				'First_enable'			: 0,
				'First_IPstart'			: "",
				'First_IPend'			: "",
				'First_run_interface'	: "vport2",
				'First_Rate_min'		: 40,
				'First_Rate_max'		: 200
			});
		}
		else 
		{
			$S('First_Schedule',Sched_text,Sched_value);
			$('First_Schedule').value = G_flow[0][7] ;
			setJSONValue({
				'First_enable'			: G_flow[0][1],
				'First_IPstart'			: G_flow[0][2],
				'First_IPend'			: G_flow[0][3],
				'First_run_interface'	: G_flow[0][6],
				'First_Rate_min'		: G_flow[0][4],
				'First_Rate_max'		: G_flow[0][5]
			});
		}

		if("vport1" == G_flow[1][6])
		{
			$S('Second_Schedule',Sched_text,Sched_value);
			$('Second_Schedule').value = "Always";
			setJSONValue({
				'Second_enable'			: 0,
				'Second_IPstart'		: "",
				'Second_IPend'			: "",
				'Second_run_interface'	: "vport2",
				'Second_Rate_min'		: 40,
				'Second_Rate_max'		: 200
			});
		}
		else 
		{
			$S('Second_Schedule',Sched_text,Sched_value);
			$('Second_Schedule').value = G_flow[1][7] ;
			setJSONValue({
				'Second_enable'			: G_flow[1][1],
				'Second_IPstart'		: G_flow[1][2],
				'Second_IPend'			: G_flow[1][3],
				'Second_run_interface'	: G_flow[1][6],
				'Second_Rate_min'		: G_flow[1][4],
				'Second_Rate_max'		: G_flow[1][5]
			});
		}

		if("vport1" == G_flow[2][6])
		{
			$S('Three_Schedule',Sched_text,Sched_value);
			$('Three_Schedule').value = "Always";
			setJSONValue({
				'Three_enable'			: 0,
				'Three_IPstart'			:"",
				'Three_IPend'			: "",
				'Three_run_interface'	: "vport2",
				'Three_Rate_min'		: 40,
				'Three_Rate_max'		: 200
			});
		}
		else 
		{
			$S('Three_Schedule',Sched_text,Sched_value);
			$('Three_Schedule').value = G_flow[2][7] ;
			setJSONValue({
				'Three_enable'			: G_flow[2][1],
				'Three_IPstart'			: G_flow[2][2],
				'Three_IPend'			: G_flow[2][3],
				'Three_run_interface'	: G_flow[2][6],
				'Three_Rate_min'		: G_flow[2][4],
				'Three_Rate_max'		: G_flow[2][5]
			});
		}
		
		if("vport1" == G_flow[3][6])
		{
			$S('Four_Schedule',Sched_text,Sched_value);
			$('Four_Schedule').value = "Always";
			setJSONValue({
				'Four_enable'			: 0,
				'Four_IPstart'			: "",
				'Four_IPend'			: "",
				'Four_run_interface'	: "vport2",
				'Four_Rate_min'			: 40,
				'Four_Rate_max'			: 200
			});
		}
		else 
		{
			$S('Four_Schedule',Sched_text,Sched_value);
			$('Four_Schedule').value = G_flow[3][7] ;
			setJSONValue({
				'Four_enable'			: G_flow[3][1],
				'Four_IPstart'			: G_flow[3][2],
				'Four_IPend'			: G_flow[3][3],
				'Four_run_interface'	: G_flow[3][6],
				'Four_Rate_min'			: G_flow[3][4],
				'Four_Rate_max'			: G_flow[3][5]
			});
		}
	}
	else
	{
		$S('First_Schedule',Sched_text,Sched_value);
		$('First_Schedule').value =G_flow[0][7] ;
		setJSONValue({
			'First_enable'			: G_flow[0][1],
			'First_IPstart'			: G_flow[0][2],
			'First_IPend'			: G_flow[0][3],
			'First_run_interface'	: G_flow[0][6],
			'First_Rate_min'		: G_flow[0][4],
			'First_Rate_max'		: G_flow[0][5],
			'Second_enable'			: G_flow[1][1],
			'Three_enable'			: G_flow[2][1],
			'Four_enable'			: G_flow[3][1],
			'Second_IPstart'		: G_flow[1][2],
			'Three_IPstart'			: G_flow[2][2],
			'Four_IPstart'			: G_flow[3][2],
			'Second_IPend'			: G_flow[1][3],
			'Three_IPend'			: G_flow[2][3],
			'Four_IPend'			: G_flow[3][3],
			'Second_run_interface'	: G_flow[1][6],
			'Three_run_interface'	: G_flow[2][6],
			'Four_run_interface'	: G_flow[3][6],
			'Second_Rate_min'		: G_flow[1][4],
			'Three_Rate_min'		: G_flow[2][4],
			'Four_Rate_min'			: G_flow[3][4],
			'Second_Rate_max'		: G_flow[1][5],
			'Three_Rate_max'		: G_flow[2][5],
			'Four_Rate_max'			: G_flow[3][5]
		});
	}
	
}
function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);