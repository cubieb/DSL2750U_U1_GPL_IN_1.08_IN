//DDNS
var G_DDNS = [];
var m = 0;
var buttoncheck=0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_DDNS. "DDNSProvider Hostname WANConnectionID Username Password Email Key DDNSState DdnsHost"
`	G_DDNS[m] = ['$01', //DDNSProvider
				 '$02', //Hostname
				 '$03', //WANConnectionID
				 '$04', //Username
				 '$05', //Password
				 '$06', //Email
				 '$07', //Key
				 '$08',//DDNSState
				 '$09',
				 'InternetGatewayDevice.X_TWSZ-COM_DDNS.$00.']; 
	m++;
`?>
//Wan Connection
var G_WanConns = [];
var n = 0;

<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection. " Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_WanConns[n] = ['<?echo $11?>',
						'InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $10?>'];
						G_WanConns[n][2]="$02";
						G_WanConns[n][3]="$03";
			n++;
			`?>
		`?>
		<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection. " Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_WanConns[n] = ['<?echo $11?>',
							 'InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $10?>'];
			G_WanConns[n][2]="$02";
			G_WanConns[n][3]="$03";
			n++;
			`?>
		`?>
	`?>
`?>

var DDNS_PeanuthullClient = 0;
<?if eq 1 $var:mod_DDNS_PEANUTHULLCLIENT
`
	DDNS_PeanuthullClient = 1;
`?>


function transform_name()
{
	var path = arguments[0];
	for (var i = 0; i < G_WanConns.length; i++)
	{
		if (G_WanConns[i][1] == path)
			return G_WanConns[i][0];
	}
	return "unknown";
}

function uiOnload(){	
	var array_ddns = [];
	var WANConnectionname = [];
	//Generate the DDNS list
	for(var i = 0, _len = G_DDNS.length; i < _len; i++){
		array_ddns[i] = [
						'<input type="checkbox" id="choose'+i+'" name="choose'+i+'">',
						G_DDNS[i][1].replace(/;/g,'<br>'), //Hostname
						G_DDNS[i][3], //USERNME
						G_DDNS[i][0], //DDNSProvider
						transform_name(G_DDNS[i][2])
						//WANConnectionname[i], //WANConnectionID					
						];
	}
	$T('td_ddnsList',array_ddns);
	//Generate the DDNS Service provider list
	if (1 == DDNS_PeanuthullClient)
		$S('SELECT_DDNSService',['dlinkddns.com','DynDNS.org','www.oray.cn']);	
		//$S('SELECT_DDNSService',['DynDNS.org','TZO', 'GnuDIP','www.oray.cn']);
	else
		$S('SELECT_DDNSService',['dlinkddns.com','DynDNS.org','3322.org', 'freedns.afraid.org','www.noip.com']);
	//Generate the WAN connection list
	createWanConns();
	//Select the different input option depends on the selected Service provider
	chooseDDNSProvider();
}



function createWanConns(){
	var _text = [], _value = [];
	var k = 0;
	
	for(var i = 0, _len = G_WanConns.length; i < _len; i++){
		if (G_WanConns[i][3].indexOf('IP_Bridged') > -1||G_WanConns[i][2]=="TR069") {
			continue;
		}
		_text[k]  = G_WanConns[i][0];
		_value[k] = G_WanConns[i][1];
		k++;
	}
	$S('SELECT_BindWanConns',_text,_value);
}

function chooseDDNSProvider(){
	var MContent;
	var provider = $('SELECT_DDNSService').value;

	if (1 == DDNS_PeanuthullClient)
		var div_type = $('dlinkddns.com','DynDNS.org','www.oray.cn'); 
	else
		var div_type = $('DynDNS.org','3322.org', 'freedns.afraid.org', 'TZO', 'GnuDIP','dlinkddns.com','www.noip.com'); 

	//hidden all Provider's element
	for(var i = 0, _len = div_type.length; i < _len; i++){
		div_type[i].style.display = 'none';
	}

	if (provider == 'www.oray.cn')
	{
		$('hostname').style.display = 'none';
		$('www.oray.cn').style.display = 'block';
	}
	else
	{
		$('hostname').style.display = 'block';
		$('www.oray.cn').style.display = 'none';
	}
	
	$(provider).style.display = 'block';
	if(MContent==$('Content').offsetHeight)
	{
		return;
	}
	else
	{
		Heigtduiqi();
	}	
}

