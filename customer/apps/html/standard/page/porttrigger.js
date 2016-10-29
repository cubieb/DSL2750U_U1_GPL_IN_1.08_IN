var selectedIdx = "";
var action = "";

var G_PortTriggerEnable;
var G_MaxNumberOfEntries;
var G_NumberOfEntries;
var G_Edit_Flag = 0;
var G_Del_Flag = 0;
<?mget :InternetGatewayDevice.X_TWSZ-COM_PortTrigger. "Enable MaxNumberOfEntries NumberOfEntries"
`   G_PortTriggerEnable = "$01";
    G_MaxNumberOfEntries = "$02";
    G_NumberOfEntries = "$03";
`?>

//Port Trigger rule list
var G_PortTrigger = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_PortTrigger.TriggerList. "Name Enable TriggerProtocol TriggerStartPort TriggerEndPort OpenProtocol OpenStartPort OpenEndPort"
`
    G_PortTrigger[m] = [];
    G_PortTrigger[m][0] = "InternetGatewayDevice.X_TWSZ-COM_PortTrigger.TriggerList.$00.";
    G_PortTrigger[m][1] = "$01";    //Name
    G_PortTrigger[m][2] = "$02";    //Enable
    G_PortTrigger[m][3] = "$03";    //TriggerProtocol
    G_PortTrigger[m][4] = "$04";    //TriggerStartPort
    G_PortTrigger[m][5] = "$05";    //TriggerEndPort
    G_PortTrigger[m][6] = "$06";    //OpenProtocol
    G_PortTrigger[m][7] = "$07";    //OpenStartPort
    G_PortTrigger[m][8] = "$08";    //OpenEndPort
    m++;
`?>

function uiOnload()
{
    Form.Checkbox("INPUT_PortTriggerEnable", G_PortTriggerEnable);
    
    var table_array = [];
    for (var i = 0; i < G_PortTrigger.length; i++)
    {
        table_array[i] = ['<input type="checkbox" id="rule_' + i + '" onClick="selectRule(' + i + ')">',
                          G_PortTrigger[i][1],
                          G_PortTrigger[i][3],
                          G_PortTrigger[i][4] + ':' + G_PortTrigger[i][5],
                          G_PortTrigger[i][6],
                          G_PortTrigger[i][7] + ':' + G_PortTrigger[i][8],
                          (G_PortTrigger[i][2] == "0")?"Off":"On"
                         ];
    }
    $T('PortTriggerList', table_array);
    
    $("remain_num").innerHTML = (G_MaxNumberOfEntries - G_NumberOfEntries);
}

function selectRule(index)
{
    if (selectedIdx == "")
    {
        selectedIdx = index;
		G_Edit_Flag = 1;
		G_Del_Flag = 1;
        return true;
    }
    else
    {
        alert("Only one rule to be selected at one time.");
        return false;
    }
}

function uiSubmit()
{
    $H({
        ':InternetGatewayDevice.X_TWSZ-COM_PortTrigger.Enable' : ($("INPUT_PortTriggerEnable").checked == true)?"1":"0",
        'obj-action' : 'set',
        'var:menu' : 'advanced',
        'var:page' : 'porttrigger',
        'getpage' : 'html/index.html',
        'errorpage' : 'html/index.html',
        'var:errorpage' : 'porttrigger',
        'var:CacheLastData' : ViewState.Save()
    });
    $('uiPostForm').submit();
}

function uiAdd()
{
    action = "add";
    $("port_setup").style.display = "block";
    $("remainPart").style.display = "block";
    $("lang_add").disabled = true;
    $("lang_edit").disabled = true;
    $("lang_delete").disabled = true;
}

function uiEdit()
{
	if(G_Edit_Flag !=0)
	{
    	action = "edit";
    	$("port_setup").style.display = "block";
    	$("lang_add").disabled = true;
    	$("lang_edit").disabled = true;
    	$("lang_delete").disabled = true;

    	$("INPUT_ServiceName").value = G_PortTrigger[selectedIdx][1];
    	$("ruleStatus").value = G_PortTrigger[selectedIdx][2];
    	$("triProto").value = G_PortTrigger[selectedIdx][3];
    	$("triStart").value = G_PortTrigger[selectedIdx][4];
    	$("triEnd").value = G_PortTrigger[selectedIdx][5];
    	$("opProto").value = G_PortTrigger[selectedIdx][6];
    	$("opStart").value = G_PortTrigger[selectedIdx][7];
    	$("opEnd").value = G_PortTrigger[selectedIdx][8];
    	$("portTriggerNodePath").value = G_PortTrigger[selectedIdx][0];
	}
	
	G_Edit_Flag = 0;
}

