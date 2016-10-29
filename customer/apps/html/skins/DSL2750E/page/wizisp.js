//wizard -> isp
var G_Conns = [];
var n = 0;
var wan_type = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`
		<?if gt $11 0
		`
			<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name"
			`
				G_Conns[n] = ["$01"];
				n++;
			`?>
		`?>
		<?if gt $12 0
		`
			<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name"
			`
				G_Conns[n] = ["$01"];
				n++;
			`?>
		`?>
	`?>
`?>
var Isp;
//PVC
var com_pvc  = '<?get :InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.DefaultPVCList?>';

var CountryList=new Array();
var ISPList=new Array();

CountryList[0 ]=new Citem("Australia", 12);
CountryList[1 ]=new Citem("Chile", 2);
CountryList[2 ]=new Citem("Colombia", 2);
CountryList[3 ]=new Citem("Cyprus", 1);
CountryList[4 ]=new Citem("Egypt", 9);
CountryList[5 ]=new Citem("India", 7);
CountryList[6 ]=new Citem("Indonesia", 7);
CountryList[7 ]=new Citem("Iran", 1);
CountryList[8 ]=new Citem("Israel", 1);
CountryList[9 ]=new Citem("Jordan", 1);
CountryList[10]=new Citem("Japan", 15);
CountryList[11]=new Citem("Korea", 4);
CountryList[12]=new Citem("Malaysia", 3);
CountryList[13]=new Citem("Moroco", 1);
CountryList[14]=new Citem("New Zealand", 2);
CountryList[15]=new Citem("Oman", 1);
CountryList[16]=new Citem("Pakistan", 7);
CountryList[17]=new Citem("Palestine", 1);
CountryList[18]=new Citem("Peru", 1);
CountryList[19]=new Citem("Philippines", 3);
CountryList[20]=new Citem("Qatar", 1);
CountryList[21]=new Citem("Saudi Arabia", 1);
CountryList[22]=new Citem("Singapore", 4);
CountryList[23]=new Citem("Syria", 2);
CountryList[24]=new Citem("Thailand", 9);
CountryList[25]=new Citem("Turkey", 2);

ISPList[0 ]=new ISPitem("Australia", "AAPT", "PPPoE", "LLC", 8, 35, 0);
ISPList[1 ]=new ISPitem("Australia", "Dodo Internet", "PPPoE", "LLC", 8, 35, 0);
ISPList[2 ]=new ISPitem("Australia", "Internode", "PPPoE", "LLC", 8, 35, 0);
ISPList[3 ]=new ISPitem("Australia", "iinet", "PPPoE", "LLC", 8, 35, 0);
ISPList[4 ]=new ISPitem("Australia", "iPrimus", "PPPoE", "LLC", 8, 35, 0);
ISPList[5 ]=new ISPitem("Australia", "Netspace", "PPPoE", "LLC", 8, 35, 0);
ISPList[6 ]=new ISPitem("Australia", "Optus", "PPPoE", "LLC", 8, 35, 0);
ISPList[7 ]=new ISPitem("Australia", "OzEmail", "PPPoE", "LLC", 8, 35, 0);
ISPList[8 ]=new ISPitem("Australia", "Pacific Internet", "PPPoE", "LLC", 8, 35, 0);
ISPList[9 ]=new ISPitem("Australia", "TelPacific", "PPPoE", "LLC", 8, 35, 0);
ISPList[10]=new ISPitem("Australia", "Telstra BigPond", "PPPoE", "LLC", 8, 35, 0);
ISPList[11]=new ISPitem("Australia", "TPG", "PPPoE", "LLC", 8, 35, 0);

ISPList[12]=new ISPitem("Chile", "Telefonica Chile(Bridge)", "Bridge", "LLC", 8, 32, 0);
ISPList[13]=new ISPitem("Chile", "Telefonica Chile(PPPoE)", "PPPoE", "LLC", 8, 34, 0);

ISPList[14]=new ISPitem("Colombia", "Emcali(PPPoA)", "PPPoA", "VCMUX", 2, 35, 0);
ISPList[15]=new ISPitem("Colombia", "Emcali(PPPoE)", "PPPoE", "LLC", 2, 35, 0);

ISPList[16]=new ISPitem("Cyprus", "CYTA", "PPPoA", "VCMUX", 8, 35, 0);

ISPList[17]=new ISPitem("Egypt", "EGYNET(PPPoA)", "PPPoA", "VCMUX", 8, 35, 0);
ISPList[18]=new ISPitem("Egypt", "EGYNET(Static IP)", "Static IP", "LLC", 8, 35, 0);
ISPList[19]=new ISPitem("Egypt", "link dot Net(PPPoA)", "PPPoA", "VCMUX", 0, 35, 0);
ISPList[20]=new ISPitem("Egypt", "link dot Net(Static IP)", "Static IP", "LLC", 0, 35, 0);
ISPList[21]=new ISPitem("Egypt", "Nile On Line", "PPPoE", "LLC", 0, 35, 0);
ISPList[22]=new ISPitem("Egypt", "Raya Telecom(PPPoA)", "PPPoA", "VCMUX", 8, 80, 0);
ISPList[23]=new ISPitem("Egypt", "Raya Telecom(Static IP)", "Static IP", "LLC", 8, 80, 0);
ISPList[24]=new ISPitem("Egypt", "TeData(PPPoA)", "PPPoA", "VCMUX", 0, 35, 0);
ISPList[25]=new ISPitem("Egypt", "TeData(Static IP)", "Static IP", "LLC", 0, 35, 0);

ISPList[26]=new ISPitem("India", "Airtel(PPPoE)", "PPPoE", "LLC", 1, 32, 0);
ISPList[27]=new ISPitem("India", "Airtel(Bridge)", "Bridge", "LLC", 1, 32, 0);
ISPList[28]=new ISPitem("India", "BSNL", "PPPoE", "LLC", 0, 35, 0);
ISPList[29]=new ISPitem("India", "MTNL(PPPoE)", "PPPoE", "LLC", 0, 32, 0);
ISPList[30]=new ISPitem("India", "MTNL(Bridge)", "Bridge", "LLC", 0, 33, 0);
ISPList[31]=new ISPitem("India", "VSNL(PPPoE)", "PPPoE", "LLC", 8, 35, 0);
ISPList[32]=new ISPitem("India", "VSNL(Bridge)", "Bridge", "LLC", 8, 35, 0);

ISPList[33]=new ISPitem("Indonesia", "PT Telkom DIVRE 1 (Sumatra)", "PPPoE", "LLC", 0, 35, 0);
ISPList[34]=new ISPitem("Indonesia", "PT Telkom DIVRE 2 (Jakarta) Alcatel", "PPPoA", "LLC", 8, 35, 0);
ISPList[35]=new ISPitem("Indonesia", "PT Telkom DIVRE 2 (Jakarta) Siemens", "PPPoA", "VCMUX", 1, 33, 0);
ISPList[36]=new ISPitem("Indonesia", "PT Telkom DIVRE 3 (West Java)", "PPPoA", "VCMUX", 1, 33, 0);
ISPList[37]=new ISPitem("Indonesia", "PT Telkom DIVRE 4 (Central Java)", "PPPoE", "LLC", 8, 35, 0);
ISPList[38]=new ISPitem("Indonesia", "PT Telkom DIVRE 5 (East Java)", "PPPoE", "LLC", 0, 35, 0);
ISPList[39]=new ISPitem("Indonesia", "PT Telkom DIVRE 7 (East Indonesia)", "PPPoE", "LLC", 0, 35, 0);

ISPList[40]=new ISPitem("Iran", "Pars-Online", "Bridge", "LLC", 0, 35, 0);

ISPList[41]=new ISPitem("Israel", "Bezeq", "PPPoE", "LLC", 8, 48, 0);

ISPList[42]=new ISPitem("Jordan", "Wanadoo", "PPPoE", "LLC", 8, 35, 0);

ISPList[43]=new ISPitem("Japan", "ACCA", "PPPoA", "LLC", 0, 35, 0);
ISPList[44]=new ISPitem("Japan", "CTnet", "PPPoA", "LLC", 0, 32, 0);
ISPList[45]=new ISPitem("Japan", "DION (KDDI)", "PPPoA", "LLC", 0, 35, 0);
ISPList[46]=new ISPitem("Japan", "eAccess", "PPPoE", "LLC", 0, 32, 0);
ISPList[47]=new ISPitem("Japan", "eAccess (PPPoA)", "PPPoA", "VCMUX", 0, 32, 0);
ISPList[48]=new ISPitem("Japan", "FLET's ADSL (NTT)", "PPPoE", "LLC", 0, 32, 0);
ISPList[49]=new ISPitem("Japan", "HTnet", "PPPoA", "LLC", 0, 32, 0);
ISPList[50]=new ISPitem("Japan", "KCN", "PPPoA", "VCMUX", 0, 32, 0);
ISPList[51]=new ISPitem("Japan", "Nagoya", "PPPoE", "LLC", 0, 32, 0);
ISPList[52]=new ISPitem("Japan", "OCN", "PPPoE", "LLC", 0, 35, 0);
ISPList[53]=new ISPitem("Japan", "ODN (J-DSL Personal)", "PPPoA", "VCMUX", 0, 32, 0);
ISPList[54]=new ISPitem("Japan", "ODN (J-DSL Business)", "PPPoA", "VCMUX", 0, 33, 0);
ISPList[55]=new ISPitem("Japan", "RMC", "Dynamic IP", "LLC", 0, 35, 0);
ISPList[56]=new ISPitem("Japan", "T-com", "PPPoA", "VCMUX", 0, 32, 0);
ISPList[57]=new ISPitem("Japan", "Yahoo!BB (SoftBankBB)", "Dynamic IP", "LLC", 0, 35, 0);

ISPList[58]=new ISPitem("Korea", "Hanaro Telecom(PPPoE)", "PPPoE", "LLC", 0, 67, 0);
ISPList[59]=new ISPitem("Korea", "Hanaro Telecom(PPPoA)", "PPPoA", "VCMUX", 0, 67, 0);
ISPList[60]=new ISPitem("Korea", "KT(Korea Telecom)(PPPoE)", "PPPoE", "LLC", 0, 32, 0);
ISPList[61]=new ISPitem("Korea", "KT(Korea Telecom)(PPPoA)", "PPPoA", "LLC", 0, 32, 0);

ISPList[62]=new ISPitem("Malaysia", "Maxis", "PPPoA", "LLC", 0, 35, 0);
ISPList[63]=new ISPitem("Malaysia", "Streamyx (Telecom Malaysia - PPPoE)", "PPPoE", "LLC", 0, 35, 0);
ISPList[64]=new ISPitem("Malaysia", "Streamyx (Telecom Malaysia - PPPoA)", "PPPoA", "LLC", 0, 35, 0);

ISPList[65]=new ISPitem("Moroco", "MAROC Telecom", "PPPoA", "VCMUX", 8, 35, 0);

ISPList[66]=new ISPitem("New Zealand", "IHUG", "PPPoA", "VCMUX", 0, 100, 0);
ISPList[67]=new ISPitem("New Zealand", "Telecom Xtra", "PPPoA", "VCMUX", 0, 100, 0);

ISPList[68]=new ISPitem("Oman", "Oman Tel", "PPPoE", "LLC", 0, 35, 0);

ISPList[69]=new ISPitem("Pakistan", "CyberNet", "PPPoE", "LLC", 0, 35, 0);
ISPList[70]=new ISPitem("Pakistan", "Dancom", "PPPoE", "LLC", 8, 35, 0);
ISPList[71]=new ISPitem("Pakistan", "GerrysNet", "PPPoE", "LLC", 0, 35, 0);
ISPList[72]=new ISPitem("Pakistan", "MultiNet", "PPPoE", "LLC", 0, 35, 0);
ISPList[73]=new ISPitem("Pakistan", "Micronet", "PPPoE", "LLC", 0, 35, 0);
ISPList[74]=new ISPitem("Pakistan", "PAKNet", "PPPoE", "LLC", 0, 35, 0);
ISPList[75]=new ISPitem("Pakistan", "WolNet", "PPPoE", "LLC", 0, 35, 0);

ISPList[76]=new ISPitem("Palestine", "PalTel/Hadara", "PPPoE", "LLC", 8, 35, 0);

ISPList[77]=new ISPitem("Peru", "Telefonica", "PPPoE", "LLC", 8, 60, 0);

ISPList[78]=new ISPitem("Philippines", "Globe", "PPPoA", "VCMUX", 0, 35, 0);
ISPList[79]=new ISPitem("Philippines", "PLDT (Manila)", "PPPoE", "LLC", 0, 100, 0);
ISPList[80]=new ISPitem("Philippines", "PLDT (Provincial)", "PPPoE", "LLC", 0, 35, 0);

ISPList[81]=new ISPitem("Qatar", "QTEL", "PPPoA", "VCMUX", 8, 35, 0);

ISPList[82]=new ISPitem("Saudi Arabia", "Saudi Telecom", "PPPoE", "LLC", 0, 35, 0);

ISPList[83]=new ISPitem("Singapore", "Pacific Internet(PPPoA)", "PPPoA", "VCMUX", 0, 100, 0);
ISPList[84]=new ISPitem("Singapore", "Pacific Internet(PPPoE)", "PPPoE", "LLC", 0, 100, 0);
ISPList[85]=new ISPitem("Singapore", "Singnet Broad Band(PPPoA)", "PPPoA", "VCMUX", 0, 100, 0);
ISPList[86]=new ISPitem("Singapore", "Singnet Broad Band(PPPoE)", "PPPoE", "LLC", 0, 100, 0);

ISPList[87]=new ISPitem("Syria", "Syrian Telecom-1", "PPPoE", "LLC", 0, 33, 0);
ISPList[88]=new ISPitem("Syria", "Syrian Telecom-2", "PPPoA", "LLC", 0, 35, 0);

ISPList[89]=new ISPitem("Thailand", "ADC", "PPPoE", "LLC", 0, 35, 0);
ISPList[90]=new ISPitem("Thailand", "Samart", "PPPoE", "LLC", 0, 35, 0);
ISPList[91]=new ISPitem("Thailand", "TOT", "PPPoE", "LLC", 1, 32, 0);
ISPList[92]=new ISPitem("Thailand", "TRUE (Nokia)", "PPPoA", "VCMUX", 0, 100, 0);
ISPList[93]=new ISPitem("Thailand", "TRUE (Huawei)", "PPPoE", "LLC", 0, 100, 0);
ISPList[94]=new ISPitem("Thailand", "TT&T (Normal)", "PPPoE", "LLC", 0, 33, 0);
ISPList[95]=new ISPitem("Thailand", "TT&T (Hinet)", "PPPoE", "LLC", 0, 66, 0);
ISPList[96]=new ISPitem("Thailand", "Qnet", "PPPoE", "LLC", 0, 40, 0);
ISPList[97]=new ISPitem("Thailand", "UCOM", "PPPoE", "LLC", 0, 100, 0);

ISPList[98]=new ISPitem("Turkey", "TTNET(PPPoE)", "PPPoE", "LLC", 8, 35, 0);
ISPList[99]=new ISPitem("Turkey", "TTNET(PPPoA)", "PPPoA", "VCMUX", 8, 35, 0);


function Citem(scountry, iispcount)
{
    this.scountry=scountry;
    this.iispcount=iispcount;
}


function ISPitem(scountry,sname,sprtcl,sencap,ivpi, ivci, imtu)
{
    this.scountry=scountry;
    this.sname=sname;
    this.sprtcl=sprtcl;
    this.sencap=sencap;
    this.ivpi=ivpi;
    this.ivci=ivci;
    this.imtu=imtu;
}

function uiOnload(){
	//get temporary data
	Isp = getJsonObj('isp');
	if(Isp == null){
		alert('Isp is null!');
		return false;
	}
	$('display_access').style.display = 'none';
	<?if eq 1 $var:mod_ETHWAN
	`	
		get_wan_type();	 //From the page cache read choice of wan connection mode
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`	
		wan_type = 'DSL';  //Form.Radio('INPUT_WanAcsType') = 0, Select the "DSL" mode
	`?>
	chgHTML();
	createProtocolOpt();
	createConnTypeOpt();
	createCountry(document.getElementById("SELECT_Country"));
	
	if(!(Isp && Isp['SELECT_Country']))
	{
		$('SELECT_Isp').options[0]=new Option("(Click to Select)", "-1");
		$('SELECT_Isp').value = "-1";
		setdsl_null();
		disCtrl('isDsl',false);
		$('ppp').style.display = 'none';
		$('ip').style.display = 'none';
		return;
	}
	$('SELECT_Country').value = Isp['SELECT_Country'];
	onchgCountry();
	chadisplay();
	if(wan_type == 'DSL'){ // -> DSL
		setJSONValue({
			'SELECT_Country': (Isp && Isp['SELECT_Country']) || '-1',
			'SELECT_Isp': (Isp && Isp['SELECT_Isp']) || '-1',
			'SELECT_Protocol': (Isp && Isp['SELECT_Protocol']) || '-1',
			'SELECT_ConnType': (Isp && Isp['SELECT_ConnType']) || '-1',
			'INPUT_VPI'  : (Isp && Isp['INPUT_VPI']) || '0',
			'INPUT_VCI'  : (Isp && Isp['INPUT_VCI']) || '35',
			//ppp
			'INPUT_Username' : (Isp && Isp['INPUT_Username']) || '',
			'INPUT_Psd'  : (Isp && Isp['INPUT_Psd']) || '',
			'INPUT_CfmPsd'   : (Isp && Isp['INPUT_Psd']) || '',
			//ip
			'INPUT_IPAddress_static': (Isp && Isp['INPUT_IPAddress_static']) || '',
			'INPUT_SubMask_static'  : (Isp && Isp['INPUT_SubMask_static']) || '',
			'INPUT_DefaultGy_static': (Isp && Isp['INPUT_DefaultGy_static']) || '',
			'INPUT_PryDnsServer_static' : (Isp && Isp['INPUT_PryDnsServer_static']) || ''
		});
	} 
	else { // -> Internet
		$('isDsl').style.display = 'none';
		setJSONValue({
			'SELECT_Protocol': (Isp && Isp['SELECT_Protocol']) || 'PPPoE',
			//ppp
			'INPUT_Username' : (Isp && Isp['INPUT_Username']) || '',
			'INPUT_Psd'  : (Isp && Isp['INPUT_Psd']) || '',
			'INPUT_CfmPsd'   : (Isp && Isp['INPUT_Psd']) || '',
			//ip
			'INPUT_IPAddress_static': (Isp && Isp['INPUT_IPAddress_static']) || '',
			'INPUT_SubMask_static'  : (Isp && Isp['INPUT_SubMask_static']) || '',
			'INPUT_DefaultGy_static': (Isp && Isp['INPUT_DefaultGy_static']) || '',
			'INPUT_PryDnsServer_static' : (Isp && Isp['INPUT_PryDnsServer_static']) || ''
		});		
	}
	
}
function get_wan_type(){
	setJSONValue({
		'INPUT_WanAcsType' :  (Isp && Isp['INPUT_WanAcsType']) || '0'
	});
	if(0 == Form.Radio('INPUT_WanAcsType')){ 
		wan_type = 'DSL';  //Form.Radio('INPUT_WanAcsType') = 0, Select the "DSL" mode
	}
	else{
		wam_type = 'Ethernet' //Form.Radio('INPUT_WanAcsType') = 1, Select the "ETHWAN" mode
	}
}

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