function Addmenu(){
	
	var MContent;
		buttoncheck=0;
		$('EntryInfo').style.display='block';
		$('input_ddnsadd').disabled=true;
		$('input_ddnsedit').disabled=true;
		$('input_ddnsdelet').disabled=true;	

		if (1 == DDNS_PeanuthullClient)
			$S('SELECT_DDNSService',['dlinkddns.com','DynDNS.org','www.oray.cn']);
		else
			$S('SELECT_DDNSService',['dlinkddns.com','DynDNS.org','3322.org', 'freedns.afraid.org','www.noip.com']);
	//Generate WAN connection list
	createWanConns();
	//Select the different input option depends on the selected Service provider
	chooseDDNSProvider(); 
		var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight + 20;
		$('Menu').style.height=ContentHeigh + 'px';
		$('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}	
	
}

function Editmenu(){
	var MContent;
	buttoncheck=1;	
		
	for (var i = 0; i < G_DDNS.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
			$('EntryInfo').style.display='block';
			var ContentHeigh;
			if(MContent==$('Content').offsetHeight)
			{
				ContentHeigh =FirthContentHeigh + $('EntryInfo').offsetHeight;
				$('Menu').style.height=ContentHeigh + 'px';
				$('Content').style.height=ContentHeigh + 'px';
			}
			else
			{
				Heigtduiqi();
			}
			$('input_ddnsadd').disabled=true;
			$('input_ddnsedit').disabled=true;
			$('input_ddnsdelet').disabled=true;	
			$('SELECT_DDNSService').value=G_DDNS[i][0];
			$('INPUT_HostName').value=G_DDNS[i][1];	
			if(G_DDNS[i][0]=='DynDNS.org')
			{
				('INPUT_UserName').value = G_DDNS[i][3];
				$('INPUT_Password').value = G_DDNS[i][4];
			}
			if(G_DDNS[i][0]=='dlinkddns.com')
			{
				$('INPUT_UserNamedlink').value = G_DDNS[i][3];
				$('INPUT_Passworddlink').value = G_DDNS[i][4];
			}
			if(G_DDNS[i][0]=='www.noip.com')
			{
				$('INPUT_UserNamenoip').value = G_DDNS[i][3];
				$('INPUT_Passwordnoip').value = G_DDNS[i][4];
			}
			if(G_DDNS[i][0]=='TZO')
			{
				$('INPUT_Email').value = G_DDNS[i][5];
				$('INPUT_Key').value = G_DDNS[i][6];		
			}
			if(G_DDNS[i][0]=='GnuDIP')
			{		
				$('INPUT_UserNameGnudip').value = G_DDNS[i][3];
				$('INPUT_PasswordGnudip').value = G_DDNS[i][4];		
				$('INPUT_DdnsHostGnudip').value = G_DDNS[i][8];
			}
			if(G_DDNS[i][0]=='www.oray.cn')
			{
				$('Peanuthull_UserName').value = G_DDNS[i][3];
				$('Peanuthull_Password').value = G_DDNS[i][4];
			}
			if(G_DDNS[i][0]=='3322.org')
			{
				$('INPUT_UserName_3322').value = G_DDNS[i][3];
				$('INPUT_Password_3322').value = G_DDNS[i][4];
			}
			if(G_DDNS[i][0]=='freedns.afraid.org')
			{
				$('INPUT_HashKey_freedns').value = G_DDNS[i][6];
			}
			$('SELECT_BindWanConns').value=G_DDNS[i][2];
			chooseDDNSProvider();  
			return;
		}			
	}
	alert(SEcode[1009]);	
}

function uiSubmit(){
	
	if( buttoncheck==0)
		AdduiSubmit(); 
	else
		EdituiSubmit();
}


