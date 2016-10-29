var activedDsliteID = -1; // 该变量记录激活隧道的实例号
var SelectDsliteID = ''; // 该变量记录隧道路径
var SelectTunnelID = 0;// 该变量记录第一个隧道的索引号
var G_IpTunnel = [];
var G_Dslite = [];
var m = 0;
var n = 0;

<?objget :InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel. "TunnelName Mode AssociatedWanIfName AssociatedLanIfName Activated TunnelCounter"
`
    G_IpTunnel[m] = [];
    G_IpTunnel[m][0] = "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$00.";
    G_IpTunnel[m][1] = "$01";    //TunnelName
    G_IpTunnel[m][2] = "$02";    //Mode
    G_IpTunnel[m][3] = "$03";    //AssociatedWanIfName
    G_IpTunnel[m][4] = "$04";    //AssociatedLanIfName
	G_IpTunnel[m][5] = "$05";    //Activated
	G_IpTunnel[m][6] = "$06";    //Counter
	G_IpTunnel[m][7] = "$00";    //{i}

	<?if eq `4in6` "$12"
	`
	  <?objget :InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$20.4in6Tunnel. " Mechanism Dynamic RemoteIpv6Address ConnStatus Select"
       `
	      G_Dslite[n] = [];
          G_Dslite[n][0] = "InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.$20.4in6Tunnel.$00.";
		  G_Dslite[n][1] = "$01";
		  G_Dslite[n][2] = "$02";
		  G_Dslite[n][3] = "$03";
		  G_Dslite[n][4] = "$04";
		  G_Dslite[n][5] = "$05";
		  n++;
	  `?>
	`?>
	m++;
`?>

//WAN Device
var G_Wanconns = [];
n=0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name ConnectionType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";
			G_Wanconns[n][4] = "$04";
			n++;
		`?>
	`?>
`?>

// LAN Device
<?objget :InternetGatewayDevice.LANDevice. "X_TWSZ-COM_DeviceName"
`	G_Wanconns[n] = [];
	G_Wanconns[n][0] = "$01";
	G_Wanconns[n][1] = "InternetGatewayDevice.LANDevice.$00";
	G_Wanconns[n][2] = "";
	n++;
