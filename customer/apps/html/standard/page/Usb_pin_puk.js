//SAMBA
<?mget :InternetGatewayDevice.X_TWSZ-COM_MODEM. "state enable EnterPinCode EnterPukCode PinChgEnable RemainPinTimes RemainPukTimes OldPinCode NewPinCode PinEnable PinActSta"
`	var G_USB3GEnable 		= "$02";
	var G_State				= "$01";
	var G_EnterPinCode 		= "$03";
	var G_EnterPukCode 		= "$04";
	var G_PinChgEnable 		= "$05";
	var G_RemainPinTimes	= "$06";
	var G_RemainPukTimes	= "$07";
	var G_OldPinCode		= "$08";
	var G_NewPinCode		= "$09";
	var G_PinEnable			= "$0A";
	var G_PinActSta			= "$0B";
`?>

var Pin_no_act = "0";
var Pin_is_act = "1";
var Pin_unknow = '0';
var Pin_ready = "1";
var Pin_need_pin = "2";
var Pin_need_puk = "3";

function setEnable(){
	if(!$('INPUT_3GEnable').checked)
	{
		$('Static').style.display = "none";
		//$('PIN_PUK').disabled = true;INPUT_PUK
		//alert("static disable");
	}
	else
	{
		$('Static').style.display = "block";
	}
}

function ctrl3GEnable(){
	if(!$('INPUT_3GEnable').checked)
	{
		$('Static').style.display = 'none';
		//$('PIN_PUK').disabled = true;INPUT_PUK
		//alert("static disable");
	}
	else
	{
		$('Static').style.display = "block";
		PinPukDisplay();
	}
}
/*
function ctrlChgPinDisplay(){
	//var _node = ${'chg_pin', 'Change_pin'};
	if($('CHG_PIN').checked){
		$('Old_pin').disabled = false;
		$('New_pin').disabled = false;
		$('Confirm_pin').disabled = false;
		$('Chg_Pin_Times').innerHTML = G_RemainPinTimes;
	}
	else {
		$('Old_pin').disabled = true;
		$('New_pin').disabled = true;
		$('Confirm_pin').disabled = true;		
	}
}
*/
function select_action(){
       var val=Form.Radio("Action");

	   $('PUK_ENTER').style.display="none";
	   $('PIN_ENTER').style.display="none";
	   $('Change_pin').style.display="none";
		
	   if(val=="En_pin"){
	      $('PUK_ENTER').style.display="none";
	      $('PIN_ENTER').style.display="block";
	      $('Change_pin').style.display="none";
	      }
	   if(val=="Dis_pin"){
	      $('PUK_ENTER').style.display="none";
	      $('PIN_ENTER').style.display="block";
	      $('Change_pin').style.display="none";
	      }
	   if(val=="Un_pin"){
	      $('PUK_ENTER').style.display="none";
	      $('PIN_ENTER').style.display="block";
	      $('Change_pin').style.display="none";
	      }
	   if(val=="Un_puk"){
	      $('PUK_ENTER').style.display="block";
	      $('PIN_ENTER').style.display="block";
	      $('Change_pin').style.display="none";
	      }
	  if(val=="Chg_pin"){
	      $('PUK_ENTER').style.display="none";
	      $('PIN_ENTER').style.display="none";
	      $('Change_pin').style.display="block";
		  $('Chg_Pin_Times').innerHTML = G_RemainPinTimes;
	      }
}

