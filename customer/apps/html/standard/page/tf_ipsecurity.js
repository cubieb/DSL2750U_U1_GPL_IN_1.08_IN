var G_Firewall = [];
var G_Rule = [];
var m = 0;
var n = 0;
var k = 0;
<?objget :InternetGatewayDevice.X_TELEFONICA-ES_Firewall.Firewall. "RuleNumberOfEntries DefaultAction Interface Type Name stats.Bytes stats.Packets DefaultChain"
`	G_Firewall[m] = ["InternetGatewayDevice.X_TELEFONICA-ES_Firewall.Firewall.$00.",//Path
						"$01", //-> G_TFFirewall["$00"][1] RuleNumberOfEntries
						"$02", //DefaultAction     
						"$03", //Interface
						"$04", //Type
						"$05", //Name
						"$06", //stats.Bytes
						"$07", //stats.Packets
						"$08"  //DefaultChain
						];
	m++;
	<?objget :InternetGatewayDevice.X_TELEFONICA-ES_Firewall.Firewall.$10.Rule. "Enabled IcmpType RejectType Action Protocol Origin.IPAddress Origin.Mask Origin.StartPort Origin.EndPort Destination.IPAddress Destination.Mask Destination.StartPort Destination.EndPort stats.Bytes stats.Packets IPProtocolVersionType Alg DSCP PacketLengthMin PacketLengthMax TcpFlags Destination.FQDNEnable Destination.FQDN AlgEnable"
	`	G_Rule[n] = ["InternetGatewayDevice.X_TELEFONICA-ES_Firewall.Firewall.$10.Rule.$00.",//Path
					  "$01", //-> G_TFFirewall["$10"][m][1] Enabled
				 	  "$02", //IcmpType
				 	  "$03", //RejectType
				 	  "$04", //Action
				 	  "$05", //Protocol
				 	  "$06", //Origin.IPAddress
				 	  "$07", //Origin.Mask
				 	  "$08", //Origin.StartPort
				 	  "$09", //Origin.EndPort
				 	  "$0a", //Destination.IPAddress     
				 	  "$0b", //Destination.Mask
				 	  "$0c", //Destination.StartPort
				 	  "$0d", //Destination.EndPort
				 	  "$0e", //stats.Bytes
				 	  "$0f", //stats.Packets
				 	  "$0g", //IP Protocol Version Type
                      "$0h", //Alg  17
                      "$0i", //DSCP 18
                      "$0j", //PacketLengthMin 19
                      "$0k", //PacketLengthMax 20
                      "$0l", //TcpFlags 21 
                      "$0m",  //Destination.FQDNEnable22
                      "$0n",  //Destination.FQDN 23
                      "$0o"  //AlgEnable 24 
				 	  ];
		n++;
	`
	?>
`?>

var G_WanConn = [];
var m = 0;

<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?if eq $var:G_IPv6Enable 0
	    `   <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled"
		`	G_WanConn[m] = [];
			G_WanConn[m][0] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$00";   // Path
			G_WanConn[m][1] = "$01";   // Name
			G_WanConn[m][2] = "$02";   // X_TWSZ-COM_ServiceList
			G_WanConn[m][3] = "$03";   //X_TELEFONICA-ES_IPv4Enabled
			m++;
		   `?>
	  `?>
	  <?if eq $var:G_IPv6Enable 1
	    `   <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled"
		`	G_WanConn[m] = [];
			G_WanConn[m][0] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANIPConnection.$00";   // Path
			G_WanConn[m][1] = "$01";   // Name
			G_WanConn[m][2] = "$02";   // X_TWSZ-COM_ServiceList
			G_WanConn[m][3] = "$03";   //X_TELEFONICA-ES_IPv4Enabled
			G_WanConn[m][4] = "$04";   //X_TELEFONICA-ES_IPv6Enabled
			m++;
		   `?>
	  `?>
	`?>
	<?if gt $12 0
	`	<?if eq $var:G_IPv6Enable 0
	     `  <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled"
		`	G_WanConn[m] = [];
			G_WanConn[m][0] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$00";   // Path
			G_WanConn[m][1] = "$01";   // Name
			G_WanConn[m][2] = "$02";   // X_TWSZ-COM_ServiceList
			G_WanConn[m][3] = "$03";   //X_TELEFONICA-ES_IPv4Enabled
			m++;
		   `?>
		`?>
		<?if eq $var:G_IPv6Enable 1
	     `  <?objget :InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection. "Name X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled"
		`	G_WanConn[m] = [];
			G_WanConn[m][0] = "InternetGatewayDevice.WANDevice.$40.WANConnectionDevice.$30.WANPPPConnection.$00";   // Path
			G_WanConn[m][1] = "$01";   // Name
			G_WanConn[m][2] = "$02";   // X_TWSZ-COM_ServiceList
			G_WanConn[m][3] = "$03";   //X_TELEFONICA-ES_IPv4Enabled
			G_WanConn[m][4] = "$04";   //X_TELEFONICA-ES_IPv6Enabled
			m++;
		   `?>
		`?>
	`?>
`?>
`?>

//LAN
var G_Wlan = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_Description"
`	G_Wlan[t] = ["InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.$00", "$01"];
	t++;
`?>
//WLAN
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID"
`	G_Wlan[t] = ["InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00", "$01"];
	t++;
