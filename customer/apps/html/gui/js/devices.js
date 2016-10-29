
var allApplications=new Object();
function loadInfoDevice(success,applications){
	allApplications=applications;
	loadIconDev(typeDevice);
	if(unknownDevice=="true"){
		$("#devName").html("<input type='text' id='nameDev' name='nameDev' value=''/>");
		setTimeout('$("#nameDev").focus();',1000);
		$('#devName').unbind("click");
	}else{
		$('#nameDevice').html(nameDevice);
	}		
	
	$('#ipDevice').html(ipDevice);
	if(connectedDevice=="true"){
		if(interfaceDevice=="Ethernet"){
			$("#connected").html(_("CONF_DEV_ETHERNET"));
			$("#connected").attr("key","CONF_DEV_ETHERNET");
		}else if (interfaceDevice=="WiFi"){
			$("#connected").html(_("CONF_DEV_WIFI"));
			$("#connected").attr("key","CONF_DEV_WIFI");
		}
	} 
	else {
		$("#connected").html(_("CONF_DEV_DISCONNECTED"));
		$("#connected").attr("key","CONF_DEV_DISCONNECTED");
		$('#minus').html("<span class='forget_dev' class='trad' key='CONF_DEV_FORGET'>Olvidar dispositivo</span><div class='minus_icon forget_dev' style='margin-top:-4px'></div>");
	}
	
	if(success){		
		loadDevAppList(scrollDevApp);	
	}else{
		showWarning(_(applications));
	}
}
function scrollDevApp(){	
	if (allApplications.length > 8) {		
			$("#app_content").removeClass('fix_height');
			$("#app_content").addClass('height');			
	}else{			
			$("#app_content").addClass('fix_height');
			$("#app_content").removeClass('height');			
	}		
	setTimeout('loadScroll("#app_content");',0);		
}
function loadDevAppList(callback){	
	$("#app_content").html("");
	var i=0;
	for(i=0;i<allApplications.length;i++){	
		var htmlDisp="";
		var app = allApplications[i];
		
		htmlDisp=htmlDisp+"<div class='appCheck_content'><div class='left'><input type='checkbox' name='"+app.ipAssigned +"' id='"+app.idApplication +"' disabled=''/></div><div class='left dev_txtCheck'>"+app.nameApplication +"</div>";
		if(app.ipAssigned != "false" && app.ipAssigned != ipDevice){			
			htmlDisp=htmlDisp+"<div class='application_assigned dev_imgCheck' idIcon='"+app.idIcon +"'></div>";
		}
		htmlDisp=htmlDisp+"</div>";	
		$("#app_content").append(htmlDisp);
	}		
	
	//Enable & check
	$('input[name='+ipDevice+']').attr('checked', 'checked');	
	$('input[name='+false+']').attr('disabled', '');
	$('input[name='+ipDevice+']').attr('disabled', '');	
	
	if (callback!=null) callback();	
}

function loadIconDev(typeDevice){
	if (	(typeDevice!=null) && 	(typeDevice.length>0)	)
	{
		idIconDevice=typeDevice + "_1";
		$('#iconBig').attr("idIcon", idIconDevice);		//modifying the attribute, the image url is updated by css
	}
}
function responseRemoveDev(success,returnData){
	if(success){
		$('#dialog').dialog("close");
		showDevices(success,returnData);
		
	}else{
		showWarning(_(returnData));
	}
}
function responseModifyDev(success,returnData){
	if(success){
		$('#dialog').dialog("close");
		showDevices(success,returnData);
		
	}else{
		showWarning(_(returnData));
	}
}
function responseAssignApp(success,returnData){
	if(success){
		var devicesJSON=createdevicesJSON();
		modifyDevice(devicesJSON, responseModifyDev);		
	}else{
		showWarning(_(returnData));
	}
}
var appAssignedDev= new Array();
function createdAppListJSON(){
	var j=0;
	var k=0;
	for(  ;j<allApplications.length;j++){		
		var app = allApplications[j];
		var object = new Object();
		if($('input[id='+app.idApplication+']').attr("checked")){
			object.idApplication=app.idApplication;
			object.nameApplication=app.nameApplication;
			object.ipAssigned=ipDevice;
			object.idIcon=idIconDevice;
			appAssignedDev[k]=object;		
			k++;
		}else
		{
			if(!($('input[id='+app.idApplication+']').attr("disabled"))){
			object.idApplication=app.idApplication;
			object.nameApplication=app.nameApplication;
			object.ipAssigned="0";
			object.idIcon="0";
			appAssignedDev[k]=object;		
			k++;
			}
			
		}
		
	}
	
	var jsonText = $.toJSON(appAssignedDev,"\t");
	jsonText="{\"APPLICATION_LIST\":"+jsonText+"}";
	return jsonText;
}
function createdevicesJSON(){
	var object = new Object();
	object.idDevice = idDev;
	if($('#nameDev').val()==null || $('#nameDev').val()==undefined){
		object.nameDevice =$('#nameDevice').html();
		nameDevice=$('#nameDevice').html();		
	}else{				
		object.nameDevice =$('#nameDev').val();
		nameDevice=$('#nameDev').val();
	}	
	object.type=$('select#selectType').val();
	typeDevice=$('select#selectType').val();
	object.idIcon =idIconDevice;
	var jsonText = $.toJSON(object,"\t");
	jsonText="{\"DEVICES\":"+jsonText+"}";
	return jsonText;
} 
 
