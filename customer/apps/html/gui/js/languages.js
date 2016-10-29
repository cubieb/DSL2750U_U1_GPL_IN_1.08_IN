//  LANGUAGES-INTERNATIONALIZATION

var LANG = null;

function loadLanguages(url){	
	if (url==null) url="languages/";
	var i=0;
	for( i=0; i<CONFIG.LANGUAGES.length;i++){
		var shortn = CONFIG.LANGUAGES[i].shortName;
		var file = url+CONFIG.LANGUAGES[i].file;	
		$.ajax({
			type:"POST",
			url: file,
			success: function(data, textStatus){
				$.i18n(shortn+".dashboard", data);
				if (LANG == shortn) {
					reloadTexts();					
				};				
   			},
			async: false,
			dataType: "json"
		});
	}
}

function loadTipsTexts(url){
	if (url==null) url="languages/";
	var i=0;
	for( i=0; i<CONFIG.LANGUAGES.length;i++){
		var shortn = CONFIG.LANGUAGES[i].shortName;
		var file = url+"tips."+shortn+ ".json";		
		$.ajax({
			type:"GET",
			url: file,
			cache: false,
			success: function(data, textStatus){
				$.i18n(shortn+".dashboard", data);
   			},
			dataType: "json",
			async: false
		});
	}
}

function loadDefaultLanguage(url){
	LANG = CONFIG.DefaultLanguage;
	if (url==null) url="languages/";
	var file = null;	
	var i=0;
	for( i=0; i<CONFIG.LANGUAGES.length;i++){
		var shortn = CONFIG.LANGUAGES[i].shortName;
		if (shortn==LANG){
			file = url+CONFIG.LANGUAGES[i].file;
			break;
		}
	}	
	if (LANG == "en") {   		setEnglish();}
	else if (LANG == "es") {	setSpanish();}
	else if (LANG == "pt") {	setPortuguese();}
	else if (LANG == "ch") {	setChinese();}
	
	jQuery.getJSON( file, function(data, textStatus){
		$.i18n(LANG + '.dashboard', data   );
		reloadTexts();
  	});
}

function _(str, args) { 
   // return $.i18n('dashboard', str, args); 
    return $.i18n("dashboard", str, args);
}
function setSpanish(){
	LANG = 'es';
	$.i18n(LANG);
	reloadTexts();		
}
function setEnglish(){	
	LANG = 'en';
	$.i18n(LANG);	
	reloadTexts();		
}
function setPortuguese(){
	LANG = 'pt';
	$.i18n(LANG);
	reloadTexts();		
}
function setChinese(){
	LANG = 'ch';
	$.i18n(LANG);	
	reloadTexts();	
}
function reloadTexts(){	
	$(".trad").each(function (i) {		
		var key =jQuery(this).attr("key");
		var value= _(key)
		jQuery(this).html(value);
	});	
}
function reloadTextsDiv(id_element){	
	$("#"+id_element+" .trad").each(function (i) {		
		var key =jQuery(this).attr("key");
		var value= _(key);
		jQuery(this).html(value);		
	});	
}