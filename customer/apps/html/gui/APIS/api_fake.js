/*----------------------------*/
/*		SESSION MANAGEMENT    */
/*----------------------------*/
function doCloseSession(session, callback){
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :
                {
                        "var:page":"*",
                        "obj-action":"auth",
			":action"   :"logout",
			":sessionid":session,
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
function closeSession(callback){
/*
	jQuery.getJSON( 'APIS/returnJSON.txt', 
		function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success,"");
			}else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
			}
		}
	);
*/
	$.ajax({
		url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnSessionJSON.txt&var:page=returnSessionJSON.txt',
		dataType: 'json',
		cache: false,
		success: function(returnData, status){
		        if(returnData.RETURN.session != "-" && returnData.RETURN.success){
        		        doCloseSession(returnData.RETURN.session, callback);
		        }
		}
	});
}

/*---------------------------------------*/
/*	 AUTHENTICATION AND PASSWORD CHANGE	 */
/*---------------------------------------*/
function getLoginStatus(callback){
/*
	jQuery.getJSON('APIS/returnLoginStatusJSON.txt', 
		function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success,returnData.STATUS);
			}else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
			}
		}
	);	
*/
	$.ajax({
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnLoginStatusJSON.txt&var:page=returnLoginStatusJSON.txt',
                dataType: 'json',
                cache: false,
                success: function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                               callback(returnData.RETURN.success,returnData.STATUS); 
                        }
			else{
				var status = '{' + '"LOGGED": false, "USER": ""' +  '}';
	                        var json = eval('(' + status + ')');
				callback(false ,json);
			}
                },
		error:function(xmlhttprequest, textStatus,errorThrown){
                        var status = '{' + '"LOGGED": false, "USER": ""' +  '}';
                        var json = eval('(' + status + ')');
                        callback(false, json);
                }
        });
}
//returnJSON structure contains a passwordJSON structure
function isPasswordDefault(callback){
/*
	jQuery.getJSON( 'APIS/returnPasswordJSON.txt', 
		function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success,returnData.PASSWORD);
			}else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
			}
		}
	);
*/ 
  //  var errorDescription="";
	$.ajax({
	 	url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnPasswordJSON.txt&var:page=returnPasswordJSON.txt',
		dataType: 'json',
		cache: false,
		success: function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success,returnData.PASSWORD);	
			}
		/*	else{
				callback(returnData.RETURN.success,errorDescription);
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
              */  }
	});
}
function doAuth(user, password, callback, sessionId)
{
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :
                {
                        "var:page":"*",
                        "obj-action":"auth",
                        ":action":"login",
                        ":username":user,
                        ":password":password,
                        "var:noredirect":"1",
                        ":sessionid":sessionId
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}

function authentication(user, password, callback){
/*	jQuery.getJSON( 'APIS/returnJSON.txt', 
		function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success,"");
			}else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
			}
		}
	);*/
    var errorDescription="ERROR_AUTHENTICATION";
	$.ajax({
		  	url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnSessionJSON.txt&var:page=returnSessionJSON.txt',
		  	dataType: 'json',
		  	cache: false,
		  	success: function(returnData, status){
				if(returnData.RETURN.session != "-" && returnData.RETURN.success){
					doAuth(returnData.RETURN.user, password, callback, returnData.RETURN.sessionid);
				}else{
				callback(returnData.RETURN.success,errorDescription);
			   } 
			},
			error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
		});
}
function changePassword(user,oldPassword,newPassword, callback){
/*	jQuery.getJSON( 'APIS/returnJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,"");
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);*/
    var errorDescription="ERROR_CHANGE_PASS";
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :
                {
                        "var:page":"*",
                        ":InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.1.UserName":user,
                        ":InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.1.Password":oldPassword+":"+newPassword,
                        "obj-action":"set",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(returnData.RETURN.success,errorDescription);
                        }
                },
				error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
        });
}