function chadisplay(){
	if(wan_type == 'DSL'){
		$('isDsl').style.display = 'block';
		$('ppp').style.display = 'block';
		$('ip').style.display = 'none';
	}
	else{
		$('isDsl').style.display = 'none';
		$('ppp').style.display = 'none';
		$('ip').style.display = 'none';
	}
}

function createCountry(cb)
{

	cb.options.length=0;
	
	cb.options[0]=new Option("(Click to Select)", "-1");

	var count =0;

	for (i=0; i < CountryList.length; i++){
		cb.options[i+1]=new Option(CountryList[i].scountry, i);
		count++;
	}

	cb.options[count + 1]=new Option("Others", "-2");

}

function setdsl_null(){
	$('SELECT_Protocol').value = "-1";
	$('SELECT_ConnType').value = "-1";
	$('INPUT_VPI').value = "enter a number";
	$('INPUT_VCI').value = "enter a number";
	$('ppp').style.display = 'none';
	$('ip').style.display = 'none';
}
//protocol
function createProtocolOpt(){
	var _text  = ['(Click to Select)','PPPoE','PPPoA','Dynamic IP','Static IP','Bridge'];
	var _value = ['-1','PPPoE','PPPoA','Dynamic IP','Static IP','Bridge'];
	
	if(wan_type == 'Ethernet'){
		_text  = ['(Click to Select)','PPPoE','Dynamic IP','Static IP','Bridge'];
		_value = ['-1','PPPoE','Dynamic IP','Static IP','Bridge'];
	}
	
	$S('SELECT_Protocol', _text, _value);
}


