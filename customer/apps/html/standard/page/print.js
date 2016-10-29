
/************************        get print info    *************************/
var print_info = [];
<?mget :InternetGatewayDevice.X_TWSZ-COM_IPPD. "IppdEnable PrinterName MakeModel Manufacturer Model CMD FWVer"
`	print_info[0] = "$01";
	print_info[1] = "$02";
	print_info[2] = "$03";
	print_info[3] = "$04";
	print_info[4] = "$05";
	print_info[5] = "$06";
	print_info[6] = "$07";
`?>
var lan_addr = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress?>"
/****************************************************************************/


function load_print()
{
		Table.Clear('print_info_table');
		if (print_info[3] == '') return;
		var array_value = [];
		array_value[0] = [
				  print_info[3],
				  print_info[4],
				  print_info[5],
				  print_info[6]
				  ]
	$T('print_info_table',array_value);
}

function load_url()
{
		if (print_info[3] == '') return;
		var path = '<b>http://' + lan_addr + ':631' + '/printers/' + $('print_name').value + '</b>';
		setJSONValue({
			'URL'     :     path
					 });
}


function uiOnload() 
{
	load_print();
	$('print_name').value = print_info[1];
	Form.Checkbox('print_enable', print_info[0]);
	load_url();
}

function Submit()
{
	var checked = $('print_enable').checked ? '1' : '0';
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_IPPD.PrinterName' : $('print_name').value,
		':InternetGatewayDevice.X_TWSZ-COM_IPPD.IppdEnable' : checked,
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action'       : 'set',
		'getpage'          : 'html/index.html',
		'errorpage'        : 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	   });
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