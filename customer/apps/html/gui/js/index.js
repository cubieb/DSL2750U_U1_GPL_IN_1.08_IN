var filesadded="";
var CONFIG = null;
var STATUS = null;
var USER_NAME =	"Admin";

function configurationLoaded(){
	var imprementation = CONFIG.Implementation;
	
	if (CONFIG.Branding == "o2") 
		replacecssfile("defaultBranding.css", "css/o2.css");
	else if (CONFIG.Branding == "telefonica") 
		replacecssfile("defaultBranding.css", "css/telefonica.css");
	else if (CONFIG.Branding == "chile") {
		replacecssfile("defaultBranding.css", "css/telefonica.css");
		loadjscssfile("css/chile.css", "css");
	}
	else 
		replacecssfile("defaultBranding.css", "css/movistar.css");
	
	if (imprementation=="api"){
		loadjscssfile("APIS/api.js", "js");		
	}else if (imprementation=="api_fake"){		
		loadjscssfile("APIS/api_fake.js", "js");		
	}
	loadDefaultLanguage();
	setTimeout("$('#panel_header').load('html/header.html',function() {reloadTextsDiv('panel_header');configureLinksHeader();});",0);
	setTimeout("$('#panel_footer').load('html/footer.html',function() {loadFooter()});",0);
	setTimeout("$('#panel_content').load('html/networkMap/network_map.html', function(){reloadTextsDiv('panel_content');});",0);
	setTimeout("loadLanguages();",0); 	
	setTimeout("loadTipsTexts();",0);
	setTimeout("disableAdvancedSettings();",0);	
	setTimeout("getLoginStatus(initializeStatus);",10);
	setTimeout("checkNavigator();",1);
}
function loadFooter(){
	reloadTextsDiv('panel_footer');
	if(CONFIG.Country=='ES')
		$('#txtFooter').html("&copy; 2010 Telefónica S.A. <span class='trad' key='FOOT_RIGHTS'>Todos los derechos reservados</span>");
	else if(CONFIG.Country=='UK')
		$('#txtFooter').html("&copy; 2010 Telef&oacute;nica O2 UK Limited");
	else if(CONFIG.Country=='BR')
		$('#txtFooter').html("&copy; 2010 Telefônica S.A.");
	
	if (CONFIG.Branding =='o2'){
		$('.imgFooter').css('display','block');
		$('.txtFooter').css('float','right');
		$('#panel_footer a').css('color','#515559');
	}
	
}
//	Dynamically load and add .js, css or php files
function loadjscssfile(filename, filetype,callback){
	var fileref= createjscssfile(filename, filetype);

 	if (typeof fileref != "undefined") {
		fileref.appendTo("head");
	}
}

function createjscssfile(filename, filetype){
	var fileref= null;
 	if (filetype=="js"){ //if filename is a external JavaScript file
		fileref=$("<script>");
		fileref.attr("type","text/javascript");
		fileref.load(function() { /*alert("Hello");*/ });
		fileref.attr("src", filename);		
 	}
	else if (filetype=="css"){ //if filename is an external CSS file
		fileref=$("<link>");
		fileref.attr("rel", "stylesheet");
		fileref.attr("type", "text/css");
		fileref.attr("href", filename);		
	}
	return fileref;
}

function replacecssfile(oldfilename, newfilename){
 	var targetelement=	"link";  	//element type to create nodelist using
 	var targetattr	=	"href";		//corresponding attribute to test for
 	var allsuspects	=	document.getElementsByTagName(targetelement);
 	
	for (var i=allsuspects.length; i>=0; i--){ //search backwards within nodelist for matching elements to remove
		
  		if (allsuspects[i] && allsuspects[i].getAttribute(targetattr)!=null && allsuspects[i].getAttribute(targetattr).indexOf(oldfilename)!=-1){
   			var newelement=createjscssfile(newfilename, "css");
   			allsuspects[i].parentNode.replaceChild(newelement[0], allsuspects[i]);
  		}
 	}
}
function initializeStatus(success,loginData){
	if(success){
		STATUS=loginData;
		USER_NAME=loginData.USER;
	}
	else{
		STATUS={"LOGGED":false, "USER":""};
	}
}
function logIn(){	
	updateLoginStatus(true, USER_NAME);	
	enableAdvancedSettings();
	if (!timer_is_on)	setTimeout("startSessionChecker();",1000);
}

