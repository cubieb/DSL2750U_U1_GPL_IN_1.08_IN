var G_InternetWanType = "<?get :InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANAccessType?>";

var G_queue = new Array();
var n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Queue. "QueuePrecedence QueueInterface Enable QueueWeight"
`	G_queue[n] = new Array();
	G_queue[n][0] = "$00."; 
	G_queue[n][1] = "$01";
	G_queue[n][2] = "$02";
	G_queue[n][3] = "$03";
	G_queue[n][4] = "$04";
	++n;
`?>

var G_interface = new Array()
n = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_QoS.Interface. "Attribute UpStreamShapingRate DownStreamShapingRate Enable QueueSchedulerAlgorithm"
`	G_interface[n] = new Array();
	G_interface[n][0] = "$00"; 
	G_interface[n][1] = "$01";
	G_interface[n][2] = "$02";
	G_interface[n][3] = "$03";
	G_interface[n][4] = "$04";
	G_interface[n][5] = "$05";
	++n;
`?>


function uiAdd()
{
	$('queue_config').style.display = 'block';
	interface_change();
	
}


function priority_change()
{

}

function interface_change()
{
	if (get_interface_mode($('select_q_interface').value) == 'SP') {
		$('select_q_priority').disabled = false;
		$('ipnut_c_wrr_weight').disabled = true;
	}
	else {
		$('select_q_priority').disabled = true;
		$('ipnut_c_wrr_weight').disabled = false;
	}
}

var edit_index = -1;
function uiEdit()
{
	for (var i = 0; i < G_queue.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) {
			edit_index = i;
			Form.Checkbox('input_q_enable',G_queue[i][3]);
			Form.Select('select_q_priority',G_queue[i][1]);
			Form.Select('select_q_interface',G_queue[i][2]); 
			$('input_q_name').value = create_name(i);
			$('queue_config').style.display = 'block';
			$('ipnut_c_wrr_weight').value = G_queue[i][4];
			if (queue_mode(i) == 'SP')
				$('ipnut_c_wrr_weight').disabled = true;
			else
				$('select_q_priority').disabled = true;
			return;
		}
		
	}
	if (i == G_queue.length) {
		if(!confirm(SEcode[1009]))
		 return false;
	}
}


 function uiDelete()
{
	if(!confirm(SEcode[1008])){
		 return false;
	}
	for (var i = 0; i < G_queue.length; i++) {
			var tmpid = "choose" + i;
			if (Form.Checkbox(tmpid) == 1) {
					var path = 'InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + G_queue[i][0];
					$H({
					'del-obj'    : path ,
					'obj-action' : 'del',
					'var:menu'     : G_Menu,
					'var:page'     : G_Page,
					'var:subpage'  : G_SubPage,
					'var:errorpage': G_SubPage,
					'getpage'   :'html/index.html',
					'errorpage' :'html/index.html',
					'var:CacheLastData': ViewState.Save()
			});
			$('uiPostForm').submit();
			return;
		 }
	}
	alert(SEcode[1009]);
}
 
function get_interface_mode(path)
{
	for (var i = 0; i < G_interface.length; i++) {
		if (G_interface[i][0] == path) {
			break;
		}
	}
	if (i == G_interface.length) {
		return "SP";
	}
	else
		return G_interface[i][5];
}


function uiSubmit()
{

		if(edit_index == -1) {
		if (get_interface_mode($('select_q_interface').value) == 'SP') {
			$H({
			 'add-obj' : 'InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.',
			 ':Enable'     : Form.Checkbox('input_q_enable'),
			 ':QueuePrecedence' : $('select_q_priority').value,
			 ':QueueInterface'  :  $('select_q_interface').value,
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:subpage'  : G_SubPage,
			'var:errorpage': G_SubPage,
			'obj-action':'add-set',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'var:CacheLastData': ViewState.Save()
			},true);
			}
		else {
			$H({
			 'add-obj' : 'InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.',
			 ':Enable'     : Form.Checkbox('input_q_enable'),
			 ':QueueInterface'  :  $('select_q_interface').value,
			 ':QueueWeight'     :   $('ipnut_c_wrr_weight').value,
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:subpage'  : G_SubPage,
			'var:errorpage': G_SubPage,
			'obj-action':'add-set',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'var:CacheLastData': ViewState.Save()
			},true);
			}
				
		}
		else {
			$H({
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:subpage'  : G_SubPage,
			'var:errorpage': G_SubPage,
			'obj-action':'set',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'var:CacheLastData': ViewState.Save()
			},true);
			var path = ':InternetGatewayDevice.X_TWSZ-COM_QoS.Queue.' + G_queue[edit_index][0];
		
			$F(path + 'Enable',Form.Checkbox('input_q_enable'));
			if (get_interface_mode($('select_q_interface').value) == 'SP') 
				$F(path + 'QueuePrecedence',$('select_q_priority').value);
			else
				$F(path + 'QueueWeight',$('ipnut_c_wrr_weight').value);
			$F(path + 'QueueInterface',$('select_q_interface').value);
			
		}
		$('uiPostForm').submit();

}

function transform_precedence(num)
{
	if (num == 1)
		return "High";
	else if (num == 2)
		return "Medium";
	else if (num == 3)
		return "Low";
	else
		return "Unknown";
}


function create_name(para)
{
 	interface = G_queue[para][2];
	for (var i = 0; i < G_interface.length; i++) {
		if (G_interface[i][0] == interface) {
			break;
		}
	}
	if (i == G_interface.length) {
		return "NONE";
	}
	if (G_InternetWanType == "DSL") {
		if (queue_mode(i) == "SP") 
			return G_interface[i][1].replace(/(\D+)(\d+)\/(\d+)/, "Que_$2_$3_") + transform_precedence(G_queue[para][1]);
		else
			return G_interface[i][1].replace(/(\D+)(\d+)\/(\d+)/, "Que_$2_$3_") + "wrr_" + G_queue[para][4];
	}
	else {
		if (queue_mode(i) == "SP") 
			return "Que_" + 'Eth_' + transform_precedence(G_queue[para][1]);
		else
			return "Que_" + 'Eth_' + + "wrr_" + G_queue[para][4];
	}
}


function load_interface()
{
	var _text = [], _value = [];
	for(var i = 0; i < G_interface.length; i++){
		_text.push(G_interface[i][1]);
		_value.push(G_interface[i][0]);
	}
	$S('select_q_interface',_text,_value);
}

function queue_mode(para)
{
	interface = G_queue[para][2];
	for (var i = 0; i < G_interface.length; i++) {
		if (G_interface[i][0] == interface) {
			break;
		}
	}
	//when we can not get the interface mode from queue, we get interface mode from selected, this may happen when we delete a pvc, the queue is reset, when add, the queue is not assiciation to any queue. 
	if (i == G_interface.length)
		get_interface_mode($('select_q_interface').value);
	else
		return G_interface[i][5];
}


function create_table_dsl()
{
		
		Table.Clear('queue_table');
		var array_value = [];
		for(var i = 0; i < G_queue.length; i++){
			var name = create_name(i);
			array_value[i] = [];
			array_value[i].push('<input type="checkbox" id="choose'+i+'" name="choose'+i+'">'); 
			array_value[i].push(name);
			if (queue_mode(i) == "SP")
				array_value[i].push(transform_precedence(G_queue[i][1]));
			else
				array_value[i].push(G_queue[i][4]);			
			array_value[i].push(G_queue[i][3]);
		}
		$T('queue_table',array_value);
		load_interface();
}

function load_dsl()
{
	create_table_dsl();
}




function uiOnload()
{
	load_dsl();
}
function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = []; 
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);