function AdduiSubmit(){
	
	var node_ddns = $('SELECT_DDNSService',             /* 0*/ 
						'INPUT_HostName',               /* 1*/ 
						'SELECT_BindWanConns',          /* 2*/ 
						'INPUT_UserName',               /* 3*/ 
						'INPUT_Password',               /* 4*/ 
						'INPUT_Email',                  /* 5*/ 
						'INPUT_Key',                    /* 6*/ 
						'INPUT_UserNameGnudip',         /* 7*/ 
						'INPUT_PasswordGnudip',         /* 8*/ 
						'INPUT_DdnsHostGnudip',         /* 9*/ 
						'Peanuthull_UserName',          /*10*/ 
						'Peanuthull_Password',          /*11*/ 
						'INPUT_UserName_3322',          /*12*/ 
						'INPUT_Password_3322',          /*13*/ 
						'INPUT_HashKey_freedns',      /*14*/ 
						'INPUT_UserNamedlink',		/*15*/ 
						'INPUT_Passworddlink',		/*16*/ 
						'INPUT_UserNamenoip',     /*17*/
						'INPUT_Passwordnoip');	  /*18*/	
	
	var ddnsService = node_ddns[0].value;
	//Generate form
	//www.oray.cn don't have "hostname" parammeter
	if (ddnsService == 'www.oray.cn')
	{
		$H({
			'add-obj'         : 'InternetGatewayDevice.X_TWSZ-COM_DDNS.',
			':DDNSProvider'   : node_ddns[0].value,
			':Hostname'       : '_',
			':WANConnectionID': node_ddns[2].value	
		}, true);
	}
	else
	{
		$H({
			'add-obj'         : 'InternetGatewayDevice.X_TWSZ-COM_DDNS.',
			':DDNSProvider'   : node_ddns[0].value,
			':Hostname'       : node_ddns[1].value,
			':WANConnectionID': node_ddns[2].value		
		}, true);
	}
	//Save reference parammeters of DynDNS.org
	if (ddnsService == 'DynDNS.org')
	{	
		$H({
			':Username'       : node_ddns[3].value,
			':Password'       : node_ddns[4].value
		});
	}
	//Save reference parammeters of TZO
	if (ddnsService == 'TZO')
	{
		$H({
			':Email'          : node_ddns[5].value,
			':Key'            : node_ddns[6].value
		});
	}
	
	if (ddnsService == 'GnuDIP')
	{
		$H({
			':Username'       : node_ddns[7].value,
			':Password'       : node_ddns[8].value,
			':DdnsHost'       :  node_ddns[9].value
		});
	}
	
	if (ddnsService == 'www.oray.cn')
	{	
		$H({
			':Username'       : node_ddns[10].value,
			':Password'       : node_ddns[11].value
		});
	}

	if (ddnsService == '3322.org')
	{
		$H({
				':Username'     : node_ddns[12].value,
				':Password'     : node_ddns[13].value
		});
	}

	if (ddnsService == 'freedns.afraid.org'){
		$H({
				':Username'     : '-',
				':Key'          : node_ddns[14].value
			});
	}
	if (ddnsService == 'dlinkddns.com')
	{
		$H({
			':Username'       : node_ddns[15].value,
			':Password'       : node_ddns[16].value
		});
	}
	
	if (ddnsService == 'www.noip.com')
	{
		$H({
			':Username'       : node_ddns[17].value,
			':Password'       : node_ddns[18].value
		});
	}
	
	$H({
		'var:menu'        : 'advanced',
		'var:page'        : 'ddns',
		'obj-action'      : 'add-set',
		'getpage'         : 'html/index.html',
		'errorpage'       : 'html/index.html',
		'var:errorpage'   : 'ddns',
		'var:CacheLastData' : ViewState.Save()
	});
	
	$('uiPostForm').submit();
}