function PinPukDisplay(){

	$('Static').style.display = 'block';
	if(G_State == Pin_need_pin)
	{
		Form.Radio("Action", "Un_pin");
		$('PIN_UNLOCK').style.display="block";
		$('PIN_ENABLE').style.display="none";
		$('PIN_DISABLE').style.display="none";
		$('PUK_UNLOCK').style.display="none";
		$('PIN_CHG').style.display="none";

		$('3G_Status').innerHTML = data_language.dynamic.status5; 
		$('Pin_Times').innerHTML = G_RemainPinTimes;
		
	}
	else if(G_State == Pin_need_puk)
	{
		Form.Radio("Action", "Un_puk");
		$('PIN_UNLOCK').style.display="none";
		$('PIN_ENABLE').style.display="none";
		$('PIN_DISABLE').style.display="none";
		$('PUK_UNLOCK').style.display="block";
		$('PIN_CHG').style.display="none";
		
		$('3G_Status').innerHTML = data_language.dynamic.status4; 
		$('Puk_Times').innerHTML = G_RemainPukTimes;
	}
	else if(G_State == Pin_ready)
	{
		$('Pin_Times').innerHTML = G_RemainPinTimes;
		$('PIN_UNLOCK').style.display="none";
		//$('PIN_ENABLE').style.display="none";
		//$('PIN_DISABLE').style.display="block";
		$('PUK_UNLOCK').style.display="none";		
		
		if(G_PinActSta == Pin_no_act)
		{
			Form.Radio("Action", "En_pin");
			$('PIN_ENABLE').style.display="block";
			$('PIN_DISABLE').style.display="none";
			$('PIN_CHG').style.display="none";
			$('3G_Status').innerHTML =data_language.dynamic.status3; 
		}
		else if(G_PinActSta == Pin_is_act)
		{
			Form.Radio("Action", "Dis_pin");
			$('PIN_ENABLE').style.display="none";
			$('PIN_DISABLE').style.display="block";
			$('PIN_CHG').style.display="block";
			$('3G_Status').innerHTML =data_language.dynamic.status2; 
		}
	}
	else if(G_State == Pin_unknow)
	{
		
		$('3G_Status').innerHTML = data_language.dynamic.status1;
		$('Static').style.display = 'none';
	}
/*
	if(G_State == Pin_need_pin)
	{
		setJSONValue({
		'INPUT_PinEnable'   : Pin_no_act
		});
	}
	else
	{
		setJSONValue({
			'INPUT_PinEnable'   : G_PinActSta
		});
	}
	*/
}
/*
function RemainTimesDisplay(){
	
	$('Puk_Times').innerHTML = G_RemainPukTimes;
	$('Pin_Times').innerHTML = G_RemainPinTimes;
	$('Chg_Pin_Times').innerHTML = G_RemainPinTimes;
}
*/
function uiOnload(){
	var state = ["No Sim card insert", "Sim card's PIN code was unlocked", "Need the PIN code to unlock sim card", "Need the PUK code to unlock PIN code"];
	setJSONValue({
		'INPUT_3GEnable'   : G_USB3GEnable
	});
	
	setEnable();	
	//ctrlChgPinDisplay();
	PinPukDisplay();
	select_action();
	dealWithError();
}

function uiSubmit(){
	
	var node_pin = $('INPUT_3GEnable','INPUT_PIN','INPUT_PUK','Old_pin','New_pin', 'En_pin', 'Confirm_pin', 'Dis_pin');

	/*
	if($('INPUT_NewSMBPasswd').value != $('INPUT_RNewSMBPasswd').value){
		DealWith.Wrong('SELECT_WanConnection',SEcode[1010]);
		return false;
	}
	*/
	if($('INPUT_3GEnable').checked ==true )
	{
		if(Form.Radio("Action") == "Chg_pin")
		{
			if(node_pin[4].value != node_pin[6].value)
			{
				alert("The confirm pin code and the new pin must be equal!");
				return false;
			}
		}

		$H({
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.USB3GEnable"       : node_pin[0].checked ? 1 : 0,
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.EnterPinCode"  : node_pin[1].value != '' ?  node_pin[1].value : undefined,
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.EnterPukCode"  : node_pin[2].value != '' ?  node_pin[2].value : undefined,
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.OldPinCode"     : node_pin[3].value != '' ? node_pin[3].value : undefined,
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.NewPinCode": node_pin[4].value != '' ? node_pin[4].value : undefined,
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.PinEnable": Form.Radio("Action") == "En_pin" ? 1 : (Form.Radio("Action") == "Dis_pin" ? 0 : undefined),
			'obj-action'   : 'set',
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:errorpage': G_Page,
			'getpage'      : 'html/index.html',
			'var:CacheLastData': ViewState.Save()
		});
	}
	else
	{
		$H({
			":InternetGatewayDevice.X_TWSZ-COM_MODEM.USB3GEnable"       : node_pin[0].checked ? 1 : 0,
			'obj-action'   : 'set',
			'var:menu'     : G_Menu,
			'var:page'     : G_Page,
			'var:errorpage': G_Page,
			'getpage'      : 'html/index.html',
			'var:CacheLastData': ViewState.Save()
		});

		$F('mid', MID_NEW3G);
		$F(':InternetGatewayDevice.X_TWSZ-COM_MODEM.action', 'dial');

		/*let webproc send the set msg to logic_3g module*/
		$F('mid', MID_NEW3G);
		
		/*let webproc known this page is belong to new3g module*/
		$F('isnew3g', '1');	

		$F(':InternetGatewayDevice.X_TWSZ-COM_MODEM.action', 'pin');
		$F(':InternetGatewayDevice.X_TWSZ-COM_MODEM.dialaction', dial_act);		
	}
	
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,Form.Action);

