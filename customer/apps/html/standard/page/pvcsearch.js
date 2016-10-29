G_DefaultPVCList = '<?get :InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.DefaultPVCList?>'

function uiOnload()
{
	$('INPUT_DefaultPVC').value = G_DefaultPVCList;
}
function uiSubmit()
{
	var DefaultPVCList = $('INPUT_DefaultPVC').value.split(',');
	for(var i=0; i<DefaultPVCList.length; i++)
	{
		var Vpi = 0, Vci = 0;
		var pvc = DefaultPVCList[i].split('/');
		Vpi = pvc[0];
		Vci = pvc[1];
		if((Vpi < 0) || (Vpi >255) || (Vci < 32) || (Vci > 65535))
		{
			alert("PVC:"+DefaultPVCList[i]+" is not available!");
			return false;
		}
	}
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.DefaultPVCList'  : $('INPUT_DefaultPVC').value,
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
addListeners(uiOnload);