//NAPT LIST
var G_PortMapping = [];
var m = 0;
var G_WanConns    = [];
var n = 0;
var buttoncheck = 0;
var choose_row = 0;
var G_del_flag = 0;
var G_Rules_Count = 0;
var G_AddIndex = 0;
var G_DelIndex = 0;
var G_RulesCount = 0;
var G_AddRules = [];
var remainNum = 32 ;
var wanidx=0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_WanConns[n] = [];
			G_WanConns[n][0] = "$01";
			G_WanConns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";
			G_WanConns[n][2] = "$02";
			G_WanConns[n][3] = "$03";
			<?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$10.PortMapping. "PortMappingEnabled PortMappingDescription RemoteHost PortMappingProtocol ExternalPort ExternalPortEndRange InternalPort InternalClient PortMappingTime"
			`	G_PortMapping[m] = [];
				G_PortMapping[m][0] = "$01";
				G_PortMapping[m][1] = "$02";
				G_PortMapping[m][2] = "$03";
				G_PortMapping[m][3] = "$04";
				G_PortMapping[m][4] = "$05";
				G_PortMapping[m][5] = "$06";
				G_PortMapping[m][6] = "$07";
				G_PortMapping[m][7] = "$08";
				G_PortMapping[m][8] = "$09";
				G_PortMapping[m][9] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$10.PortMapping.$00.";
				G_PortMapping[m][10] = $07+$06-$05;
				G_PortMapping[m][11] = G_WanConns[n][0] ;
				m++;
			`?>
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_WanConns[n] = [];
			G_WanConns[n][0] = "$01";
			G_WanConns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";
			G_WanConns[n][2] = "$02";
			G_WanConns[n][3] = "$03";
			<?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$10.PortMapping. "PortMappingEnabled PortMappingDescription RemoteHost PortMappingProtocol ExternalPort ExternalPortEndRange InternalPort InternalClient PortMappingTime"
			`	G_PortMapping[m] = [];
				G_PortMapping[m][0] = "$01";
				G_PortMapping[m][1] = "$02";
				G_PortMapping[m][2] = "$03";
				G_PortMapping[m][3] = "$04";
				G_PortMapping[m][4] = "$05";
				G_PortMapping[m][5] = "$06";
				G_PortMapping[m][6] = "$07";
				G_PortMapping[m][7] = "$08";
				G_PortMapping[m][8] = "$09";
				G_PortMapping[m][9] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$10.PortMapping.$00.";
				G_PortMapping[m][10] = $07+$06-$05;
				G_PortMapping[m][11] = G_WanConns[n][0] ;
				m++;
			`?>
			n++;
		`?>
	`?>
`?>
`?>

var G_RACL = [];
var G_RPath = [];
var t = 0, l = 0;
<?setvar var:num <?get :InternetGatewayDevice.X_TWSZ-COM_ACL.RACLNumberOfEntries?>?>
<?if gt $var:num 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL. "DevPath ServiceNumberOfEntries"
	`	G_RPath[t] = "<?echo $11?>," + "<?echo $10?>,"+ "<?echo $12?>";
		nRACL = 0;
		<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL.$10.Service. "Name Enable SrcIP SrcMask Protocol InternalPort ExternalPort"
		`	G_RACL[l] = [];
			G_RACL[l][0] = "$01"; //Name
			G_RACL[l][1] = "$02"; //enable
			G_RACL[l][2] = "$03"; //ip
			G_RACL[l][3] = "$04"; //mask
			G_RACL[l][4] = "$05"; //protocol
			G_RACL[l][5] = "$06"; //InternalPort
			G_RACL[l][6] = "$07"; //ExternalPort
			G_RACL[l][7] = "$11"; //DevPath
			l++;
			nRACL++;
		`?>
		t++;
	`?>
