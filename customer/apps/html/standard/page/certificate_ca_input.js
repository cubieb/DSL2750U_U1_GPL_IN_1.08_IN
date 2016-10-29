//CERTIFICAFE
function toBack(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=nwtools&var:subpage=certificate_ca';
}

function CancelCertificate(){
	$('certificateName').value='';
	$('TEXTAREA_IncertCert').value='';
	
}

function isInProgress() {
	var node_value = $('config').value;

	if(node_value == "" ){
		alert(data_language.dynamic.lang_select_update_file);
		return false;
	}
	
	if (progress == 0 )  {
		progress = 1;
		return true;
	}
	alert(data_language.dynamic.lang_config_file_uploading);
	return false;
}

function ApplyCertificate(){
    alert("Not Available!");	
}

var G_CAFilePath = "<?get :InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath?>";

function uiOnload(){
	if(G_CAFilePath == ''){
		$('TEXTAREA_IncertCert').value = data_language.dynamic.certificate + "\\n" +  data_language.dynamic.certificate1 + "\\n" +  data_language.dynamic.certificate2;
	} else {
		$('apply').disabled = true;
	}
}

function uiSubmit(){
	$H({
	    ':InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAname' : $('certificateName').value,
		':InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFile' : $('TEXTAREA_IncertCert').value,
		'obj-action'   :'set',
		'var:menu'     :'advanced',
		'var:page'     :'certificate_ca',
		'var:subpage'  :'certificate_ca',
		'var:errorpage':'certificate_ca_input',
		'getpage'      :'html/index.html',
		'errorpage'    :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}

function uiAdd()
{
	$('add').enabled = true;
	if ($('AddAuthfile1').style.display == 'none')
		{	
			$('AddAuthfile1').style.display = 'block';
		}
	else if(($('AddAuthfile1').style.display == 'block') && ($('AddAuthfile2').style.display == 'none'))
		{	
			$('AddAuthfile2').style.display = 'block';
	       }
	else if (($('AddAuthfile1').style.display == 'block') && ($('AddAuthfile2').style.display == 'block'))
		{
			alert("A maximum 3 files can be uploaded ");
			
		}
	
}
function uiDelete(){
	if(G_CAFilePath == '' || !confirm(data_language.dynamic.confirmMsg)){//如果为空，则不发送
		return false;
	}
	$H({
		':InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath' : '',
		'obj-action'   :'set',
		'var:menu'     :'management',
		'var:page'     :'tr069',
		'var:subpage'  :'importcert',
		'var:errorpage':'importcert',
		'getpage'      :'html/index.html',
		'errorpage'    :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();	
}

function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}



addListeners(uiOnload,dealWithError,Form.Action);