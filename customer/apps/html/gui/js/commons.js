
function preloadDialog(title, width){
	$('#dialog').dialog("destroy");		
	try	{					
		$('#dialog').dialog({
			autoOpen: false,
			bgiframe: true,
			//height: '720',	
			width: width,
			position:["center", "top"],	
			closeOnEscape: true,
			draggable:false,
			resizable:false,
			show:"blind", //jquery.1.4 does not support it
			hide:"blind",
			title: title,
			open: function(event, ui) {//$('#help_dialog').dialog("destroy");
			},
			close: function(event, ui) {//$('#help_dialog').dialog("destroy");
			},
			closeText: '',			
			modal: true
		});		
	}catch(e){}	
}

function loadDialog(page, title, width, callback){
	preloadDialog(title, width);		
	//Callback to control execution time
	$('#dialog').load(page, function(){				
		reloadTextsDiv('dialog');									
   		$('#dialog').dialog('open');	
		$(".ui-widget-overlay").click(function(){			
			$('#dialog').dialog("close");
		});
		/*$(".dialog_faq").click(function(){				
			loadHelpDialog(_("HELP_TITLE"));		
		});*/		
		effectHover('cancel', 'cancel_icon');				
		effectHover('accept', 'accept_icon');	
		
		$('#cancel').click(function(){
			closeDialog();
		});			
				
		if (callback!=null) callback();									
	});						
}

function closeDialog(){
	$('#dialog').dialog('close');			
}
/*
 * FUNTION showInfoDialog.
 * Parameters:
 * 		text: Text to show in the info box
 * 		iswarning: 
 * 			if false an info box is shown
 *			if true an alert box is shown
 *		isoptional: 
 *			if false, only accept link appears
 *			if true, accept and cancel links are shown
 *		callback: (Optional)
 *			The funtion to be invoked after user chooses or closes the info box
 *			The callback function receives a boolean parameter containing the user's response
*/
var CALLBACK_RESPONSE = false;
function showInfoDialog(text, iswarning, isoptional, callback,urlPage)
{
	$('#dialogWarning').dialog("destroy");		
	try	{					
		$('#dialogWarning').dialog({
			autoOpen: false,
			bgiframe: true,
			//height: 130,	
			width: '330px',
			position:["center", 150],	
			closeOnEscape: true,
			draggable:false,
			resizable:false,
			show:"blind", //jquery.1.4 does not support it
			hide:"blind",
			title: "titulo",
			closeText: '',			
			modal: true,
			close: function(event, ui) {
				 if (	(callback!=undefined)	&&	(callback!=null)	)	
				 	callback(CALLBACK_RESPONSE);		
			}

		});		
		if (urlPage!= null && urlPage!=undefined && urlPage!=""){
			var page = urlPage;
		}else{
			var page = "html/warning.html"
			if(isoptional==true) page = "html/warning_optional.html"
		}

		$('#dialogWarning').load(page, function(){
			var obj = $('#dialogWarning').parent().children(".ui-dialog-titlebar");
			obj.css("visibility","hidden");
			$(".warning_text").html(text);
			reloadTextsDiv('dialogWarning');

			if (iswarning==true) $("#dialogWarning").addClass("warnning");
			else				$("#dialogWarning").removeClass("warnning");
			if (isoptional==true) $("#dialogWarning").addClass("optional");
			else $("#dialogWarning").removeClass("optional");
			
			if (callback!=null) {
				$("#dialogWarning #acceptWarning").attr("onclick", "");
				$("#dialogWarning #acceptWarning").click(function(){	CALLBACK_RESPONSE=true; closeInfoDialog();	});
				$("#dialogWarning .cancel").attr("onclick", "");
				$("#dialogWarning .cancel").click(function(){	CALLBACK_RESPONSE=false;	closeInfoDialog();		});
				$("#dialogWarning #acceptApp").attr("onclick", "");
				$("#dialogWarning #acceptApp").click(function(){	CALLBACK_RESPONSE=true;	closeInfoDialog();	});
			}
					
	   			$('#dialogWarning').dialog('open');	
				effectHover('cancel', 'cancel_icon');				
				effectHover('accept', 'accept_icon');			
		});
	}catch(e){ alert(e);}	
}

function showInfo(text){
	showInfoDialog(text, false, false);
}

function showWarning(text, urlPage){
	showInfoDialog(text, true, false,null,urlPage);
}

function showWarningOptional(text, callback){
	showInfoDialog(text, true, true, callback)
}

function closeInfoDialog(){
	$('#dialogWarning').dialog('close');			
}

function loadScroll(id){	
	$(id).jScrollPane(
		{showArrows:false,
		maintainPosition: false		
	});
}

function effectHover(nameDiv,nameClass){
	var element= $('#' + nameDiv);
	if (nameDiv=="accept" || nameDiv=="cancel" || nameDiv=="usb" || nameDiv=="print"){			
		element = $('.' + nameDiv);		
	}	
	element.mouseover(function(){		
		$(this).children('span').addClass('txt_hover');
		$(this).children('.' + nameClass).addClass('hover');		
	});
	element.mouseout(function(){
		$(this).children('span').removeClass('txt_hover');
		$(this).children('.' + nameClass).removeClass('hover');		
	});
}

function load_iphone_butons(){	
	var checks = $("input[type=checkbox]");
	load_some_iphone_butons(checks);
}

function load_some_iphone_butons(buttons){
	$.each(buttons, function() {
		var button = jQuery(this);
		var alreadyApplied = button.parent().hasClass("iPhoneCheckContainer");
		if (!alreadyApplied)
		{
			button.iphoneStyle({
				uncheckedLabel: button.attr("uncheckedLabel"),
				checkedLabel: button.attr("checkedLabel"),
				resizeContainer: false,
				resizeHandle: false
			}); 
		}
	});
}
function trim(textbox){ 
return textbox.replace(/^\s+/,'').replace(/\s+$/,''); 
 
}
function isEmpty(s){   
	if (s!=null) {
		s = trim(s);
	}
	return ((s == null) || (s.length == 0) );
}
function isNumeric(s){ 
	var exp=/^[0-9]*$/;
    if (isEmpty(s)) return false;
       
    if(exp.test(s))
    	return true;

	return false;  
}
function checkNumbers(event){
	if(document.all){	// Internet Explorer
		if ((event.keyCode < 48) || (event.keyCode > 57))
			event.returnValue = false;
	}
	else if(document.getElementById){	// Mozilla
		if (((event.which < 48) || (event.which > 57)) && (event.which!=8) && (event.which!=0))
			event.preventDefault();
	}
}

function preloadHelpDialog(title, callback){
	$('#help_dialog').dialog("destroy");	
	try	{					
		$('#help_dialog').dialog({
			autoOpen: false,
			closeOnEscape: true,
			draggable:true,
			resizable:false,
			width: '680',
			height:'440',
			show: 'drop',
			hide: 'drop',
			title: title,	
			open: function(event, ui) {setTimeout(callback,0);},					
			closeText: ''
		});		
	}catch(e){}		
}

function loadHelpDialog(title,callback){	
	preloadHelpDialog(title, callback);
	$('#help_dialog').load("html/help.html", function(){				
		reloadTextsDiv('help_dialog');							
   		$('#help_dialog').dialog('open');   		 		
	});					
}