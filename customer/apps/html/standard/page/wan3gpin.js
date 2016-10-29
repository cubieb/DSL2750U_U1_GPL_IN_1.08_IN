<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "PinStatus   RemaindPINRetryTimes RemaindPUKRetryTimes RemainChgPinTimes RemaindLOCKRetryTimes"
`	
	/*sim card status:NO USB CARD/lock disable/
	* lock enable/NEED PIN CODE/NEED PUK CODE 
	*/
	var G_State		 		= "$01";
	var G_RemainPinTimes	= "$02";
	var G_RemainPukTimes	= "$03";
	var G_RemainCHGTimes	= "$04";
	var G_RemainLOCKTimes	= "$05";
`?>

/*sim card status:NO USB CARD/lock disable/lock enable/NEED PIN CODE/NEED PUK CODE */
var constSimStsNoUsb = 'NO USB CARD';
var constSimStsLockDis = 'lock disable';
var constSimStsLockEnbl = 'lock enable';
var constSimStsLockNeedPin = 'NEED PIN CODE';
var constSimStsLockNeedPuk = 'NEED PUK CODE';

var MID_NEW3G = '0x049a';

var _path_pin =':InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.';
var _path_modem =':InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.';

function ctrl3GEnable() {
		$('Static').style.display = "block";
		PinPukDisplay();
}

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
		  $('Chg_Pin_Times').innerHTML = G_RemainCHGTimes;
	      }
}

function PinPukDisplay(){

	$('Static').style.display = 'block';
	if(G_State == constSimStsLockNeedPin)
	{
		Form.Radio("Action", "Un_pin");
		$('PIN_UNLOCK').style.display="block";
		$('PIN_ENABLE').style.display="none";
		$('PIN_DISABLE').style.display="none";
		$('PUK_UNLOCK').style.display="none";
		$('PIN_CHG').style.display="none";

		$('3G_Status').innerHTML = G_State; 
		$('Pin_Times').innerHTML = G_RemainPinTimes;
		
	}
	else if(G_State == constSimStsLockNeedPuk)
	{
		Form.Radio("Action", "Un_puk");
		$('PIN_UNLOCK').style.display="none";
		$('PIN_ENABLE').style.display="none";
		$('PIN_DISABLE').style.display="none";
		$('PUK_UNLOCK').style.display="block";
		$('PIN_CHG').style.display="none";
		$('Change_pin').style.display="none";
		
		//$('3G_Status').innerHTML = data_language.dynamic.status4; 
		$('3G_Status').innerHTML = G_State;
		$('Puk_Times').innerHTML = G_RemainPukTimes;
	}
	else if(G_State == constSimStsLockDis)
	{
		$('Pin_Times').innerHTML = G_RemainLOCKTimes;
		$('PIN_UNLOCK').style.display="none";
		//$('PIN_ENABLE').style.display="none";
		//$('PIN_DISABLE').style.display="block";
		$('PUK_UNLOCK').style.display="none";

		Form.Radio("Action", "En_pin");
		$('PIN_ENABLE').style.display="block";
		$('PIN_DISABLE').style.display="none";
		$('PIN_CHG').style.display="none";
		$('3G_Status').innerHTML = G_State;
	}
	else if(G_State == constSimStsLockEnbl)
	{
		$('Pin_Times').innerHTML = G_RemainLOCKTimes;
		$('PIN_UNLOCK').style.display="none";
		//$('PIN_ENABLE').style.display="none";
		//$('PIN_DISABLE').style.display="block";
		$('PUK_UNLOCK').style.display="none";
		
		Form.Radio("Action", "Dis_pin");
		$('PIN_ENABLE').style.display="none";
		$('PIN_DISABLE').style.display="block";
		$('PIN_CHG').style.display="block";
		$('3G_Status').innerHTML = G_State; 
	}
	else if(G_State == constSimStsNoUsb)
	{
		
		$('3G_Status').innerHTML = G_State;
		$('Static').style.display = 'none';
	}
}

function uiOnload(){
	//ctrlChgPinDisplay();
	PinPukDisplay();
	select_action();
	dealWithError();
}

function uiSubmit(){
	var enablePin = Form.Radio("Action") == "En_pin" ? 
				1 : (Form.Radio("Action") == "Dis_pin" ? 
					0 : undefined);	
	var PinAction = Form.Radio("Action");	

	if(Form.Radio("Action") == "Chg_pin")
	{
		if($('New_pin').value != $('Confirm_pin').value)
		{
			alert("The confirm pin code and the new pin must be equal!");
			return false;
		}
	}
	if(!$('INPUT_PIN').value.match(/^[0-9]{4}$/) && $('INPUT_PIN').value!='')
	{
		alert("Pin Code should be 4 numerals(0-9) ");
		return false;
	}

	if(!$('Old_pin').value.match(/^[0-9]{4}$/) && $('Old_pin').value!='')
	{
		alert("Old Pin Code should be 4 numerals(0-9)");
		return false;
	}
	if(!$('New_pin').value.match(/^[0-9]{4}$/) && $('New_pin').value!='')
	{
		alert("New Pin Code should be 4 numerals(0-9)");
		return false;
	}
	
	$H({
		":InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.PIN"  : $('INPUT_PIN').value ,
		":InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.PUK"  : $('INPUT_PUK').value,
		":InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.OldPinCode"     : $('Old_pin').value,
		":InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.NewPinCode"	: $('New_pin').value,
		":InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.PinEnable"	: enablePin,
		'obj-action'   : 'set',
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:errorpage': G_Page,
		'var:subpage'	:	'wan3gpinresult',
		'getpage'      : 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});

	

	$F(_path_pin +  'PinAction', PinAction);
	
	/*let webproc send the set msg to logic_3g module*/
	$F('mid', MID_NEW3G);
	
	/*let webproc known this page is belong to new3g module*/
	$F('isnew3g', '1');	

	$F(_path_modem + 'action', 'pin');
	
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1)
		return false;
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,Form.Action);

