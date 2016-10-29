// DSL Set
<?mget :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig. "StandardsSupported Enable"
`	var G_StandardsSupported   = "$01";
	var G_EnableDSL            = "$02";
`?>

function Changeadslmode(value){
	var checkbox_node = document.getElementsByTagName('INPUT');

	if(value=='1'){
		for(var i = 1; i < checkbox_node.length; i++)
		{
			if(checkbox_node[i].type == 'checkbox' && (checkbox_node[i].name!="bitswap" && checkbox_node[i].name!="sra"&&checkbox_node[i].name!="bit1"))
		    {
			   checkbox_node[i].checked=true;
			   checkbox_node[i].disabled=true;
		    }
		}
	}else{
		for(var i = 1; i < checkbox_node.length; i++)
		{
			if(checkbox_node[i].type == 'checkbox' && (checkbox_node[i].name!="bitswap" && checkbox_node[i].name!="sra"&&checkbox_node[i].name!="bit1"))
		    {
			   checkbox_node[i].checked=false;
			   checkbox_node[i].disabled=false;
		    }
		}
		var StandardSupport = G_StandardsSupported.split(',');
		if(StandardSupport[0]=="all")
		{
           
		   for(var i = 1; i < checkbox_node.length; i++)
		   {
			if(checkbox_node[i].type == 'checkbox' && (checkbox_node[i].name!="bitswap" && checkbox_node[i].name!="sra"&&checkbox_node[i].name!="bit1"))
		    {
			   checkbox_node[i].checked=true;
		    }
		   }
		}
	    for(var i = 1,temp_node; i < StandardSupport.length; i++){
		    temp_node = $(StandardSupport[i]);
			if((StandardSupport[i] == "G.992.3_Annex_M") || (StandardSupport[i] == "G.992.5_Annex_M"))
			{
				$('G.992.3_Annex_M,G.992.5_Annex_M').checked = true;
			}
		    // 判断节点是否存在
		    if(temp_node)
			{ 
			      temp_node.checked = true;
		    }		
	    }
	}	
}

function uiOnload(){
       
       $('EnableDSL').checked = G_EnableDSL == '1' ?  true : false ; 
	   
	//支持的模式列表
	
	var StandardSupport = G_StandardsSupported.split(',');
	for(var i = 1,temp_node; i < StandardSupport.length; i++){
		temp_node = $(StandardSupport[i]);
		// 判断节点是否存在
		if(temp_node){ 
			temp_node.checked = true;
		}
	}
    if(StandardSupport[0]=="all")
	{
	
		$('all').checked=true ;
		$('multimode').checked=false ;
		Changeadslmode(1);
	}
	else
	{
		if(StandardSupport[0]=="multimode")
		{
			$('all').checked=false ;
		   $('multimode').checked=true ;
		   Changeadslmode(0);
		}
	}
	//电源管理模式
	//$('SELECT_PowerManagementState').value = G_PowerManagementState;
	//提示用户重启设备
	//if('<?echo $var:tipTag?>' == '1'){
	//	alert(SEcode[1006]);
	//}
	dealWithError();
}

function uiSubmit()
{
	var checkbox_node = document.getElementsByTagName('INPUT');
	var support_string = '';
    if($('all').checked == true)
	{
		support_string += "all" + ',';
		for(var i = 10; i < checkbox_node.length; i++)
	    {
		    if(checkbox_node[i].type == 'checkbox' && checkbox_node[i].checked)
		    {
			    support_string += checkbox_node[i].id + ',';
		    }
	    }
	}
	else
	{
		support_string += "multimode" + ',';
	    for(var i = 1; i < checkbox_node.length; i++)
	    {
		    if(checkbox_node[i].type == 'checkbox' && checkbox_node[i].checked)
		    {
			    support_string += checkbox_node[i].id + ',';
		    }
	    }
	}
	//alert($('EnableDSL').checked);
	$H({
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Enable'  : $('EnableDSL').checked ? '1':'0',
		':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.StandardsSupported'  : support_string.delcomma(),
		//':InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.PowerManagementState': $('SELECT_PowerManagementState').value,
		//'var:tipTag'   : support_string.delcomma() == (G_StandardsSupported || '') ? undefined : 1,//当勾上大于1个的选项时,就提示用户重启设备
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action'   : 'set',
		'getpage'      : 'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);