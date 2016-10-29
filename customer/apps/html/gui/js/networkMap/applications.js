var current_rules=0;
var applicationsInfoList=new Array();
var devicesList=new Array();
var idApplicationSelected = -1;
var flagChange = false;
var flagModify = false;
var flagSave = false;
var currentPage = 1;
var ruleDeleted = -1;
function showApplicationsList(success, returnData){
	if (success){
		applicationsInfoList= returnData;
		showPageApplicationsList(1);
	}else{
		showWarning(_(returnData));
	}
}
function showPageApplicationsList(page){
	currentPage = page;
	var applicationsLength = applicationsInfoList.length;
	var i=0;
	$("#applications_list").html("");
	if(applicationsInfoList.length <13){
		$('div[class=controls_right]').css("display","none");
	}else if(applicationsInfoList.length <25){
		$('div[class=controls_right]').css("display","block");
		$('div[id=lineRight]').css("display","none");
		$('div[id=page_number3]').css("display","none");
	}else if (applicationsInfoList.length <37){
		$('div[id=lineRight]').css("display","block");
		$('div[id=page_number3]').css("display","block");
	}
	if (page == 1){
		if (applicationsInfoList.length >12){
			applicationsLength = 12;
		}
		
	}else if (page == 2){
		var i=12;
		if (applicationsInfoList.length > 24){
			applicationsLength = 24;
		}
		
	}else if (page == 3){
		var i=24;
		if (applicationsInfoList.length > 36){
			applicationsLength = 36;
		}
	}
	$("#applications_list").html("");
	var htmlApplicationsList="";
	for (; i < applicationsLength; i++) {
 		var app = applicationsInfoList[i];
		htmlApplicationsList=htmlApplicationsList+"<div id='"+app.idApplication+"' class='application' onclick='showApplicationSelected("+app.idApplication+")'>"		
		htmlApplicationsList=htmlApplicationsList+"<div class='application_txt'>"+app.nameApplication+"</div>";
		htmlApplicationsList=htmlApplicationsList+"<div ipAssigned='"+app.ipAssigned+"' class='application_assigned' idIcon='"+app.idIcon +"'></div>";	
		htmlApplicationsList=htmlApplicationsList+"<div id='arrow"+app.idApplication+"' class='application_arrow'></div></div>";				
	}
	$("#applications_list").append(htmlApplicationsList);
	loadDefaultAppListStyle(page);
}
function loadDefaultAppListStyle(page){
	$('div[ipAssigned='+false+']').css('display', 'none');
	$('div[class=application_arrow]').css('display', 'none');
	$('div[class=page_numbers_selected]').attr("class","page_numbers right hand");
	$('div[id=page_number'+page+']').attr("class","page_numbers_selected");
	if(idApplicationSelected!=-1){
		loadSelectedAppStyle(idApplicationSelected);
	}
}

function loadCreateRuleForm(){
	
	var id = current_rules++;
	var newRule = $("#sample_rule>div").clone();
	newRule.css("display", "block");
	newRule.attr("id", "rule"+id);
	newRule.find("input[id=checkPortsN]").bind("change" , function(){
		var thisrule = $(this).parents("#rules_list>div");
		if (thisrule!=null && thisrule.length>0)
		updateRuleForm(thisrule);
		changeFlag();	
	});
	newRule.find("input[id=checkPortsN]").attr("id", "checkPorts"+ id);
	newRule.find("span[id=fromN]").attr("id", "from"+ id);
	newRule.find("input[id=ruleStartWanN]").attr("id", "ruleStartWan"+ id);
	newRule.find("div[id=untilN]").attr("id", "until"+ id);
	newRule.find("input[id=ruleEndWanN]").attr("id", "ruleEndWan"+ id);
	newRule.find("select[id=select_trafficN]").attr("id", "select_traffic"+ id);
	newRule.find("div.minus_icon").bind("click", function(){
		var thisrule = $(this).parents("#rules_list>div");
		deleteRule(thisrule);  	
	});
	
	$("#rules_list").prepend(newRule);

	$("#rules_list #checkPorts"+id).attr("checkedLabel",_("APPLICATIONS_PORT_ON"));
	$("#rules_list #checkPorts"+id).attr("uncheckedLabel",_("APPLICATIONS_PORT_OFF"));
	reloadTextsDiv('rules_list');
	$("#rules_list #checkPorts"+id).attr("checked",true); 
	
	updateRuleForm(newRule);
	
	$("#select_traffic"+id).selectbox();
	changeFlag();
	updateSelectEffects();
	updateScroll();
}