function uiDelete()
{
	if(G_Del_Flag != 0)
	{
    	action = "delete";
    	$H({
        	'del-obj' : G_PortTrigger[selectedIdx][0],
        	'obj-action' : 'del',
        	'var:menu' : 'advanced',
        	'var:page' : 'porttrigger',
        	'getpage' : 'html/index.html',
        	'errorpage' : 'html/index.html',
        	'var:errorpage' : 'porttrigger',
        	'var:CacheLastData' : ViewState.Save()
    	});
    	$('uiPostForm').submit();
	}else{
		alert("please select an item to delete!");
	}

	G_Del_Flag = 0;
}

function btnApply()
{   
     var entryLeft = G_MaxNumberOfEntries - G_NumberOfEntries;
    //The value we got from path is string,so change it to integer.(Added on 2010.1.19)
    if ((parseInt($("triStart").value) > parseInt($("triEnd").value))
        || (parseInt($("opStart").value) > parseInt($("opEnd").value)))
    {
        alert("Port range is not correct!");
        return false;
    }
    if ($("INPUT_ServiceName").value == "")
    {
        alert("Service name should not be empty!");
        return false;
    }

    for (var i = 0; i < G_PortTrigger.length; i++)
    {	
        if(($("triProto").value==G_PortTrigger[i][3])&&($("triStart").value==G_PortTrigger[i][4])&&($("triEnd").value==G_PortTrigger[i][5])&&($("opProto").value==G_PortTrigger[i][6])&&($("opStart").value==G_PortTrigger[i][7])&&($("opEnd").value==G_PortTrigger[i][8])&&($("ruleStatus").value==G_PortTrigger[i][2])&&($("INPUT_ServiceName").value == G_PortTrigger[i][1])){
	 			alert("entered the same rule");
	 			return false;
        }
    }
    if (action == "edit")
    {
        var path = ':' + $("portTriggerNodePath").value;
        $F(path + 'Name', $("INPUT_ServiceName").value);
        $F(path + 'Enable', $("ruleStatus").value);
        $F(path + 'TriggerProtocol', $("triProto").value);
        $F(path + 'TriggerStartPort', $("triStart").value);
        $F(path + 'TriggerEndPort', $("triEnd").value);
        $F(path + 'OpenProtocol', $("opProto").value);
        $F(path + 'OpenStartPort', $("opStart").value);
        $F(path + 'OpenEndPort', $("opEnd").value);
        $H({
            'obj-action' : 'set',
            'var:menu' : 'advanced',
            'var:page' : 'porttrigger',
            'getpage' : 'html/index.html',
            'errorpage' : 'html/index.html',
            'var:errorpage' : 'porttrigger',
            'var:CacheLastData' : ViewState.Save()
        });
    }
    else if (action == "add")
    {

	  if(entryLeft == 0){
	      alert("reach the max number of entries!");
	      return false;
	  }
        $H({
            'add-obj' : 'InternetGatewayDevice.X_TWSZ-COM_PortTrigger.TriggerList.',
            ':Name' : $("INPUT_ServiceName").value,
            ':Enable' : $("ruleStatus").value,
            ':TriggerProtocol' : $("triProto").value,
            ':TriggerStartPort' : $("triStart").value,
            ':TriggerEndPort' : $("triEnd").value,
            ':OpenProtocol' : $("opProto").value,
            ':OpenStartPort' : $("opStart").value,
            ':OpenEndPort' : $("opEnd").value,
            'obj-action' : 'add-set',
            'var:menu' : 'advanced',
            'var:page' : 'porttrigger',
            'getpage' : 'html/index.html',
            'errorpage' : 'html/index.html',
            'var:errorpage' : 'porttrigger',
            'var:CacheLastData' : ViewState.Save()
        });
    }
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
