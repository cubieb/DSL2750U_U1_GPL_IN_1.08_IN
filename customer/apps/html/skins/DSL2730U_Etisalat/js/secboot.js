/*+-----------------------------------------+
  | FileName : Javascript Common Controller |
  | Edtion   : To Trunk R2                  |
  | Author   : TBS Software                 |
  +-----------------------------------------+*/
var ArrayListenners = []; // 加载事件收集器
var Collector = []; //表单数据收集器
var timeout;
var FirthContentHeigh;

/*----  根据不同分辨率改变css样式  ----*/
(function (){
	/*
	var Link = document.getElementsByTagName('link');
	
	if(screen.width == 800 && screen.height == 600){
		Link[0].href = '/html/skin/common.css';
	} else if(screen.width >= 1024 && screen.width < 1280){
		Link[0].href = '/html/skin/common.css';
	}
	*/
})();

//没有，则添加；有，则删除
var $id = getOrDelArrayIdx;
var _array_id = [];

function getOrDelArrayIdx(_value){
	if(!_array_id.remove(_value)){
		_array_id.push(_value);
	}
}

/*----- 定义错误提示样式的函数 ------*/
var DealWith = {
	Wrong: function(node,message){
		try{
			var _node  = $(node), _parent = _node.parentNode;
		} catch(e) {
			alert(e + ' -> DealWith.Wrong');
			return false;
		}
		var _error;
		if((_error = $('errorMessage')) == null){
			_error    = document.createElement("span");
			_error.id = 'errorMessage';
			_error.style.color = '#FF9122';
		}
		_error.innerHTML = "<img src='/html/skin/ErrorMark.gif' alt='error'> " + message;
		_parent.appendChild(_error);
		
		if (!_node.disabled){
			_node.focus();
		}
	},
	Right: function(node){
		var _node = $('errorMessage');
		if(_node != null){
			_node.innerHTML = "&nbsp;";
		}
	}
}

/*------- 对错误码的处理 -------*/
//错误码的格式:节点名=错误码
function dealErrorMsg(arrayNode, errorMsg, isLongNode){
	//恢复出错前的页面数据
	ViewState.Load(G_ViewState);
	
	if (typeof errorMsg != 'string') return;
	//将错误信息一分为二：前段寻找错误码对应的节点；后段寻找错误码对应的提示；
	var errorFragment = errorMsg.split('=');
	var errorCode, errorString, leafNode;
	//寻找错误码对应的节点;
	if (errorFragment != null && errorFragment[0].indexOf('.') > -1) {
		var path = errorFragment[0];
		//更精确的路径对应方式,速度比较慢
		if( isLongNode != undefined) {
			for(var node in arrayNode){
				var nodeIndex = path.lastIndexOf(node);
				if(nodeIndex > -1 && (nodeIndex + node.length) == path.length) {
					leafNode = arrayNode[node];
					break;
				}
			}
		}
		//快速处理方式:速度比较快
		else {
			var _fragment = path.split('.');
			var node = _fragment[_fragment.length - 1];
			leafNode = arrayNode[node];
		}
	}

	//在获取错误码后，立即寻找相应的错误码。
	//寻找的方式是：一、从UEcode里开始找，如果找到，则不再往下找；二、如果没有找到，则从SEcode里找;
	if (errorFragment[1] != null){
		if (errorFragment[1].length < 9)
			errorCode = errorFragment[1].match(/^[0-9]{4}/)[0];
		else
			errorCode = errorFragment[1].match(/^[0-9]{5}/)[0];
	} else {
		if (errorMsg.length < 9)
			errorCode = errorMsg.match(/^[0-9]{4}/)[0];
		else
			errorCode = errorMsg.match(/^[0-9]{5}/)[0];
	}
	
	if(UEcode[errorCode] !== undefined){
		errorString = UEcode[errorCode];
	} else if (SEcode[errorCode] !== undefined ) {
		errorString = SEcode[errorCode];
	} else {
		errorString = SEcode[1000];
	}
	
	//选择两种不同的错误提示方式：1、将错误准确打印在对应的输入框之后；2、对于未知错误，以告警框来提示；
	if (leafNode != undefined){
		DealWith.Wrong(leafNode,errorString);// <- 1
	} else {
		alert(errorString + "!");// <- 2
	}
}