function logOutAdmin(){	
	updateLoginStatus(false,"");
	disableAdvancedSettings();
	stopSessionChecker();
	closeSession(function(response){
		//alert(response);	
	});
}
function updateLoginStatus(loged, user){
	STATUS.LOGGED=loged;
	STATUS.USER=user;	
}
function configureLinksHeader(){
	$('#faq').click(function() {		
		loadHelpDialog(_("HELP_TITLE"), function() { linkContent("1","0"); });		
	});
		
	var htmlLanguagesList="";
	var long_name="";	
	var lang_name="";
	var line=0;
	for (i = 0; i < CONFIG.LANGUAGES.length; i++) {
		var shortn = CONFIG.LANGUAGES[i].shortName;		
		if (shortn == 'es') {long_name = 'Spanish'; lang_name="Espa&ntilde;ol";
		}else if (shortn == 'en') {long_name = 'English'; lang_name="English";
		}else if(shortn == 'pt'){long_name = 'Portuguese'; lang_name="Portugu&eacute;s";
		}else if(shortn == 'ch'){long_name = 'Chinese'; lang_name="中文";}
		if(CONFIG.LANGUAGES.length==1){
			if($('#lineLanguages').length>0) $('#lineLanguages').attr("style","display:none");
		}else if(CONFIG.LANGUAGES.length==2){
			htmlLanguagesList = htmlLanguagesList + "<span id='link_"+shortn+"'+ style='display:none' class='first'>";
			htmlLanguagesList = htmlLanguagesList + "<a class='right' href='javascript:set"+long_name+"();changeLink();'>"+lang_name+"</a></span>";
		}else{
			htmlLanguagesList = htmlLanguagesList + "<span id='link_"+shortn+"'+ style='display:none'";
			if (line==0){htmlLanguagesList = htmlLanguagesList + "class='first'>";}else if (line==0){htmlLanguagesList = htmlLanguagesList + ">";};		
			if (line>0){ htmlLanguagesList = htmlLanguagesList + "><div id='first' class='line_small right'></div>";}
			htmlLanguagesList = htmlLanguagesList + "<a class='right' href='javascript:set"+long_name+"();changeLink();'>"+lang_name+"</a></span>";
			line=line+1;
		}
	};
	$("#languages").append(htmlLanguagesList);
	changeLink();
	for (i = 0; i < CONFIG.LANGUAGES.length; i++) {
		var shortn = CONFIG.LANGUAGES[i].shortName;
		if (shortn != CONFIG.DefaultLanguage) {			
			$("#link_" + shortn).css("display", "inline");			
		}else{			
			$(".network_map").css("background", "url(./images/brd_"+CONFIG.Branding+"/net_map_"+shortn+".png) no-repeat");	
		}
	}
}

function disableAdvancedSettings(){	
	$('.enabled').each(function(){		
		if ($(this).children().hasClass('disabled_opacity')) {			
			$(this).children().fadeTo("slow", 0.4);
		};
		if ($(this).hasClass('disabled_opacity')){
			$(this).fadeTo("slow", 0.4);			
		}		
		$(this).removeClass('enabled');
		$(this).addClass('disabled');
	});	
}

function enableAdvancedSettings(){	
	$('.disabled').each(function(){		
		if ($(this).children().hasClass('disabled_opacity')) {			
			$(this).children().fadeTo("slow", 1.0);
		};
		if ($(this).hasClass('disabled_opacity')) {			
			$(this).fadeTo("slow", 1.0);
		};		
		$(this).removeClass('disabled');
		$(this).addClass('enabled');
	});		
} 

function loadAuthentication(callback){
	authentication(USER_NAME, $('#pass').val(),callback)
}

function loadDialogLogin(page, title, width, reload){
	preloadDialog(title, width);
	$('#dialog').load(page, function(){				
		reloadTextsDiv('dialog');							
		$('#dialog').dialog('open');	
		$(".ui-widget-overlay").click(function(){			
			$('#dialog').dialog("close");
		});	
		/*$(".dialog_faq").click(function(){				
			loadHelpDialog(_("HELP_TITLE"));		
		});*/	
		
		isPasswordDefault(paintLoginPage);
		setTimeout('$("#pass").focus();',1500);
		effectHover('cancel', 'cancel_icon');
		effectHover('accept', 'accept_icon');
		$('#cancel').click(function(){
			closeDialog();
		});	
		$('#pass').keypress(function(event) {
			if(event.keyCode == 13){
				loadNextDialog(reload)
			}
		});
		$('#acceptLogin').click(function(){
			loadNextDialog(reload)
		});				
	});			
}
function loadNextDialog(reload){
	if (reload == "wifi") {
		loadAuthentication(loadModalWifi);
	}else if (reload == "applications") {
		loadAuthentication(loadModalApplications);
	}else if (reload == "devices") {
		loadAuthentication(loadModalDevice);
	}else if (reload == "password") {
		loadAuthentication(loadModalPassword);
	}else if (reload == "adsl") {
		loadAuthentication(loadModalAdsl);
	}else if (reload == "threeG") {
		loadAuthentication(loadModalThreeG);
	}else if (reload == "USB") {
		loadAuthentication(loadModalUSB);
	}else if(reload=="Printer"){
		loadAuthentication(loadModalPrinter);
	}
}
function paintLoginPage(success,password){
	if(success){
		if(password.isDefault){
			//texts for default password
			$("#divLogin").removeClass("passTop");
			$("#divLogin").addClass("divLogin");
			$("#divLogin").html("<span class='textLogin trad' key='LOGIN_TEXT_2'></span>"+
					"<span class='left'><div class='imgDefaultStiker' /></span>");
			$('#forgetQuestion').html(_("COMMON_EMPTY"));
			$("#forgetQuestion").attr("key","COMMON_EMPTY");
			reloadTextsDiv('divLogin');
		}else{
			//texts for modified password
			$("#divLogin").removeClass("divLogin");
			$("#divLogin").addClass("passTop");
			$('#divLogin').html("<span id='textLogin'></span>")
			$('#forgetQuestion').html(_("LOGIN_FORGET"));
			$("#forgetQuestion").attr("key","LOGIN_FORGET");	
			$("#forgetQuestion").click(function(){			
				loadForgetDiv();
			});					
		}
	}else{ 
		//password is errorDescription
		showWarning(_(password));
	}
}
function loadForgetDiv(){
	$('#forget').html("<div class='line_grey'></div>" +
			"<div class='left exclamationForget'><img src='images/dialog/exclamation.png'/></div>" +
			"<div class='left txtForget' id='forget'>" +
				"<div class='passTop trad paragraph1' key='LOGIN_FORGET_1'> </div>" +
				"<div class='passTop'>" + 
					"<div class='right'><div class='gateway_reset_ico' /></div>" +
					"<div class='trad' key='LOGIN_FORGET_2' />" + 
					
				"</div>" +
			"</div>" +
			"");
	reloadTextsDiv('forget');
}

