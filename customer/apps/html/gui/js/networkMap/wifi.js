var securitypass = "";
var securitypassWPA2 = "";
var securitypassAUTO = "";
var securitypassWPA = "";
var securitypassWEP = "";
var wifiwarningsactived = false;
var securitySelected = "";

function dhcpstatusmodified(check){
    if (check.checked) 
        setTimeout("activeDHCP();", 0);
    else {
        if (wifiwarningsactived) {
            showInfoDialog($("#wifi_alert4").html(), false, true, function(resp){
                if (resp == true) {
                    setTimeout("deactiveDHCP();", 0);
                }
                else {
                    $('#dhcp_status').attr('checked', true).change();
                }
            });
        }
        else 
            setTimeout("deactiveDHCP();", 0);
    }
}

function activeDHCP(){
    $("#DHCPpool").show('blind', 500, function(){
    });
}

function deactiveDHCP(){
    $("#DHCPpool").hide('blind', 500, function(){
    });
}

function wifistatusmodified(check){
    if (check.checked) {
        setTimeout("activedWifi();", 0);
    }
    else {
        if (wifiwarningsactived) {
            showInfoDialog($("#wifi_alert1").html(), false, true, function(resp){
                if (resp == true) 
                    setTimeout('deactivedWifi();', 0);
                else {
                    $('#wifi_status').attr('checked', true).change();
                }
            });
        }
        else 
            setTimeout("deactivedWifi();", 0);
    }
}

function activedWifi(){
    $("#wifiConf").css('display', 'block');
    load_iphone_butons();
    $("#select_numcanal").selectbox();
    $("#wifi_sec").selectbox();
    $("#wifiConf").show('blind', 500, function(){
    });
    $("#wifi .wifi_ico").attr("color", "green");
}

function deactivedWifi(){
    $("#wifiConf").hide('blind', 500, function(){
    });
    $("#wifi .wifi_ico").attr("color", "grey");
}

function ssidmodemodified(check){
    if (check.checked) 
        setTimeout("$('#num_canal').hide(500);", 0);
    else 
        setTimeout("$('#num_canal').show(500);", 0);
}

function wifivisibilitymodified(check){
    var message = $("#wifi_alert2").html();
    var iswarning = false;
    var isoptional = true;
    //if (check.checked) 	showInfoDialog(message, iswarning, isoptional);
    if (check.checked) {
    }
    else {
        if (wifiwarningsactived) {
            showInfoDialog(message, iswarning, isoptional, function(resp){
                if (resp != true) {
                    $('#wifi_visible').attr('checked', true).change();
                }
            });
        }
    }
    //showInfoDialog(message, iswarning, isoptional,callback);
}

function wifisecuritymodified(select){
    if (!(select.value == "WPA2" || select.value == "WPA" || select.value == "AUTO")) {
        if (wifiwarningsactived) {
            showWarningOptional($("#wifi_alert3").html(), function(resp){
                if (resp == true) {
                }
                else {
                    if (securitySelected == "WPA2") {
                        $("#wifi_sec")[0].options[0].selected = true; //I set WPA2
                        $("#wifi_sec").parents('.jquery-selectbox').unselectbox();
                        $("#wifi_sec").selectbox();
                    }
                else 
                    if (securitySelected == "AUTO") {
                        $("#wifi_sec")[0].options[1].selected = true; //I set AUTO
                        $("#wifi_sec").parents('.jquery-selectbox').unselectbox();
                        $("#wifi_sec").selectbox();
                    }	
                    else 
                        if (securitySelected = "WPA") {
                            $("#wifi_sec")[0].options[2].selected = true; //I set WPA
                            $("#wifi_sec").parents('.jquery-selectbox').unselectbox();
                            $("#wifi_sec").selectbox();
                        }
                }
                // if finaly WEP is selected, password change
                if ($("#wifi_sec")[0].options[3].selected == true) {
				
                    securitySelected = "WEP";
                    securitypass = securitypassWEP;
                    $("#wifiPass").attr("value", securitypass);
                    $("#wifiPass").unbind('keyup');
                    $("#wifiPass").keyup(function(event){
                        updateWEPstrength(false);
                    });
                    $("#wifiPass").keyup();
                }
                // if finaly no-security is selected, password line is hidden
                if ($("#wifi_sec")[0].options[4].selected == true) {
                    securitySelected = "None";
                    $("#wifi_pass").hide(500);
                }
            });
        }
    }
    if ($("#wifi_sec")[0].options[0].selected == true) {
        securitySelected = "WPA2";
        securitypass = securitypassWPA2;
        $("#wifiPass").attr("value", securitypass);
    }
    if ($("#wifi_sec")[0].options[1].selected == true) {
        securitySelected = "AUTO";
        securitypass = securitypassAUTO;
        $("#wifiPass").attr("value", securitypass);
    }	
    if ($("#wifi_sec")[0].options[2].selected == true) {
        securitySelected = "WPA";
        securitypass = securitypassWPA;
        $("#wifiPass").attr("value", securitypass);
    }
    $('#wifiPass').unbind("keypress");
    $('#wifiPass').unbind("keydown");
    $('#wifiPass').unbind("keyup");
    if (select.value == "WPA" || select.value == "WPA2") {
        $("#wifiPass").keyup(function(event){
            updateWPAstrength(false);
        });
        $("#wifiPass").keyup();
    }
    if (select.value == "WEP") {
        $("#wifiPass").keyup(function(event){
            updateWEPstrength(false);
        });
        $("#wifiPass").keyup();
    }
    //if (select.value=="None") $("#wifi .wifi_ico").attr("color","red");
    //if WPA, WPA2 or WEP, password line is shown
    if ($("#wifi_sec")[0].options[4].selected != true && ($("#wifi_pass").css("display") == "none")) 
        $("#wifi_pass").show(500);
}

