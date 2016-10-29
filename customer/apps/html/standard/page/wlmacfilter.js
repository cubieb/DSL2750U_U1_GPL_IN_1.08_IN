//ACL list
var G_MACAddressControlEnabled="<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.MACAddressControlEnabled?>";

var G_AllowedMACAddresses="<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses?>";

var NewMACAddressesList=[];
var DelIdx=0;

function uiOnload()
{

	if(1 == G_MACAddressControlEnabled||G_MACAddressControlEnabled=='true')
		$('ACLEnabled').checked = true;
	else
		$('ACLEnabled').checked = false;
	
    var MACAddressesList=G_AllowedMACAddresses.split(',');
	var array_value=[];
    var j=0;
	for(var i=0; i<MACAddressesList.length; i++)
	{
		if(MACAddressesList[i].length == 17){   //  strlen("00:00:00:00:00:00")==17;
		    array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="Enable_'+j+'" name="rule_index'+j+'" onclick="$id(' + j + ')">'); 
			array_value[i].push(MACAddressesList[i]);
			NewMACAddressesList[j]=MACAddressesList[i];
			j++;
		}
	}

	$T('td_ACLlist',array_value);
	
  
}
function uiAdd()
{
    //var rows = $('td_ACLlist').rows.length ;
	//alert(rows);
	//alert(NewMACAddressesList.length);
	//alert(MACAddressesList.length);
	$('ACLAddMAC').style.display = 'block';
	$('Add').disabled = true;
	$('Delete').disabled = true;
	$('SrcMAC').value = "";
	Heigtduiqi();
}

function uiCancel()
{
	$('ACLAddMAC').style.display = 'none';
	$('Add').disabled = false;
	$('Delete').disabled = false;
    Heigtduiqi();
}

function CreateNewMACList(opt)
{
    var MACList='';
    switch(opt){
		case 'Add':
		{
			for(var i=0; i<NewMACAddressesList.length; i++)
			{
			    if(NewMACAddressesList[i].length==17)
				{
				    if(i==0)
					{
						MACList=NewMACAddressesList[i];
					}
					else
					{	
						MACList=MACList+','+NewMACAddressesList[i];
					}
				}
			}
			return MACList;
			break;
		}
		case 'Del':
		{
			for(var i=0; i<NewMACAddressesList.length; i++)
			{
			    if(NewMACAddressesList[i].length==17)
				{
				    if(DelIdx==i)
						continue;
				    if(i==0)
					{
						MACList=NewMACAddressesList[i];
					}
					else
					{	
						MACList=MACList+','+NewMACAddressesList[i];
					}
				}
			}
			return MACList;
			break;
		}
			
			
	}
}

function uiSubmit()
{
		var newAllowedMACAddresses;
		if(NewMACAddressesList.length==0)
		{
			newAllowedMACAddresses=$('SrcMAC').value;
		}
		else
		{
			newAllowedMACAddresses=CreateNewMACList('Add')+','+$('SrcMAC').value;	
		}
		
		$H({
			':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses' : newAllowedMACAddresses,
			'obj-action'       : 'set',		
    		'getpage'           :'html/index.html',
       		'errorpage'         :'html/index.html',
	   		'var:menu'          :'<?echo $var:menu?>',
	   		'var:page'          :'<?echo $var:page?>',
	   		'var:subpage'     : 'wlmacfilter',
	   		'var:errorpage'    :'wlmacfilter',
			'var:CacheLastData': ViewState.Save()
		});
		
	$('uiPostForm').submit();
}

function uiDelete()
{
    var index = -1;
	var delNum=0;

    if(_array_id.length > 1)
    {
	 alert(SEcode[1009]);
	 return false;
    }
    if(0 == _array_id.length)
    {
	 alert(SEcode[9888]);
	 return false;
    }
	
    for(var i = 0; i < NewMACAddressesList.length; i++)
    {
       var node = "Enable_"+i;
       if(Form.Checkbox(node) ==1)
	   {
	     DelIdx = i;
	     break;
	   }		
    }
	
    if(!confirm(SEcode[1008]))
    {
        return false;
    }

    if(1 == Form.Checkbox('ACLEnabled'))
    {
        if(1 == G_AllowedMACAddresses.length)
        {
            alert("You can not delete the last IP when you enable Access Control Mode!");
            return false;
        }
    }
	var newAllowedMACAddresses=CreateNewMACList('Del');
	//alert(newAllowedMACAddresses);
    $H({
			':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses' : newAllowedMACAddresses,
			'obj-action'       : 'set',		
    		'getpage'           :'html/index.html',
       		'errorpage'         :'html/index.html',
	   		'var:menu'          :'<?echo $var:menu?>',
	   		'var:page'          :'<?echo $var:page?>',
	   		'var:subpage'     : 'wlmacfilter',
	   		'var:errorpage'    :'wlmacfilter',
			'var:CacheLastData': ViewState.Save()
	});
	
    $('uiPostForm').submit();

}
function uiEnabled()
{		
	$H({
	   ':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.MACAddressControlEnabled'  :Form.Checkbox('ACLEnabled') ,
	   ':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.MACAddressControlEnabled'  :Form.Checkbox('ACLEnabled') ,
	   ':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.MACAddressControlEnabled'  :Form.Checkbox('ACLEnabled') ,
	   ':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.MACAddressControlEnabled'  :Form.Checkbox('ACLEnabled') ,
	   'obj-action'       :'set',
	   'getpage'           :'html/index.html',
       'errorpage'         :'html/index.html',
	   'var:menu'          :'<?echo $var:menu?>',
	   'var:page'          :'<?echo $var:page?>',
	   'var:subpage'     : 'wlmacfilter',
	   'var:errorpage'    :'wlmacfilter'
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
