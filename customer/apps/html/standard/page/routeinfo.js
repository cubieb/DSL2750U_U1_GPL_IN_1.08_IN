//get route info
var G_RouteInfo = new Array();
var m = 0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_RouteInfo. "DestIPAddress GatewayIPAddress DestSubnetMask  Flags Ref ForwardingMetric Interface"
	`	G_RouteInfo[m] = new Array();
		G_RouteInfo[m][0] = "$01"; //DestIPAddress
		G_RouteInfo[m][1] = "$02"; //GatewayIPAddress
		G_RouteInfo[m][2] = "$03"; //DestSubnetMask
		G_RouteInfo[m][3] = "$04"; //Flags
		G_RouteInfo[m][4] = "$05"; //Ref
		G_RouteInfo[m][5] = "$06"; //ForwardingMetric
		G_RouteInfo[m][6] = "$07"; //Interface
		G_RouteInfo[m][7] = "$00"; //index
		m++;
	`?>
		
function uiOnload()
{
	var array_value = [];
	for(var i = 0; i < G_RouteInfo.length; i++)
	{
		array_value[i] = [G_RouteInfo[i][0],
					      G_RouteInfo[i][1],
					      G_RouteInfo[i][2],
					      G_RouteInfo[i][3],
					      G_RouteInfo[i][5],
					      G_RouteInfo[i][4],
					      G_RouteInfo[i][6]];
	}
	$T('td_routeinfo',array_value);
}

addListeners(uiOnload);