function updateRuleForm(rule){
	var value = rule.find("input[type=checkbox]").attr("checked");
	var from = rule.find(".fromField");
	var until = rule.find(".untilField");
	
	if (value){
		from.find("span").text(_("APPLICATIONS_RULES_NUMBER"));
		from.find("span").attr("key","APPLICATIONS_RULES_NUMBER");
		until.css('visibility', 'hidden');
		until.find("input").css('width', '31px');
				
	}else{
		from.find("span").text(_("APPLICATIONS_RULES_FROM"));
		from.find("span").attr("key","APPLICATIONS_RULES_FROM");
		until.css('visibility', 'visible');
		until.find("input").css('width', '40px');
	}
}

function showApplicationSelected(idapp){
	if(flagChange){
		showInfoDialog(_("APPLICATIONS_CONFIRM_LOSE_CHANGES"), false, true,function (resp){
			if (resp == true) {
				if(flagModify){
					if (validateForm()){
						applicationJSON = createApplicationJSON();
						if(applicationJSON != false  ){
							modifyApplication(applicationJSON,reloadList);
							showApp(idapp);
							flagChange = false;
						}
					}					
					renoveSession(); 
				}else{
					if (validateForm()){
						applicationJSON = createApplicationJSON();
						if(applicationJSON != false  ){
							newApplication(applicationJSON,reloadList);
							showApp(idapp);
							flagChange = false;
						}
					}					
					renoveSession(); 
				}
			}else {
				flagChange = false;
				showApp(idapp);
			}
		});	
	}else	showApp(idapp);
}
function showApp(idapp){
	flagModify = true;
	idApplicationSelected = idapp;
	loadSelectedAppStyle(idapp);
	$('#acceptApp').unbind('click'); 						
	$('#acceptApp').click(function() {
		modifyApplicationSelected();
	});
	var i=0;
	for (; i < applicationsInfoList.length;i++) {
		var app = applicationsInfoList[i];
		if(app.idApplication == idApplicationSelected){
			showRules(app)
		}
	}
}
function reloadList(success,returnData){
	applicationsInfoList= returnData;
	showPageApplicationsList(currentPage);
}
function loadSelectedAppStyle(idapp){
	
	$('div[class=application_arrow]').css('display', 'none');
	$('div[class=application_selected]').attr("class","application");
	
	$('div[id=arrow'+idapp+']').css('display', 'block');
	$('div[id=arrow'+idapp+']').css('display', 'block');
	$('div[id='+idapp+']').attr("class","application_selected");
}

function showRulesEx(success, application)
{
	if (success){
		var thisrule = $(this).parents("#rules_list>div");
		deleteRule(thisrule);

		//update left application list
		var newRules = [];
		var n = 0;
		for (var m=0; m<application.RULES.length; m++){
			if (application.RULES[m].idRule == ruleDeleted){
				continue;
			}
			newRules[n] = application.RULES[m];
			n++;
		}

		for (m=0; m<applicationsInfoList.length; m++){
			if (applicationsInfoList[m].idApplication != application.idApplication){
				continue;
			}
			applicationsInfoList[m].RULES = newRules;
		}
		showRules(application);
	}
	else {
		var errorMessage = application;
		showWarning(_(errorMessage));
	}

	unChangeFlag();
}