function updateDhcp(){
    var ipLan1 = $("#wifiLan1").val();
    $("#poolStart").html("192.168." + ipLan1 + ".");
    $("#poolEnd").html("192.168." + ipLan1 + ".");
}

function wifiInfoReceived(success, data){
    renoveSession();
    if (!success) {
        showWarning(_(data));
        showInfoDialog(_(data), true, false, function(){ //showwarning con callback
            closeDialog();
        }, null);
        
        return;
    }
    
    var status = data.WIFI.status;

    if (status == "0" || status == "false") 
        status = 0;
    if (status == "1" || status == "true") 
        status = 1;
 
    var ssidname = data.WIFI.ssidName;
    var visibility = data.WIFI.ssidVisibility;
    if (visibility == "0" || visibility == "false") 
        visibility = 0;
    if (visibility == "1" || visibility == "true") 
        visibility = 1;
    
    var channelMode = data.WIFI.channelMode;
    if (channelMode == "MANUAL") 
        channelMode = 0;
    else 
        channelMode = 1;
    var channel = data.WIFI.channel;
    
    var security = "None";
    
    if (data.WIFI.SECURITY != undefined) {
        security = data.WIFI.SECURITY.cipherAlgorithm;
        securitypassWPA = data.WIFI.SECURITY.passwordWPA;
        securitypassWPA2 = data.WIFI.SECURITY.passwordWPA2;
	securitypassAUTO = data.WIFI.SECURITY.passwordAUTO;
        securitypassWEP = data.WIFI.SECURITY.passwordWEP;
    }
    
    var dhcpstatus = false;
    var start = "33";
    var end = "255";
    var common = "";
    if (data.DHCP != undefined) {
        dhcpstatus = data.DHCP.status;
        if (dhcpstatus == "0" || dhcpstatus == "false") 
            dhcpstatus = 0;
        if (dhcpstatus == "1" || dhcpstatus == "true") 
            dhcpstatus = 1;
        
        start = data.DHCP.poolStart;
        if (start != null && start.length > 0) {
            var ip_parts = start.split(".");
            var n = ip_parts.length;
            if (n > 1) {
                start = ip_parts[n - 1]; //I take the last one
            }
            
        }
        end = data.DHCP.poolEnd;
        if (end != null && end.length > 0) {
            var ip_parts = end.split(".");
            var n = ip_parts.length;
            if (n > 1) 
                end = ip_parts[n - 1]; //I take the last one
        }
    }
    
    // UPDATING WIFI VALUES IN PAGE	
    if ($("#wifi_status").attr("checked") != status) 
        $("#wifi_status").attr("checked", status).change();
    
    if (!status) {
        $("#wifi .wifi_txt").attr("key", "MAP_WIFI_DISABLED");
    }
    else {
        if (security == "WPA2") 
            $("#wifi .wifi_txt").attr("key", "MAP_WIFI_WPA2");
        else 
            if (security == "WPA") 
                $("#wifi .wifi_txt").attr("key", "MAP_WIFI_WPA");
            else 
                if (security == "WEP") 
                    $("#wifi .wifi_txt").attr("key", "MAP_WIFI_WEP");
                else 
                    if (security == "None") 
                        $("#wifi .wifi_txt").attr("key", "MAP_WIFI_NONE");
    }
        reloadTextsDiv('wifi');
    if (ssidname != null) 
        $("#ssidname").attr("value", ssidname);
    if (visibility != undefined) 
        $("#wifi_visible").attr("checked", visibility).change();
    if (channelMode != undefined) 
        $("#ssid_mode").attr("checked", channelMode).change();
    if (channel != undefined && channel != 0) 
        $("#select_numcanal")[0].options[channel - 1].selected = true;
    if (security != undefined) {
        if (security == "WPA2") {
            securitySelected = "WPA2";
            $("#wifi_sec")[0].options[0].selected = true;
            securitypass = securitypassWPA2;
            $("#wifiPass").unbind('keypress');
            $("#wifiPass").unbind('keydown');
            $("#wifiPass").unbind('keyup');
            $("#wifiPass").keyup(function(event){
                updateWPAstrength(false);
            });
            updateWPAstrength(true);
        }
        else 
            if (security == "AUTO") {
                securitySelected = "AUTO";
                $("#wifi_sec")[0].options[1].selected = true;
            	securitypass = securitypassAUTO;
            	$("#wifiPass").unbind('keypress');
            	$("#wifiPass").unbind('keydown');
            	$("#wifiPass").unbind('keyup');
            	$("#wifiPass").keyup(function(event){
                	updateWPAstrength(false);
            });
            updateWPAstrength(true);
            }		
        else 
            if (security == "WPA") {
                securitySelected = "WPA";
                $("#wifi_sec")[0].options[2].selected = true;
                securitypass = securitypassWPA;
                $("#wifiPass").unbind('keypress');
                $("#wifiPass").unbind('keydown');
                $("#wifiPass").unbind('keyup');
                $("#wifiPass").keyup(function(event){
                    updateWPAstrength(false);
                });
                updateWPAstrength(true);
            }
            else 
                if (security == "WEP") {
                    securitySelected = "WEP";
                    $("#wifi_sec")[0].options[3].selected = true;
                    securitypass = securitypassWEP;
                    $("#wifiPass").unbind('keypress');
                    $("#wifiPass").unbind('keydown');
                    $("#wifiPass").unbind('keyup');
                    $("#wifiPass").keyup(function(event){
                        updateWEPstrength(false);
                    });
                    updateWEPstrength(true);
                }
                else {
                    securitySelected = "None";
                    $('#wifi_pass').hide();
                    $("#wifi_sec")[0].options[4].selected = true;
                }
    }
    if (securitypass != undefined) 
        $("#wifiPass").attr("value", securitypass);
    //LAN 
    if (data.LAN != undefined && data.LAN != null) {
    	//
        var ipLan = data.LAN.ip;
        var ipMask = data.LAN.mask;
        $("#wifiLanMask").html(ipMask);
        var ip = ipLan.split(".");
        common = ip[0] + "." + ip[1] + "." + ip[2] + ".";
        if (ip.length > 1) {
            $("#wifiLan0").html(ip[0] + "." + ip[1] + ".");
            $("#wifiLan1").val(ip[2]);
            $("#wifiLan2").val(ip[3]);
        }
    }
    //UPDATING DHCP VALUES IN PAGE
    if (dhcpstatus != undefined) {
        if ($("#dhcp_status").attr("checked") != dhcpstatus) 
            $("#dhcp_status").attr("checked", dhcpstatus).change();
    }
    if (start != undefined) 
        $("#pool_start").attr("value", start);
    if (end != undefined) 
        $("#pool_end").attr("value", end);
    if (common != undefined) {
        $("#poolStart").html(common);
        $("#poolEnd").html(common);
    }
    wifiwarningsactived = true;
}