function EdituiSubmit(){

	var node_ddns = $('SELECT_DDNSService',       /* 0*/ 
						'INPUT_HostName',         /* 1*/ 
						'SELECT_BindWanConns',    /* 2*/ 
						'INPUT_UserName',         /* 3*/ 
						'INPUT_Password',         /* 4*/ 
						'INPUT_Email',            /* 5*/ 
						'INPUT_Key',              /* 6*/ 
						'INPUT_UserNameGnudip',   /* 7*/ 
						'INPUT_PasswordGnudip',   /* 8*/ 
						'INPUT_DdnsHostGnudip',   /* 9*/ 
						'Peanuthull_UserName',    /*10*/ 
						'Peanuthull_Password',    /*11*/ 
						'INPUT_UserName_3322',    /*12*/ 
						'INPUT_Password_3322',    /*13*/ 
						'INPUT_HashKey_freedns',		/*14*/ 
						'INPUT_UserNamedlink',		/*15*/ 
						'INPUT_Passworddlink',		/*16*/ 
						'INPUT_UserNamenoip',     /*17*/
						'INPUT_Passwordnoip');	  /*18*/	
	var ddnsService = node_ddns[0].value;
	
	for (var i = 0; i < G_DDNS.length; i++) 
	{
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{
			//Generate save path
			var path= ':' + G_DDNS[i][9] ;
			$F(path + 'DDNSProvider',  node_ddns[0].value);
			if (ddnsService == 'www.oray.cn')
			{
				$F(path + 'Hostname', '_');
			}
			else
			{
				$F(path + 'Hostname',  node_ddns[1].value);
			}

			$F(path + 'WANConnectionID',  node_ddns[2].value);
		
			if (ddnsService == 'DynDNS.org')
			{
				$F(path + 'Username',  node_ddns[3].value);
				$F(path + 'Password',  node_ddns[4].value);	
			}
			
			if (ddnsService == 'TZO')
			{
				$F(path + 'Email',  node_ddns[5].value);
				$F(path + 'Key',  node_ddns[6].value);				
			}
	
			if (ddnsService == 'GnuDIP')
			{		
				$F(path + 'Username',  node_ddns[7].value);
				$F(path + 'Password',  node_ddns[8].value);					
				$F(path + 'DdnsHost',  node_ddns[9].value);		
			}
			
			if (ddnsService == 'www.oray.cn')
			{
		
				$F(path + 'Username',  node_ddns[10].value);
				$F(path + 'Password',  node_ddns[11].value);
			}

			if (ddnsService == '3322.org')
			{
				$F(path +   'Username'  ,node_ddns[12].value);
				$F(path +   'Password'  ,node_ddns[13].value);
			}

			if (ddnsService == 'freedns.afraid.org'){
				$F(path +   'Username'  , '-');
				$F(path +   'Key'  ,node_ddns[14].value);
			}
			
			if (ddnsService == 'dlinkddns.com')
			{
				$F(path + 'Username',  node_ddns[15].value);
				$F(path + 'Password',  node_ddns[16].value);	
			}
			
			if (ddnsService == 'www.noip.com')
			{
				$F(path + 'Username',  node_ddns[17].value);
				$F(path + 'Password',  node_ddns[18].value);	
			}
	
		$H({
			'var:menu'        : 'advanced',
			'var:page'        : 'ddns',
			'obj-action'      : 'set',
			'getpage'         : 'html/index.html',
			'errorpage'       : 'html/index.html',
			'var:errorpage'   : 'ddns',
			'var:CacheLastData' : ViewState.Save()
		});
	
		$('uiPostForm').submit();	
		}
	}
	
}


function DeleteClick()
{   
	if(!confirm(SEcode[1008])){
		return false;
	}
	
	for (var i = 0; i < G_DDNS.length; i++) {
			var tmpid = "choose" + i;
			if (Form.Checkbox(tmpid) == 1) {				
					$H({
					'del-obj'    : G_DDNS[i][9] ,
					'obj-action' : 'del',
					'var:menu'   : 'advanced',
					'var:page'   : 'ddns',
					'getpage'    : 'html/index.html',
					'errorpage'  : 'html/index.html',
					'var:errorpage' : 'ddns',
					'var:CacheLastData' : ViewState.Save()
			});
			$('uiPostForm').submit();
			return;
		 }
	}
}


function dealWithError(){

	if (G_Error != 1){ return false; }
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