function showRules(application){
	$("#rules_list").html("");
	current_rules = application.RULES.length;
	
	//adding every rule to the form
	var i=0;
 	for (; i < current_rules; i++) {	
	
		var rule = application.RULES[i];
		var newRule = $("#sample_rule>div").clone();
		newRule.css("display", "block");
		var id = i;
		newRule.attr("id", "rule"+id);
		newRule.attr("idRule", ""+rule.idRule);
		var idrul = id;
		newRule.find("input[id=checkPortsN]").attr("id", "checkPorts"+ id);
		newRule.find("span[id=fromN]").attr("id", "from"+ id);
		newRule.find("input[id=ruleStartWanN]").attr("value", rule.startWan);		//Setting the 'from' port
		newRule.find("input[id=ruleStartWanN]").attr("id", "ruleStartWan"+ id);		
		newRule.find("div[id=untilN]").attr("id", "until"+ id);
		newRule.find("input[id=ruleEndWanN]").attr("value", rule.endWan);
		newRule.find("input[id=ruleEndWanN]").attr("id", "ruleEndWan"+ id);		
		newRule.find("select[id=select_trafficN]").attr("id", "select_traffic"+ id);
		
		
		newRule.find("div.minus_icon").bind("click", function(){
			ruleDeleted = rule.idRule;
			deleteApplicationRule(application.idApplication, rule.idRule, showRulesEx);
		});
		
		newRule.find("input[id=checkPorts"+id+"]").bind("change" , function(){
			var thisrule = $(this).parents("#rules_list>div");
			if (thisrule!=null && thisrule.length>0)
			updateRuleForm(thisrule);
			changeFlag();	
		});
		
		$("#rules_list").append(newRule);
		
		$("#rules_list #checkPorts"+id).attr("checkedLabel",_("APPLICATIONS_PORT_ON"));
		$("#rules_list #checkPorts"+id).attr("uncheckedLabel",_("APPLICATIONS_PORT_OFF"));
		if(rule.startWan == rule.endWan){
			newRule.find("input[type=checkbox]").attr("checked",true);
			updateRuleForm(newRule);
		}
		
		$("select#select_traffic"+id+" option[value='"+rule.protocol+"']").attr("selected", "selected");
		$("#select_traffic"+id).selectbox();
	}
	load_some_iphone_butons($("#rules_list input[type=checkbox]"));
 	loadNameApplication(application.nameApplication);
 	selectDevicesList(application.ipAssigned);
	reloadTextsDiv('rules_list');	
	//flagChange = true;	
	updateSelectEffects();
	updateScroll();
}

function updateSelectEffects(){

	var rules = $("#rules_list>div");
	var N = rules.length;
	var lastone = null;
	var penultimateOne = null;
	
	//initial status
	rules.find(".jquery-selectbox-moreButton").unbind( "click",behaviourLastSelect);
	rules.find(".jquery-selectbox-moreButton").unbind( "click",behaviourPenultimateSelect);
	rules.find("select").unbind( "change");

	if (N>0) lastone=jQuery(rules[N-1]);
	if (N>1) penultimateOne=jQuery(rules[N-2]);
	if (N>=3){
		lastone.find(".jquery-selectbox-moreButton").bind( "click", behaviourLastSelect);
		penultimateOne.find(".jquery-selectbox-moreButton").bind( "click", behaviourPenultimateSelect);
		
		lastone.find("select").bind( "change", closeLastSelect);
		penultimateOne.find("select").bind( "change", closeLastSelect);
	}
}


function behaviourLastSelect(){
	var select = jQuery(this).parent();
	if (select.hasClass("selectOpen")) {	//hide
		closeLastSelect();
	}
	else{								//show
		openLastSelect(101);
	}
}

function behaviourPenultimateSelect(){
	var select = jQuery(this).parent();
	if (select.hasClass("selectOpen")) {	//hide
		closeLastSelect();
	}
	else{								//show
		openLastSelect(67);
	}
}

function openLastSelect(width){
	var widthLast = "101";
	if (width!=undefined && width>0){
		widthLast = ""+width;
	}

	//adding space for the list of options at the end of the scroll layer	
	$("#rules_list>div:last-child").css("height", widthLast+ "px");
	updateScroll();
	
	jQuery(document).bind('click', addBlurEvent);
}

function closeLastSelect(){
	jQuery(document).unbind('click', addBlurEvent);
	
	//removing not used space at the end of the scroll layer
	$("#rules_list>div:last-child").css("height", "34px");
	updateScroll();
}

function addBlurEvent(e){
	var trgt = e.target;
	var currentListElements = jQuery('.jquery-selectbox-list:visible').parent().find('*').andSelf();
	if(jQuery.inArray(trgt, currentListElements)<0 ) {
		closeLastSelect();
	}
	return false;
}

function deleteRule(rule){
	rule.remove();
	//current_rules--;
	changeFlag();
	updateSelectEffects();
	updateScroll();
}