function createConnTypeOpt(){
	var _text  = ['(Click to Select)','VC-Mux','LLC'];
	var _value = ['-1','VCMUX','LLC'];
	
	$S('SELECT_ConnType', _text, _value);	
}

function changetypeSelect(value){
	if(0 == value){
		wan_type = 'DSL';
		$('isDsl').style.display = 'block';
		$('ppp').style.display = 'block';
		$('ip').style.display = 'none';
		createProtocolOpt();
		createConnTypeOpt();
		onchgProtocol();
	}
	else{
		wan_type = 'Ethernet';
		$('isDsl').style.display = 'none';
		$('ppp').style.display = 'none';
		$('ip').style.display = 'none';
		createProtocolOpt();
		onchgProtocol();
	}
}

function onchgCountry(){
	
	var cb1 = $('SELECT_Country');
	var cb2 = $('SELECT_Isp');
	var cb3 = $('SELECT_Protocol');
	var cb4 = $('SELECT_ConnType');
    var value = cb1.value;
	cb2.options.length=0;
	cb2.options[0]=new Option("(Click to Select)", "-1");
	if ((value == -1)||(value == -2))
	{
		if(value == -1)
		{
			setdsl_null();
			disCtrl('isDsl',false);
		}
		else if (value == -2)
		{
			cb2.options[1]=new Option("Others", "-2");
			cb2.selectedIndex = cb2.length - 1;
			cb2.value = "-2";
			setdsl_null();
			disCtrl('isDsl',true);
		}
		return;
	}
	var count =0;
	for (i=0; i < ISPList.length; i++)
	{
		if (CountryList[cb1.value].scountry == ISPList[i].scountry)
		{
			cb2.options[count+1]=new Option(ISPList[i].sname, i);
			count++;
		}
		if (count == CountryList[cb1.value].iispcount)
		{
			break;
		}
	}

	cb2.options[count + 1]=new Option("Others", "-2");
	
	cb3.options[0]=new Option("(Click to Select)", "-1");
	
	cb4.options[0]=new Option("(Click to Select)", "-1");
	
}