`?>

/*  点击某隧道时，先清除上一个隧道的dslite信息，然后再显示该隧道的dslite信息 */
function SelectTunnelEntry(index)
{
    Table.Clear('DsliteList');
	createDsliteTable(index);
	return 0;
}

/* 选择dslite隧道子节点条目 */
function ctrlSelectItem(_id)
{
  /* 当前该条目已经被勾选，再次点击复选框后，该条目取消 */
   if(G_Dslite[_id][5] == "1" )	
   {
	   SelectDsliteID = '';


		$H({
			'obj-action'    : 'set',
			'var:menu'      : 'advanced',
			'var:page'      : 'iptunnel',
			'var:subpage'   :'dslite',
			'var:errorpage' : 'dslite',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);
		$F(':'+G_Dslite[_id][0] + 'Select','0');
		$('uiPostForm').submit();
   }
   /* 如果该条目没有被勾选 */
   else 
   {
	   /* 有其他条目被勾选，则选将其他条目去掉，然后勾选本条目 */
	   if(SelectDsliteID != '')
	   {	
		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   : 'dslite',
				'var:errorpage' : 'dslite',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
            $F(':'+SelectDsliteID + 'Select','0');
			$F(':'+G_Dslite[_id][0] + 'Select','1');
			$('uiPostForm').submit();
			SelectDsliteID = G_Dslite[_id][0];    
	   }
	   else
	   {
		   SelectDsliteID = G_Dslite[_id][0];

		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'dslite',
				'var:errorpage' : 'dslite',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
			$F(':'+G_Dslite[_id][0] + 'Select','1');
			$('uiPostForm').submit(); 
	   }
	}
}
/* 创建dslite表结构，该函数的参数为隧道索引 */
function createDsliteTable(TunnelIndex)
{
	var array_value = [];
	var idx=0;
	var flag = -1;
	
    for(var i=0; i< G_Dslite.length ; i++)
	{
		if(G_Dslite[i][5] == "1")
		{
			SelectDsliteID = G_Dslite[i][0];
		}
		
		if(G_Dslite[i][0].indexOf(G_IpTunnel[TunnelIndex][0])>-1)
		{
            flag = 1;
			array_value[i] = [];
			array_value[i].push('<input type="radio" id="Dslite_index'+i+'" name="Dslite_index" value="'+i+'" >');
			array_value[i].push(G_Dslite[i][1]+'&nbsp;'); //Mechanism
			array_value[i].push(G_Dslite[i][2]+'&nbsp;'); //Dynamic
			array_value[i].push(G_Dslite[i][3]+'&nbsp;'); //RemoteIpv6Address
			array_value[i].push(G_Dslite[i][4]+'&nbsp;'); //ConnStatus
			array_value[i].push('<input type="checkbox" id="INPUT_Select' + i + '" onclick="ctrlSelectItem(' + i + ')"' + ("1" == G_Dslite[i][5] ? 'checked' : '') + '>'); //Select
		}
	}
	$T('DsliteList',array_value);
	
	if(G_IpTunnel.length==0)
	{
		$('addDslite').disabled=true;
		$('editDslite').disabled=true;
		$('deleteDslite').disabled=true;
		$('editTunnel').disabled=true;
		$('removeTunnel').disabled=true;
	}
	else
	{

		if(flag == 1)
		{
			$('editDslite').disabled=false;
		    $('deleteDslite').disabled=false;
            $('addDslite').disabled=false;
		}
		else
		{
			$('editDslite').disabled=true;
		    $('deleteDslite').disabled=true;
            $('addDslite').disabled=false;
		}
	}
}

/* 创建iptunnel隧道节点，在dslite中，该节点的类型为4in6 */
function createTunnelTable()
{	
	Table.Clear('Iptunnel_table');

	var array_value = [];
	var m = 0;
	var flagCheck = -1;
	for(var i = 0; i < G_IpTunnel.length; i++)
	{
		array_value[m] = [];
		/* 过滤节点，只取4in6类型即dslite隧道 */
		if(G_IpTunnel[i][2] == "4in6")
		{
			if(G_IpTunnel[i][5] == "1")
			{
				activedDsliteID = G_IpTunnel[i][7];
			}
		    if(flagCheck==-1)
		    {
			    array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'" onclick="SelectTunnelEntry(' + i + ')" checked>');
		        flagCheck = 0;
				SelectTunnelID = i;
			}
		    else
		    {
			    array_value[m].push('<input type="radio" id="Tunnel_index'+i+'" name="Tunnel_index" value="'+i+'" onclick="SelectTunnelEntry(' + i + ')">');
		    }

		    array_value[m].push(G_IpTunnel[i][1]+'&nbsp;'); //Name
		    array_value[m].push(G_IpTunnel[i][2]+'&nbsp;'); //Mode
		    array_value[m].push(transform_name(G_IpTunnel[i][3])+'&nbsp;'); //WanInterfaceName
		    array_value[m].push("LAN:"+transform_name(G_IpTunnel[i][4])+'&nbsp;'); //LanInterfaceName
            array_value[m].push('<input type="checkbox" id="INPUT_Actived' + i + '" onclick="ctrlActived(' + i + ')"' + ("1" == G_IpTunnel[i][5] ? 'checked' : '') + '>'); //actived
		    array_value[m].push((G_IpTunnel[i][6])+'&nbsp;'); //Counter
		    m++;
		}
	}
	$T('Iptunnel_table',array_value);	
	if(m > 0)
	{
		$('editTunnel').disabled=false;
		$('removeTunnel').disabled=false;
	}
	else
	{
       		$('editTunnel').disabled=true;
		$('removeTunnel').disabled=true;
	}
}

/* 激活隧道  */
function ctrlActived(_id)
{
   var path;
   var paht_ex;

   /* 如果隧道本来是激活状态，又点击了一下，使该隧道处于不活跃状态  */
   if(G_IpTunnel[_id][5] == "1" )	
   {
	   activedDsliteID = -1;
	   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		$H({
			'obj-action'    : 'set',
			'var:menu'      : 'advanced',
			'var:page'      : 'iptunnel',
			'var:subpage'   :'dslite',
			'var:errorpage' : 'dslite',
			'getpage'       : 'html/index.html',
			'errorpage'     : 'html/index.html'
		},true);
		$F(path + '.Activated','0');
		$('uiPostForm').submit();
   }
   /* 如果该隧道没有处于激活状态，则要判断是否有隧道处于激活状态，如果有，则使其他隧道处于不激活状态  */
   else 
   {
	   if(activedDsliteID != -1)
	   {
		   var tmpid = activedDsliteID; 
		   activedDsliteID = G_IpTunnel[_id][7];
		   path_ex = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+tmpid;
		   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		   $H({
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'dslite',
				'var:errorpage' : 'dslite',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
            $F(path_ex + '.Activated','0');
			$F(path + '.Activated','1');
			$('uiPostForm').submit();
           
	   }
	   else
	   {
		   activedDsliteID = G_IpTunnel[_id][7];
		   
		   path = ':InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.'+G_IpTunnel[_id][7];
		   $H({
				 path: '1',
				'obj-action'    : 'set',
				'var:menu'      : 'advanced',
				'var:page'      : 'iptunnel',
				'var:subpage'   :'dslite',
				'var:errorpage' : 'dslite',
				'getpage'       : 'html/index.html',
				'errorpage'     : 'html/index.html'
			},true);
			$F(path + '.Activated','1');
			$('uiPostForm').submit(); 
	   }
	}
}

/* 加载页面  */
function uiOnload()
{
	selectDslitemode();
	wanAndLanPath();
	createTunnelTable();
	createDsliteTable(SelectTunnelID);
	uiDynamicChange();
}


/* 设置下拉菜单wan接口和lan接口的可选项   */
function wanAndLanPath()
{
	var text_wan = [], value_wan = [];
	var text_lan = [], value_lan = [];

	for(var i = 0; i < G_Wanconns.length; i++)
	{
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0)
		{
			if (G_Wanconns[i][2].indexOf('IP_Bridged') < 0 && G_Wanconns[i][2].indexOf('PPP_Bridged') < 0)
			{
				if (G_Wanconns[i][3] == 'TR069')
				{
					continue;
				}
				if (G_Wanconns[i][4] == '0')
				{
					continue;
				}
				text_wan.push(G_Wanconns[i][0]);
				value_wan.push(G_Wanconns[i][1]);
			}
		}
		else if (G_Wanconns[i][1].indexOf('LANDevice') > 0)
		{
			if (G_Wanconns[i][0].indexOf('br1') < 0 && G_Wanconns[i][0].indexOf('br0') < 0)
			{
				continue;
			}
			text_lan.push("LAN:"+G_Wanconns[i][0]);
			value_lan.push(G_Wanconns[i][1]);
		}
	}
	$S('SELECT_WanInterface', text_wan, value_wan);
	$S('SELECT_LanInterface', text_lan, value_lan);
}
function selectDslitemode()
{
    var text_Dslite_mode = [], value_Dslite_mode = [];
	text_Dslite_mode.push('4in6');
	value_Dslite_mode.push('4in6');
	$S('SELECT_TunnelMode', text_Dslite_mode, value_Dslite_mode);
}


function transform_name(path)
{
	for (var i = 0; i < G_Wanconns.length; i++)
	{
		if (G_Wanconns[i][1] == path)
			return G_Wanconns[i][0];
	}

	return "unknown";
}


var addTunnelAction=0;
var editTunnelIdx=0;
function AddTunnel(){
	
	addTunnelAction = 1;
  	$('addTunnel').disabled=true;
	$('addDslite').disabled=true;
	$('editTunnel').disabled=true;
	$('removeTunnel').disabled=true;
	$('tunnel_setup').style.display='block';
	$('DsliteSetting').style.display='none';
	$('dslite_setup').style.display='none';

}

function EditTunnel(){
	
	addTunnelAction = 2;
	editTunnelIdx=Form.Radio('Tunnel_index');
	$('addTunnel').disabled=true;	
	$('editTunnel').disabled=true;
	$('removeTunnel').disabled=true;
	$('tunnel_setup').style.display='block';

	$('DsliteSetting').style.display='none';
	$('dslite_setup').style.display='none';
    $('addDslite').disabled=true;

	$('INPUT_TunnelName').value=G_IpTunnel[editTunnelIdx][1];
	$('SELECT_TunnelMode').value=G_IpTunnel[editTunnelIdx][2];
	$('SELECT_WanInterface').value=G_IpTunnel[editTunnelIdx][3];
    $('SELECT_LanInterface').value=G_IpTunnel[editTunnelIdx][4];	
}

function RemoveTunnel(){
	

	if(!confirm(SEcode[1008])){return false;}

	var DelIndex=Form.Radio('Tunnel_index');

	
	$H({
		'del-obj'   :G_IpTunnel[DelIndex][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'iptunnel',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'   :'dslite',
		'var:errorpage':'dslite'
	});
	$('uiPostForm').submit();
}

/* 隧道提交处理函数  */
function uiTunnelSubmit(){

	var Value_Nodes = $('INPUT_TunnelName', 'SELECT_TunnelMode', 'SELECT_WanInterface',
						'SELECT_LanInterface');

    var i=0;
	for(i=0;i<G_IpTunnel.length;i++)
	{

		if(addTunnelAction != 2 && Value_Nodes[0].value==G_IpTunnel[i][1] )
			break;

	}
	/* 如果是添加隧道，并且名字有重复，则错误退出   */
	if(i!=G_IpTunnel.length)
	{
		alert("Error: This Tunnel has exists in Tunnel table!");
		return false;
	}
    /* 添加隧道 */
	if(addTunnelAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TELEFONICA-ES_IPTunnel.',
			':TunnelName'	: 	Value_Nodes[0].value,
			':Mode'	:  Value_Nodes[1].value,
			':AssociatedWanIfName'   : Value_Nodes[2].value,
			':AssociatedLanIfName'   : Value_Nodes[3].value,

			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'dslite',
			'var:page' : 'iptunnel',
			'var:errorpage'     : 'dslite',
			'var:CacheLastData': ViewState.Save()
		},true);

	}
	else if(addTunnelAction=='2')     //edit
	{
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'dslite',
			'var:page' : 'iptunnel',
			'var:errorpage'    : 'dslite',
			'var:CacheLastData': ViewState.Save()
		},true);		

		var path=':'+G_IpTunnel[editTunnelIdx][0];
		$F(path+'TunnelName',Value_Nodes[0].value);
		$F(path+'Mode',Value_Nodes[1].value);
		$F(path+'AssociatedWanIfName',Value_Nodes[2].value);
		$F(path+'AssociatedLanIfName',Value_Nodes[3].value);
	}
	$('uiPostForm').submit();
	addTunnelAction = 0;
}

function uiDynamicChange() {
	var dynamic = $('SELECT_TunnelDynamic').value;

	if (dynamic == 0) {
		$('div_remoteAddress').style.display = 'block';
	} else if (dynamic == 1) {
		$('div_remoteAddress').style.display = 'none';
	}
}

var addDsliteAction=0;
var editDsliteIdx=0;

function uiAddDslite()
{
	addDsliteAction = 1;
	$('TunnelSetting').style.display='none';
	$('tunnel_setup').style.display='none';

	$('addDslite').disabled=true;
	$('editDslite').disabled=true;
	$('deleteDslite').disabled=true;
	

	$('DsliteSetting').style.display='block';
	$('dslite_setup').style.display='block';

	uiDynamicChange();
}

function uiEditDslite()
{

	var DsliteIdx=Form.Radio('Dslite_index');
	addDsliteAction = 2;
	if(G_Dslite.length==0)
	{
    	return;
	}

    $('TunnelSetting').style.display='none';
	$('tunnel_setup').style.display='none';
	$('DsliteSetting').style.display='none';
	$('dslite_setup').style.display='block';

/*
	for (var i=0; i < G_Dslite.length; i++)
    {
		if(G_Dslite[i][0].indexOf(G_IpTunnel[TunnelIdx][0])>0)
		{
           editDsliteIdx = i;
		   break;
		}
    } 
	*/
	editDsliteIdx = DsliteIdx;
	setJSONValue({
		'SELECT_DsliteMechanism'  : G_Dslite[editDsliteIdx][1],
		'SELECT_TunnelDynamic'    : G_Dslite[editDsliteIdx][2],
		'INPUT_RemoteIpv6Address' : G_Dslite[editDsliteIdx][3]
	});

	uiDynamicChange();
}

function uiDeleteDslite()
{
	if(!confirm(SEcode[1008])){return false;}

	var Index=Form.Radio('Dslite_index');
   // var dsliteIndex = 0;
	/*
	for (var i=0; i < G_Dslite.length; i++)
    {
		if(G_Dslite[i][0].indexOf(G_IpTunnel[Index][0])>0)
		{
           dsliteIndex = i;
		   break;
		}
    } */
	if(Index == undefined)
	{
		alert("[error] Please Select one item!");
		return false;
	}
	$H({
		'del-obj'   :G_Dslite[Index][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'iptunnel',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'   :'dslite',
		'var:errorpage':'dslite'
	});
	$('uiPostForm').submit();
}

function btnApplyDSlite()
{
    var TunnelIdx=Form.Radio('Tunnel_index');

	var Path=G_IpTunnel[TunnelIdx][0]+'4in6Tunnel.';

	if(addDsliteAction == 1)
	{	
		$H({
			'add-obj' 	  	: Path,
		    ':Mechanism'	: 	$('SELECT_DsliteMechanism').value,
			':Dynamic'	: 	$('SELECT_TunnelDynamic').value,
			':RemoteIpv6Address'  :    $('INPUT_RemoteIpv6Address').value,
			
			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'dslite',
			'var:page' : 'iptunnel',
			'var:errorpage'    : 'dslite',
			'var:CacheLastData': ViewState.Save()
		},true);

	}
	else if(addDsliteAction == 2)     //edit
	{
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'dslite',
			'var:page' : 'iptunnel',
			'var:errorpage'    : 'dslite',
			'var:CacheLastData': ViewState.Save()
		},true);		
		
		var path=':'+G_Dslite[editDsliteIdx][0];

		$F(path + 'Mechanism',$('SELECT_DsliteMechanism').value);
		$F(path + 'Dynamic',$('SELECT_TunnelDynamic').value);
		$F(path + 'RemoteIpv6Address',$('INPUT_RemoteIpv6Address').value);	
	}
	$('uiPostForm').submit();
	addDsliteAction=0;
}
/*
function uiRuleCancle()
{
	addRuleAction = 0;

	$('addRuleEntry').disabled=false;
	$('editRuleEntry').disabled=false;
	$('removeRuleEntry').disabled=false;
	$('addFilterEntry').disabled=false;
	$('editFilterEntry').disabled=false;
	$('removeFilterEntry').disabled=false;
	
	$('FilterSetting').style.display='block';
	$('FilterEntryInfo').style.display='none';
	$('RuleSetting').style.display='block';
	$('RuleEntryInfo').style.display='none';
}
*/


function dealWithError()
{
	if (G_Error != 1)
	{
		return false;
	}

	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

