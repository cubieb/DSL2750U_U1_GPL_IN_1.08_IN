var urlConfig = "";
var urlExternal = "";
var urladdress = "";
function setLanguages(){	
	$.getScript("../../js/languages.js", function(){
		setTimeout('loadDefaultLanguage("../../languages/");',0);		
		setTimeout('configureLinksHeader();',0);	
		setTimeout('loadLanguages("../../languages/")',0);		
		setTimeout("$('#panel_general').css('visibility', 'visible');",0);	
	});				
}

function closeWelcome(success,returnData){
	urlExternal = "http://"+urladdress+"/html/gui/";
	top.location=urlExternal;
}
function goConfigMHS(){
	top.location=urlConfig;
}
function addFavorites(){
	var navInfo = window.navigator.appVersion.toLowerCase();  
	if(navInfo.indexOf('mac') != -1){  
		showWarning(_("WELCOME_FAVORITES_INSTR_MAC"),"../../html/warning.html");
	}else {	
		if (window.sidebar) { // Mozilla Firefox Bookmark  
			 window.sidebar.addPanel("Home Station", urlConfig,"");  
		}else if((navigator.userAgent.toLowerCase().indexOf('chrome/')) > -1){//chrome not support bookmarking scripts
			showWarning(_("WELCOME_FAVORITES_INSTR_WIN"),"../../html/warning.html");
		}else if( window.external ) { // IE Favorite  
			 window.external.AddFavorite( urlConfig, "Home Station");  
		} else{ // for Safari, Konq etc - browsers who do not support bookmarking scripts (that i could find anyway)  
			showWarning(_("WELCOME_FAVORITES_INSTR_WIN"),"../../html/warning.html");  
		 }
	}
}
function loadUrl(success,returnData){
	if (success){
		urlConfig= returnData.configuration_url;
		urlExternal= returnData.external_url;
		urladdress = returnData.ipaddress;
		$('#urlMHS').text(urlConfig);
		$('#divUrlMHS').children('a').attr('href',urlConfig);
		$('#imgConfigPage').children('a').attr('href',"http://192.168.1.1/html/gui/");
	}else{
		showWarning(_(returnData));
	}
}