function prepareWifiJSON(){
    var wifi = new Object();
	
    wifi.status = $("#wifi_status").attr("checked");
    if (wifi.status) {
        wifi.ssidName = ($("#ssidname").attr("value")).replace( new RegExp('"', 'g'), '\\"');;
        wifi.ssidVisibility = $("#wifi_visible").attr("checked");
        wifi.channelMode = ($("#ssid_mode").attr("checked")) == true ? "AUTO" : "MANUAL";
        if (wifi.channelMode == "MANUAL") 
            wifi.channel = 1 * $("#select_numcanal")[0].value;
        
        var security = new Object();
        security.cipherAlgorithm = $("#wifi_sec")[0].value;
        if (security.cipherAlgorithm != "None") {
            if (security.cipherAlgorithm == "WPA2") {
                security.algVersion = "WPA2";
                security.passwordWPA2 = $("#wifiPass").attr("value");
                security.passwordWEP = securitypassWEP;
                security.passwordWPA = securitypassWPA;
		security.passwordAUTO = securitypassAUTO;
                
            }
            else 
                if (security.cipherAlgorithm == "WPA") {
                    security.algVersion = "WPA1";
                    security.passwordWPA = $("#wifiPass").attr("value");
                    security.passwordWEP = securitypassWEP;
                    security.passwordWPA2 = securitypassWPA2;
		   security.passwordAUTO = securitypassAUTO;
                }
			else if (security.cipherAlgorithm == "AUTO") {
                    security.algVersion = "AUTO";
                    security.passwordAUTO = $("#wifiPass").attr("value");
                    security.passwordWEP = securitypassWEP;
                    security.passwordWPA2 = securitypassWPA2;
		   security.passwordWPA = securitypassWPA;
				}
                else {
                    security.passwordWEP = $("#wifiPass").attr("value");
                    security.passwordWPA = securitypassWPA;
                    security.passwordWPA2 = securitypassWPA2;
		   security.passwordAUTO = securitypassAUTO;
                    if (security.passwordWEP.length == 5) 
                        security.algVersion = "64";
                    if (security.passwordWEP.length == 13) 
                        security.algVersion = "128";
                }
        }
        else {
            security.algVersion = "None";
        }
        wifi.SECURITY = security;
    }
    
    var dhcp = new Object();
    dhcp.status = $("#dhcp_status").attr("checked");
    if (dhcp.status) {
        dhcp.poolStart = jQuery($("#DHCPpool .inicio_dir")[0]).html() + $("#pool_start").attr("value");
        dhcp.poolEnd = jQuery($("#DHCPpool .inicio_dir")[0]).html() + $("#pool_end").attr("value");
    }
    var lan = new Object();
    var ip = "" + $("#wifiLan0").html() + $("#wifiLan1").val() + "." + $("#wifiLan2").val();
    lan.ip = ip;
    lan.mask = $("#wifiLanMask").html()
    
    var WifiJSON = new Object();
    WifiJSON.WIFI = wifi;
    WifiJSON.DHCP = dhcp;
    WifiJSON.LAN = lan;
    
    var WifiJSONstring = $.toJSON(WifiJSON);
    //alert(WifiJSONstring);
    return WifiJSONstring;
}