/*---------------------------*/
/*	 DEVICES CONFIGURATION 	 */
/*---------------------------*/
//returnJSON structure contains a devicesJSON structure with information about all devices
function listDevices(callback){
		$.ajax({
		  	url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnDevicesJSON.txt&var:page=returnDevicesJSON.txt',
		  	dataType: 'json',
		  	cache: false,
		  	success: function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.DEVICES);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		});
}
function tbsItemBegin(itemHash, itemValue)
{
	//jsonString += '{';
	return '{' + '"' + itemHash + '":"' + itemValue +'"';
}
function tbsFormatAItem(itemHash, itemValue)
{
	//jsonString += ',' + '"' + itemHash +'":"' + itemValue + '"';
	return ',' + '"' + itemHash +'":"' + itemValue + '"';
}
function tbsItemEnd(itemEndString)
{
	//jsonString += '}';
	return '}';
}
//returnJSON structure contains a devicesJSON structure with information about all devices to paint networkMap
function modifyDevice(devicesJSON,callback){
/*
	//global variable allDevices
	var allDev=allDevices;
	var i=0;
	for( ;i<allDev.length;i++){
		if(allDev[i].idDevice==idDev){
			allDev[i].nameDevice=nameDevice;
			allDev[i].idIcon=idIconDevice;
			allDev[i].type=typeDevice;
		}
	}
	var devJSON = $.toJSON(allDev);
	devJSON="{\"RETURN\":{\"success\": true},\"DEVICES\":"+devJSON+"}";
	$.ajax({
	  	url: 'jsps/conf_devices.jsp',
	  	dataType: 'json',
	  	type:'POST',
	  	data: {
			operation: 'modifyDevice',
			JSON: devJSON
		},
		cache: false,
	  	success:function(returnData, status){
					if (returnData.RETURN != undefined && returnData.RETURN.success) {
	 					callback(returnData.RETURN.success,returnData.DEVICES);
	 				}else{
	 					callback(false,"ERROR_GENERAL");
	 				}
	 	},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
	 	}
	 	
	});
*/
    var errorDescription="ERROR_MODIFY_DEVICE";
	//modify one device info
	var objDevice = eval('('+ devicesJSON+ ')'); 
	var devObjPath = ":InternetGatewayDevice.X_TWSZ-COM_Device_list.Customer_Device." + objDevice.DEVICES.idDevice + ".";
	var dataJson = "";
	dataJson = tbsItemBegin("var:page", "*");
	//add webproc needed item
	dataJson += tbsFormatAItem("obj-action", "set");
	dataJson += tbsFormatAItem("var:noredirect", "1");
	//add device name
	dataJson += tbsFormatAItem(devObjPath+'NameDevice', objDevice.DEVICES.nameDevice);
	//add device type
	dataJson += tbsFormatAItem(devObjPath+'Type', objDevice.DEVICES.type);
	//add device icon
	dataJson += tbsFormatAItem(devObjPath+'IdIcon', objDevice.DEVICES.idIcon);
	dataJson += tbsItemEnd("");	
	$.ajax({
		type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :eval('(' +dataJson+')'),
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(returnData.RETURN.success,errorDescription);
                        }
                },
				error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
        });
}

