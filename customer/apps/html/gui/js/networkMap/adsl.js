function setAdslData(){
	var internetJSON=createinternetPPPJSON()
	setInternet(internetJSON,internetCallback)
}

function createinternetPPPJSON(){
	var object = new Object();
	//object.type=$('#adslType').html();
	object.username=$('#adslUser').val();
	object.password=$('#adslPass').val();
	var jsonText = $.toJSON(object,"\t");
	jsonText="{\"INTERNET\": {\"PPP\":"+jsonText+"}}";
	return jsonText;
}
function internetCallback(success, respInternet){
  	if(success){
		//getInternetInfo(updatePPPstatus);
  	}else{
		showWarning(_(respInternet));
		return;
  	}
	$('#dialog').dialog("close");
}

function adslDataReceived(success,internet){
	if(success){
		var adsltype=internet.PPP.type;
		var adsluser=internet.PPP.username; 
		var adslpassword=internet.PPP.password;
		paintAdslData(adsltype,adsluser,adslpassword);
	}else{
		showWarning(_(internet));
	}
}

function paintAdslData(adsltype,adslUser,adslPassword){
	$('#adslType').html(adsltype);
	$('#adslUser').val(adslUser);
	$('#adslUser').attr("disabled", false);
	$('#adslPass').val(adslPassword);
	$('#adslPass').attr("disabled", false);
}