function wifiaccept(){
    wifiwarningsactived = false;
    renoveSession();
    var resultLong = checkPassLong();
    if (resultLong) {
		
		if (CONFIG.LanConfEnabled) {
			var resultLan = checkIpLan();
			if (!resultLan) 
				return;
		}
		
        var test = testWifiFields();
        if (test == true) {
            var wifiJSON = prepareWifiJSON();
            setWifiConfiguration(wifiJSON, function(success, error_desc){
                if (!success) 
                    showWarning(_(error_desc));
                else {
                    closeDialog();
                    getWifiInfo(updateWifiInNetworkmap);
                }
            });
            
        }
    }
    
    
}

function wificancel(){
    //self.status="closing dialog";
    renoveSession();
    getWifiInfo(updateWifiInNetworkmap);
}

function checkIpLan(){
    var ipPart1 = $("#wifiLan1").val();
    var ipPart2 = $("#wifiLan2").val();
    if (!(isNumeric(ipPart1) && (ipPart1 > -1) && (ipPart1 < 256))) {
        showWarning(_("WIFI_ERROR_LAN_IP"));
        return false;
    }
    if (!(isNumeric(ipPart2) && (ipPart2 > 0) && (ipPart2 < 255))) {
        showWarning(_("WIFI_ERROR_LAN_IP"));
        return false;
    }
    return true;
}