`?>
    
function SelectFilterEntry(index)
{
  Table.Clear('td_RuleList');
	createRuleTable(index);
	return 0;
}

function createRuleTable(FilterIndex)
{
	var array_value = [];
	var idx=0;
  for(var i=0; i< G_Rule.length ; i++)
	{
		
		if(G_Firewall.length-1 >= FilterIndex&&(G_Rule[i][0].indexOf(G_Firewall[FilterIndex][0])>-1))
		{
		  var _origIP=G_Rule[i][6]+'/'+G_Rule[i][7];
			var _dstIP=G_Rule[i][10]+'/'+G_Rule[i][11];
			var _origport=G_Rule[i][8]+':'+G_Rule[i][9];
			var _dstport=G_Rule[i][12]+':'+G_Rule[i][13];
			array_value[i] = [];
			if(idx==0){
				array_value[i].push('<input type="radio" id="rule_index'+i+'" name="rule_index" value="'+i+'" checked>');
				idx++;
			}
			else
			{
				array_value[i].push('<input type="radio" id="rule_index'+i+'" name="rule_index" value="'+i+'">');
			}
			array_value[i].push(G_Rule[i][1]+'&nbsp;'); //Enabled
			array_value[i].push(G_Rule[i][5]+'&nbsp;'); //Protocol
			array_value[i].push(G_Rule[i][16]+'&nbsp;'); //IP Protocol Version Type
			array_value[i].push(G_Rule[i][4]+'&nbsp;'); //Action
			array_value[i].push(G_Rule[i][3].ellipsis(50)+'&nbsp;'); //RejectType
			array_value[i].push(G_Rule[i][2].ellipsis(50)+'&nbsp;'); //IcmpType
			array_value[i].push(_origIP.ellipsis(4)+'&nbsp;'); //OrigIP/Mask
			array_value[i].push(_origport.ellipsis(4)+'&nbsp;'); //OrigPort
			array_value[i].push(_dstIP.ellipsis(4)+'&nbsp;'); //DestIP/Mask
			array_value[i].push(_dstport.ellipsis(4)+'&nbsp;'); //DestPort
			array_value[i].push(G_Rule[i][14]+'&nbsp;'); //Bytes
			array_value[i].push(G_Rule[i][15]+'&nbsp;'); //Pkts
		}
	}
	$T('td_RuleList',array_value);
	
	if(G_Firewall.length==0)
	{
		$('addRuleEntry').disabled=true;
		$('editRuleEntry').disabled=true;
		$('removeRuleEntry').disabled=true;
		$('editFilterEntry').disabled=true;
		$('removeFilterEntry').disabled=true;
	}
	else
	{
		$('addRuleEntry').disabled=false;
		$('editRuleEntry').disabled=false;
		$('removeRuleEntry').disabled=false;
		$('editFilterEntry').disabled=false;
		$('removeFilterEntry').disabled=false;
	}
}

function createFilterTable(){
	
	Table.Clear('td_IPList');
	var InternalIPShow=0;
	var ExternalIPShow=0;
	
	var array_value = [];
	for(var i = 0; i < G_Firewall.length; i++){

		array_value[i] = [];
		if(i==0)
		{
			array_value[i].push('<input type="radio" id="Filter_index'+i+'" name="Filter_index" value="'+i+'" onclick="SelectFilterEntry(' + i + ')" checked>');
		}
		else
		{
			array_value[i].push('<input type="radio" id="Filter_index'+i+'" name="Filter_index" value="'+i+'" onclick="SelectFilterEntry(' + i + ')">');
		}
		array_value[i].push(G_Firewall[i][5]+'&nbsp;'); //Name
		if(G_Firewall[i][3]=="LAN")
		{
			array_value[i].push('LAN&nbsp;'); //Interface
		}
		else if(G_Firewall[i][3]=="WAN")
		{
			array_value[i].push('WAN&nbsp;'); //Interface
		}
		else if ((G_Firewall[i][3]==""))
		{
		    array_value[i].push('WAN/LAN&nbsp;'); 
		}
		else if(G_Firewall[i][3].indexOf('WANDevice')>-1)//special wan
		{
		    var j = 0;
		    for(j=0; j<G_WanConn.length; j++)
			{
				if(G_Firewall[i][3] == G_WanConn[j][0])
					break;
			}
			if(j==G_WanConn.length)
			{
				array_value[i].push('&nbsp;'); //Interface
			}
			else
			{
			    array_value[i].push(G_WanConn[j][1]); //Interface
			}
		}
        else if(G_Firewall[i][3].indexOf('LANDevice')>-1) //special lan
		{
		    var k = 0;
		    for(k=0; k<G_Wlan.length; k++)
			{
				if(G_Firewall[i][3] == G_Wlan[k][0])
					break;
			}
			if(k==G_Wlan.length)
			{
				array_value[i].push('&nbsp;'); //Interface
			}
			else
			{
			    array_value[i].push(G_Wlan[k][1]); //Interface
			}
		}
		else
		{
			array_value[i].push('&nbsp;'); //Interface
		}
		
		array_value[i].push(G_Firewall[i][4]+'&nbsp;'); //Type
		array_value[i].push(G_Firewall[i][2]+'&nbsp;'); //DefaultAction
		array_value[i].push(G_Firewall[i][6]+'&nbsp;'); //stats.Bytes
		array_value[i].push(G_Firewall[i][7]+'&nbsp;'); //stats.Packet
		array_value[i].push(G_Firewall[i][8]+'&nbsp;'); //DefaultChain
	}
	$T('td_IPList',array_value);
	
}

function uiOnload(){
	createFilterTable();
	createRuleTable(0);
	var _text = [], _value = [], protocol;
	_text.push("LAN");
	_value.push("LAN");

	_text.push("WAN");
	_value.push("WAN");
	_text.push("WAN/LAN");
	_value.push("");
    /*wan*/
	for(var i = 0; i < G_WanConn.length; i++){
		if (G_WanConn[i][1].indexOf('br') > -1||G_WanConn[i][2]=="TR069") {
			continue;
		}
		_text.push(G_WanConn[i][1]);
		_value.push(G_WanConn[i][0]);
	}
    /*lan*/
	for(var j = 0; j < G_Wlan.length; j++){
		_text.push(G_Wlan[j][1]);
		_value.push(G_Wlan[j][0]);
	}    
	$S('SELECT_Interface',_text,_value);
}


function cleanInputArea(){
	var _input=$('EntryInfo').getElementsByTagName('INPUT');

	for(var k = 0, _len = _input.length; k < _len; k++){
		_input[k].value = '';
	}
}

var addFilterAction=0;
var editFilterIdx=0;
function AddFilterEntry(){	
	addFilterAction = 1;
  $('addFilterEntry').disabled=true;
	$('addRuleEntry').disabled=true;
	$('editFilterEntry').disabled=true;
	$('removeFilterEntry').disabled=true;
	$('FilterEntryInfo').style.display='block';
	$('RuleSetting').style.display='none';
	$('RuleEntryInfo').style.display='none';

}

function EditFilterEntry(){
	addFilterAction = 2;
	editFilterIdx=Form.Radio('Filter_index');
	$('addFilterEntry').disabled=true;
	$('addRuleEntry').disabled=true;
	$('editFilterEntry').disabled=true;
	$('removeFilterEntry').disabled=true;
	$('FilterEntryInfo').style.display='block';
	$('RuleSetting').style.display='none';
	$('RuleEntryInfo').style.display='none';
    
	
	$('INPUT_EntryName').value=G_Firewall[editFilterIdx][5];
	$('SELECT_Interface').value=G_Firewall[editFilterIdx][3];
	$('SELECT_Type').value=G_Firewall[editFilterIdx][4];
  $('SELECT_Action').value=G_Firewall[editFilterIdx][2];
	$('SELECT_Chain').value = G_Firewall[editFilterIdx][8];
}

function RemoveFilterEntry(){
	

	if(!confirm(SEcode[1008])){return false;}

	var DelIndex=Form.Radio('Filter_index');

	
	$H({
		'del-obj'   :G_Firewall[DelIndex][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'filtering_options',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'  :'tf_ipsecurity',
		'var:errorpage':'tf_ipsecurity'
	});
	$('uiPostForm').submit();
}

function CancelFilterEdit(){
	addFilterAction=0;
	$('FilterEntryInfo').style.display = 'none';
	$('RuleSetting').style.display='none';
	$('RuleEntryInfo').style.display='block';
	
  $('addFilterEntry').disabled=false;
	$('editFilterEntry').disabled=false;
	$('removeFilterEntry').disabled=false;	
	$('addRuleEntry').disabled=false;
}

function isValidPort(port) {
   var fromport = 0;
   var toport = 100;

   portrange = port.split(':');
   if ( portrange.length < 1 || portrange.length > 2 ) {
       return false;
   }
   if ( isNaN(portrange[0]) )
       return false;
   fromport = parseInt(portrange[0]);
   
   if ( portrange.length > 1 ) {
       if ( isNaN(portrange[1]) )
          return false;
       toport = parseInt(portrange[1]);
       if ( toport <= fromport )
           return false;      
   }
   
   if ( fromport < 1 || fromport > 65535 || toport < 1 || toport > 65535 )
       return false;
   
   return true;
}

function isNum(num)
{
   var i = 0;

   for(i = 0; i < num.length && num.charAt(i) >= '0' && num.charAt(i) <= '9'; i++);
   if (i == num.length)
      return true;
   else
      return false;
}

function isValidIpAddress(address) {    
   ipParts = address.split('/');
   if (ipParts.length > 2) return false;
   if (ipParts.length == 2) {
      num = parseInt(ipParts[1]);
      if (num <= 0 || num > 32)
         return false;
   }

   if (ipParts[0] == '0.0.0.0' ||
       ipParts[0] == '255.255.255.255' )
      return false;

   addrParts = ipParts[0].split('.');
   if ( addrParts.length != 4 ) return false;
   for (kk = 0; kk < 4; kk++) {
      if (isNaN(addrParts[kk]) || addrParts[kk] =="")
         return false;
      num = parseInt(addrParts[kk]);
      if ( num < 0 || num > 255 )
         return false;
   }
   num = parseInt(addrParts[0]);
   if((num < 1)||(num == 127)||(num >223))
   {
        return false;
   }
   var newadd =String(parseInt(addrParts[0])) + '.' + String(parseInt(addrParts[1])) + '.' + 
        String(parseInt(addrParts[2])) + '.' +String(parseInt(addrParts[3]));
   if(newadd != ipParts[0])
   {
        return false;    
   }
   return true;   
}

function getLeftMostZeroBitPos(num) {
   var i = 0;
   var numArr = [128, 64, 32, 16, 8, 4, 2, 1];

   for ( i = 0; i < numArr.length; i++ )
      if ( (num & numArr[i]) == 0 )
         return i;

   return numArr.length;
}

function getRightMostOneBitPos(num) {
   var i = 0;
   var numArr = [1, 2, 4, 8, 16, 32, 64, 128];

   for ( i = 0; i < numArr.length; i++ )
      if ( ((num & numArr[i]) >> i) == 1 )
         return (numArr.length - i - 1);

   return -1;
}

function isValidSubnetMask(mask) {
   var i = 0, num = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;
   var c = '';

   if ( mask == '0.0.0.0' )
      return false;

   for (i = 0; i < mask.length; i++) {
     c = mask.charAt(i);
     if((c>='0'&&c<='9')||(c=='.'))
       continue;
     else
     {
        return false;
      }
   }
/*   if ( mask.indexOf(' ') != -1 )
      return false;*/

   maskParts = mask.split('.');
   if ( maskParts.length != 4 ) return false;

   for (i = 0; i < 4; i++) {
      if ( isNaN(maskParts[i]) || maskParts[i] ==""){
         return false;
      }
      num = parseInt(maskParts[i]);
      if ( num < 0 || num > 255 )
         return false;
      if (maskParts[i].length > 3)
      {
         return false;
	}
      if ( zeroBitExisted == true && num != 0 )
         return false;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }

   return true;
}

function isValidIpAddress6(address) {
   var i = 0, num = 0;

   addrParts = address.split(':');
   if (addrParts.length < 3 || addrParts.length > 8)
      return false;
   for (i = 0; i < addrParts.length; i++) {
      if ( addrParts[i] != "" )
         num = parseInt(addrParts[i], 16);
      if ( i == 0 ) {
//         if ( (num & 0xf000) == 0xf000 )
//            return false;	//can not be link-local, site-local or multicast address
      }
      else if ( (i + 1) == addrParts.length) {
         if ( num == 0 || num == 1)
            return false;	//can not be unspecified or loopback address
      }
      if ( num != 0 )
         break;
   }
   return true;
}

function isValidInput()
{
    var ruleParameterArray = $ ( 'SELECT_Protocol',              /*0*/
					             'SELECT_IPProtocolVersionType', /*1*/
		                         'INPUT_origIPAddress',          /*2*/ 
		                         'INPUT_origMask',               /*3*/
		                         'INPUT_origStartPort',          /*4*/ 
		                         'INPUT_origEndPort',            /*5*/
		                         'INPUT_destIPAddress',          /*6*/
		                         'INPUT_destMask',               /*7*/
		                         'INPUT_destStartPort',          /*8*/
		                         'INPUT_destEndPort');           /*9*/
			
   /* origIPAddress and origMask are valid */
   if(ruleParameterArray[2].value == '' && ruleParameterArray[3].value == '')
      ;//do nothing
   else
   {
       if (ruleParameterArray[1].value == "IPv4")
       {
          if ( isValidIpAddress(ruleParameterArray[2].value) == false )
          {
             alert('origIPAddress "' + ruleParameterArray[2].value + '" is invalid IP address.');
             return false;
          }
          if ( isValidSubnetMask(ruleParameterArray[3].value) == false ) 
          {
            alert('origMask "' + ruleParameterArray[3].value + '" is invalid mask address.');
            return false;
          }
       }
       else if (ruleParameterArray[1].value == "IPv6")
       {
           if (ruleParameterArray[2].value.indexOf('/') != -1) {
             	alert('origIPAddress shall not have network prefix length.');
             	return;
           }         
           if ((ruleParameterArray[2].value != '') && (isValidIpAddress6(ruleParameterArray[2].value) == false)) {
              	alert('Invalid IPv6 address.');
              	return;
           }
           if ( isNum(ruleParameterArray[3].value) == false || parseInt(ruleParameterArray[3].value) > 128) 
           {
               alert('origPrefix Length "' + ruleParameterArray[3].value + '" is invalid prefix length.');
               return false;
           }
       }
   }

   /* destIPAddress and destMask are valid */
   if(ruleParameterArray[6].value == '' && ruleParameterArray[7].value == '')
      ;//do nothing
   else
   {
       if (ruleParameterArray[1].value == "IPv4")
       {
          if ( isValidIpAddress(ruleParameterArray[6].value) == false )
          {
             alert('destIPAddress "' + ruleParameterArray[6].value + '" is invalid IPv4 address.');
             return false;
          }
          if ( isValidSubnetMask(ruleParameterArray[7].value) == false ) 
          {
            alert('destMask "' + ruleParameterArray[7].value + '" is invalid mask address.');
            return false;
          }
       }
       else if (ruleParameterArray[1].value == "IPv6")
       {
           if (ruleParameterArray[6].value.indexOf('/') != -1) {
             	alert('destIPAddress shall not have network prefix length.');
             	return false;
           }         
           if ((ruleParameterArray[6].value != '') && (isValidIpAddress6(ruleParameterArray[6].value) == false)) {
            	alert('Invalid IPv6 address.');
            	return false;
           }
           if ( isNum(ruleParameterArray[7].value) == false || parseInt(ruleParameterArray[7].value) > 128) 
           {
              alert('destPrefix Length "' + ruleParameterArray[7].value + '" is invalid prefix length.');
              return false;
           }
       }
   }

	/*origStartPort and origEndPort are valid*/
    if (isValidPort(ruleParameterArray[4].value) == false || isNum(ruleParameterArray[4].value) == false) 
    {
       alert('origStartPort "' + ruleParameterArray[4].value + '" is invalid.');
       return false;
    }
    if (isValidPort(ruleParameterArray[5].value) == false || isNum(ruleParameterArray[5].value) == false) {
       alert('origEndPort "' + ruleParameterArray[5].value + '" is invalid.');
       return false;
    }
	
    if (isValidPort(ruleParameterArray[8].value) == false || isNum(ruleParameterArray[8].value) == false) {
       alert('destStartPort "' + ruleParameterArray[8].value + '" is invalid.');
       return false;
    }
    if (isValidPort(ruleParameterArray[9].value) == false || isNum(ruleParameterArray[9].value) == false) {
       alert('destEndPort "' + ruleParameterArray[9].value + '" is invalid.');
       return false;
    }
    var oS = parseInt(ruleParameterArray[4].value);
    var oE = parseInt(ruleParameterArray[5].value);
    if (oS > oE) {
       alert('Orig port range [' + oS + ':' + oE + '] is invalid.');
       return false;
    }
    var dS = parseInt(ruleParameterArray[8].value);
    var dE = parseInt(ruleParameterArray[9].value);
    if ( dS > dE ) {
       alert('Dest port range [' + dS + ':' + dE + '] is invalid.');
       return false;
    }

	return true;
}


function selectAllALGClick()
{
    var algMatchArray = new Array();
    var algSetArray = ['ftp','tftp','pptp', 'sip', 'rtsp'];
    algMatchArray['ftp']    = 'INPUT_ALG_FTP';
    algMatchArray['tftp']   = 'INPUT_ALG_TFTP';
    algMatchArray['pptp']   = 'INPUT_ALG_PPTP'; 
    algMatchArray['sip']    = 'INPUT_ALG_SIP'; 
    algMatchArray['rtsp']   = 'INPUT_ALG_RTSP';
    
    for (var i = 0; i < algSetArray.length; i++){
        $( algMatchArray[ algSetArray[i] ] ).checked = true;
    }
}

function selectAllTcpFlagsClick()
{
    var tcpFlagsMatchArray = new Array();
	  var tcpFlagsSetArray = ['SYN','ACK','FIN','RST','URG','PSH'];
    tcpFlagsMatchArray['SYN']    = 'INPUT_TCP_SYN';
    tcpFlagsMatchArray['ACK']   = 'INPUT_TCP_ACK';
    tcpFlagsMatchArray['FIN']   = 'INPUT_TCP_FIN';
    tcpFlagsMatchArray['RST']   = 'INPUT_TCP_RST';
    tcpFlagsMatchArray['URG']  = 'INPUT_TCP_URG';
    tcpFlagsMatchArray['PSH']    = 'INPUT_TCP_PSH';
    
    for (var i = 0; i < tcpFlagsSetArray.length; i++){
        $( tcpFlagsMatchArray[ tcpFlagsSetArray[i] ] ).checked = true;
    }

}

function uiFilterSumbit(){
	var Value_Nodes = $('INPUT_EntryName', 'SELECT_Interface', 'SELECT_Type',
						'SELECT_Action', 'SELECT_Chain');
	for(i=0;i<G_Firewall.length;i++)
	{

		if(addFilterAction != 2 && Value_Nodes[1].value==G_Firewall[i][3] && Value_Nodes[2].value==G_Firewall[i][4])
			break;

	}
	if(i!=G_Firewall.length)
	{
		alert("Error: This filter table has exists in filter firewall!");
		return false;
	}
  
   if ((Value_Nodes[3].value == 'Drop') && ((Value_Nodes[1].value == 'LAN') || (Value_Nodes[1].value == '')))
   {
        var answer = confirm("This Firewall settings may be will let you can not access Router. Are you sure?");
        if (!answer)
        {
            return false;
        }       
   }


	if(addFilterAction=='1'){	
		$H({
			'add-obj' 	  	: 'InternetGatewayDevice.X_TELEFONICA-ES_Firewall.Firewall.',
			':Name'	: 	Value_Nodes[0].value,
			':Interface'	:  Value_Nodes[1].value,
			':Type'   : Value_Nodes[2].value,
			':DefaultAction'    	: Value_Nodes[3].value,
			':DefaultChain'    	: Value_Nodes[4].value,

			'obj-action' 		: 'add-set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'tf_ipsecurity',
			'var:page' : 'filtering_options',
			'var:errorpage'    : 'tf_ipsecurity',
			'var:CacheLastData': ViewState.Save()
		},true);

	}else if(addFilterAction=='2'){     //edit
	
		$H({
			'obj-action' 		: 'set',
			'getpage'  : 'html/index.html',
			'errorpage': 'html/index.html',
			'var:menu' : 'advanced',
			'var:subpage'   :'tf_ipsecurity',
			'var:page' : 'filtering_options',
			'var:errorpage'    : 'tf_ipsecurity',
			'var:CacheLastData': ViewState.Save()
		},true);		

		var path=':'+G_Firewall[editFilterIdx][0];
		$F(path+'Name',Value_Nodes[0].value);
		$F(path+'Interface',Value_Nodes[1].value);
		$F(path+'Type',Value_Nodes[2].value);
		$F(path+'DefaultAction',Value_Nodes[3].value);
		$F(path+'DefaultChain',Value_Nodes[4].value);
	}
	$('uiPostForm').submit();
	addFilterAction=0;
}
var addRuleAction='';
var editRuleIdx=0;

/*************************************************************************************************
需要按需显示的div一共有13�?
ICMPRejectType，ICMPType, SetOfALG, ALGOptions, SetOfDSCP, SetOfPacketLength, SetOfOrigPort, 
hide-origmask, SetOfDestPort, hide-destmask, SetOfDestIP, SetOfFQDN, ICMPTypev6, SetOfDest

关系如下�?

1. 只有protocol �?ICMP 时显�?ICMPRejectType，ICMPType
2. 只有 protocol �?all，tcp, udp时才显示 SetOfOrigPort�?SetOfDestPort 
3. 只有 protocol �?all 时才显示 SetOfALG
4. ip protocol �?ipv4 时所�?protcol 都显�?SetOfDSCP，hide-origmask，hide-destmask
5. protocol �?eigrp、ospf时，目的ip地址是固定的组播地址, SetOfDest 隐藏
6. ALG enabled时， SetOfPacketLength�?SetOfDSCP 隐藏，即 ALGOptions �?（SetOfDSCP，SetOfPacketLength�?互斥
7. SetOfPacketLength �?ALG enabled 外都显示
8. SetOfFQDN �?SetOfDestIP，hide-destmask 互斥
9. SELECT_Icmpv6Type �?ICMPType 互斥

关系整理如下�?
由一个变量决定的
protocol        : ICMPRejectType，SetOfALG，SetOfOrigPort�?SetOfDestPort�?SetOfDest
ip protocol     : hide-origmask
ALG enabled     : ALGOptions ,SetOfPacketLength
FQDN enabled    : SetOfFQDN(show), SetOfDestIP(hidden)

由多个变量决定的
ip protocol & ALG enable    : SetOfDSCP,
ip protocol & FQDN enable   : hide-destmask
protocol & ip protocol      �?ICMPType <-> ICMPTypev6

*************************************************************************************************/
function dynamicElementSwith(){
    var dynamicElementArray = $ ( 'IPv4RejectType',         /* 0 */
                                  'ICMPType',               /* 1 */
                                  'SetOfALG',               /* 2 */
                                  'ALGOptions' ,            /* 3 */
                                  'SetOfDSCP',              /* 4 */
                                  'SetOfPacketLength',      /* 5 */
                                  'SetOfOrigPort',          /* 6 */
                                  'hide-origmask',          /* 7 */
                                  'SetOfDestPort',          /* 8 */
                                  'hide-destmask',          /* 9 */
                                  'SetOfDestIP',            /* 10 */
                                  'SetOfFQDN',              /* 11 */
                                  'SetOfDest',              /* 12 */
                                  'SetOfTcpFlags',          /* 13 */
					              'SetOfTC',                /*14*/
								  'ICMPTypev6',             /* 15 */
					              'IPv6RejectType'); 		/*16*/  
    
    var elementSwitchArray = $ ( 'SELECT_Protocol', 'SELECT_IPProtocolVersionType', 'INPUT_ALG_Enable', 'INPUT_FQDNEnable', 'SELECT_Action_Rule');
    var protocol;
	var filterIdx=Form.Radio('Filter_index');
	
    for (var i = 0; i < dynamicElementArray.length; i++ ){
        dynamicElementArray[i].style.display = "none";
    }
	
    if ((elementSwitchArray[0].value == 'TCP') ||
        (elementSwitchArray[0].value == 'UDP')){
        dynamicElementArray[6].style.display = "block";     /* OrigPort */
        dynamicElementArray[8].style.display = "block";     /* DestPort */
    }

    if (elementSwitchArray[0].value == 'TCP'){
        dynamicElementArray[13].style.display = "block";     /*TcpFlags */
    }else{
	    dynamicElementArray[13].checked = false;
    }
		
    if (elementSwitchArray[0].value == 'all' && elementSwitchArray[1].value == "IPv4"){
        dynamicElementArray[2].style.display = "none";    /* SetOfALG : Telefonica requirements, close it on D-Link*/
    }else{
        elementSwitchArray[2].checked = false;             
    }
    if ( elementSwitchArray[2].checked ){
        dynamicElementArray[3].style.display = "block";     /* ALG options */
    }else{
        dynamicElementArray[5].style.display = "block";   /* SetOfPacketLength = ALG false */
    }
	

    if (( ! elementSwitchArray[2].checked)){
        dynamicElementArray[4].style.display = "block";   /* SetOfDSCP = ipv4 && ALG false */
    }	
    if((elementSwitchArray[1].value == "IPv6")){
	 dynamicElementArray[14].style.display = "block";
	  dynamicElementArray[4].style.display = "none";
    }		

    if( elementSwitchArray[0].value == 'ICMP' || elementSwitchArray[0].value == 'TCP' 
        || elementSwitchArray[0].value == 'UDP')
	{
	    if (elementSwitchArray[4].value == 'Reject') 
        {
            if ( elementSwitchArray[1].value == "IPv4" )
            {
                dynamicElementArray[0].style.display = "block";     /* ipv4 reject type */
                dynamicElementArray[16].style.display = "none";     /* ipv6 reject type */
            }
            else
            {
                dynamicElementArray[0].style.display = "none";     /* ipv4 reject type */
                dynamicElementArray[16].style.display = "block";     /* ipv6 reject type */
            }
	    }
    }
    
	if( elementSwitchArray[0].value == 'ICMP')
    {
        if (elementSwitchArray[4].value != 'Reject') 
        {
            if ( elementSwitchArray[1].value == "IPv4" )
            {
                dynamicElementArray[1].style.display = "block";     /* icmp type */
            }
            else
            {
                dynamicElementArray[15].style.display = "block";     /* icmp type v6*/
            }
        }
    }
    
    if ( (elementSwitchArray[1].value == "IPv4")
		  || (elementSwitchArray[1].value == "IPv6")){
        dynamicElementArray[7].style.display = "block";      /* hide-origmask */		
    }

    if ( (elementSwitchArray[0].value != 'eigrp') && (elementSwitchArray[0].value != 'ospf') ){
        dynamicElementArray[12].style.display = "block";     /* set of dest */
        if ( elementSwitchArray[3].checked ){
            dynamicElementArray[11].style.display = "block";     /* FQDN enabled*/
			dynamicElementArray[8].style.display = "none";       /* DestPort */
        }else {
            dynamicElementArray[10].style.display = "block";      /* dest IP address = FQDN disabled */
            if ( (elementSwitchArray[1].value == "IPv4")
		          || (elementSwitchArray[1].value == "IPv6")) {
                dynamicElementArray[9].style.display = "block";   /* hide-destmask = ipv4 && FQDN false */
            }
        }
    }
}

function setALGCheckbox( RuleIdx )
{
    var algMatchArray = new Array();
    algMatchArray['ftp']    = 'INPUT_ALG_FTP';
    algMatchArray['tftp']   = 'INPUT_ALG_TFTP';
    algMatchArray['pptp']   = 'INPUT_ALG_PPTP';
    algMatchArray['sip']    = 'INPUT_ALG_SIP';
    algMatchArray['rtsp']   = 'INPUT_ALG_RTSP';
    
    if (RuleIdx == ''){
        var algSetArray = ['ftp','tftp','pptp','sip','rtsp'];
        for (var i = 0; i < algSetArray.length; i++){
            $( algMatchArray[ algSetArray[i] ] ).checked = false;
        }
    }else{  
        /* set checked */
        var algSetArray = (G_Rule[RuleIdx][17] == '') ? '' : G_Rule[RuleIdx][17].split(',');
        for (var i = 0; i < algSetArray.length; i++){
            $( algMatchArray[ algSetArray[i] ] ).checked = true;
        }
    }
}

function setTcpFlagsCheckbox( RuleIdx )
{
    var tcpFlagsMatchArray = new Array();
    tcpFlagsMatchArray['SYN']    = 'INPUT_TCP_SYN';
    tcpFlagsMatchArray['ACK']   = 'INPUT_TCP_ACK';
    tcpFlagsMatchArray['FIN']   = 'INPUT_TCP_FIN';
    tcpFlagsMatchArray['RST']   = 'INPUT_TCP_RST';
    tcpFlagsMatchArray['URG']  = 'INPUT_TCP_URG';
    tcpFlagsMatchArray['PSH']    = 'INPUT_TCP_PSH';
    
    if (RuleIdx == ''){
        var tcpFlagsSetArray = ['SYN','ACK','FIN','RST','URG','PSH'];
        for (var i = 0; i < tcpFlagsSetArray.length; i++){
            $( tcpFlagsMatchArray[ tcpFlagsSetArray[i] ] ).checked = false;
        }
    }else{  
        /* set checked */
        var tcpFlagsSetArray = (G_Rule[RuleIdx][21] == '') ? '' : G_Rule[RuleIdx][21].split(',');
        for (var i = 0; i < tcpFlagsSetArray.length; i++){
            $( tcpFlagsMatchArray[ tcpFlagsSetArray[i] ] ).checked = true;
        }
    }
}

function setProtocolVersionTypeCheckbox(filterIdx)
{
    var protocol;
    for(var t = G_WanConn.length ; t >0; t--){
    	if(G_Firewall[filterIdx][3] == G_WanConn[t-1][0]){
    		if (G_WanConn[t-1][3]=='1')
			 {
    			protocol = 'IPv4';
			 }
			<?if eq $var:G_IPv6Enable 1
			`
    		else if (G_WanConn[t-1][4]=='1')
			{
    			protocol = 'IPv6';
			}
			`?>
    		$('SELECT_IPProtocolVersionType').value = protocol;
    		break;
    	}
	}
	if(t!=0)
	{
		 <?if eq $var:G_IPv6Enable 1
		  `
		if((G_WanConn[t-1][3] == '1') && (G_WanConn[t-1][4] == '1'))
		{
			$('SELECT_IPProtocolVersionType').disabled = false;
		}
		else
		{
			$('SELECT_IPProtocolVersionType').disabled = true;
		}
		`?>
		 <?if eq $var:G_IPv6Enable 0
		  `
			$('SELECT_IPProtocolVersionType').disabled = true;
		`?>
	}
}    
function AddRuleEntry()
{
	addRuleAction = 'add';
	var filterIdx=Form.Radio('Filter_index');
	$('addRuleEntry').disabled=true;
	$('editRuleEntry').disabled=true;
	$('removeRuleEntry').disabled=true;
	
	$('FilterSetting').style.display='none';
	$('FilterEntryInfo').style.display='none';
	$('RuleSetting').style.display='block';
	$('RuleEntryInfo').style.display='block';

	$('Menu').style.height = '980'+ 'px';
	$('Content').style.height = '980' +'px';
    /* clean input */
    setJSONValue({	
					'INPUT_Enabled'                 : '0',
					'SELECT_Protocol'               : 'all',
					'SELECT_IPProtocolVersionType'  : 'IPv4',
					'SELECT_Action_Rule'            : 'Permit',
					'SELECT_IPv4RejectType'         : '',
					<?if eq $var:G_IPv6Enable 1
					`
					'SELECT_IPv6RejectType'         : '',
					`?>
					'SELECT_IcmpType'               : '',
					<?if eq $var:G_IPv6Enable 1
					`
					'SELECT_Icmpv6Type'             : '',
					`?>
					'INPUT_origIPAddress'           : '',
					'INPUT_origMask'                : '',
					'INPUT_origStartPort' 		    : '',
					'INPUT_origEndPort' 		    : '',
					'INPUT_destIPAddress' 		    : '',
					'INPUT_destMask'		        : '',
					'INPUT_destStartPort'		    : '',
					'INPUT_destEndPort'		        : '',
		                    'INPUT_DSCP'                    : '',
		                    'INPUT_TC'                    : '',
		                    'INPUT_packetLengthMin'         : '',
		                    'INPUT_packetLengthMax'         : '',
		                    'INPUT_FQDNEnable'              : '0',
		                    'INPUT_FQDN'                    : '',
		                    'INPUT_ALG_Enable'              : '0'
				});
    setProtocolVersionTypeCheckbox(filterIdx);
    setALGCheckbox('');
	setTcpFlagsCheckbox('');
    dynamicElementSwith();
}

function EditRuleEntry()
{
	var filterIdx=Form.Radio('Filter_index');
	addRuleAction = 'edit';
	editRuleIdx=Form.Radio('rule_index');
	if(G_Rule.length==0||editRuleIdx==undefined){
    	return;
	}

    //setProtocolVersionTypeCheckbox(filterIdx);


	$('addFilterEntry').disabled=true;
	$('addRuleEntry').disabled=true;
	$('editFilterEntry').disabled=true;
	$('removeFilterEntry').disabled=true;
	
	$('FilterSetting').style.display='none';
	$('FilterEntryInfo').style.display='none';
	$('RuleSetting').style.display='block';
	$('RuleEntryInfo').style.display='block';
	$('Menu').style.height = '980'+ 'px';
	$('Content').style.height = '980' +'px';
	
	setJSONValue({	
					'INPUT_Enabled'                 : G_Rule[editRuleIdx][1],
					'SELECT_Protocol'               : G_Rule[editRuleIdx][5],
					'SELECT_IPProtocolVersionType'  : G_Rule[editRuleIdx][16],
					'SELECT_Action_Rule'            : G_Rule[editRuleIdx][4],
					'SELECT_IPv4RejectType'         : (G_Rule[editRuleIdx][16] == 'IPv4')?G_Rule[editRuleIdx][3]:'',
					<?if eq $var:G_IPv6Enable 1
					`
					'SELECT_IPv6RejectType'         : (G_Rule[editRuleIdx][16] == 'IPv6')?G_Rule[editRuleIdx][3]:'',
					`?>
					'SELECT_IcmpType'               : (G_Rule[editRuleIdx][16] == 'IPv4')?G_Rule[editRuleIdx][2]:'',
					<?if eq $var:G_IPv6Enable 1
					`
					'SELECT_Icmpv6Type'             : (G_Rule[editRuleIdx][16] == 'IPv6')?G_Rule[editRuleIdx][2]:'',
					`?>
					'INPUT_origIPAddress'           : G_Rule[editRuleIdx][6],
					'INPUT_origMask'                : G_Rule[editRuleIdx][7],
					'INPUT_origStartPort' 		    : G_Rule[editRuleIdx][8],
					'INPUT_origEndPort' 		    : G_Rule[editRuleIdx][9],
					'INPUT_destIPAddress' 		    : G_Rule[editRuleIdx][10],
					'INPUT_destMask'		        : G_Rule[editRuleIdx][11],
					'INPUT_destStartPort'		    : G_Rule[editRuleIdx][12],
					'INPUT_destEndPort'		        : G_Rule[editRuleIdx][13],
		                    'INPUT_DSCP'                    : G_Rule[editRuleIdx][18],
		                    'INPUT_TC'			      : G_Rule[editRuleIdx][18],
		                    'INPUT_packetLengthMin'         : G_Rule[editRuleIdx][19],
		                    'INPUT_packetLengthMax'         : G_Rule[editRuleIdx][20],
		                    'INPUT_FQDNEnable'              : G_Rule[editRuleIdx][22],
		                    'INPUT_FQDN'                    : G_Rule[editRuleIdx][23],
		                    'INPUT_ALG_Enable'              : G_Rule[editRuleIdx][24]
				});
    setALGCheckbox(editRuleIdx);
	setTcpFlagsCheckbox(editRuleIdx);
    dynamicElementSwith();
   	$('SELECT_IPProtocolVersionType').disabled = true; /*can't edit IP version type value*/

}

function RemoveRuleEntry()
{
	if(!confirm(SEcode[1008])){return false;}

	var Index=Form.Radio('rule_index');

	if(Index==undefined){
		$('removeRuleEntry').disabled=true;
		return;
	}	
	
	$H({
		'del-obj'   :G_Rule[Index][0],
		'obj-action':'del',
		'var:menu'  :'advanced',
		'var:page'  :'filtering_options',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:subpage'   :'tf_ipsecurity',
		'var:errorpage':'tf_ipsecurity'
	});
	$('uiPostForm').submit();
}

function uiRuleSubmit()
{
    var filterIdx=Form.Radio('Filter_index');
    var Protocol = $('SELECT_Protocol').value;      // tcp, udp, icmp
	var IpProtocol = $('SELECT_IPProtocolVersionType').value;   // ipv4, ipv6
    var path = '';

    var RejectType = (IpProtocol == 'IPv4')? $('SELECT_IPv4RejectType').value : $('SELECT_IPv6RejectType').value;
    var IcmpType = (Protocol == 'ICMP')? ( (IpProtocol == 'IPv4')? $('SELECT_IcmpType').value : $('SELECT_Icmpv6Type').value ) : '';
    var ALGEnable =  Form.Checkbox('INPUT_ALG_Enable');

    /*rule �����Ϸ�����֤*/
	if (isValidInput() == false)
		return;

    /*checked RejectType*/
    if (($("SELECT_Action_Rule").value == 'Reject') && (Protocol != 'TCP') && (RejectType == 'tcp-reset'))
    {
        alert('UDP and ICMP protocol can not support tcp-reset.');
        return false;
    }
    
    if ((Protocol == 'UDP') || (Protocol == 'TCP')){
        var OrigPortStart = $('INPUT_origStartPort').value;
        var OrigPortEnd   = $('INPUT_origEndPort').value;
        var DestPortStart = $('INPUT_destStartPort').value;
        var DestPortEnd   = $('INPUT_destEndPort').value;
    
    }else{
        var OrigPortStart = '';
        var OrigPortEnd   = '';
        var DestPortStart = '';
        var DestPortEnd   = ''; 
    }

    if ((Protocol == 'eigrp') || (Protocol == 'OSPF')){
        var DestMask      = '';
        var DestIP        = '';
        var DestFQDN      = '';
        var DestFQDNEnable = '0';
    }else{
        var DestMask      = (Form.Checkbox('INPUT_FQDNEnable'))? ('') : ($('INPUT_destMask').value);
        var DestIP        = (Form.Checkbox('INPUT_FQDNEnable') ? '' :$('INPUT_destIPAddress').value);
        var DestFQDN      = (Form.Checkbox('INPUT_FQDNEnable') ? $('INPUT_FQDN').value : '');
        var DestFQDNEnable = Form.Checkbox('INPUT_FQDNEnable');
    }
	
    /*creat ALG's checkbox element array */
    var arrayOfALG = $('INPUT_ALG_FTP','INPUT_ALG_TFTP','INPUT_ALG_PPTP',
                       'INPUT_ALG_SIP', 'INPUT_ALG_RTSP'); 
    for (var i=0; i < arrayOfALG.length;){
        if ( arrayOfALG[i].checked ){
            arrayOfALG[i] = arrayOfALG[i].value;
            i++
        }else
            arrayOfALG.splice(i,1);  
    }

    /*creat TcpFlags's checkbox element array */
    var arrayOfTcpFlags = $('INPUT_TCP_SYN', 'INPUT_TCP_ACK', 'INPUT_TCP_FIN', 'INPUT_TCP_RST',
		                     'INPUT_TCP_URG', 'INPUT_TCP_PSH');
    for (var i=0; i < arrayOfTcpFlags.length;){
        if ( arrayOfTcpFlags[i].checked ){
            arrayOfTcpFlags[i] = arrayOfTcpFlags[i].value;
            i++
        }else
            arrayOfTcpFlags.splice(i,1);  
    }
	
	for(var t = G_WanConn.length ; t >0; t--){
		if(G_Firewall[filterIdx][3] == G_WanConn[t-1][0]){
			break;
		}
	}

    /*
	if(($("SELECT_Action_Rule").value == 'Reject') && ($('SELECT_Protocol').value == 'UDP')){
		alert("'UDP' and 'Reject' can not be configured");
		return false;
	}*/
     
    if(addRuleAction=='add'){
		$H({
			'add-obj' 	  	    : G_Firewall[filterIdx][0]+'Rule.',	
			'obj-action' 		: 'add-set',
			'getpage'           : 'html/index.html',
			'errorpage'         : 'html/index.html',
			'var:menu'          : 'advanced',
			'var:subpage'       : 'tf_ipsecurity',
			'var:page'          : 'filtering_options',
			'var:errorpage'     : 'tf_ipsecurity',
			'var:CacheLastData' : ViewState.Save()
		},true);
        path = ':';

	}else if(addRuleAction=='edit'){
	    var ruleIdx=Form.Radio('rule_index');
		$H({
            'add-obj' 	  	    : G_Rule[ruleIdx][0],	
			'obj-action' 		: 'set',
			'getpage'           : 'html/index.html',
			'errorpage'         : 'html/index.html',
			'var:menu'          : 'advanced',
			'var:subpage'       : 'tf_ipsecurity',
			'var:page'          : 'filtering_options',
			'var:errorpage'     : 'tf_ipsecurity',
			'var:CacheLastData' : ViewState.Save()
		},true);
        path = ':' + G_Rule[ruleIdx][0];		
	}
		
    $F(path+'Enabled',Form.Checkbox('INPUT_Enabled'));
 	  $F(path+'Protocol',$('SELECT_Protocol').value);
	  $F(path+'IPProtocolVersionType' ,IpProtocol);
    $F(path+'AlgEnable'             , ALGEnable );
    $F(path+'Alg'                   , ( ALGEnable ? arrayOfALG.join(',') : '' ) );
    if(IpProtocol == 'IPv4')
    {
    	    $F(path+'DSCP'          , ( ALGEnable ? '' : $('INPUT_DSCP').value ) );	
    }
    else
    {
            $F(path+'DSCP'            ,( ALGEnable ? '' : $('INPUT_TC').value ) );
    }
    $F(path+'PacketLengthMin'       , ( ALGEnable ? '' : $('INPUT_packetLengthMin').value ) );
    $F(path+'PacketLengthMax'       , ( ALGEnable ? '' : $('INPUT_packetLengthMax').value ) );
    $F(path+'TcpFlags'              , ( ALGEnable ? '' : arrayOfTcpFlags.join(',') ));
	  $F(path+'IcmpType'              , IcmpType);
	  $F(path+'RejectType'            , RejectType);
	  $F(path+'Action'                , $('SELECT_Action_Rule').value);
	  $F(path+'Origin.IPAddress'      , $('INPUT_origIPAddress').value);
	  $F(path+'Origin.Mask'           , $('INPUT_origMask').value);
	  $F(path+'Origin.StartPort'      , OrigPortStart );
	  $F(path+'Origin.EndPort'        , OrigPortEnd );
    $F(path+'Destination.FQDNEnable', DestFQDNEnable );
    $F(path+'Destination.FQDN'      , DestFQDN );
	  $F(path+'Destination.IPAddress' , DestIP );
	  $F(path+'Destination.Mask'      , DestMask );
	  $F(path+'Destination.StartPort' , DestPortStart );
	  $F(path+'Destination.EndPort'   , DestPortEnd );	

	  $('uiPostForm').submit();
	  addFilterAction=0;
}

function uiRuleCancle()
{
	addRuleAction = '';

	$('addRuleEntry').disabled=false;
	$('editRuleEntry').disabled=false;
	$('removeRuleEntry').disabled=false;
	$('addFilterEntry').disabled=false;
	$('editFilterEntry').disabled=false;
	$('removeFilterEntry').disabled=false;
	
	$('FilterSetting').style.display='block';
	$('FilterEntryInfo').style.display='none';
	$('RuleSetting').style.display='block';
	$('RuleEntryInfo').style.display='none';
}
function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}


addListeners(uiOnload, dealWithError);