function loadNameApplication(nameApplication){
	$("#application_name").html(nameApplication+"<input id='applicationName'  onkeyup='changeFlag()' style='display:none' name='applicationName' value='"+nameApplication+"'></input>");
 	$('div[id=edit_icon]').css('display', 'block');
 	$("#divNameApp").click(function() {
 		$("#application_name").html("<input id='applicationName' onkeyup='changeFlag()' name='applicationName' maxlength ='17' value='"+nameApplication+"'></input>");
 		$('div[id=edit_icon]').css('display', 'none');
 		$("#applicationName").focus();  
 		$('#divNameApp').unbind("click");
	});
 }
function loadNameNewApplication(){
	$("#application_name").html("<span class='trad' key='APPLICATIONS_NEW_APPLICATION'></span><input onkeyup='changeFlag()' id='applicationName' style='display:none' name='applicationName'></input>");
	$('div[id=edit_icon]').css('display', 'block');
 	$("#divNameApp").click(function() {
 		$("#application_name").html("<input id='applicationName' onkeyup='changeFlag()' name='applicationName' maxlength ='17' ></input>");
 		$('div[id=edit_icon]').css('display', 'none');
 		$("#applicationName").focus();
 		$('#divNameApp').unbind("click");
	});
 	reloadTextsDiv('application_name');
 	$("#applicationName").attr("value",_("APPLICATIONS_NEW_APPLICATION"));
 }

function loadDevicesList(){
		devicesList = allDevices;
		$("#application_device").html("");
		var i=0;
		var htmlDevicesList="<div class='title_grey' style='margin-right: 8px;'><span id='devices_list' class='trad' key='APPLICATIONS_DEVICES_LIST'>Para el dispositivo</span></div><select id='selectDevices' class='select' title='' onchange='changeFlag()'>";
		htmlDevicesList=htmlDevicesList+"<option value='false' icon='false' class='trad' key='APPLICATIONS_DEVICES_NONE'>Ninguno</option>	";
		for (; i < devicesList.length; i++) {	
			device = devicesList[i];
			if(device.interfaceType!="USB"){
				var nameDevice =device.nameDevice;
				if(device.unknown || nameDevice==null || nameDevice==""){
					nameDevice=device.ipAddress;	
				}
				htmlDevicesList=htmlDevicesList+"<option icon="+device.idIcon+" value="+device.ipAddress+"><span>"+nameDevice+"</span></option>	";
			}
		}

		htmlDevicesList=htmlDevicesList+"</select>";
		$("#application_device").append(htmlDevicesList);
		reloadTextsDiv('application_device');
		$("#selectDevices").selectbox();
}
function selectDevicesList(ipAssigned){
		if(ipAssigned == 0)
		{
			loadDevicesList();
		}
		else
		{
			$("#selectDevices").parents('.jquery-selectbox').unselectbox();
			$("select#selectDevices option[value='"+ipAssigned+"']").attr("selected", "selected");
	 		$("#selectDevices").selectbox();
		}
}

function loadApplicationsList(success, returnData){
	if (success){
		showApplicationsList(success, returnData)
		loadNewApplication();
		
	}else{
		showWarning(_(returnData));
	}
}
function loadNewApplication(){
	loadNameNewApplication();
	$("#rules_list").html("");
 	loadCreateRuleForm();
 	selectDevicesList("false");
 	$('div[class=application_selected]').attr("class","application");
 	$('#acceptApp').unbind('click'); 						
	$('#acceptApp').click(function() {
		createNewApplication();
	});
 	idApplicationSelected =-1;
 	loadDefaultAppListStyle(1);
	load_some_iphone_butons($("#rules_list input[type=checkbox]"));
 	flagChange = false;
 	showPageApplicationsList(1);
 }