//AFTER LANGUAGE CHANGE
function changeLink(){
	var i=0;
	var language = LANG;
	for (i = 0; i < CONFIG.LANGUAGES.length; i++) {
		var shortn = CONFIG.LANGUAGES[i].shortName;
		if (shortn == language) {
			//Hide the link		
			$("#link_" + language).css("display", "none");
			if ($("#link_" + language).hasClass('first')) {
				$("#first").css("display", "none");				
			};
		}
		else {
			//Show the link
			$("#link_" + shortn).css("display", "inline");
			if ($("#link_" + shortn).hasClass('first')) {
				$("#first").css("display", "inline");				
			};
		}
	}
	
	//update "network map" image
	$(".network_map").css("background", "url(./images/brd_"+CONFIG.Branding+"/net_map_"+LANG+".png) no-repeat");	
}

function checkNavigator(){
	if(/MSIE 6.0/i.test(navigator.userAgent)) {
		showWarning(_("INDEX_NAVIGATOR"));
	}
}
/*******************************************************************
 *  	FUNCTIONS TO CONTROL THE SESSION TIMEOUT
 *******************************************************************/
//var sessionMaxTime = 15;
var remainSession=-1;
var sessionTimer;
var timer_is_on=false;

function startSessionChecker(){
	if (!timer_is_on)	{
		timer_is_on=true;
		remainSession=CONFIG.SessionMaxTime;
		setTimeout("timedInterval();",500);
		//$("#remaining_session_box").show();
	}
}

function stopSessionChecker(){
	if (timer_is_on)	{
		timer_is_on=false;
		remainSession=-1;
		updateTimerPage();
		//setTimeout("$('#remaining_session_box').hide();",5000);
	}
}

function timedInterval(){
	if (!timer_is_on) return;	
	updateTimerPage();
	remainSession=remainSession-10;
	if (remainSession>=0){
		sessionTimer=setTimeout("timedInterval()",10000);
	} 
	else expiredSession();
}

function updateTimerPage(){
	//if (remainSession>0)	$("#remaining_session").html(""+remainSession+ " sg");
	//else 			$("#remaining_session").html("OFF");
}

function renoveSession(){
	remainSession = CONFIG.SessionMaxTime;	
}

function expiredSession(){
	stopSessionChecker();
	timer_is_on = false;
	logOutAdmin();
	if (CONFIG.SessionWarning) showWarning(_("COMMON_SESSION_WARNING"));
}

/*******************************************************************
 *  	FUNCTIONS TO CONTROL DEVICE BAG ACTUALIZATION
 *******************************************************************/
var timer_updater_is_on=false;
var updaterTimer;

function startBagUpdaterThread(){
	if (!timer_updater_is_on)	{
		timer_updater_is_on=true;
		updaterTimer = setTimeout("timedUpdate();",1000*CONFIG.UpdateStep);
	}
}

function stopUpdater(){
	if (timer_updater_is_on)	{
		timer_updater_is_on=false;
		//updateTimerPage();
		//setTimeout("$('#remaining_session_box').hide();",5000);
	}
}

function timedUpdate(){
	if (!timer_updater_is_on) return;	
	//updateTimerPage();
	//alert("update"); 
	//self.status =self.status + " update"; 

	updateNetworkMap();
	updaterTimer=setTimeout("timedUpdate()",1000*CONFIG.UpdateStep);
}

function updateNetworkMap(){
	listDevices(updateDevicesBag);
	getInternetInfo(showInternetStatus);
	getWifiInfo(updateWifiInNetworkmap);
}


