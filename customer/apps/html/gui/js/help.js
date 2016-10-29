var last_topic=null;
var last_tip="";
function scroll(){		
	try{	//applying or reapplying scroll plugin
		$('#help_scroll').jScrollPane();		
	}catch(e){};
}

function setHelpLinks()
{
	$('.help_topic_txt').hide();
	$('.help_title_topic').click(function(){			
		showTip($(this).parent());
	});	
}

function linkContent(Ntopic, Ntip) {
	var topicLink = $("#navi li:nth-child("+Ntopic+") div"	);
	var topicKey = topicLink.attr("topic_key");
	linkTip(topicKey, Ntip);
}

function linkTip(topicKey, tip) {
	//tip can be a string (tipkey) or a number (tip index)
	var topic_selector = ".help_topic[topic_key="+topicKey+"]";
	$('#help_scroll').load("html/topics.html " + topic_selector, function(){
		reloadTextsDiv('help_scroll');
		setHelpLinks();

		updateTopicByKey(topicKey);
		//show the content in right side
		if (isNaN(tip))		{	//tip is a key_tip identificator
			var tipDiv = $(".help_topic>div[tip_key="+tip+"]");
			showTip(tipDiv);
		}
		else{					//tip is an index in the tip list
			showTipByIndex(tip);
		}
	});
}

function updateTopicByKey(topicKey){
	//topicLink: objeto jquery q apunta al link del topic en la izq
	var topicLink = $("#navi div[topic_key="+topicKey+"]"	);
	updateTopicLink(topicLink);
}

function updateTopicByIndex(Ntopic){
	//topicLink: objeto jquery q apunta al link del topic en la izq
	var topicLink = $("#navi li:nth-child("+Ntopic+") div"	);
	updateTopicLink(topicLink);
}

function updateTopicLink(topicLink){
	//topicLink: objeto jquery q apunta al link del topic en la izq

	//checking if topic changes
	//if so, the topic index is updated (left side)
	if(!topicLink.hasClass("help_menuentry_selec")){		//linking to a topic not selected						
		if (last_topic != undefined  & last_topic != topicLink) {
			last_topic.removeClass("help_menuentry_selec");			//unselecting the previous one		
		}
		topicLink.addClass("help_menuentry_selec");	
		scroll();
		last_topic=topicLink;
	}
	
	//Updating Topic title	
	var key_language = topicLink.attr("key");
	$('#topic_title>span').attr("key", key_language);
	$('#topic_title>span').html(_(key_language));
}

function showTipByIndex(Ntip){
	if (isNaN(Ntip)) return;
	var tipDiv = $(".help_topic>div:nth-child("+Ntip+")");
	if (tipDiv!=null && tipDiv.length==1)
		showTip(tipDiv);	
}
function showTip(tipDiv){	
	var response = tipDiv.children(".help_topic_txt");
	if (tipDiv.attr('id') != null) {
		if (tipDiv.hasClass("shown")) {
			tipDiv.removeClass("shown");
			last_tip = "";
			response.hide('blind', 500);
		}
		else {
			if (last_tip != "" & last_tip != tipDiv) {
				last_tip.removeClass("shown");
				last_tip.children(".help_topic_txt").hide('blind', 500);
			}			
			tipDiv.addClass("shown");
			last_tip = tipDiv;
			
			response.show('blind', 500, function(){
				scroll();
				var index = $(".help_topic>div").index(tipDiv)+1;
				//alert($("#help_scroll").css("height"));
				if ($("#help_scroll").css("height")=="auto")
					$(".help_scroll")[0].scrollTo(".help_topic>div:nth-child("+index+") .help_title_num");
			});
		}
	}
}