function setvalue(){
	
	disCtrl('isDsl',true);
	var ISP = $('SELECT_Isp').value;
	
	$('SELECT_Protocol').value = ISPList[ISP].sprtcl;
	$('SELECT_ConnType').value = ISPList[ISP].sencap;
	$('INPUT_VPI').value = ISPList[ISP].ivpi;
	$('INPUT_VCI').value = ISPList[ISP].ivci;
	
	onchgProtocol();
}

function onchgISP(){

	if($('SELECT_Isp').value == -1)
	{
		setdsl_null();
		disCtrl('isDsl',false);
		return;
	}
	else if($('SELECT_Isp').value == -2)
	{
		setdsl_null();
		disCtrl('isDsl',true);
		return;
	}
	setvalue();
}


function onchgProtocol(){
	var MContent = $('Content').offsetHeight;
	var nodes = $('ppp','ip');
	var value_protocol = $('SELECT_Protocol').value;
	var ppp = 'none', ip = 'none';
	
	switch(value_protocol){
		case 'PPPoE' :
		case 'PPPoA' :
			ppp = 'block';
		break;
			case 'Dynamic IP' : 
			break;
		case 'Static IP' : 
			ip = 'block';
			break;
		case 'Bridge' :
	}
	
	nodes[0].style.display = ppp;
	nodes[1].style.display = ip;
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + nodes[0].offsetHeight + nodes[1].offsetHeight;
		//$('Menu').style.height=ContentHeigh + 'px';
		//$('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
		Heigtduiqi();
	}
}