function deleteApplicationSelected(){
	if (idApplicationSelected!=-1){
		showWarningOptional(_("APPLICATIONS_CONFIRM_DELETE"), function (resp){
			if (resp == true) {
				deleteApplication(idApplicationSelected,loadApplicationsList);
			}
		});	
	}else{
		showWarning(_("APPLICATIONS_WARNING_DELETE"));
	}
	flagChange = false;
	renoveSession();
}
function validateForm(){
	var i=0;
	var nameValue = $("#applicationName").attr("value");
	if (nameValue  == ""){
		showWarning(_("APPLICATIONS_WARNING_NAME_EMPTY"));
		return false;
	}

	var res = true;
	$("#rules_list>div").each( function(index){
		var thisrule = $(this);
		var check = thisrule.find("input[type=checkbox]");
		if (check.attr("checked")){
			if(thisrule.find(".fromField>input").val()==""){
				res = false;		
			}
		}else{
			if(	(thisrule.find(".fromField>input").val()=="") || (thisrule.find(".untilField>input").val()=="")) {
				res = false;		
			}
		}
	});
	
	if(!res){
		showWarning(_("APPLICATIONS_WARNING_PORT_EMPTY"));
		return  false;	
	}
	
	return true;
}
var applicationJSON;
function createNewApplication(){
	if (applicationsInfoList.length>35){
		showWarning(_("APPLICATIONS_WARNING_MAX_NUMBER_APPS"));
	}else{
		if (validateForm()){
			applicationJSON = createApplicationJSON();
			if(applicationJSON != false  ){
				newApplication(applicationJSON,loadApplicationsList);
			}
		}
		renoveSession();
	}
}

function modifyApplicationSelected(){
	if (validateForm()){
		applicationJSON = createApplicationJSON();
		if(applicationJSON != false  ){
			modifyApplication(applicationJSON,loadApplicationsList);
		}
	}
	renoveSession();
}
var objectApp= new Object();
function createApplicationJSON(){
	objectApp.idApplication = idApplicationSelected;
	objectApp.nameApplication = $("#applicationName").val();
	objectApp.ipAssigned = $("#selectDevices").val();
	objectApp.idIcon = $("#selectDevices option[value='"+$("#selectDevices").val()+"']").attr("icon");
	var rules = new Array();
	var i=0
	var k=0;
	$("#rules_list>div").each( function(index){
		var objetcRule = new Object();
		var thisrule = $(this);
		var select_traffic = thisrule.find("select");
		
		if (thisrule!=null && thisrule.length==1){
			objetcRule.idRule   = thisrule.attr("idRule");
			objetcRule.protocol = thisrule.find("select").val();
			objetcRule.startWan	= thisrule.find(".fromField>input").val();
			if(thisrule.find("input[type=checkbox]").attr("checked")){
				objetcRule.endWan   = objetcRule.startWan;
				objetcRule.startLan	= objetcRule.startWan;
				objetcRule.endLan   = objetcRule.endWan;
			}else{
				objetcRule.endWan = thisrule.find(".untilField>input").val();
				objetcRule.startLan	= objetcRule.startWan;
				objetcRule.endLan   = objetcRule.endWan;
				if(eval(objetcRule.startWan) > eval(objetcRule.endWan) ){
					showWarning(_("APPLICATIONS_WARNING_RANGE"));
					return false;
				}
			}
				
			rules[k]= objetcRule;
			k++;
		}
	});
	
	if(k == 0 ){
		showWarning(_("APPLICATIONS_WARNING_RULES"));
		return false;
	}else{
		objectApp.RULES = rules;
		var applicationJSON = $.toJSON(objectApp);
		applicationJSON="{\"APPLICATION\":"+applicationJSON+"}";

		return applicationJSON;
	}
}
function updateScroll(){	
	setTimeout("loadScroll('#rules_list')",4);
}
function checkPort(obj){
	var i=0;
	var j=0;
	for (; i < applicationsInfoList.length;i++) {
		var application = applicationsInfoList[i];
		current_rules = application.RULES.length;
		for (j = 0; j < current_rules; j++) {
			var rule = application.RULES[j];
			if(obj.value == rule.startWan)
			{
				showWarning(_("APPLICATIONS_WARNING_PORT_ERROR2"));
				obj.value="";
				return false;
			}
		}	
		
	}
	
	var port = obj.value;
	if(port!=""){
		if((port<1)||(port>65535)){
			showWarning(_("APPLICATIONS_WARNING_PORT_ERROR"));
			obj.value="";
			return false;
		}
	}
	return true;	
}
function unChangeFlag(){
	flagChange = false;
}
function changeFlag(){
	flagChange = true;
}