//returnJSON structure contains a devicesJSON structure with information about all devices to paint networkMap
function removeDevice(idDevice,callback){
/*
	//global variable allDevices
	var allDev=allDevices;
	var i=0;
	for( ;i<allDev.length;i++){
		if(allDev[i].idDevice==idDev){
			allDev.splice(i,1);
		}
	}
	var devJSON = $.toJSON(allDev);
	devJSON="{\"RETURN\":{\"success\": true},\"DEVICES\":"+devJSON+"}";
	$.ajax({
	  	url: 'jsps/conf_devices.jsp',
	  	dataType: 'json',
	  	type:'POST',
	  	data: {
			operation: 'deleteDevice',
			JSON: devJSON
		},
		cache: false,
	  	success:function(returnData, status){
					if (returnData.RETURN != undefined && returnData.RETURN.success) {
	 					callback(returnData.RETURN.success,returnData.DEVICES);
	 				}else{
	 					callback(false,"ERROR_GENERAL");
	 				}
	 	},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
	 	}
	 	
	});
*/
    var errorDescription="ERROR_MOVE_DEVICE";
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :
                {
                        "var:page":"*",
                        "del-obj":"InternetGatewayDevice.X_TWSZ-COM_Device_list.Customer_Device."+idDevice+".",
                        "obj-action":"del",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(false,errorDescription);
                        }
                },
		error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
        });	
}
/*-------------------*/
/*		INTERNET   	 */
/*-------------------*/
//returnJSON structure contains an internetJSON structure
var pppPath="";
function getInternetInfo(callback){
/*
	$.ajax({
		url: 'APIS/returnInternetJSON.txt',
		dataType: 'json',
		type:'POST',
		cache: false,
		success: function(returnData, status){
				if(returnData.RETURN.success){
					pppPath = returnData.INTERNET.PPP.pppPath;
					callback(returnData.RETURN.success,returnData.INTERNET);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		}
	);	
*/
	$.ajax({
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnInternetJSON.txt&var:page=returnInternetJSON.txt',
                dataType: 'json',
                cache: false,
                success: function(returnData, status){
                        if(returnData.RETURN.success){
				pppPath = returnData.INTERNET.PPP.pppPath;
				callback(returnData.RETURN.success,returnData.INTERNET);
                        }
                }
        });
}
//returnJSON structure contains a internetJSON structure
function setInternet(internetJSON, callback){
/*
	jQuery.getJSON( 'APIS/returnWifiJSON.txt',
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.INTERNET);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
	);
*/

	var pppInfo = eval('(' + internetJSON + ')');
	var errorDescription="";
	var dataJson = "";
	var pppUsername = ":" + pppPath + "Username";
	var pppPassword = ":" + pppPath + "Password";

	dataJson = tbsItemBegin("var:page", "*");
	dataJson += tbsFormatAItem("obj-action", "set");
	dataJson += tbsFormatAItem("var:noredirect", "1");

	dataJson += tbsFormatAItem(pppUsername, pppInfo.INTERNET.PPP.username);
	dataJson += tbsFormatAItem(pppPassword, pppInfo.INTERNET.PPP.password);

	dataJson += tbsItemEnd("");

	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data : eval('(' +dataJson+')'),
                success:function(returnData, status){
                        if(returnData.RETURN.success){
				callback(returnData.RETURN.success,returnData.INTERNET);
                        }else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}

function get3GInfo(callback){
	$.ajax({
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/return3GJSON.txt&var:page=return3GJSON.txt',
                dataType: 'json',
                cache: false,
                success: function(returnData, status){
                        if(returnData.RETURN.success){
							callback(returnData.RETURN.success,returnData.ThreeG);
                        }
                }
        });
}

function set3G(threeG_JSON, callback){	
	var threeGInfo = eval('(' + threeG_JSON + ')');
	var threeGPath = "InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.";
	var errorDescription="";
	var dataJson = "";
	dataJson = tbsItemBegin("var:page", "*");
	dataJson += tbsFormatAItem("obj-action", "set");
	dataJson += tbsFormatAItem("var:noredirect", "1");
	if(threeGInfo.Enable == "1")
	{
		threeGInfo.Enable = 1;
	}
	else
	{
		threeGInfo.Enable = 0;
	}
	dataJson += tbsFormatAItem(":" + threeGPath + "PinEnable", threeGInfo.Enable);
       dataJson += tbsFormatAItem(":" + threeGPath + "PIN", threeGInfo.pinCode);
	   dataJson += tbsFormatAItem(":" + threeGPath + "PinAction",  "Un_pin");
       dataJson += tbsFormatAItem(":" + threeGPath + "action",  "pin");
       dataJson += tbsFormatAItem("mid",  "0x049a");
       dataJson += tbsFormatAItem("isnew3g",  "1");
       
	dataJson += tbsItemEnd("");
       var errorDescription="ERROR_SET3G";
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data : eval('(' +dataJson+')'),
                success:function(returnData, status){
                        if(returnData.RETURN.success){		
				//callback(returnData.RETURN.success,returnData.INTERNET);
                        }else{
					showWarning(_(errorDescription));
                        }
                }
        });
}

