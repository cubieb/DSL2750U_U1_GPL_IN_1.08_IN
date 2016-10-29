//ACL list
var G_ACLEnable   = "<?get :InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACLEnable?>";


var G_ACLList = new Array();
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACL. "SrcIP"
	`	G_ACLList[m] = new Array();
		G_ACLList[m][0] = "$01"; //SrcIP
		G_ACLList[m][1] = "$00"; //index
		m++;
	`?>
		
function uiOnload()
{

	if(1 == G_ACLEnable)
		$('ACLEnabled').checked = true;
	else
		$('ACLEnabled').checked = false;


	var array_value = [];
	for(var i = 0; i < G_ACLList.length; i++)
	{
	      array_value[i] = [];
		array_value[i].push('<input type="checkbox" id="Enable_'+i+'">');
		array_value[i].push(G_ACLList[i][0]);
	}
	
	$T('td_ACLlist',array_value);

}
function uiAdd()
{
	$('ACLAddIP').style.display = 'block';
	$('Add').disabled = true;
	$('Delete').disabled = true;
	$('SrcIP').value = "";
	Heigtduiqi();
}

function uiCancel()
{
	$('ACLAddIP').style.display = 'none';
	$('Add').disabled = false;
	$('Delete').disabled = false;
    Heigtduiqi();
}

function uiSubmit()
{
		$H({
			'add-obj'          : 'InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACL.',
			':SrcIP'            : $('SrcIP').value,
			'obj-action'       : 'add-set',		
			'getpage'          :'html/index.html',
			'errorpage'        :'html/index.html',
			'var:menu'         :'maintenance',
			'var:page'         :'ipaddrs',
			'var:errorpage'    :'ipaddrs',
			'var:CacheLastData': ViewState.Save()
		});
		
	$('uiPostForm').submit();
}

function uiDelete()
{
    var index = -1;
    for(var i = 0; i < G_ACLList.length; i++)
    {
        var node = "Enable_"+i;
        if(Form.Checkbox(node) ==1)
	  {
	    index = i;
	    break;
	  }		
    }

    if(-1 == index)
    {
        alert(SEcode[1009]);
        return false;
    }
	
    if(!confirm(SEcode[1008]))
    {
        return false;
    }

    if(1 == Form.Checkbox('ACLEnabled'))
    {
        if(1 == G_ACLList.length)
        {
            alert("You can not delete the last IP when you enable Access Control Mode!");
            return false;
        }
    }
	
    $H({
    'del-obj'            :'InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACL.' + G_ACLList[index][1]+'.',
    'obj-action'       :'del',
    'getpage'          :'html/index.html',
    'errorpage'        :'html/index.html',
    'var:menu'        :'maintenance',
    'var:page'        :'accessctrl',
    'var:subpage'    : 'ipaddrs',
    'var:errorpage'  :'ipaddrs',
    'var:CacheLastData': ViewState.Save()
    });
	
    $('uiPostForm').submit();

}
function uiEnabled()
{		
	$H({
	   ':InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACLEnable'  :Form.Checkbox('ACLEnabled') ,
	   'obj-action'       :'set',
	   'getpage'           :'html/index.html',
         'errorpage'         :'html/index.html',
	   'var:menu'         :'maintenance',
	   'var:page'          :'accessctrl',
	   'var:subpage'     : 'ipaddrs',
	   'var:errorpage'    :'ipaddrs'
	});
	
	$('uiPostForm').submit();

}

function dealWithError()
{
         if (G_Error != 1)
		 	{ return false; }

         var arrayHint = [];

         dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);
