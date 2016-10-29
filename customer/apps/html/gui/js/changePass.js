
function checkPasswords(){
	var strength=testPassword($('#newPass').val());
	$('#textErrorPass').addClass("changeWarning")
	$('#textErrorPass').addClass("password_txt1")
	if($('#newPass').val()!=$('#confirmPass').val()){
		//New password and confirmation password do not match
		$('#textErrorPass').html(_("LOGIN_CHANGE_WARN_MATCH"));
		$("#textErrorPass").attr("key","LOGIN_CHANGE_WARN_MATCH");
		emptyForm();
	}else if(strength==0){
		//New password too short
		$('#textErrorPass').html(_("LOGIN_CHANGE_WARN_SHORT"));
		$("#textErrorPass").attr("key","LOGIN_CHANGE_WARN_SHORT");
		emptyForm();
	}else if(strength<3){
		//New password too weak
		$('#textErrorPass').html(_("LOGIN_CHANGE_WARN_WEAK"));
		$("#textErrorPass").attr("key","LOGIN_CHANGE_WARN_WEAK");
		emptyForm();
	}else {
		//send to MHS 
		//changePassword(USER_NAME,$("#currentPass").val(),$("#newPass").val(), responseChangePass);
		$.ajax({
                        url: '/cgi-bin/webproc?getpage=html/gui/APIS/returnSessionJSON.txt&var:page=returnSessionJSON.txt',
                        dataType: 'json',
                        cache: false,
                        success: function(returnData, status){
                                if(returnData.RETURN.success){
                                        changePassword(returnData.RETURN.user, $("#currentPass").val(), $("#newPass").val(), responseChangePass);
                                }
                        }
                });
	}
}
function emptyForm(){
	$("#currentPass").val("");
	$("#newPass").val("");
	$("#confirmPass").val("");
	$("#textSecurity").html(_("COMMON_EMPTY"));
	$("#textSecurity").attr("key","COMMON_EMPTY");
	$(".dialog_security_img").css({'background-position': '0 0px'}); 
}
function paintChangePassPage(success,password){
	if(success){
		if(password.isDefault){
			//texts for default password
			$('#textChangePass').html(_("LOGIN_CHANGE_TEXT_1"));
			$('#textChangePass').attr("key","LOGIN_CHANGE_TEXT_1");
			$('.imgStiker').css('display','block');
		}
	}
}
function checkpassword() {
	var strength=testPassword($('#newPass').val());
	
	if(strength==0){
		if($('#newPass').val()==0){
			$("#textSecurity").html(_("COMMON_EMPTY"));
			$("#textSecurity").attr("key","COMMON_EMPTY");
			$(".dialog_security_img").css({'background-position': '0 0px'}); 
		}else{
			$(".dialog_security_img").css({'background-position': '0 -10px'}); 
			$("#textSecurity").html(_("LOGIN_CHANGE_TOO_SHORT"));
			$("#textSecurity").attr("key","LOGIN_CHANGE_TOO_SHORT");
		}
	}else if(strength==1){
		$(".dialog_security_img").css({'background-position': '0 -10px'}); 
		$("#textSecurity").html(_("LOGIN_CHANGE_VERY_WEAK"));
		$("#textSecurity").attr("key","LOGIN_CHANGE_VERY_WEAK");
	}else if(strength==2){
		$(".dialog_security_img").css({'background-position': '0 -20px'});
		$("#textSecurity").html(_("LOGIN_CHANGE_WEAK"));
		$("#textSecurity").attr("key","LOGIN_CHANGE_WEAK");
	}else if(strength==3){
		$(".dialog_security_img").css({'background-position': '0 -30px'});
		$("#textSecurity").html(_("LOGIN_CHANGE_GOOD"));
		$("#textSecurity").attr("key","LOGIN_CHANGE_GOOD");
	}else if(strength==4){
		$(".dialog_security_img").css({'background-position': '0 -40px'});
		$("#textSecurity").html(_("LOGIN_CHANGE_STRONG"));
		$("#textSecurity").attr("key","LOGIN_CHANGE_STRONG");
	}else if(strength==5){
		$(".dialog_security_img").css({'background-position': '0 -50px'});
		$("#textSecurity").html(_("LOGIN_CHANGE_VERY_STRONG"));
		$("#textSecurity").attr("key","LOGIN_CHANGE_VERY_STRONG");
	}
}

function testPassword(passwd){
		var intScore   = 0
		if(passwd.length<8)
			return 0;
		
		if (passwd.length<5)                                   // length 4 or less
		{
			intScore = (intScore+2)
		}else if (passwd.length>4 && passwd.length<8)           // length between 5 and 7
		{
			intScore = (intScore+4)
		}else if (passwd.length>7 && passwd.length<16)          // length between 8 and 15
		{
			intScore = (intScore+12)
		}else if (passwd.length>15)                    // length 16 or more
		{
			intScore = (intScore+18)
		}
		if (passwd.match(/[a-z]/))                             // at least one lower case letter
		{
			intScore = (intScore+1)
		}
		if (passwd.match(/[A-Z]/))                             // at least one upper case letter
		{
			intScore = (intScore+5)
		}
		
		if (passwd.match(/[0-9]/))                               // at least one number
		{
			intScore = (intScore+5)
		}
		
		if (passwd.match(/(.*[0-9].*[0-9].*[0-9])/))           // at least three numbers
		{
			intScore = (intScore+5)
		}
		
		if (passwd.match(/[!,¡,@,#,$,%,&,*,?,¿,+,<,>,:,;,_,~]/))                              // at least one special character
		{
			intScore = (intScore+5)
		}
		if (passwd.match(/(.*[!,¡,@,#,$,%,^,&,*,?,¿,+,<,>,;,:,_,~].*[!,¡,@,#,$,%,^,&,*,?,¿,+,<,>,;,:,_,~])/))
			// at least two special characters
		{
			intScore = (intScore+5)
		}
	
		// both upper and lower case letters
		if (passwd.match(/([a-z])/) && passwd.match(/([A-Z])/))
		{
			intScore = (intScore+2)
		}

		// both letters and numbers letters
		if (passwd.match(/([a-zA-Z])/) && passwd.match(/([0-9])/)) 
		{
			intScore = (intScore+2)
		}
 									
		// letters, numbers, and special characters
		if (passwd.match(/([a-zA-Z])/) && passwd.match(/([0-9])/) && passwd.match(/.[!,¡,@,#,$,%,^,&,*,?,¿,+,<,>,;,:,_,~]/))	
		{
			intScore = (intScore+3)
		}
	
		if(intScore < 16){
			return 1;
		}else if (intScore > 15 && intScore < 25){
			return 2;
		}else if(intScore > 24 && intScore < 35) {
			return 3;
		}else if(intScore > 34 && intScore < 45){
			return 4;
		}else{
			return 5;
		}
}
function responseChangePass(success,returnChange){
	if(success){
		$('#dialog').dialog("close");
	}else{
		showWarning(_(returnChange));
	}
	
}