/*-----------------------*/
/*		APPLICATIONS   	 */
/*-----------------------*/
//returnJSON structure contains an applicationsListJSON structure
function listApplications(callback){
/*
		$.ajax({
		  	url: 'APIS/returnApplicationsListJSON.txt',
		  	dataType: 'json',
		  	cache: false,
		  	success: function(returnData, status){
				if (returnData.RETURN.success) {
					callback(returnData.RETURN.success,returnData.APPLICATION_LIST);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		});
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsListJSON.txt",
                data :
                {
                        "var:page":"*",
                        "var:typeApp":"All",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATION_LIST);
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}

//returnJSON structure contains an applicationsListJSON structure
function listFreeApplications(callback){
/*
	jQuery.getJSON( 'APIS/returnApplicationsListJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION_LIST);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsListJSON.txt",
                data :
                {
                        "var:page":"*",
                        "var:typeApp":"Free",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATION_LIST);
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
 
//returnJSON structure contains an applicationsListJSON structure
function listAssignedApplications(idDevice,callback){
/*
	jQuery.getJSON( 'APIS/returnApplicationsListJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION_LIST);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsListJSON.txt",
                data :
                {
                        "var:page":"*",
                        "var:typeApp":"Assigned",
                        "var:idDevice":idDevice,
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATION_LIST);
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
//returnJSON structure contains an applicationJSON structure
function infoApplication(idApplication, callback){
/*
	jQuery.getJSON( 'APIS/returnApplicationJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationJSON.txt",
                data :
                {
                        "var:page":"*",
                        "var:idApplication":idApplication,
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATION_LIST);
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
//returnJSON structure contains an applicationsJSON structure
function infoApplications(callback){
/*
	$.ajax({
	     url: 'APIS/returnApplicationsJSON.txt',
	     dataType: 'json',
	     cache: false,
	     success: function(returnData, status){
	    if (returnData.RETURN.success) {
	     callback(returnData.RETURN.success,returnData.APPLICATIONS);
	    }else{
	     callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
	    }
	   }
	  });
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsJSON.txt",
                data :
                {
                        "var:page":"*",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATIONS);
                        }else{
                                //callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}

function addApplicationRule(idApplication, applicationRuleJSON)
{
	var objRule = applicationRuleJSON;

        $.ajax({
                type: 'POST',
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt&var:page=returnJSON.txt',
                dataType: 'json',
                cache: false,
                data:{
                        "add-obj" : "InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications."+idApplication+".Rules.",
                        "obj-action" : "add-set",
                        ":Protocol" : objRule.protocol,
                        ":StartWan" : objRule.startWan,
                        ":EndWan" : objRule.endWan,
                        ":StartLan" : objRule.startLan,
                        ":EndLan" : objRule.endLan
                },
                success: function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                return true;//add ok
                        }
                        else {
                                return false;//add fail
                        }
                },
		error:function(xmlhttprequest, textStatus,errorThrown){
                        return false; //send post fail
                }
        });
}

//returnJSON structure contains an applicationJSON structure
function newApplication(applicationJSON,callback){
/*
	objectApp.idApplication = Math.random();
	applicationsInfoList[applicationsInfoList.length]=objectApp;
	var applicationsInfoJSON = $.toJSON(applicationsInfoList);
	applicationsInfoJSON="{\"RETURN\":{\"success\": true},\"APPLICATIONS\":"+applicationsInfoJSON +"}";
	$.ajax({
	  	url: 'jsps/applications.jsp',
	  	dataType: 'json',
	  	data: {
			JSON: applicationsInfoJSON
		},
		type:"POST",
		cache: false,
		success: function(returnData, status){
			if (returnData.RETURN != undefined && returnData.RETURN.success) {
				callback(returnData.RETURN.success, returnData.APPLICATIONS);
			}
			else {
				callback(returnData.RETURN.success, returnData.RETURN.errorDescription);
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
		}
	});
*/
        var errorDescription = "ERROR_NEW_APPLICATION";
        
	//add a new application
	var objApp = eval('('+ applicationJSON + ')');

	$.ajax({
		type: 'POST',
		url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationJSON.txt',
		dataType: 'json',
		cache: false,
		data:{
			"var:page" :"*",
			"add-obj" : "InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications.",
			"obj-action" : "add-set",
			":Enable" : "1",
			":NameApplication" : objApp.APPLICATION.nameApplication,
			":IpAssigned" : objApp.APPLICATION.ipAssigned,
			":IdIcon" : objApp.APPLICATION.idIcon
		},
		success: function(returnData, status){
			if(returnData.RETURN != undefined && returnData.RETURN.success){
				//add rules
				for (var i = 0; i  < objApp.APPLICATION.RULES.length; i++){
					if (addApplicationRule(returnData.APPLICATION.idApplication, objApp.APPLICATION.RULES[i]) == false){
						break;
					}
				}

				infoApplications(callback);
			}
			else {
				callback(false, errorDescription); 
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
                       callback(false, errorDescription); 
                }
	});
	
}

//returnJSON structure contains an applicationJSON structure
function newApplicationRule(idApplication,applicationRuleJSON,callback){
/*
	jQuery.getJSON( 'APIS/returnApplicationJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
        var errorDescription = "ERROR_NEW_APPLICATION_RULE";
	var objRule = eval('('+ applicationRuleJSON + ')');

        $.ajax({
                type: 'POST',
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationJSON.txt',
                dataType: 'json',
                cache: false,
                data:{
			"var:page" :"*",
                        "add-obj" : "InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications."+idApplication+".Rules.",
                        "obj-action" : "add-set",
                        ":Enable" : "1",
                        ":Protocol" : objRule.protocol,
                        ":StartWan" : objRule.startWan,
                        ":EndWan" : objRule.endWan,
                        ":StartLan" : objRule.startLan,
                        ":EndLan" : objRule.endLan
                },
                success: function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                //add rules
				callback(returnData.RETURN.success,returnData.APPLICATION);
                        }
                        else {
                                callback(false, errorDescription);
                        }
                },
                error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,errorDescription);
	 	}
        });
}

function deleteApplication(idApplication,callback){	
/*
	var i=0;
	for (; i < applicationsInfoList.length;i++) {
		var app = applicationsInfoList[i];
		if(app.idApplication == idApplicationSelected){
			applicationsInfoList.splice(i,1);
		}
	}
	  
	var applicationsInfoJSON = $.toJSON(applicationsInfoList);
	applicationsInfoJSON="{\"RETURN\":{\"success\": true},\"APPLICATIONS\":"+applicationsInfoJSON +"}";
	$.ajax({
	  	url: 'jsps/applications.jsp',
	  	dataType: 'json',
	  	data: {
			JSON: applicationsInfoJSON
		},
		type:"POST",
		cache: false,
		success: function(returnData, status){
			if (returnData.RETURN != undefined && returnData.RETURN.success) {
				callback(returnData.RETURN.success, returnData.APPLICATIONS);
			}
			else {
				callback(returnData.RETURN.success, returnData.RETURN.errorDescription);
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
		}
	});
*/
        var errorDescription = "ERROR_DEL_APPLICATION";

	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsJSON.txt",
                data :
                {
                        "var:page":"*",
                        "del-obj":"InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications."+idApplication+".",
                        "obj-action":"del",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
                                callback(returnData.RETURN.success, returnData.APPLICATIONS);
                        }else{
                                callback(false,errorDescription);
                        }
                },
                error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
        });
	
}
//returnJSON structure contains an applicationJSON structure
function deleteApplicationRule(idApplication,idRule,callback){
/*
	jQuery.getJSON( 'APIS/returnApplicationJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
        var errorDescription = "ERROR_DEL_APPLICATION_RULE";
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationJSON.txt",
                data :
                {
                        "var:page":"*",
                        "del-obj":"InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications."+idApplication+".Rules." +idRule+".",
                        "obj-action":"del",
			            "var:idApplication":idApplication,
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
				callback(returnData.RETURN.success,returnData.APPLICATION);
                        }else{
                                callback(false,errorDescription);
                        }
                },
                error:function(xmlhttprequest, textStatus,errorThrown){
                        callback(false,errorDescription);
                }
        });
}
 
//returnJSON structure contains an applicationJSON structure
function modifyApplication(applicationJSON,callback){	
/*
	var i=0;
	for (; i < applicationsInfoList.length;i++) {
		var app = applicationsInfoList[i];
		if(app.idApplication == idApplicationSelected){
			applicationsInfoList[i]=objectApp;
		}
	}
	var applicationsInfoJSON = $.toJSON(applicationsInfoList);
	applicationsInfoJSON="{\"RETURN\":{\"success\": true},\"APPLICATIONS\":"+applicationsInfoJSON +"}";
	$.ajax({
	  	url: 'jsps/applications.jsp',
	  	dataType: 'json',
	  	data: {
			JSON: applicationsInfoJSON
		},
		type:"POST",
		cache: false,
		success: function(returnData, status){
			if (returnData.RETURN != undefined && returnData.RETURN.success) {
				callback(returnData.RETURN.success, returnData.APPLICATIONS);
			}
			else {
				callback(returnData.RETURN.success, returnData.RETURN.errorDescription);
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
		}
	});
*/
        var errorDescription = "ERROR_MODIFY_APPLICATION";

	var objApp = eval('('+ applicationJSON + ')');
	var appPath = ":InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications."  + objApp.APPLICATION.idApplication + ".";

	var dataJson = "";

	dataJson = tbsItemBegin("var:page", "*");
	dataJson += tbsFormatAItem("obj-action", "set");
	dataJson += tbsFormatAItem("var:noredirect", "1");

	//application parameter
	dataJson += tbsFormatAItem(appPath + "Enable", "1");
	dataJson += tbsFormatAItem(appPath + "NameApplication", objApp.APPLICATION.nameApplication);
	dataJson += tbsFormatAItem(appPath + "IpAssigned", objApp.APPLICATION.ipAssigned);
	dataJson += tbsFormatAItem(appPath + "IdIcon", objApp.APPLICATION.idIcon);


	//rule parameters
	if (objApp.APPLICATION.RULES.length > 0){
		var len = objApp.APPLICATION.RULES.length;
		var rulesObj = objApp.APPLICATION.RULES;

		for (var i=0; i<len; i++){
			//new rule
			if (rulesObj[i].idRule == "-1"){
				if (addApplicationRule(objApp.APPLICATION.idApplication, rulesObj[i]) == false){
					callback(false, errorDescription);
					return;
				}
				continue;
			}
			var rulePath = ":InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications." + objApp.APPLICATION.idApplication + ".Rules." + rulesObj[i].idRule + ".";
			dataJson += tbsFormatAItem(rulePath + "Protocol", rulesObj[i].protocol);
			dataJson += tbsFormatAItem(rulePath + "StartWan", rulesObj[i].startWan);
			dataJson += tbsFormatAItem(rulePath + "EndWan", rulesObj[i].endWan);
			dataJson += tbsFormatAItem(rulePath + "StartLan", rulesObj[i].startLan);
			dataJson += tbsFormatAItem(rulePath + "EndLan", rulesObj[i].endLan);
		}
	}

	dataJson += tbsItemEnd("");

        $.ajax({
                type: 'POST',
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsJSON.txt',
                dataType: 'json',
                cache: false,
                data:eval('(' +dataJson+')'),
                success: function(returnData, status){
                        if(returnData.RETURN != undefined && returnData.RETURN.success){
				callback(returnData.RETURN.success, returnData.APPLICATIONS);
                        }
                        else {
				callback(returnData.RETURN.success, errorDescription);
                        }
                },
                error:function(xmlhttprequest, textStatus,errorThrown){
                       callback(false, errorDescription);
                }
        });
	  			
}
//returnJSON structure contains an applicationJSON structure
function modifyApplicationRule(applicationRuleJSON,idApplication,callback){
	jQuery.getJSON( 'APIS/returnApplicationJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.APPLICATION);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
}

function modifyListAssignedApplications(idDevice,applicationsListJSON,callback){
/*
	var i=0;
	var j=0;
	for( ;j<allApplications.length;j++){
		var appAll = allApplications[j];
		if(appAll.ipAssigned == ipDevice){
			appAll.ipAssigned="false";
			appAll.idIcon="false";
		}
	}
	var allApp=allApplications;
	for(j=0;j<allApp.length;j++){
		for( i=0;i<appAssignedDev.length;i++){
			if(allApp[j].idApplication == appAssignedDev[i].idApplication){
				allApp[j].ipAssigned=appAssignedDev[i].ipAssigned;
				allApp[j].idIcon=appAssignedDev[i].idIcon;
			}
		}
	}
	var appListJSON = $.toJSON(allApp);
	appListJSON="{\"RETURN\":{\"success\": true},\"APPLICATION_LIST\":"+appListJSON+"}";
	$.ajax({
	  	url: 'jsps/conf_devices.jsp',
	  	dataType: 'json',
	  	data: {
			operation: 'modifyListAssignedApp',
			JSON: appListJSON
		},
		type:'POST',
		cache: false,
	  	success: function(returnData, status){
					if (returnData.RETURN != undefined && returnData.RETURN.success) {
 						callback(returnData.RETURN.success,"");
 					}else{
 						callback(false,"ERROR_GENERAL");
 					}
 		},
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,"ERROR_GENERAL");
 		}
 		
	});
*/

        var errorDescription = "ERROR_ASSIGN_APPLICATIONS";

	//only set device info now, the other to be done
	//callback(true, "");
	var objAPPlist = eval('('+ applicationsListJSON+ ')'); 
	var dataJson = "";
	dataJson = tbsItemBegin("var:page", "*");
	dataJson += tbsFormatAItem("obj-action", "set");
	dataJson += tbsFormatAItem("var:noredirect", "1");
	for(var i = 0,AppObjPath; i < objAPPlist.APPLICATION_LIST.length; i++)
	{
		AppObjPath = ":InternetGatewayDevice.X_TWSZ-COM_Device_list.Applications." + objAPPlist.APPLICATION_LIST[i].idApplication + ".";
	    dataJson += tbsFormatAItem(AppObjPath+'IdApplication', objAPPlist.APPLICATION_LIST[i].idApplication);
	    dataJson += tbsFormatAItem(AppObjPath+'NameApplication', objAPPlist.APPLICATION_LIST[i].nameApplication);
	    dataJson += tbsFormatAItem(AppObjPath+'IpAssigned', objAPPlist.APPLICATION_LIST[i].ipAssigned);
	    dataJson += tbsFormatAItem(AppObjPath+'IdIcon', objAPPlist.APPLICATION_LIST[i].idIcon);
		if(objAPPlist.APPLICATION_LIST[i].ipAssigned != "0")
		{
			dataJson += tbsFormatAItem(AppObjPath+'Enable', "1");
		}else{
			dataJson += tbsFormatAItem(AppObjPath+'Enable', "0");
		}
	}
	dataJson += tbsItemEnd("");
	$.ajax({
		type: 'POST',
            dataType: 'json',
            cache: false,
            url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnApplicationsListJSON.txt",
            data :eval('(' +dataJson+')'),
            success:function(returnData, status){
                if(returnData.RETURN.success){
                    callback(returnData.RETURN.success,"");
                }else{
                    callback(returnData.RETURN.success,errorDescription);
                }
            },
		error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,errorDescription);
 		}
	});		
	//objAPPlist.APPLICATION_LIST[i]

	
}