var ajax;
var pvcIdx = 0; //Record the first N group of PVC

function autoSearchPVC(){
	var temp_pvc = com_pvc.split(',');
	$('Scan').disabled = true;
	$('display_access').style.display = 'block';
	
	//Every time just send a PVC
	if(!temp_pvc[pvcIdx]){
		$('lang_pvcresult').style.display = 'none';
		$('SPAN_PvcResult').innerHTML = data_language.dynamic.no_pvc;
		$('Scan').disabled = false;
		pvcIdx = 0;
		return false;
	}
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.Enable': '1',
		':InternetGatewayDevice.X_TWSZ-COM_Diagnostics.AutoPVCSearch.SearchPVC' : temp_pvc[pvcIdx],
		'obj-action' : 'set',
		'var:page'   : G_Page,
		'getpage': 'html/page/autopvc_ajax.js',
		'errorpage'  : 'html/page/autopvc_ajax.js'
	});
	
	pvcIdx++;
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_getPvc);
	ajax.post($('uiPostForm'));
}

function ajax_getPvc(_text){
	//Success, processing pvc
	try{
		eval(_text);
	} catch (ex){
	}
	
	if (G_Error == '0' && G_FoundPvc != ''){
		$('lang_pvcresult').style.display = 'none',
		$('SPAN_PvcResult').innerHTML = 'Found Available PVC ' + G_FoundPvc + '.';
		//assignment
		$('INPUT_VPI').value = G_FoundPvc.split('/')[0];
		$('INPUT_VCI').value = G_FoundPvc.split('/')[1];
	} else if(G_FoundPvc == ''){
		//Restart search
		autoSearchPVC();
	}	
}

