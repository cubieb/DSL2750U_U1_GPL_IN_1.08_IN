//wizard -> nsp

var G_WanConns = [];
var G_WanConns_num = 0;

<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
   `<?if gt $12 0
	 `	 <?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name Username"
		 `	  G_WanConns[G_WanConns_num] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
							   "$01",      //name
							   "$02",      //Username
							   ];
							   ++G_WanConns_num;
	  `?>
	`?>
`?>


function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//Hide the main menu
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//Hidden secondary menu
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}

function uiOnload(){
	//get temporary data
	Isp = getJsonObjn('nsp');
	var have_pvc = 0;
	
	if(Isp == null){
		alert('Isp is null!');
		return false;
	}
	chgHTML();
		
	for(var i = 0; i < G_WanConns.length; i++){
		if(G_WanConns_num != 0 && G_WanConns[i][1] == "PVC:1/32"){
			have_pvc = 1;
			setJSONValue({
				'INPUT_Username' : G_WanConns[i][2],
				'INPUT_Psd'  :"********"
			});
			break;
		}
	}
	if(have_pvc == 0){
		setJSONValue({
			'INPUT_Username' : (Isp && Isp['INPUT_Username']) || '',
			'INPUT_Psd'  : (Isp && Isp['INPUT_Psd']) || ''
		});
	}
}

//check password or username
function createJsonData(){
	var _json = '{';
	_json += 'INPUT_Username:"' + $('INPUT_Username').value + '",';
	_json += 'INPUT_Psd:"' + $('INPUT_Psd').value + '",';
	//Adjust JSON data
	_json = _json.delcomma();
	_json += '}';
	addJsonObjn('nsp', _json);
	return true;
}

function uiNextPage(){
	
	
	var _bool = createJsonData();
	if(!_bool) 
		return false;
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage'  : 'wizwl_login',
		'var:page'  : 'wizisp_login',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();	
}

function dealWithError(){

}

addListeners(uiOnload, dealWithError);