/*---------------------------*/
/*		 WIFI AND DHCP   	 */
/*---------------------------*/
//returnJSON structure contains a wifiJSON structure
var lanIpPath = "";
function getWifiInfo(callback){
/*
	$.ajax({
		url: 'jsps/wifi_get.jsp',
		dataType: 'json',
		type:'POST',
		cache: false,
		success: function(returnData, status){
			if (returnData.RETURN != undefined && returnData.RETURN.success) {
				callback(returnData.RETURN.success, returnData);
			}
			else {
				callback(returnData.RETURN.success, returnData.RETURN.errorDescription);
			}
		}
	});
*/
	$.ajax({
        	url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnWifiJSON.txt&var:page=returnWifiJSON.txt',
	        dataType: 'json',
        	cache: false,
	        success: function(returnData, status){
        		if(returnData.RETURN.success){
				lanIpPath = returnData.LAN.ipLeafPath;
                		callback(returnData.RETURN.success, returnData);
		        }else{
        		        callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
            		}
        	}
	});
}

//returnJSON structure 
function setWifiConfiguration(wifiJSON, callback){
/*
	$.ajax({
	  	url: 'jsps/wifi_set.jsp',
	  	dataType: 'json',
		type:'POST',
		cache: false,
	  	data: {
			JSON: wifiJSON
		},
	  	success: function(returnData, status){
			if(returnData.RETURN.success){
				callback(returnData.RETURN.success, "");
			}else{
				callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
			}
		},
		error:function(xmlhttprequest, textStatus,errorThrown){
			//something was wrong
			callback(false,"ERROR_GENERAL");
		}
	});
*/
        var errorDescription = "ERROR_SET_WIFI_INFO";

	var confToBeSet = eval('(' + wifiJSON + ')');
	var wifiObjPath = ":InternetGatewayDevice.LANDevice.1.WLANConfiguration.";
	var dhcpObjPath = ":InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.";
        var dataJson = "";
        dataJson = tbsItemBegin("var:page", "*");
        //add webproc needed item
        dataJson += tbsFormatAItem("obj-action", "set");
        dataJson += tbsFormatAItem("var:noredirect", "1");
        //add wifi enable
        dataJson += tbsFormatAItem(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable', confToBeSet.WIFI.status==true?"1":"0");
        //add wifi other setting
	if (confToBeSet.WIFI.status==true)
	{
		dataJson += tbsFormatAItem(wifiObjPath+'1.SSID', confToBeSet.WIFI.ssidName);
		dataJson += tbsFormatAItem(wifiObjPath + '1.SSIDAdvertisementEnabled', confToBeSet.WIFI.ssidVisibility);
		dataJson += tbsFormatAItem(wifiObjPath+'1.Channel', confToBeSet.WIFI.channelMode=="MANUAL"?confToBeSet.WIFI.channel:"0");
		
		//security
		if (confToBeSet.WIFI.SECURITY.cipherAlgorithm == "None")
		{
			dataJson += tbsFormatAItem(wifiObjPath+'1.BeaconType', "Basic");
			dataJson += tbsFormatAItem(wifiObjPath+'1.BasicEncryptionModes', "None");
			dataJson += tbsFormatAItem(wifiObjPath+'1.BasicAuthenticationMode', "None");
		}
		else if (confToBeSet.WIFI.SECURITY.cipherAlgorithm == "WPA2")
		{
			dataJson += tbsFormatAItem(wifiObjPath+'1.BeaconType', "11i");
			dataJson += tbsFormatAItem(wifiObjPath+'1.PreSharedKey.1.KeyPassphrase', confToBeSet.WIFI.SECURITY.passwordWPA2);
			dataJson += tbsFormatAItem(wifiObjPath+'1.IEEE11iAuthenticationMode', "PSKAuthentication");
			dataJson += tbsFormatAItem(wifiObjPath+'1.IEEE11iEncryptionModes', "TKIPandAESEncryption");
			if (confToBeSet.WIFI.SECURITY.passwordWPA2.length < 64)
			{
				dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			}
			else
			{
				dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "PreSharedKey");
			}
		}
		else if (confToBeSet.WIFI.SECURITY.cipherAlgorithm == "AUTO")
		{
			dataJson += tbsFormatAItem(wifiObjPath+'1.BeaconType', "WPAand11i");
			dataJson += tbsFormatAItem(wifiObjPath+'1.PreSharedKey.1.KeyPassphrase', confToBeSet.WIFI.SECURITY.passwordAUTO);
			dataJson += tbsFormatAItem(wifiObjPath+'1.IEEE11iAuthenticationMode', "PSKAuthentication");
			dataJson += tbsFormatAItem(wifiObjPath+'1.IEEE11iEncryptionModes', "TKIPandAESEncryption");
			if (confToBeSet.WIFI.SECURITY.passwordAUTO.length < 64)
			{
				dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			}
			else
			{
				dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "PreSharedKey");
			}
		}	
		else if (confToBeSet.WIFI.SECURITY.cipherAlgorithm == "WPA")
		{
			dataJson += tbsFormatAItem(wifiObjPath+'1.BeaconType', "WPA");
			dataJson += tbsFormatAItem(wifiObjPath+'1.WPAEncryptionModes', "TKIPandAESEncryption");
                        dataJson += tbsFormatAItem(wifiObjPath+'1.PreSharedKey.1.KeyPassphrase', confToBeSet.WIFI.SECURITY.passwordWPA);
                        dataJson += tbsFormatAItem(wifiObjPath+'1.IEEE11iAuthenticationMode', "PSKAuthentication");
			if (confToBeSet.WIFI.SECURITY.passwordWPA.length < 64) 
                        {
                                dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
                        }   
                        else
                        {   
                                dataJson += tbsFormatAItem(wifiObjPath+'1.X_TWSZ-COM_PSKExpression', "PreSharedKey");
                        }
		}
		else if (confToBeSet.WIFI.SECURITY.cipherAlgorithm == "WEP")
		{
			dataJson += tbsFormatAItem(wifiObjPath+'1.BeaconType', "Basic");
			dataJson += tbsFormatAItem(wifiObjPath+'1.BasicEncryptionModes', "WEPEncryption");
			dataJson += tbsFormatAItem(wifiObjPath+'1.WEPKeyIndex', "1");
			dataJson += tbsFormatAItem(wifiObjPath+'1.WEPKey.1.WEPKey', confToBeSet.WIFI.SECURITY.passwordWEP);
			if (confToBeSet.WIFI.SECURITY.algVersion == "128")	
				dataJson += tbsFormatAItem(wifiObjPath+'1.WEPEncryptionLevel', "104-bit");
			else
				dataJson += tbsFormatAItem(wifiObjPath+'1.WEPEncryptionLevel', "40-bit");
		}
	}
	
	dataJson += tbsFormatAItem(':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_ProxyArp', "0");
	//set dhcp setting
	dataJson += tbsFormatAItem(dhcpObjPath+'DHCPServerEnable', confToBeSet.DHCP.status == true ? "1" : "0");
	if (confToBeSet.DHCP.status)
	{
		dataJson += tbsFormatAItem(dhcpObjPath + 'MinAddress', confToBeSet.DHCP.poolStart);
		dataJson += tbsFormatAItem(dhcpObjPath + 'MaxAddress', confToBeSet.DHCP.poolEnd);
	}

	//set lan ip
	if (CONFIG.LanConfEnabled)
	{
		dataJson += tbsFormatAItem(":"+lanIpPath, confToBeSet.LAN.ip);
		if (confToBeSet.DHCP.status){
			dataJson += tbsFormatAItem(dhcpObjPath+"IPRouters", confToBeSet.LAN.ip);
		}
	}

        dataJson += tbsItemEnd("");
        $.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :eval('(' +dataJson+')'),
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,"");
                        }else{
                                callback(returnData.RETURN.success,errorDescription);
                        }
                },
                error:function(xmlhttprequest, textStatus,errorThrown){
			callback(false,errorDescription);
	 	}
        });
}