function uiNextPage(){
	if(wan_type == 'DSL')
	{ // -> DSL
		if(($('INPUT_VPI').value < 0) ||($('INPUT_VPI').value > 255))
		{
			alert("Invalid VPI value (0~255)!");
			return false;
		}
		if(($('INPUT_VCI').value < 32) ||($('INPUT_VCI').value > 65535))
		{
			alert("Invalid VCI value (32~65535)!");
			return false;
		}
	}
	if($('SELECT_Protocol').value =="PPPoE" || $('SELECT_Protocol').value =="PPPoA")
	{
		if(!checkValue())
		{
			return false;
		}
	}
	
	var _bool = createJsonData();
	if(!_bool) 
		return false;
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage'  : 'wizwl',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();	
}

function createJsonData(){
	var _json = '{';
	var nodes_isp = $('SELECT_Protocol','SELECT_ConnType','INPUT_VPI','INPUT_VCI',
	//PPPoA/E
	'INPUT_Username','INPUT_Psd',
	//Static IP
	'INPUT_IPAddress_static','INPUT_SubMask_static','INPUT_DefaultGy_static','INPUT_PryDnsServer_static',
	//country and isp
	'SELECT_Country','SELECT_Isp');
	switch(nodes_isp[0].value){
		case 'PPPoA' :
		case 'PPPoE' : 
			_json += 'INPUT_Username:"' + nodes_isp[4].value + '",';
			_json += 'INPUT_Psd:"' + nodes_isp[5].value + '",';
			break;
		case 'Static IP' :
			_json += 'INPUT_IPAddress_static:"' + nodes_isp[6].value + '",';
			_json += 'INPUT_SubMask_static:"' + nodes_isp[7].value + '",';
			_json += 'INPUT_DefaultGy_static:"' + nodes_isp[8].value + '",';
			_json += 'INPUT_PryDnsServer_static:"' + nodes_isp[9].value + '",';
			break;
		case 'Dynamic IP' :
		case 'Bridge' :
	}

	if(wan_type == 'DSL'){
		_json += 'SELECT_ConnType:"' + nodes_isp[1].value + '",';
		_json += 'INPUT_VPI:"' + nodes_isp[2].value + '",';
		_json += 'INPUT_VCI:"' + nodes_isp[3].value + '",';
	}
	_json += 'SELECT_ServiceName:"' + getServiceName() + '",';
	_json += 'SELECT_Protocol:"' + nodes_isp[0].value + '",';
	_json += 'SELECT_Country:"' + nodes_isp[10].value + '",';
	_json += 'SELECT_Isp:"' + nodes_isp[11].value + '",';
	<?if eq 1 $var:mod_ETHWAN
	`	
		_json += 'INPUT_WanAcsType:"' + Form.Radio('INPUT_WanAcsType') + '",';
	`?>
	<?if neq 1 $var:mod_ETHWAN
	`	
		_json += 'INPUT_WanAcsType:"' + "0" + '",';
	`?>
	//Adjust JSON data
	_json = _json.delcomma();
	_json += '}';
	addJsonObj('isp', _json);
	return true;
	}