`?>


var wanEditable = '<?echo $var:wanEditable?>';
function uiOnload(){
	//生成WAN连接下拉柿
	remain = remainNum - G_PortMapping.length;
	$('remain_num').innerHTML = remain;
	var _text = [],_value = [];
   // alter(G_WanConns[i][2]);
	for(var i = 0; i < G_WanConns.length; i++){
		//alter(G_WanConns[i][2]);
		if (G_WanConns[i][0].indexOf('br') > -1||G_WanConns[i][2]=="TR069") {
			continue;
		}
		
		if((G_WanConns[i][3]=="IP_Bridged") || (G_WanConns[i][3]=="PPPoE_Bridged"))
			{
			    continue;
			}
			
		_text.push(G_WanConns[i][0]);
		_value.push(G_WanConns[i][1]);
	}
	$('port_setup').style.display = 'none';
	$('port_setup_edit').style.display = 'none';
	$('add').disabled = false;
	$('edit').disabled = false;
	$('Delete').disabled = false;
	$S('SELECT_WanConns',_text,_value);
	$('SELECT_WanConns').value = (wanEditable == '-' ? (G_WanConns[0] ? G_WanConns[0][1] : '') : wanEditable);
	//生成配置列表	
	load_time();
	load_edit_time();
	createTable('All');
	$('SELECT_WanConns').selectedIndex=0;
}

function createTable(showType){
	if(showType=='All'){
		var array_value = [];
	
		for(var k = 0,i = 0; k < G_PortMapping.length; k++){
		
			array_value[k] = [];
			array_value[k].push('<input type="checkbox" id="choose'+k+'" name="choose'+k+'">');
			array_value[k].push(G_PortMapping[k][1].ellipsis(8)); //描述
			array_value[k].push(G_PortMapping[k][11].ellipsis(8)); //Wan 接口名 
			array_value[k].push(G_PortMapping[k][4]+'/'+G_PortMapping[k][5]); //外部公共端口
		   //array_value[k].push(G_PortMapping[k][5]); //外部公共端口
			array_value[k].push(G_PortMapping[k][3]); //协议
			array_value[k].push(G_PortMapping[k][6]+'/'+G_PortMapping[k][10]); //LAN端主机端卿
		   //array_value[k].push(G_PortMapping[k][10]); //LAN端主机端卿
			array_value[k].push(G_PortMapping[k][7]); //LAN端IP
			array_value[k].push(transform_time(G_PortMapping[k][8])); //Schedule Rule
			array_value[k].push(G_PortMapping[k][2]); //远端主机IP
			i++;
	
		}
	}else{
	var wanConns    = $('SELECT_WanConns').value;
	var array_value = [];
	
	for(var k = 0,i = 0; k < G_PortMapping.length; k++){
		if(G_PortMapping[k][9].indexOf(wanConns) > -1){
			array_value[k] = [];
			array_value[k].push('<input type="checkbox" id="choose'+k+'" name="choose'+k+'">');
			array_value[k].push(G_PortMapping[k][1].ellipsis(8)); //描述
			array_value[k].push(G_PortMapping[k][11].ellipsis(8)); 
			array_value[k].push(G_PortMapping[k][4]+'/'+G_PortMapping[k][5]); //外部公共端口
		   //array_value[k].push(G_PortMapping[k][5]); //外部公共端口
			array_value[k].push(G_PortMapping[k][3]); //协议
			array_value[k].push(G_PortMapping[k][6]+'/'+G_PortMapping[k][10]); //LAN端主机端卿
		   //array_value[k].push(G_PortMapping[k][10]); //LAN端主机端卿
			array_value[k].push(G_PortMapping[k][7]); //LAN端IP
			array_value[k].push(transform_time(G_PortMapping[k][8])); //Schedule Rule
			array_value[k].push(G_PortMapping[k][2]); //远端主机IP
			i++;
		}
	}
	}
	$T('PortMappingList',array_value);
	var node_tr = $('PortMappingList').getElementsByTagName('tr');
	/*for(var b = 0; b < G_PortMapping.length; b++){
	$('PortMappingList_' + b + '1').title = G_PortMapping[b][1];
	}
		
	for(var i = 0; i < G_PortMapping.length; i++){
	$('PortMappingList_' + i + '2').title = G_PortMapping[i][11];
	
	}*/
	
	for(var j = 0, new_node; j < node_tr.length - 1; j++){
		new_node = $(j + '2');
		if(new_node){
			new_node.title     = new_node.innerHTML;
			new_node.innerHTML = new_node.innerHTML.ellipsis();
		}
	}
}

function uiDelete(){
	var num = 0;
	for (var i = 0; i < G_PortMapping.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{
			G_del_flag = i;
			num++;
		}			
	}
	if(0 == num)
	{
		alert(SEcode[9888]);
		return false;
	}
	else if(num > 1)
	{
		alert(SEcode[1009]);
		return false;
	}
	if(!confirm(SEcode[1008])){return false;}
	tmpid = "choose" + G_del_flag;
	$(tmpid).checked = false;
	$(tmpid).disabled = true;
	$('add').disabled = true;
	$('edit').disabled = true;
	$('Delete').disabled = true;
	
	$H({
		'del-obj'			: G_PortMapping[G_del_flag][9],
		'obj-action'		: 'del',
		'var:menu'			: 'advanced',
		'var:page'			: 'portforwd',
		'errorpage'         : 'html/index.html',
		'getpage'			: 'html/index.html'
	});
	
	$('uiPostForm').submit();
	
}

function uiAdd(){
	var MContent = $('Content').offsetHeight;
	$('port_setup').style.display = 'block';
	$('port_setup_edit').style.display = 'none';
	buttoncheck = 0;
	$('sIp').value = "";
	$('add').disabled = true;
	$('edit').disabled = true;
	$('Delete').disabled = true;
	G_del_flag = 0;
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + $('port_setup').offsetHeight;
	    $('Menu').style.height=ContentHeigh + 'px';
	    $('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
	
}

function  matchWanConnction(j){

	for(var k=0; k<G_WanConns.length; k++){
		if(G_PortMapping[j][9].indexOf(G_WanConns[k][1])>-1){
			return G_WanConns[k][1];
		}
	};
	
}
function uiEdit(){
	var num = 0;
	for (var i = 0; i < G_PortMapping.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{
			G_del_flag = i;
			num++;
		}			
	}
	if(0 == num)
	{
		alert(SEcode[9888]);
		return false;
	}
	else if(num > 1)
	{
		alert(SEcode[1009]);
		return false;
	}
	
	$('port_setup_edit').style.display = 'block';
	$('port_setup').style.display = 'none';
	Heigtduiqi();
	$('add').disabled = true;
	$('edit').disabled = true;
	$('Delete').disabled = true;
	$('iEndEdit').disabled = true;
	buttoncheck=1;	
	if(G_PortMapping[G_del_flag][3] == 'TCP' || G_PortMapping[G_del_flag][3] == 'tCP' || G_PortMapping[G_del_flag][3] == 'tcP'||G_PortMapping[G_del_flag][3] == 'tCp' || G_PortMapping[G_del_flag][3] == 'TCp' || G_PortMapping[G_del_flag][3] == 'Tcp' || G_PortMapping[G_del_flag][3] == 'TcP')
	{
		G_PortMapping[i][3] = 'tcp';
	}
	
	$('editCusSrvName').value=G_PortMapping[G_del_flag][1];
	$('eStartEdit').value=G_PortMapping[G_del_flag][4];
	$('eEndEdit').value=G_PortMapping[G_del_flag][5];
	$('protoEdit').value=(G_PortMapping[G_del_flag][3] == 'tcp' ? 1 :2);
	$('iStartEdit').value=G_PortMapping[G_del_flag][6];
	$('iEndEdit').value=G_PortMapping[G_del_flag][10];
	$('iSrcIpEdit').value=G_PortMapping[G_del_flag][2];
	$('ipEdit').value=G_PortMapping[G_del_flag][7];
	$('time_schedule_edit').value=G_PortMapping[G_del_flag][8];
	$('SELECT_WanConns').value=matchWanConnction(G_del_flag);

    //FTP virtual server's port must be 21
    if($('eStartEdit').value == '21' && $('eEndEdit').value == '21' && $('protoEdit').value == 1)
    {
        $('eStartEdit').disabled = 1;
        $('eEndEdit').disabled = 1;
    }
    
	choose_row=G_del_flag;    
	G_del_flag = 0;
	return;
}	   

function uiSubmit(){
	if(!checkIllegalValue()){
		return false;
	}
	if(($('sIp').value == "" && 0 == buttoncheck) ||($('ipEdit').value == "" && 1 == buttoncheck))
	{
		alert(data_language.dynamic.serviceip);
		return false;
	}
	//提交分两种情况：添加和编輿
	var node_napt = $('cusSrvName','iSrcIp','INPUT_RemoteMask','proto','eStart','eEnd','iStart','sIp','srvName');
	var node_napt_edit = $('editCusSrvName','iSrcIpEdit','INPUT_RemoteMask','protoEdit','eStartEdit','eEndEdit','iStartEdit','ipEdit','time_schedule_edit','iEndEdit');

	//关于外部公共端口的判政
	var boolean   = (node_napt[4].value == '1' && node_napt[5].value == '65535');
	var boolean_edit   = (node_napt_edit[4].value == '1' && node_napt_edit[5].value == '65535');

	var wanConns    = $('SELECT_WanConns').value;
	var _path = wanConns.split('.');
	//var _path = $('SELECT_WanConns').value;
	var WanInterface=$('SELECT_WanConns').value;
	var _path_edit = G_PortMapping[choose_row] ? G_PortMapping[choose_row][9].split('.') : '';

    
    var portStart;
    var portEnd;
    var protocol;
    var serviceName;
    if(0 == buttoncheck)
    {
        portStart = node_napt[4].value;
        portEnd = node_napt[5].value;
        protocol = node_napt[3].value == 1 ? 'TCP' : 'UDP';
        serviceName = node_napt[8].value;
    }
    else
    {
        portStart = node_napt_edit[4].value;
        portEnd = node_napt_edit[5].value;
        protocol = node_napt_edit[3].value == 1 ? 'TCP' : 'UDP';
        serviceName = node_napt_edit[0].value;
    }
        
    for(var i = 0; i < G_RACL.length; i++)
    {
        if($('SELECT_WanConns').value == G_RACL[i][7])
        {
            if(parseInt(portStart) <= parseInt(G_RACL[i][6])
            && parseInt(portEnd) >= parseInt(G_RACL[i][6])
            && protocol == G_RACL[i][4])
            {
                //FTP virtual server port must be 21
                if(G_RACL[i][6] == '21' && protocol == 'TCP')
                {
                    alert("The port is confilict with Access Control Service FTP, Please change the port of Access Control Service FTP.");
                    return false;
                }
                else
                {
                    alert("The Access Control Service port "+G_RACL[i][6]+" is in the external port range, please reset the external port range or the Access Control Service port!");
                    return false;
                }
            }
        }
    }
    
	if(0 == buttoncheck){
		
		G_AddRules = [];
	    G_AddIndex = 0;
	    ajax_Add(G_AddIndex);
		$('apply1').disabled = true;
		$('cancle1').disabled = true;
	} else {
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.PortMappingEnabled' , '1');
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.PortMappingDescription' , node_napt_edit[0].value);
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.RemoteHost' , node_napt_edit[1].value);
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.PortMappingProtocol' , (node_napt_edit[3].value == 1 ? 'tcp' : 'udp'));
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.ExternalPort' , node_napt_edit[4].value);
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.ExternalPortEndRange' ,node_napt_edit[5].value);
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.InternalPort' , boolean ? undefined : node_napt_edit[6].value);
			//$F(':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.InternalPortEndRange' , boolean ? undefined : node_napt_edit[9].value);
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.InternalClient' , node_napt_edit[7].value),
			$F(':InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.'+ _path_edit[8] +'.PortMappingTime' , node_napt_edit[8].value);
			$F('obj-action','set');	
		
		$H({
			'var:menu'     : 'advanced',
			'var:page'     : 'portforwd',
			'var:WanType' : (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP'),
			'var:FstIndex': _path[4],
			'var:SecIndex': _path[6],
			'var:ThdIndex': _path_edit[8],
			'getpage'      : 'html/index.html',
			'errorpage'    : 'html/index.html',
			'var:errorpage': 'portforwd',
			'var:wanEditable' : 'InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4] +'.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path_edit[8] +'',
			'var:CacheLastData': ViewState.Save()
		});
		$('uiPostForm').submit();
	}
}
//判断值是否存在为0的函数
function checkIllegalValue(){
	var nodes = $('eStart','eEnd','iStart','iEnd');
	if(nodes[0].value == '0')
	{
		DealWith.Wrong('INPUT_ExternalPort',SEcode[1003]);
		return false;
	} 
	else if(nodes[2].value == '0') 
	{
		DealWith.Wrong('INPUT_InternalPort',SEcode[1003]);
		return false;
	}
	if(nodes[1].value == '0')
	{
		nodes[1].value = nodes[0].value;
		nodes[3].value = nodes[2].value;	
	}

	return true;
}


var ajax;
function clearOldForm(){
	var _form   = document.forms[1];
	var _inputs = _form.getElementsByTagName('input');
	
	for(var i = _inputs.length; 0 < i; i--){
		_form.removeChild(_inputs[i-1]);
	}	
}

function ajax_Add(i){
	
	 with (document.vsFrm) {
		//关于外部公共端口的判政
		var boolean   = (eStart[i].value == '1' && eEnd[i].value == '65535');
		var WanInterface=$('SELECT_WanConns').value;
		var _path = WanInterface.split('.');
		
		clearOldForm();
		$H({
				'add-obj' : 'InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4]+ '.WAN'+ (WanInterface.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.',
				':PortMappingEnabled'     : '1',
				':PortMappingDescription' : (radiosrv[0].checked == true) ? srvName.value : cusSrvName.value,
				':RemoteHost'             : iSrcIp[i].value,
				':PortMappingProtocol'    : (proto[i].value == 1 ? 'tcp' : 'udp'),
				':ExternalPort'           :  eStart[i].value,
				':ExternalPortEndRange'   :  eEnd[i].value,
				':InternalPort'           : iStart[i].value || undefined,
				//':InternalPortEndRange'           : iEnd[i].value || undefined,
				':InternalClient'         : sIp.value,
				':PortMappingTime'   :  $('time_schedule').value,
				'obj-action' : 'add-set',
				'var:menu'			: 'advanced',
				'var:page'			: 'portforwd',
				'getpage'			: 'html/page/portforwd.ajax.js'
			});
	  }
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_AddProc);
	ajax.post($('uiPostForm'));
}


//解析结果
function ajax_AddProc(responseText){
	try
	{
		eval(responseText);
	}
	catch (ex)
	{
		G_Error == '1'
		return;
	}
	//添加成功，如果有下一条则继续添加下一条规则，否则跳转到规则列蟿
	if (G_Error == '0')
	{
		G_AddIndex++;
		
		G_AddRules.push(G_NewObjIndex);
		
		if(G_AddIndex < G_Rules_Count)
		{
			ajax_Add(G_AddIndex);
		}
		else
		{
			$G('/cgi-bin/webproc',{
				'getpage'			: 'html/index.html',
				'var:menu'			: 'advanced',
				'var:page'			: 'portforwd',
				'errorpage'			: 'html/index.html'
			});	
		}
	}
	else
	{//添加失败，删除已经添加成功的规则
		//已经有添加规则，需要删除添加成功的规则
		if (G_AddRules.length > 0)
		{
			G_DelIndex = 0;
			ajax_Del(G_DelIndex);
		}
		else
		{
			$('apply1').disabled = false;
			$('cancle1').disabled = false;
			dealWithError();
		}
	}
}


//启动ajax删除规则
function ajax_Del(i){
	var wanValue= matchWanConnction(i);
	var _path = wanValue.split('.');
//	var _action = new Date();

	var del_obj = 0;
	if(1 == G_del_flag){
		del_obj = G_PortMapping[i][9];
	}
	else{
		var _index = G_AddRules[i];
		del_obj = 'InternetGatewayDevice.WANDevice.'+ _path[2] +'.WANConnectionDevice.'+ _path[4]+ '.WAN'+ (wanValue.indexOf('PPP') > 0 ? 'PPP' : 'IP') +'Connection.'+ _path[6] +'.PortMapping.' + _index + ".";
	}
	
	clearOldForm();
	$H({
		'del-obj'			: del_obj,
		'obj-action'		: 'del',
		'var:menu'			: 'advanced',
		'var:page'			: 'portforwd',
		'getpage'			: 'html/page/portforwd.ajax.js',		
		'errorpage'         : 'html/index.html'
	});
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_DelProc);
	ajax.post($('uiPostForm'));
}

//解析结果
function ajax_DelProc(responseText){
	if(1 == G_del_flag){
		//if(!confirm(SEcode[1001])){return false;}
		for (var i = 0; i < G_PortMapping.length; i++) {
			var tmpid = "choose" + i;
			if (Form.Checkbox(tmpid) == 1) 
			{
				$(tmpid).checked = false;
				$(tmpid).disabled = true;
				ajax_Del(i);
			}
			else
			{
				$G('/cgi-bin/webproc',{
					'getpage'			: 'html/index.html',					
					'var:menu'			: 'advanced',
					'var:page'			: 'portforwd',
					'errorpage'			: 'html/index.html'
				});	
			}
		}
	}
	else{
		//没删完，就继续删除下一替
		if (G_DelIndex < (G_AddRules.length))
		{
			G_DelIndex++;
			ajax_Del(G_DelIndex);
		}
		//删除完，则清空一些变量，并处理错譿
		else
		{
			$('apply1').disabled = false;
			$('cancle1').disabled = false;
			dealWithError();
		}
	}
}


/**********************           for time schedule *****************************/
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

//将Path转换为Schedule的Name
function transform_time(Path){
	if(Path =='Always')
		return 'Always';
	if(Path =='Never')
		return 'Never';
	for(var i = 0; i < G_SchedEntry.length; i++){
		if(G_SchedEntry[i][0] == Path){
			return G_SchedEntry[i][1];
		}
	}
   return 'Always';
}
/*****************************************************************************/

function load_time()
{
	var _text = [], _value = [];
	_text.push(data_language.dynamic.always);
	_value.push("Always");
	for(var i = 0; i < G_SchedEntry.length; i++){
		_text.push(G_SchedEntry[i][1]);
		_value.push(G_SchedEntry[i][0]);
	}
	_text.push(data_language.dynamic.never);
	_value.push("Never");
	$S('time_schedule',_text,_value);

}

function load_edit_time()
{
	var _text = [], _value = [];
	_text.push(data_language.dynamic.always);
	_value.push("Always");
	_text.push(data_language.dynamic.never);
	_value.push("Never");
	for(var i = 0; i < G_SchedEntry.length; i++){
		_text.push(G_SchedEntry[i][1]);
		_value.push(G_SchedEntry[i][0]);
	}
	$S('time_schedule_edit',_text,_value);

}

<!-- modify end by LX 2008-09-18 --><script language="javascript">
TOTAL_ROW = 12
TOTAL_APP = 134
var usedName = '';

FIRST_APP = "Select One";
//RE_ALL_MSG = 'Require BOTH External/Internal Port range(Start/End values).';
keyPressed = false;

var doEdit = false;
var editId;

function cV(name, entryNum)
{
   this.name = name;
   this.eNum = entryNum;
   this.e = new Array(5);
}

function iVe(eStart, eEnd, proto, iStart, iEnd)
{
   this.eStart = eStart;
   this.eEnd = eEnd;
   this.proto = proto;
   this.iStart = iStart;
   this.iEnd = iEnd;
}

// Declare an array (TOTAL_APP) of vs applications
var v = new Array(TOTAL_APP);

v[0] = new cV("Active Worlds",4);
v[0].e[0] = new iVe("3000", "3000", "1", "3000", "3000");
v[0].e[1] = new iVe("5670", "5670", "1", "5670", "5670");
v[0].e[2] = new iVe("7777", "7777", "1", "7777", "7777");
v[0].e[3] = new iVe("7000", "7000", "1", "7000", "7000");

v[1] = new cV("Age of Empires",4);
v[1].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[1].e[1] = new iVe("6073",   "6073", "1", "6073", "6073");
v[1].e[2] = new iVe("2300",   "2400", "1", "2300", "2400");
v[1].e[3] = new iVe("2300",   "2400", "2", "2300", "2400");

v[2] = new cV("Age of Empires Expansion: The Rise of Rome",4);
v[2].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[2].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[2].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[2].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[3] = new cV("Age of Empires II Expansion: The Conquerors",4);
v[3].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[3].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[3].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[3].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[4] = new cV("Age of Empires II: The Age of Kings",4);
v[4].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[4].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[4].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[4].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[5] = new cV("Age of Kings",4);
v[5].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[5].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[5].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[5].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[6] = new cV("Age of Wonders",4);
v[6].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[6].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[6].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[6].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[7] = new cV("Aliens vs. Predator",3);
v[7].e[0] = new iVe("80", "80", "2", "80", "80");
v[7].e[1] = new iVe("2300", "2400", "2", "2300", "2400");
v[7].e[2] = new iVe("8000", "8999", "2", "8000", "8999");
v[7].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[8] = new cV("AOL Instant Messenger",1);
v[8].e[0] = new iVe("443", "443", "1", "443", "443");

v[9] = new cV("Audiogalaxy Satellite",2);
v[9].e[0] = new iVe("41000", "50000", "1", "41000", "50000");
v[9].e[1] = new iVe("1117", "5190", "1", "1117", "5190");

v[10] = new cV("AUTH",1);
v[10].e[0] = new iVe("113", "113", "1", "113", "113");

v[11] = new cV("Baldur's Gate",2);
v[11].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[11].e[1] = new iVe("47624", "47624", "2", "47624", "47624");

v[12] = new cV("BattleCom",4);
v[12].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[12].e[1] = new iVe("47624", "47624", "2", "47624", "47624");
v[12].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[12].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[13] = new cV("Battlefield Communicator",2);
v[13].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[13].e[1] = new iVe("47624", "47624", "2", "47624", "47624");

v[14] = new cV("Black and White",4);
v[14].e[0] = new iVe("2611", "2612", "1", "2611", "2612");
v[14].e[1] = new iVe("6667", "6667", "1", "6667", "6667");
v[14].e[2] = new iVe("6500", "6500", "2", "6500", "6500");
v[14].e[3] = new iVe("27900", "27900", "2", "27900", "27900");

v[15] = new cV("Blizzard Battle.net",3);
v[15].e[0] = new iVe("4000", "4000", "1", "4000", "4000");
v[15].e[1] = new iVe("6112", "6112", "1", "6112", "6112");
v[15].e[2] = new iVe("6112", "6112", "2", "6112", "6112");

v[16] = new cV("Buddy Phone",1);
v[16].e[0] = new iVe("700", "701", "2", "700", "701");

v[17] = new cV("Bungie.net",1);
v[17].e[0] = new iVe("3453", "3453", "1", "3453", "3453");

v[18] = new cV("Camerades",2);
v[18].e[0] = new iVe("2047", "2048", "1", "2047", "2048");
v[18].e[1] = new iVe("2047", "2048", "2", "2047", "2048");

v[19] = new cV("CART Precision Racing",4);
v[19].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[19].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[19].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[19].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[20] = new cV("Close Combat for Windows",4);
v[20].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[20].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[20].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[20].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[21] = new cV("Close Combat III: The Russian Front",4);
v[21].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[21].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[21].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[21].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[22] = new cV("Close Combat: A Bridge Too Far",4);
v[22].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[22].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[22].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[22].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[23] = new cV("Combat Flight Simulator 2: WWII Pacific Theater",4);
v[23].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[23].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[23].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[23].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[24] = new cV("Combat Flight Simulator: WWII Europe Series",4);
v[24].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[24].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[24].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[24].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[25] = new cV("Crimson Skies",4);
v[25].e[0] = new iVe("28805", "28805", "1", "28805", "28805");
v[25].e[1] = new iVe("28801", "28801", "1", "28801", "28801");
v[25].e[2] = new iVe("3040", "3040", "1", "3040", "3040");
v[25].e[3] = new iVe("1121", "1121", "1", "1121", "1121");

v[26] = new cV("CuSeeMe",12);
v[26].e[0] = new iVe("24032", "24032", "2", "24032", "24032");
v[26].e[1] = new iVe("1414", "1414", "2", "1414", "1414");
v[26].e[2] = new iVe("1424", "1424", "2", "1424", "1424");
v[26].e[3] = new iVe("1503", "1503", "1", "1503", "1503");
v[26].e[4] = new iVe("1720", "1720", "1", "1720", "1720");
v[26].e[5] = new iVe("1812", "1813", "2", "1812", "1813");
v[26].e[6] = new iVe("7640", "7640", "2", "7640", "7640");
v[26].e[7] = new iVe("7642", "7642", "1", "7642", "7642");
v[26].e[8] = new iVe("7648", "7648", "1", "7648", "7648");
v[26].e[9] = new iVe("7648", "7648", "2", "7648", "7648");
v[26].e[10] = new iVe("7649", "7649", "1", "7649", "7649");
v[26].e[11] = new iVe("56800", "56800", "2", "56800", "56800");

v[27] = new cV("Dark Reign 2",2);
v[27].e[0] = new iVe("26214", "26214", "1", "26214", "26214");
v[27].e[1] = new iVe("26214", "26214", "2", "26214", "26214");

v[28] = new cV("Delta Force 2",1);
v[28].e[0] = new iVe("3568", "3569", "2", "3568", "3569");

v[29] = new cV("Delta Three PC to Phone",6);
v[29].e[0] = new iVe("12053", "12053", "1", "12053", "12053");
v[29].e[1] = new iVe("12083", "12083", "1", "12083", "12083");
v[29].e[2] = new iVe("12080", "12080", "2", "12080", "12080");
v[29].e[3] = new iVe("12120", "12120", "2", "12120", "12120");
v[29].e[4] = new iVe("12122", "12122", "2", "12122", "12122");
v[29].e[5] = new iVe("24150", "24179", "2", "24150", "24179");

v[30] = new cV("Descent 3",3);
v[30].e[0] = new iVe("7170", "7170", "1", "7170", "7170");
v[30].e[1] = new iVe("2092", "2092", "2", "2092", "2092");
v[30].e[2] = new iVe("3445", "3445", "2", "3445", "3445");

v[31] = new cV("Descent Freespace",5);
v[31].e[0] = new iVe("3999", "3999", "1", "3999", "3999");
v[31].e[1] = new iVe("4000", "4000", "2", "4000", "4000");
v[31].e[2] = new iVe("7000", "7000", "2", "7000", "7000");
v[31].e[3] = new iVe("3493", "3493", "2", "3493", "3493");
v[31].e[4] = new iVe("3440", "3440", "2", "3440", "3440");

v[32] = new cV("Diablo I",2);
v[32].e[0] = new iVe("6112", "6112", "1", "6112", "6112");
v[32].e[1] = new iVe("6112", "6112", "2", "6112", "6112");

v[33] = new cV("Diablo II",2);
v[33].e[0] = new iVe("4000", "4000", "1", "4000", "4000");
v[33].e[1] = new iVe("6112", "6119", "2", "6112", "6119");

v[34] = new cV("DialPad.Com",5);
v[34].e[0] = new iVe("51200", "51201", "2", "51200", "51201");
v[34].e[1] = new iVe("7175", "7175", "1", "7175", "7175");
v[34].e[2] = new iVe("51210", "51210", "1", "51210", "51210");
v[34].e[3] = new iVe("8680", "8686", "1", "8680", "8686");
v[34].e[4] = new iVe("1584", "1585", "1", "1584", "1585");

v[35] = new cV("DirectX 7 Games",3);
v[35].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[35].e[1] = new iVe("2300", "2400", "1", "2300", "2400");
v[35].e[2] = new iVe("2300", "2400", "2", "2300", "2400");

v[36] = new cV("DirectX 8 Games",2);
v[36].e[0] = new iVe("6073", "6073", "1", "6073", "6073");
v[36].e[1] = new iVe("2300", "2400", "2", "2300", "2400");

v[37] = new cV("Domain Name Server (DNS)",2);
v[37].e[0] = new iVe("53", "53", "1", "53", "53");
v[37].e[1] = new iVe("53", "53", "2", "53", "53");

v[38] = new cV("Doom",2);
v[38].e[0] = new iVe("666", "666", "1", "666", "666");
v[38].e[1] = new iVe("666", "666", "2", "666", "666");

v[39] = new cV("Dune 2000",2);
v[39].e[0] = new iVe("4000", "4000", "1", "4000", "4000");
v[39].e[1] = new iVe("1140", "1234", "2", "1140", "1234");

v[40] = new cV("Dwyco Video Conferencing",4);
v[40].e[0] = new iVe("12000", "16090", "2", "12000", "16090");
v[40].e[1] = new iVe("1024", "5000", "1", "1024", "5000");
v[40].e[2] = new iVe("6700", "6702", "1", "6700", "6702");
v[40].e[3] = new iVe("6880", "6880", "1", "6880", "6880");

v[41] = new cV("Elite Force",4);
v[41].e[0] = new iVe("26000", "26000", "2", "26000", "26000");
v[41].e[1] = new iVe("27500", "27500", "2", "27500", "27500");
v[41].e[2] = new iVe("27910", "27910", "2", "27910", "27910");
v[41].e[3] = new iVe("27960", "27960", "2", "27960", "27960");

v[42] = new cV("Everquest",3);
v[42].e[0] = new iVe("1024", "6000", "1", "1024", "6000");
v[42].e[1] = new iVe("1024", "6000", "2", "1024", "6000");
v[42].e[2] = new iVe("6001", "7000", "1", "6001", "7000");

v[43] = new cV("F-16",1);
v[43].e[0] = new iVe("3862", "3863", "2", "3862", "3863");

v[44] = new cV("F-22 Lightning 3",3);
v[44].e[0] = new iVe("3875", "3875", "2", "3875", "3875");
v[44].e[1] = new iVe("4533", "4534", "2", "4533", "4534");
v[44].e[2] = new iVe("4660", "4670", "2", "4660", "4670");

v[45] = new cV("F-22 Raptor",1);
v[45].e[0] = new iVe("3874", "3875", "2", "3874", "3875");

v[46] = new cV("F22 Raptor (Novalogic)",1);
v[46].e[0] = new iVe("3874", "3874", "2", "3874", "3874");

v[47] = new cV("Falcon 4.0",1);
v[47].e[0] = new iVe("2934", "2935", "2", "2934", "2935");

v[48] = new cV("Fighter Ace II",6);
v[48].e[0] = new iVe("50000", "50100", "1", "50000", "50100");
v[48].e[1] = new iVe("50000", "50100", "2", "50000", "50100");
v[48].e[2] = new iVe("47624", "47624", "1", "47624", "47624");
v[48].e[3] = new iVe("6073", "6073", "1", "6073", "6073");
v[48].e[4] = new iVe("2300", "2400", "1", "2300", "2400");
v[48].e[5] = new iVe("2300", "2400", "2", "2300", "2400");

v[49] = new cV("Flight Simulator 2000",4);
v[49].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[49].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[49].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[49].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[50] = new cV("Freetel",1);
v[50].e[0] = new iVe("21300", "21303", "2", "21300", "21303");

v[51] = new cV("FTP Server",1);
v[51].e[0] = new iVe("21", "21", "1", "21", "21");

v[52] = new cV("GNUtella",2);
v[52].e[0] = new iVe("6346", "6347", "1", "6346", "6347");
v[52].e[1] = new iVe("6346", "6347", "2", "6346", "6347");

v[53] = new cV("Golf 2001 Edition",4);
v[53].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[53].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[53].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[53].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[54] = new cV("Go2Call",2);
v[54].e[0] = new iVe("2090", "2091", "2", "2090", "2091");
v[54].e[1] = new iVe("2090", "2090", "1", "2090", "2090");

v[55] = new cV("Half Life",6);
v[55].e[0] = new iVe("6003", "6003", "1", "6003", "6003");
v[55].e[1] = new iVe("6003", "6003", "2", "6003", "6003");
v[55].e[2] = new iVe("7001", "7001", "1", "7001", "7001");
v[55].e[3] = new iVe("7001", "7001", "2", "7001", "7001");
v[55].e[4] = new iVe("27005", "27005", "2", "27005", "27005");
v[55].e[5] = new iVe("27010", "27015", "2", "27010", "27015");

v[56] = new cV("Half Life Server",1);
v[56].e[0] = new iVe("27015", "27015", "2", "27015", "27015");

v[57] = new cV("Heretic II Server",2);
v[57].e[0] = new iVe("28910", "28910", "1", "28910", "28910");
v[57].e[1] = new iVe("28910", "28910", "2", "28910", "28910");

v[58] = new cV("I76",1);
v[58].e[0] = new iVe("21154", "21156", "2", "21154", "21156");

v[59] = new cV("ICUII Client",6);
v[59].e[0] = new iVe("2019", "2019", "1", "2019", "2019");
v[59].e[1] = new iVe("2000", "2038", "1", "2000", "2038");
v[59].e[2] = new iVe("2050", "2051", "1", "2050", "2051");
v[59].e[3] = new iVe("2069", "2069", "1", "2069", "2069");
v[59].e[4] = new iVe("2085", "2085", "1", "2085", "2085");
v[59].e[5] = new iVe("3010", "3030", "1", "3010", "3030");

v[60] = new cV("IPSEC",1);
v[60].e[0] = new iVe("500", "500", "2", "500", "500");

v[61] = new cV("Ivisit",2);
v[61].e[0] = new iVe("9943", "9943", "2", "9943", "9943");
v[61].e[1] = new iVe("56768", "56768", "2", "56768", "56768");

v[62] = new cV("IRC",3);
v[62].e[0] = new iVe("1024", "5000", "1", "1024", "5000");
v[62].e[1] = new iVe("6660", "6669", "1", "6660", "6669");
v[62].e[2] = new iVe("113", "113", "2", "113", "113");

v[63] = new cV("IStreamVideo2HP",2);
v[63].e[0] = new iVe("8076", "8077", "1", "8076", "8077");
v[63].e[1] = new iVe("8076", "8077", "2", "8076", "8077");

v[64] = new cV("KaZaA",1);
v[64].e[0] = new iVe("1024", "1024", "1", "1024", "1024");

v[65] = new cV("Kohan Immortal Sovereigns",4);
v[65].e[0] = new iVe("3855", "3855", "1", "3855", "3855");
v[65].e[1] = new iVe("3855", "3855", "2", "3855", "3855");
v[65].e[2] = new iVe("17437", "17437", "1", "17437", "17437");
v[65].e[3] = new iVe("17437", "17437", "2", "17437", "17437");

v[66] = new cV("LapLink Gold",1);
v[66].e[0] = new iVe("1547", "1547", "1", "1547", "1547");

v[67] = new cV("Links 2001",4);
v[67].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[67].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[67].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[67].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[68] = new cV("Lotus Notes Server",1);
v[68].e[0] = new iVe("1352", "1352", "1", "1352", "1352");

v[69] = new cV("Mail (POP3)",1);
v[69].e[0] = new iVe("110", "110", "1", "110", "110");

v[70] = new cV("Mail (SMTP)",1);
v[70].e[0] = new iVe("25", "25", "1", "25", "25");

v[71] = new cV("MechCommander 2.0",4);
v[71].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[71].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[71].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[71].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[72] = new cV("MechWarrior 4",4);
v[72].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[72].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[72].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[72].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[73] = new cV("Media Player 7",2);
v[73].e[0] = new iVe("1755", "1755", "1", "1755", "1755");
v[73].e[1] = new iVe("70", "7000", "2", "70", "7000");

v[74] = new cV("Midtown Madness 2",4);
v[74].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[74].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[74].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[74].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[75] = new cV("Mig 29",1);
v[75].e[0] = new iVe("3862", "3863", "2", "3862", "3863");

v[76] = new cV("Monster Truck Madness 2",4);
v[76].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[76].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[76].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[76].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[77] = new cV("Motocross Madness 2",4);
v[77].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[77].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[77].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[77].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[78] = new cV("Motorhead Server",2);
v[78].e[0] = new iVe("16000", "16000", "1", "16000", "16000");
v[78].e[1] = new iVe("16000", "16000", "2", "16000", "16000");

v[79] = new cV("MSN Gaming Zone",6);
v[79].e[0] = new iVe("6667", "6667", "1", "6667", "6667");
v[79].e[1] = new iVe("28800", "29000", "1", "28800", "29000");
v[79].e[2] = new iVe("47624", "47624", "1", "47624", "47624");
v[79].e[3] = new iVe("6073", "6073", "1", "6073", "6073");
v[79].e[4] = new iVe("2300", "2400", "1", "2300", "2400");
v[79].e[5] = new iVe("2300", "2400", "2", "2300", "2400");

v[80] = new cV("MSN Messenger",5);
v[80].e[0] = new iVe("6891", "6901", "1", "6891", "6901");
v[80].e[1] = new iVe("1863", "1963", "1", "1863", "1963");
v[80].e[2] = new iVe("1863", "1863", "2", "1863", "1863");
v[80].e[3] = new iVe("5190", "5190", "2", "5190", "5190");
v[80].e[4] = new iVe("6901", "6901", "2", "6901", "6901");

v[81] = new cV("Myth",1);
v[81].e[0] = new iVe("3453", "3453", "1", "3453", "3453");

v[82] = new cV("Myth II Server",1);
v[82].e[0] = new iVe("3453", "3453", "1", "3453", "3453");

v[83] = new cV("Myth: The Fallen Lords",1);
v[83].e[0] = new iVe("3453", "3453", "1", "3453", "3453");

v[84] = new cV("Need for Speed",2);
v[84].e[0] = new iVe("9442", "9442", "1", "9442", "9442");
v[84].e[1] = new iVe("6112", "6112", "2", "6112", "6112");

v[85] = new cV("NetMech",1);
v[85].e[0] = new iVe("21154", "21156", "2", "21154", "21156");

v[86] = new cV("Netmeeting 2.0, 3.0, Intel Video Phone",6);
v[86].e[0] = new iVe("1024", "65534", "2", "1024", "65534");
v[86].e[1] = new iVe("1024", "1502", "1", "1024", "1502");
v[86].e[2] = new iVe("1504", "1730", "1", "1504", "1730");
v[86].e[3] = new iVe("1732", "65534", "1", "1732", "65534");
v[86].e[4] = new iVe("1503", "1503", "1", "1503", "1503");
v[86].e[5] = new iVe("1731", "1731", "1", "1731", "1731");

v[87] = new cV("Network Time Protocol (NTP)",1);
v[87].e[0] = new iVe("123", "123", "2", "123", "123");

v[88] = new cV("News Server (NNTP)",1);
v[88].e[0] = new iVe("119", "119", "1", "119", "119");

v[89] = new cV("OKWeb",3);
v[89].e[0] = new iVe("80", "80", "1", "80", "80");
v[89].e[1] = new iVe("443", "443", "1", "443", "443");
v[89].e[2] = new iVe("5210", "5220", "1", "5210", "5220");

v[90] = new cV("OKWin",3);
v[90].e[0] = new iVe("1729", "1729", "1", "1729", "1729");
v[90].e[1] = new iVe("1909", "1909", "1", "1909", "1909");
v[90].e[2] = new iVe("80", "80", "1", "80", "80");

v[91] = new cV("Outlaws",2);
v[91].e[0] = new iVe("5310", "5310", "1", "5310", "5310");
v[91].e[1] = new iVe("5310", "5310", "2", "5310", "5310");

v[92] = new cV("Pal Talk",5);
v[92].e[0] = new iVe("2090", "2090", "2", "2090", "2090");
v[92].e[1] = new iVe("2091", "2091", "2", "2091", "2091");
v[92].e[2] = new iVe("2090", "2090", "1", "2090", "2090");
v[92].e[3] = new iVe("2091", "2091", "1", "2091", "2091");
v[92].e[4] = new iVe("2095", "2095", "1", "2095", "2095");

v[93] = new cV("pcAnywhere v7.5",2);
v[93].e[0] = new iVe("5631", "5631", "1", "5631", "5631");
v[93].e[1] = new iVe("5631", "5631", "2", "5631", "5631");

v[94] = new cV("PhoneFree",5);
v[94].e[0] = new iVe("1034", "1035", "1", "1034", "1035");
v[94].e[1] = new iVe("1034", "1035", "2", "1034", "1035");
v[94].e[2] = new iVe("9900", "9901", "2", "9900", "9901");
v[94].e[3] = new iVe("2644", "2644", "1", "2644", "2644");
v[94].e[4] = new iVe("8000", "8000", "1", "8000", "8000");

v[95] = new cV("Polycom ViaVideo H.323",1);
v[95].e[0] = new iVe("3230", "3235", "1", "3230", "3235");

v[96] = new cV("Polycom ViaVideo H.324",1);
v[96].e[0] = new iVe("3230", "3235", "2", "3230", "3235");

v[97] = new cV("PPTP",1);
v[97].e[0] = new iVe("1723", "1723", "1", "1723", "1723");

v[98] = new cV("Quake",2);
v[98].e[0] = new iVe("26000", "26000", "1", "26000", "26000");
v[98].e[1] = new iVe("26000", "26000", "2", "26000", "26000");

v[99] = new cV("Quake II (Client/Server)",1);
v[99].e[0] = new iVe("27910", "27910", "2", "27910", "27910");

v[100] = new cV("Quake III",1);
v[100].e[0] = new iVe("27660", "27680", "2", "27660", "27680");

v[101] = new cV("Red Alert",1);
v[101].e[0] = new iVe("5009", "5009", "2", "5009", "5009");

v[102] = new cV("RealAudio",1);
v[102].e[0] = new iVe("6790", "32000", "2", "6790", "32000");

v[103] = new cV("Real Player 8 Plus",1);
v[103].e[0] = new iVe("7070", "7070", "2", "7070", "7070");

v[104] = new cV("Rise of Rome",2);
v[104].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[104].e[1] = new iVe("47624", "47624", "2", "47624", "47624");

v[105] = new cV("Roger Wilco",2);
v[105].e[0] = new iVe("3782", "3782", "1", "3782", "3782");
v[105].e[1] = new iVe("3782", "3783", "2", "3782", "3783");

v[106] = new cV("Rogue Spear",1);
v[106].e[0] = new iVe("2346", "2346", "1", "2346", "2346");

v[107] = new cV("Secure Shell Server (SSH)",1);
v[107].e[0] = new iVe("22", "22", "1", "22", "22");

v[108] = new cV("Secure Web Server (HTTPS)",1);
v[108].e[0] = new iVe("443", "443", "1", "443", "443");

v[109] = new cV("ShoutCast",1);
v[109].e[0] = new iVe("8000", "8005", "1", "8000", "8005");

v[110] = new cV("SNMP",1);
v[110].e[0] = new iVe("161", "161", "2", "161", "161");

v[111] = new cV("SNMP Trap",1);
v[111].e[0] = new iVe("162", "162", "2", "162", "162");

v[112] = new cV("Speak Freely",1);
v[112].e[0] = new iVe("2074", "2076", "2", "2074", "2076");

v[113] = new cV("StarCraft",1);
v[113].e[0] = new iVe("6112", "6112", "2", "6112", "6112");

v[114] = new cV("Starfleet Command",4);
v[114].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[114].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[114].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[114].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[115] = new cV("StarLancer",4);
v[115].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[115].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[115].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[115].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[116] = new cV("SWAT3",2);
v[116].e[0] = new iVe("16639", "16639", "1", "16639", "16639");
v[116].e[1] = new iVe("16638", "16638", "2", "16638", "16638");

v[117] = new cV("Telnet Server",1);
v[117].e[0] = new iVe("23", "23", "1", "23", "23");

v[118] = new cV("TFTP",1);
v[118].e[0] = new iVe("69", "69", "2", "69", "69");

v[119] = new cV("The 4th Coming",2);
v[119].e[0] = new iVe("11677", "11677", "2", "11677", "11677");
v[119].e[1] = new iVe("11679", "11679", "2", "11679", "11679");

v[120] = new cV("Tiberian Sun: C&C III",1);
v[120].e[0] = new iVe("1234", "1234", "2", "1234", "1234");

v[121] = new cV("Total Annihilation",1);
v[121].e[0] = new iVe("47624", "47624", "2", "47624", "47624");

v[122] = new cV("Ultima",5);
v[122].e[0] = new iVe("5001", "5010", "1", "5001", "5010");
v[122].e[1] = new iVe("7775", "7777", "1", "7775", "7777");
v[122].e[2] = new iVe("8800", "8900", "1", "8800", "8900");
v[122].e[3] = new iVe("9999", "9999", "1", "9999", "9999");
v[122].e[4] = new iVe("7875", "7875", "1", "7875", "7875");

v[123] = new cV("Unreal Tournament",3);
v[123].e[0] = new iVe("7777", "7790", "2", "7777", "7790");
v[123].e[1] = new iVe("27900", "27900", "2", "27900", "27900");
v[123].e[2] = new iVe("8080", "8080", "1", "8080", "8080");

v[124] = new cV("Urban Assault",4);
v[124].e[0] = new iVe("47624", "47624", "1", "47624", "47624");
v[124].e[1] = new iVe("6073", "6073", "1", "6073", "6073");
v[124].e[2] = new iVe("2300", "2400", "1", "2300", "2400");
v[124].e[3] = new iVe("2300", "2400", "2", "2300", "2400");

v[125] = new cV("VoxPhone 3.0",2);
v[125].e[0] = new iVe("12380", "12380", "1", "12380", "12380");
v[125].e[1] = new iVe("12380", "12380", "2", "12380", "12380");

v[126] = new cV("Warbirds 2",1);
v[126].e[0] = new iVe("912", "912", "1", "912", "912");

v[127] = new cV("Web Server (HTTP)",1);
v[127].e[0] = new iVe("80", "80", "1", "80", "80");

v[128] = new cV("WebPhone 3.0",1);
v[128].e[0] = new iVe("21845", "21845", "1", "21845", "21845");

v[129] = new cV("Westwood Online",3);
v[129].e[0] = new iVe("4000", "4000", "2", "4000", "4000");
v[129].e[1] = new iVe("1140", "1234", "1", "1140", "1234");
v[129].e[2] = new iVe("1140", "1234", "2", "1140", "1234");

v[130] = new cV("Windows 2000 Terminal Server",2);
v[130].e[0] = new iVe("3389", "3389", "1", "3389", "3389");
v[130].e[1] = new iVe("3389", "3389", "2", "3389", "3389");

v[131] = new cV("X Windows",2);
v[131].e[0] = new iVe("6000", "6000", "1", "6000", "6000");
v[131].e[1] = new iVe("6000", "6000", "2", "6000", "6000");

v[132] = new cV("Yahoo Pager",1);
v[132].e[0] = new iVe("5050", "5050", "1", "5050", "5050");

v[133] = new cV("Yahoo Messenger Chat",2);
v[133].e[0] = new iVe("5000", "5001", "1", "5000", "5001");
v[133].e[1] = new iVe("5055", "5055", "2", "5055", "5055");



function clearAll()
{
   with (document.vsFrm) {
      for (i = 0; i < TOTAL_ROW; i++) {
         eStart[i].value = eEnd[i].value = iStart[i].value = iEnd[i].value = iSrcIp[i].value = "";
         proto[i].value = 1;
         iEnd[i].disabled = 1;
      }
   }
}

function appSelect(sName) {
   
   clearAll();

   with (document.vsFrm) {
   	var i;
      if (sName == "FIRST_APP") {
         usedName = '';
		 
         return;
      }
    
      for(i = 0; i < 134; i++) {
    	   if(v[i].name == sName) {
			  
            var entryNum = v[i].eNum;
            usedName = sName;
    	    	for (j = 0; j < entryNum; j++) {
    	    	   eStart[j].value = v[i].e[j].eStart;
    	    	   eEnd[j].value = v[i].e[j].eEnd;
    	    	   iStart[j].value = v[i].e[j].iStart;
    	    	   iEnd[j].value = v[i].e[j].iEnd;
    	    	   proto[j].value = v[i].e[j].proto;

                   if(sName == "FTP Server")
                   {
                       eStart[j].disabled = 1;
                       eEnd[j].disabled = 1;
                   }
                   else
                   {
                       eStart[j].disabled = 0;
                       eEnd[j].disabled = 0;
                   }
            }
            break;
         }
      }
   }
}

function btnApply() {
	var eStartLoc = '';
	var protoLoc ='';
	var eEndLoc = '';
	var iStartLoc = '';
	var iEndLoc = '';
	var iSrcIpLoc = '';
   with (document.vsFrm) {
       if (radiosrv[0].checked == true) {
         if (usedName == '') {
            alert(data_language.dynamic.nullname);
            return;
         }
      }
      for (i = 0; i < TOTAL_ROW; i++) {
         if (eStart[i].value == "" && eEnd[i].value == "" && iStart[i].value == "" && iEnd[i].value == "")
            break;
         if (eStart[i].value == "" || eEnd[i].value == "" || iStart[i].value == "" || iEnd[i].value == "") {
            alert(UEcode[1005]);
            return;
         }
         if(parseInt(eStart[i].value) > parseInt(eEnd[i].value)){
            alert(data_language.dynamic.porterror);
            return;
         }
      }
      if (i == 0) {
         alert(UEcode[1005]);
         return;
      }
      if (i > remain) {
	  alert(data_language.dynamic.maximum);
        return;
      }
	  //frank,060727,check if have a blank row between configured rows
	  G_Rules_Count = i ;    
      eCount = i;
      for (i = 0; i < eCount; i++) {
         eStartLoc = eStartLoc + eStart[i].value + ',';
         eEndLoc = eEndLoc + eEnd[i].value + ',';
         protoLoc = protoLoc + proto[i].value + ',';
         iStartLoc = iStartLoc + iStart[i].value + ',';
         iEndLoc = iEndLoc + iEnd[i].value + ',';
         iSrcIpLoc = iSrcIpLoc + iSrcIp[i].value + ',';
      }
      eStartLocText.value = eStartLoc;
      eEndLocText.value = eEndLoc;
      protoLocText.value= protoLoc;
      iStartLocText.value= iStartLoc;
      iEndLocText.value= iEndLoc;
      iSrcIpLocText.value= iSrcIpLoc;
      listLen.value = i;
	  uiSubmit();
   }
}

function radioClick()
{
    with ( document.forms[0] ) {
   		if (document.vsFrm.radiosrv[0].checked == true) {
			document.vsFrm.cusSrvName.value = '';
			appSelect(document.vsFrm.srvName.value);

			changeBlockState('id_cusSrvName',DISABLED);
			changeBlockState('id_autoSrvName',ENABLED);

   		} else{
			appSelect(FIRST_APP);
			changeBlockState('id_cusSrvName',ENABLED);
			changeBlockState('id_autoSrvName',DISABLED);
		}
	}

}

function setiPt(flgEnd, i)
{
   if (flgEnd) 
      document.vsFrm.iEnd[i].value = parseInt(document.vsFrm.iStart[i].value) +  parseInt(document.vsFrm.eEnd[i].value) -parseInt(document.vsFrm.eStart[i].value);
   else
   {
      document.vsFrm.iStart[i].value = document.vsFrm.eStart[i].value;
      document.vsFrm.iEnd[i].value = parseInt(document.vsFrm.iStart[i].value) +  parseInt(document.vsFrm.eEnd[i].value) -parseInt(document.vsFrm.eStart[i].value);
   }
   if(document.vsFrm.iEnd[i].value == 'NaN')
   {
      document.vsFrm.iEnd[i].value = "";
   }
}

function setiPtEdit(flgEnd)
{
   if (flgEnd)
      document.vsFrm.iEndEdit.value = parseInt(document.vsFrm.iStartEdit.value) + parseInt(document.vsFrm.eEndEdit.value) - parseInt(document.vsFrm.eStartEdit.value);
   else
   {
      document.vsFrm.iStartEdit.value = document.vsFrm.eStartEdit.value;
      document.vsFrm.iEndEdit.value = parseInt(document.vsFrm.iStartEdit.value) + parseInt(document.vsFrm.eEndEdit.value) - parseInt(document.vsFrm.eStartEdit.value);
   }
   if(document.vsFrm.iEndEdit.value == 'NaN')
   {
      document.vsFrm.iEndEdit.value = "";
   }
}

function kPressed()
{
   keyPressed = true;
}

function setiEnd(i)
{
   if (keyPressed) {
      //document.vsFrm.iEnd[i].value = document.vsFrm.eEnd[i].value;
      document.vsFrm.iEnd[i].value = parseInt(document.vsFrm.iStart[i].value) +  parseInt(document.vsFrm.eEnd[i].value) -parseInt(document.vsFrm.eStart[i].value);
      if(document.vsFrm.iEnd[i].value == 'NaN'){
         document.vsFrm.iEnd[i].value = "";
      }  
      keyPressed = false;
   }
}

function setiEndEdit(i)
{
   if (keyPressed) {
      //document.vsFrm.iEndEdit.value = document.vsFrm.eEndEdit.value;
      document.vsFrm.iEndEdit.value = parseInt(document.vsFrm.iStartEdit.value) + parseInt(document.vsFrm.eEndEdit.value) - parseInt(document.vsFrm.eStartEdit.value);
      if(document.vsFrm.iEnd[i].value == 'NaN'){
         document.vsFrm.iEnd[i].value = "";
      } 
      keyPressed = false;
   }
}

function onclkS_Server(){	
	var bool = Form.Radio('radiosrv') == '0' ? false : true;	
	
	if(bool){
		disCtrl('c_server', !bool);
		disCtrl('s_service', bool);
	} else {
		appSelect(document.vsFrm.srvName.value);
		disCtrl('s_service', !bool);
		disCtrl('c_server', bool);
	}
}

function onclkC_Server(){	
	var bool = Form.Radio('radiosrv') == '0' ? false : true;	
	
	if(bool){
		appSelect(FIRST_APP);
		disCtrl('s_service', !bool);
		disCtrl('c_server', bool);
	} else {
		disCtrl('c_server', !bool);
		disCtrl('s_service', bool);
	}
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);