/*-------------------*/
/*		  USB   	 */
/*-------------------*/
//returnJSON structure contains a usbJSON structure
function getUSBInfo(serialNumber,id,callback){
/*	jQuery.getJSON( 'APIS/returnUsbJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.USB);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
	);   */
	$.ajax({
        	url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnUsbJSON.txt&var:page=returnUsbJSON.txt&var:index='+id,
	        dataType: 'json',
        	cache: false,
	        success: function(returnData, status){
        		if(returnData.RETURN.success){
				var partition;
				for (var i=0; i<returnData.USB.PARTITION.length; i++){
					partition = returnData.USB.PARTITION[i];
					partition.usedSpace = partition.usedSpace*1024;
					partition.freeSpace = partition.freeSpace*1024*1024 - partition.usedSpace;
				}
                		callback(returnData.RETURN.success, returnData.USB);
		        }else{
        		        callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
            		}
        	}
	});


}
/*----------------------------*/
/*		  Welcome Splash  	  */
/*----------------------------*/
//returnJSON structure contains an welcomeInfoJSON structure
function urlWelcomeSplash(callback){
/*
	jQuery.getJSON( '../../APIS/returnWelcomeInfoJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,returnData.URL);
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
	$.ajax({
                url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnWelcomeInfoJSON.txt&var:page=returnWelcomeInfoJSON.txt',
                dataType: 'json',
                cache: false,
                success: function(returnData, status){
                        if(returnData.RETURN.success){
                                callback(returnData.RETURN.success,returnData.URL); 
                        }else{
                                callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
/*-------------------*/
/*		REBOOT   	 */
/*-------------------*/
function reboot(callback){
/*
	jQuery.getJSON( 'APIS/returnJSON.txt', 
			function(returnData, status){
				if(returnData.RETURN.success){
					callback(returnData.RETURN.success,"");
				}else{
					callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
				}
			}
		);
*/
	$.ajax({
                type: 'POST',
                dataType: 'json',
                cache: false,
                url: "/cgi-bin/webproc?getpage=html/gui/APIS/returnJSON.txt",
                data :
                {
                        "var:page":"*",
                        "obj-action":"reboot",
                        "var:noredirect":"1"
                },
                success:function(returnData, status){
                        if(returnData.RETURN.success){
                                //callback(returnData.RETURN.success,"");
                        }else{
                                //callback(returnData.RETURN.success,returnData.RETURN.errorDescription);
                        }
                }
        });
}