function getServiceName(){
	var nodes = $('SELECT_Protocol','INPUT_VPI','INPUT_VCI');
	var name = '';
	
	switch(nodes[0].value){
		case 'PPPoE' :
			name = 'pppoe_';
			break;
		case 'PPPoA' :
			name = 'pppoa_';
			break;
		case 'Dynamic IP' : 
			name = 'mer_';
			break;
		case 'Static IP' : 
			name = 'mer_';
			break;
		case 'Bridge' :
			name = 'br_';
			break;
	}
	
	if(wan_type == 'DSL'){ // -> DSL
		name += nodes[1].value + '_' + nodes[2].value + '_' + '0_'+ n+'_Internet';
		return name;
	} else { // -> Ethernet
		name += '0_' + n +'_Internet';
		return name;
	}
}

function uiBack(){
	var _bool = createJsonData();
	if(!_bool)
		return false;
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage' : 'wizsntp',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();
}
	
function checkValue(){
	//check vpi
	if((parseInt($('INPUT_VPI').value)<0) || (parseInt($('INPUT_VPI').value)>255))
	{
		alert('Please enter the VPI with right format !');
		return false;
	}
	
	//check vci
	if((parseInt($('INPUT_VCI').value)<32) || (parseInt($('INPUT_VCI').value)>65535))
	{
		alert('Please enter the VCI with right format !');
		return false;
	}
	
	var node_psd = $('INPUT_Psd','INPUT_CfmPsd');
	var node_type = getServiceName().split('_');
	
	/*for(var i = 0, _type, _len = G_Conns.length; i < _len; i++){
		_type = G_Conns[i][0].split('_');
		if( _type[1] == node_type[1] && _type[2] == node_type[2]){
			alert(SEcode[9999]);
			return false;
		}
	}*/
	
	if(node_psd[0].value != node_psd[1].value){
		alert(SEcode[1010]);
		return false;
	}
	//check username
	if(!checkPswd($('INPUT_Username').value))
	{
		alert('Please enter the username with right format !');
		return false;
	}
	//check password
	if(!checkPswd(node_psd[0].value))
	{
		alert('Please enter the password with right format !');
		return false;
	}
	
	return true;
}

//check password or username
function checkPswd(x)
{
	var patrn=/^[a-zA-Z0-9]{1}([a-zA-Z0-9]|[_]|[%]|[@]|[/]|[*]|[.]|[?]|[-]){0,62}$/;
	if (!patrn.exec(x))
		return false;
	else
		return true;
}

function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	},true);
	
	$('uiPostForm').submit();
}

function dealWithError(){

}

addListeners(uiOnload, dealWithError);