function checkPassLong(){
    var pass = $("#wifiPass").attr("value");
    if ($("#wifi_sec").val() == 'WPA') {
        if (pass.length >= 8) 
            return true;
        else {
            showWarning(_("WIFI_WARNING5"));
            return false;
        }
    }
    else 
        if ($("#wifi_sec").val() == 'WEP') {
            var i = pass.length;
            if (i == 5 || i == 13) 
                return true;
            else {
                showWarning(_("WIFI_WARNING6"));
                return false;
            }
        }
        else {
            return true;
        }
}

function testWifiFields(){
    var dhcp = new Object();
    if ($("#dhcp_status").attr("checked") == true) {
        var poolStart = $("#pool_start").attr("value");
        var poolEnd = $("#pool_end").attr("value");        
        if (!(isNumeric(poolStart) && (poolStart > 0) && (poolStart < 256))) {
            showWarning(_("WIFI_ERROR_POOL1"));
            return false;
        }
        if (!(isNumeric(poolEnd) && (poolEnd > 0) && (poolEnd < 256))) {
            showWarning(_("WIFI_ERROR_POOL2"));
            return false;
        }
        if ((1 * poolStart) > (1 * poolEnd)) {
            showWarning(_("WIFI_ERROR_POOL3"));
            return false;
        }
        var ipPart = $("#wifiLan2").val();
        ipPart = ipPart * 1;
        if ((ipPart >= poolStart) && (ipPart <= poolEnd)) {
        	if (CONFIG.LanConfEnabled) showWarning(_("WIFI_ERROR_POOL4"));
        	else showWarning(_("WIFI_ERROR_POOL5"));
        	return false;
        }
    }

	var wifiName = $("#ssidname").attr("value");
	if( wifiName.indexOf('<?') != -1 )
	{
		alert("Error: The string of SSID not support contain <? ");
		return false;
	}
    return true;
}

function checkAscii(event){
    if (document.all) { // Internet Explorer
        if (((event.keyCode < 32) || (event.keyCode > 126)) &&
        (event.keyCode != 8) &&
        (event.keyCode != 46)) {
            event.returnValue = false;
            return false;
        }
    }
    else 
        if (document.getElementById) { // Mozilla
            if (((event.which < 32) || (event.which > 126)) &&
            (event.which != 8) &&
            (event.which != 46) &&
            (event.which != 0)) {
                event.preventDefault();
                return false;
            }
        }
    return true;
}

function checkKey(event){
    alert("checkKey");
    if (checkAscii(event)) {
        if ($("#wifi_sec").val() == "WPA") 
            setTimeout("updateWPAstrength(false)", 10);
        
        if ($("#wifi_sec").val() == "WEP") 
            setTimeout("updateWEPstrength(false)", 10);
        
    }
}

function updateWPAstrength(b){
    var value;
    if (b) 
        value = securitypass;
    else 
        value = $("#wifiPass").attr("value");
    var n = 0;
    if (value != null) 
        n = value.length;
    if (n < 0) 
        n = 0;
    var L = 0;
    if (n == 0) 
        L = 0;
    else 
        if (n < 8) 
            L = 1;
        else 
            if (n == 8) 
                L = 3;
            else 
                if (n == 9) 
                    L = 4;
                else 
                    if (n >= 10) 
                        L = 5;
    $(".dialog_security_img").css("background-position", "0 " + (L * -10) + "px");
    $("#pass_level_value").attr("key", "WIFI_SEC_PASS_STRENGTH_" + L);
    reloadTextsDiv("wifi_pass");
}

function updateWEPstrength(b){ //alert(b);
    var value;
    if (b) 
        value = securitypass;
    else 
        value = $("#wifiPass").attr("value");
    //alert(value);
    //console.log(value);
    var n = 0;
    if (value != null) 
        n = value.length;
    if (n < 0) 
        n = 0;
    var L = 0;
    if (n == 0) 
        L = 0;
    else 
        if (n < 5) 
            L = 1;
        else 
            if (n >= 5 && n < 13) 
                L = 2;
            else 
                if (n >= 13) 
                    L = 3;
    
    $(".dialog_security_img").css("background-position", "0 " + (L * -10) + "px");
    $("#pass_level_value").attr("key", "WIFI_SEC_PASS_STRENGTH_" + L);
    reloadTextsDiv("wifi_pass");
}

function goToOriginalConfig(){
    showWarningOptional($("#wifi_alert7").html(), function(resp){
        if (resp == true) {
            window.open(CONFIG.VENDORCONFIGURATION.Link);
        }
    });
}