/* Get Current Page Url */
function uiGetPageUrl()
{
    var strUrl = '/cgi-bin/webproc?getpage=html/index.html'
        + '&var:menu=' + G_Menu 
        + '&var:page=' + G_Page;
    if(G_SubPage != '0')
        strUrl += '&var:subpage=' + G_SubPage;
    if(G_ErrorPage != '0')
        strUrl += '&var:errorpage=' + G_ErrorPage;
    
    return strUrl;
}

/* Refresh Current Page */
function uiPageRefresh(){
    document.location.href = uiGetPageUrl();
}

/*---  检查Css样式表 ---*/
var Check = {
	xHeight: function(){
		var _node = $('Menu','Content');
		for(var i = 0; i < _node.length; i++){
			_node[i].style.height  = arguments[0] + 'px';
		}
	}
}

/*---- 检查参数合法性的函数 -----*/
var CheckValidity = {
	toAll: function(Node,Msg,Pattern){
		var PatternValue,Errormsg,GetValue = $(Node).value;
		if(GetValue == ""){
			Errormsg = Msg + SEcode.empty;
			DealWith.Wrong(Node,Errormsg);
			return false;			
		}
		
		PatternValue = GetValue.match(Pattern);
		if(PatternValue == null){
			Errormsg = Msg + SEcode.vilidity;
			DealWith.Wrong(Node,Errormsg);
			return false;
		}
		DealWith.Right(Node);
		return GetValue;
	},
	IP: function(Node,Msg){
		var A_minIP = 16777217,   A_MaxIP = 2130706430;
		var B_minIP = 2147483649, B_MaxIP = 3221225470;
		var C_minIP = 3221225473, C_MaxIP = 3758096382;
		var D_minIP = 3758096384, D_MaxIP = 4026531839;

		var Pattern = /^(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$/;
		var VALUE_IP = $(Node).value;
		var Message;
		
		if (VALUE_IP == '' && arguments[2] != undefined && arguments[2] == false){ //这里是针对某些特殊情况进行处理
			return true;
		} else if (arguments[2] != undefined && String(arguments[2]).indexOf(VALUE_IP) > -1){
			return VALUE_IP;
		} else if (this.toAll(Node,Msg,Pattern) == false){
			return false;
		}
		
		var varIP = VALUE_IP.split('.');
		value = (Number(varIP[0])*16777216) + (Number(varIP[1])<<16) + (Number(varIP[2])<<8) + (Number(varIP[3]));

		if ((value >= A_minIP) && (value <= A_MaxIP)){ //这里是针对普通的IP规则进行处理
			return VALUE_IP;
		} else if ((value >= B_minIP) && (value <= B_MaxIP)){
			return VALUE_IP;
		} else if ((value >= C_minIP) && (value <= C_MaxIP)){
			return VALUE_IP;
		}

		Message = Msg + SEcode.vilidity;
		DealWith.Wrong(Node,Message);
		return false;
	},
	Mask: function(Node,Msg){
		var Pattern      = /^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$/;
		var PATTERN_Mask = /^1+0+$/;
		var value_mask   = this.toAll(Node,Msg,Pattern);
		var mask,Result;
		
		if(value_mask == false){ // 这里处理子网掩码的特殊情况
			return false;
		} else if(arguments[2] != undefined && String(arguments[2]).indexOf(value_mask) > -1){
			return value_mask;
		}
		
		
		mask   = value_mask.split("."); //这里处理子网掩码的一般情况
		Result = Number(mask[0])*16777216 + Number(mask[1])*65536 + Number(mask[2])*256 + Number(mask[3]);
		mask   = Result.toString(2);
		Result = mask.match(PATTERN_Mask);
		
		if((Result == null) || (mask.length != 32)){
			var Errorsg = Msg + SEcode.vilidity;
			DealWith.Wrong(Node,Errorsg);
			return false;
		}
		return value_mask;
	},
	isNetIP: function(IP,Mask){ // if use this function,use the function of IP and Mask at first;true stand for notNetIP,false stand for isNetIP.
		if(IP == false || Mask == false) return false;
		
		var array_ip   = IP.split('.');
		var array_mask = Mask.split('.');
		if (!((Number(array_ip[0]) & ~Number(array_mask[0]))
		 || (Number(array_ip[1])  & ~Number(array_mask[1]))
		 || (Number(array_ip[2])  & ~Number(array_mask[2]))
		 || (Number(array_ip[3])  & ~Number(array_mask[3]))))
		 return false;

		return true;
	},
	isBroastIP: function(IP,Mask){ // if use this function,use the function of IP and Mask at first;true stand for notBroastIP,false stand for isBroastIP.
		if(IP == false || Mask == false) return false;

		var array_ip   = IP.split('.');
		var array_mask = Mask.split('.');
		//IP与掩码的反码求与，得到IP的主机位
		var resultAnd = [];
		resultAnd[0] = Number(array_ip[0]) & ~(Number(array_mask[0]));
		resultAnd[1] = Number(array_ip[1]) & ~(Number(array_mask[1]));
		resultAnd[2] = Number(array_ip[2]) & ~(Number(array_mask[2]));
		resultAnd[3] = Number(array_ip[3]) & ~(Number(array_mask[3]));
		
		//把主机位与掩码求异或，二进制结果应该是32个1
		var resultXor = [];
		resultXor[0] = resultAnd[0] ^ (Number(array_mask[0]));
		resultXor[1] = resultAnd[1] ^ (Number(array_mask[1]));
		resultXor[2] = resultAnd[2] ^ (Number(array_mask[2]));
		resultXor[3] = resultAnd[3] ^ (Number(array_mask[3]));
		
		var varMask = (resultXor[0]*16777216 + (resultXor[1]<<16) + (resultXor[2]<<8) + resultXor[3]).toString(2);
		var PATTERN_MASK = /^1{32}$/;
		if ((varMask.match(PATTERN_MASK) != null)) return false;
		
		return true;
	},
	MAC: function(Node,Msg){
		var Pattern   = /^([0-9a-fA-F]){2}:([0-9a-fA-F]){2}:([0-9a-fA-F]){2}:([0-9a-fA-F]){2}:([0-9a-fA-F]){2}:([0-9a-fA-F]){2}$/;
		var value_mac = this.toAll(Node,Msg,Pattern);
		return value_mac;
	},
	//删除了多余的mask地址检查于2008-07-14.
	DomainName: function(Node,Msg){
		var Pattern      = /^[_0-9a-zA-Z][-_.@0-9a-zA-Z]+$/;
		var value_domain = this.toAll(Node,Msg,Pattern);
		return value_domain;
	},
	Letter: function(Node,Msg){
		var Pattern      = /^[_0-9a-zA-Z][-_.@0-9a-zA-Z]*$/;
		var value_letter = this.toAll(Node,Msg,Pattern); 
		return value_letter;
	},
	Keys: function(Node,Msg){
		var Pattern      = /^[-_.@0-9a-zA-Z]{8,64}$/;
		var value_letter = this.toAll(Node,Msg,Pattern); 
		return value_letter;
	},
	Time: function(Node,Msg){
		var Pattern    = /^\d{1,}$|^-1$/;
		var value_time = this.toAll(Node,Msg,Pattern);
		
		if(value_time === false) return false;
		if(((Number(value_time) < 60) && (Number(value_time) != -1)) || (Number(value_time) > 2147483647)){
			var Errorsg = Msg + "取值范围:60~2147483647";
			DealWith.Wrong(Node,Errorsg);
			return false;
		}
		return value_time;
	},
	Port: function(Node,Msg){
		var Pattern    = /^\d{1,5}$/;
		var value_port = this.toAll(Node,Msg,Pattern);
		
		if($(Node).value === '0' && arguments[2] != undefined && arguments[2] == false){
			return '0';
		} else if (value_port === false){
			return false;
		}
		
		
		if((Number(value_port) > 65535) || (Number(value_port) < 1)){
			var Errormsg = Msg + SEcode.vilidity;
			DealWith.Wrong(Node,Errormsg);
			return false;			
		}
		return value_port;
	},
	ACSII: function(Node,Msg){
		var value_acsii = $(Node).value;
		var startChar = ' ', endChar = '~';
		var Errormsg = Msg + SEcode.vilidity;
		
		if((value_acsii.length < 8) || (value_acsii.length > 64)){
			DealWith.Wrong(Node,Errormsg);
			return false;			
		}
		
		for (var i = 0; i < value_acsii.length; i++)
		{
			var tmpChar = value_acsii.charAt(i);
			if((tmpChar <= startChar) || (tmpChar > endChar))
			{
				DealWith.Wrong(Node,Errormsg);
				return false;
			}
		}
		DealWith.Right(Node);
		return value_acsii;
	}
}


/*---------   顶层菜单、子菜单的加载函数  --------*/
function TopSelfMenu(){
	//替换主菜单
	//Heigtduiqi;
	for(var node in top_menu){
		if($(node) != null)
			$(node).innerHTML = top_menu[node];
	}

	//替换子菜单
	for(var node in sub_menu){
		if($(node) != null) $(node).innerHTML = sub_menu[node];
	}

	//替换页面文字
	chg_language(data_language);

	//语言替换完后再显示页面
	
	document.body.style.visibility = 'visible';
}

addListeners(TopSelfMenu);
/*-------- Base64编码/解码 -------*/
var Base64 ={
      keyStr : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",

      //将Ansi编码的字符串进行Base64编码
      Encode : function(input){
		var output = "";
		var chr1, chr2, chr3 = "";
		var enc1, enc2, enc3, enc4 = "";
		var i = 0;
		do {
			chr1 = input.charCodeAt(i++);
			chr2 = input.charCodeAt(i++);
			chr3 = input.charCodeAt(i++);
			
			enc1 = chr1 >> 2;
			enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
			enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
			enc4 = chr3 & 63;
			
			if (isNaN(chr2))
				enc3 = enc4 = 64;
			else if (isNaN(chr3)) 
				enc4 = 64;			
			
			output = output + 
			this.keyStr.charAt(enc1) + 
			this.keyStr.charAt(enc2) + 
			this.keyStr.charAt(enc3) + 
			this.keyStr.charAt(enc4);
			chr1 = chr2 = chr3 = "";
			enc1 = enc2 = enc3 = enc4 = "";
		} while (i < input.length);
		
		return output;
      },

      //将Base64编码字符串转换成Ansi编码的字符串
      Decode : function(input){
		var output = "";
		var chr1, chr2, chr3 = "";
		var enc1, enc2, enc3, enc4 = "";
		var i = 0;
		if(input.length%4 != 0)
			return "";
			
		var base64test = /[^A-Za-z0-9\+\/\=]/g;
		if (base64test.exec(input))
			return "";
		
		do 
		{
			enc1 = this.keyStr.indexOf(input.charAt(i++));
			enc2 = this.keyStr.indexOf(input.charAt(i++));
			enc3 = this.keyStr.indexOf(input.charAt(i++));
			enc4 = this.keyStr.indexOf(input.charAt(i++));
			
			chr1 = (enc1 << 2) | (enc2 >> 4);
			chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
			chr3 = ((enc3 & 3) << 6) | enc4;
			
			output = output + String.fromCharCode(chr1);
			
			if (enc3 != 64) 
				output+=String.fromCharCode(chr2);
			if (enc4 != 64) 
				output+=String.fromCharCode(chr3);
			
			chr1 = chr2 = chr3 = "";
			enc1 = enc2 = enc3 = enc4 = "";
		} while (i < input.length);
		
		return output;
      }
}

/*
 *   保存/加载页面状态
 */
var ViewState = {
	Save : function()
	{
		var _NodeStates = [];
	
		//<select>
		var _Nodes = document.getElementsByTagName("SELECT");
		for(var i = 0; i < _Nodes.length; i++){
			_NodeStates.push(_Nodes[i].id + "=" + escape(_Nodes[i].value));
		}
		//<input>
		var _Nodes = document.getElementsByTagName("INPUT");
		for(var i = 0; i < _Nodes.length; i++){
			if(_Nodes[i].type == "text")
				_NodeStates.push(_Nodes[i].id + "=" + escape(_Nodes[i].value));
			else if( _Nodes[i].type == "checkbox" || _Nodes[i].type == "radio" )
				_NodeStates.push(_Nodes[i].id + "=" + escape(_Nodes[i].checked));
		}
		//<textarea>
		var _Nodes = document.getElementsByTagName("TEXTAREA");
		for(var i = 0; i < _Nodes.length; i++){
			_NodeStates.push(_Nodes[i].id + "=" + escape(_Nodes[i].value));
		}

		return Base64.Encode(_NodeStates.join("|"));
	},
	Load : function(s)
	{
		if(s == null || s == "" || s == "-")
			return false;
		
		var _NodeStates = Base64.Decode(s).split("|");
		for(var i = 0; i < _NodeStates.length; i++)
		{
			var _NodePair = _NodeStates[i].split("=");
			if(_NodePair.length == 2)
			{
				var _Node = document.getElementById(_NodePair[0]);
				if(_Node != null)
				{
					//节点值是否变化
					var changed = false;
					
					//<select>,<textarea>
					if(_Node.nodeName == "SELECT" || _Node.nodeName == "TEXTAREA")
					{
						var newValue = unescape(_NodePair[1]);
						if(_Node.value.toString() != newValue)
						{
							_Node.value = newValue;
							changed = true;
						}
						
						//事件处理
						if(changed && _Node.onchange != null)
							_Node.onchange();
					}
					
					//<input>
					else if(_Node.nodeName  == "INPUT")
					{
						var newValue = unescape(_NodePair[1]);
						if(_Node.type == "checkbox" || _Node.type == "radio")
						{
							if(_Node.checked.toString() != newValue)
							{
								_Node.checked = newValue.parseBoolean();
								changed = true;
							}
						}
						else
						{
							if(_Node.value.toString() != newValue)
							{
								_Node.value = newValue;
								changed = true;
							}
						}
						
						//事件处理
						if(changed && _Node.onchange != null)
							_Node.onchange();
						if(changed && _Node.onclick != null)
							_Node.onclick();
					}
				}
			}
		}
		return true;
	}
}

/* 帮助相关操作函数 */
var Help = {
    _HelpUrl  : '/html/help/help.html#',
    _HelpPage : null,
    
    Go : function(helpKey){
    	var lang = Cookie.Get('language');
	
		this._HelpPage = window.open('/html/help/help_'+ lang +'.html#' + helpKey,'tbs_help', '');
        this._HelpPage.focus();
    }
}

/* 重定向到指定页面 */
function uiOpenRequstUrl(urlBase, urlParaObj){
	var strUrl = "";
	
	if(typeof urlParaObj == "string"){
	    strUrl = urlParaObj;
	} else if (typeof urlParaObj == "object"){
	    var arrParams=new Array();
		for(var keyName in urlParaObj){
		    if(typeof keyName == 'string'){
    			var strParam = keyName;
    			strParam += "=";
    			strParam += urlParaObj[keyName];
    			arrParams.push(strParam);
    		}
		}
		strUrl = arrParams.join("&");
	}
	
	if(strUrl != "")
	    document.location.href = urlBase + "?" + strUrl;   
}
var $G = uiOpenRequstUrl;

/*---   根据用户级别来判断界面是否可写   ----*/
var spareFunc; // 备用函数，可以普通用户打开某些页面
function ctrlWebEditable(){//暂时分为两级:admin/guest
	var Tagged; //被打上标记的标签
	
	if(userLevel == '1'){// <- admin
	} else if(userLevel == '2'){ // -> guest
		Tagged = document.getElementsByClassName('SecLevel');

		for(var i = 0,_len = Tagged.length; i < _len; i++){
			Tagged[i].style.display = 'none';
		}
		//备用处理函数
		if(spareFunc){ spareFunc();}
	} else if(userLevel == '3'){ // -> support
		Tagged = document.getElementsByClassName('ThdLevel');

		for(var i = 0,_len = Tagged.length; i < _len; i++){
			Tagged[i].style.display = 'none';
		}
		//备用处理函数
		if(spareFunc){ spareFunc();}
		
	}
}

//使node=id的节点文字和input与select属性的控件:灰显(bool=false)/回显(bool=true)
function disCtrl(id, bool){
	var element = $(id);
	var _inut    = element.getElementsByTagName('INPUT');
	var _select  = element.getElementsByTagName('SELECT');
	
	element.style.color = bool ? '#000000' : '#AAAAAA';
	for(var i = 0, _len = _inut.length; i < _len; i++){
		_inut[i].disabled = !bool;
	}
	
	for(var j = 0, _len = _select.length; j < _len; j++){
		_select[j].disabled = !bool;
	}
}


/*
* 子菜单处理
*/
/* 关闭/打开子菜单 */
function ctrlSubMenu(mode){
	
	var _up = document.getElementsByClassName(mode);//获取状态'up'对应的className
	var _switch = document.getElementsByClassName('SubMenuSwitch'); //获取是否存在树形菜单
	
	//形成树形菜单
	var _tag;
	for(var j = 0; j < _switch.length; j++){
		if(_switch[j].className.indexOf(mode) == -1){
			_tag = _switch[j].className.split(' ')[1];
			
			var _tagged =  document.getElementsByClassName(_tag);
			if(_tagged != undefined){
				for(var k = 1; k < _tagged.length; k++){
					_tagged[k].style.display = 'none';
				}
				//检查IMG是否存在
				if(_tagged[0].getElementsByTagName('img')[0] != undefined){
					//_tagged[0].getElementsByTagName('img')[0].src = '/html/skin/open.gif'; // 显示open图标
				}
			}
			
		}
	}
	//如果不存在对应的className
	if(mode == '-') return false;
	for(var i = 1; i < _up.length; i++){
		_up[i].style.display = 'block';
	}
	//检查IMG是否存在
	//if(_up[0].getElementsByTagName('img')[0] != undefined){
	//	_up[0].getElementsByTagName('img')[0].src = '/html/skin/closed.gif'; // 显示closed图标
	//}
	
}

/* 初始化菜单 */
function initSubMenu(){
	var up = $('up');
	var thirdup = $('thirdup');
	if(up==null&&thirdup==null)
	{
		return;
	}
	if(up==null&&thirdup!=null)
	{
	   ctrlSubMenu(thirdup.className || '-');
	}
	else
	{
		var _split = up.className.split(' ');
	   
		for(var i = 0; i < _split.length; i++){
			if(_split[i] != 'SecLevel'){
				ctrlSubMenu(_split[i] || '-');
			}	   
		}
	   
	}
	
}

/*---------   加载事件监视器 ---------*/
function addListeners(){  // 传入函数作为函数并执行
	for(var i = 0; i < arguments.length; i++){
		ArrayListenners.push(arguments[i]);
	}	
}


/*页面注销*/
function uiLogout(){
	var form_name = 'uiPostForm';
	
	if(document.forms[0]){
	} else {
		form_name = 'uiLogoutForm';
		$('loading').innerHTML = '<form id="uiLogoutForm" method="post" action="webproc"></form>';
	}
	
	$H({
		'getpage'   :'html/main.html',
		':sessionid':arguments[0],
		'obj-action':'auth',
		':action'   :'logout'
	},true);
	$(form_name).submit();
}

function Heigtduiqi(){
	

}

/* 页面初始化 */

function removeListeners(func){

         for(var i = 0; i < ArrayListenners.length; i++){

                   if(typeof func == 'function' && ArrayListenners[i] == func){

                            ArrayListenners.remove(func);
							

                   }

         }
}

//addListeners(TopSelfMenu);
addListeners(initSubMenu);

window.onload = function (){
	for(var i = 0; i < ArrayListenners.length; i++){
		var func = ArrayListenners[i];
		if(typeof func == 'function'){
			func();
		}
	}
	ctrlWebEditable();
	//语言替换完后再显示页面
	document.body.style.visibility = 'visible';
}

//获取浏览器的类型及版本型号
function getBrowserName(){
	var type    = navigator.appName;
	var version = navigator.appVersion;
	
	if(type == 'Microsoft Internet Explorer'){
		return version.match(/\bMSIE\s\d+/)[0];
	} else if(type == 'Netscape'){
		return 'Netscape' + version.substr(0,1);
	} else if(type == 'Opera'){
		return 'Opera' + version.substr(0,1);



	}
}

//获取node左偏移的距离(offsetLeft)兼容各种浏览器
function getOffsetLeft(node){
    var left = 0;
    var offsetParent = node;
	var browser = getBrowserName();

	//IE的左偏移是针对上一级的父对象,因此需要循环到BODY；而FF/OPERA/CHORME/SAFA是针对BODY
    while (offsetParent != null && offsetParent != document.body){
        left += offsetParent.offsetLeft;
        offsetParent = offsetParent.offsetParent;
    }
	//对于BODY自动居中，IE8.0以下的浏览器无法计算其偏移的距离
	//因此设计思路是，（屏幕的实际宽度 - BODY的宽度）/2，即可以得到左偏移距离
	if(browser.indexOf('IE') > -1 && Number(browser.split(' ')[1]) < 8){
		//屏幕实际的宽度
		var screenWidth = document.documentElement.clientWidth;
		//BODY的实际宽度
		var bodyWidth   = document.body.clientWidth;
		
		if(screenWidth > bodyWidth){
			left += (screenWidth - bodyWidth)/2;
		}
	}
	
	return left;
}

//获取node上偏移的距离(offsetTop)兼容各种浏览器
function getOffsetTop(node){
    var top = 0;
    var offsetParent = node;

	//IE的左偏移是针对上一级的父对象,因此需要循环到BODY；而FF/OPERA/CHORME/SAFA是针对BODY
    while (offsetParent != null && offsetParent != document.body){
        top += offsetParent.offsetTop;
        offsetParent = offsetParent.offsetParent;
    }
	
	return top;
}

//寻找下一个节点(排除空白节点)
function nextSibling(node){
	var nextNode = node.nextSibling;
	
	while(nextNode.nodeType == 3){
		nextNode = nextNode.nextSibling;
	}
	
	return nextNode;
}

//开始菜单的实现方式，取决于onShowMenu函数
function onShowMenu(node){
	//如果先前存在定时器,则先清除
	onClearTimeOut();
	var old_ul = searchUl();
	var parentNode = node.parentNode;
	var ul = nextSibling(parentNode);
	
	var p_width = 131; //二级菜单P标签的宽度
	var left = getOffsetLeft(parentNode);
	var top  = getOffsetTop(parentNode);
	
	if(old_ul){//清除旧的菜单
		old_ul.style.display = 'none';
	}
	ul.style.cssText = 'display:block; position:absolute; background:red; top:' + top + 'px; left:' + (left + p_width) + 'px;';
}

function onCloseMenu(){
	timeout = setTimeout('displayUl();', 300);
}

function searchUl(){
	var node_ul = document.getElementsByClassName('thirdMenu');

	for(var i = 0, _len = node_ul.length; i < _len; i++){
		if(node_ul[i] != null && node_ul[i].style.display == 'block'){
			return node_ul[i];
		}
	}
	
	return null;
}

function displayUl(){
	var ul = searchUl();
	ul.style.display = 'none';
}

function onClearTimeOut(){
	if(timeout){
		clearTimeout(timeout);
	}
}

function onHiddenMenu(e){
	var browser_type = getBrowserName();
	var menuobj = searchUl();
	
	//对事件冒泡的情况进行处理
	if (browser_type.indexOf('IE') > -1 && !menuobj.contains(e.toElement)){ //IE
		menuobj.style.display = 'none';
	}
	else if (e.currentTarget != e.relatedTarget && !contains_ns6(e.currentTarget, e.relatedTarget)){ //FF/OPERA/SAFA
		menuobj.style.display = 'none';
	}
}

function contains_ns6(a, b) {
	while(b.parentNode){
		if ((b = b.parentNode) == a){
			return true;
		}
	}
	
	return false;
}

//Following codes for wizard 
function addJsonObj(mode_id, obj){
	var __data = Cookie.Get('cache_wiz');
	var _index = __data.indexOf(mode_id);
	var _len = __data.length; //字符串总长
	//寻找断点
	var _start = _index + 4;
	_index = __data.indexOf('}',_start);
	var _end   = _index + 1;
	//合成新的数据
	var new_data = __data.substring(0,_start) + obj + __data.substring(_end,_len);
	
	Cookie.Set('cache_wiz',new_data);
}

function getJsonObj(mode_id){
	var __data = Cookie.Get('cache_wiz');
	var _json  = eval('(' + __data + ')');
	
	return _json[mode_id];
}
