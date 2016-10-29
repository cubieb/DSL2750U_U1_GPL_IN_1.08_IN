var urlBase="";
function errorReady() {
	$('#accept_ierror').click(function(){
		loadImage();
	});
	if(LANG=="es"){
		urlBase="images/tes/";
		$("#error_step img").attr('src',urlBase+"0.png");
	}else{ //English
		urlBase="images/ten/";
		$("#error_step img").attr('src',urlBase+"0.png");
	}
}
function loadImage() {
	var num=$("#error_step img").attr('src');
	var punt=num.indexOf(".png");
	num=num.substr(11,punt-11);
	var i=parseInt(num,10);
	var url=urlBase+'0.png';
	if (i==10) { $("#error_step img").attr('src',url); closeDialog(); }
	i++;
	var str=urlBase;
	//if (i<10) str+='0';
	str+=i.toString();str+='.png';
	$("#error_step img").attr('src',str);
}