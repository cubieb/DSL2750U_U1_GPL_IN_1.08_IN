//get route info
var G_IPv6RouteInfo = new Array();
var m = 0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_IPv6RouteInfo. "Destination NextHop Flags Metric Ref Use Iface"
`	G_IPv6RouteInfo[m] = new Array();
	G_IPv6RouteInfo[m][0] = "$01"; //Destination
	G_IPv6RouteInfo[m][1] = "$02"; //NextHop
	G_IPv6RouteInfo[m][2] = "$03"; //Flags
	G_IPv6RouteInfo[m][3] = "$04"; //Metric
	G_IPv6RouteInfo[m][4] = "$05"; //Ref
	G_IPv6RouteInfo[m][5] = "$06"; //Use
	G_IPv6RouteInfo[m][6] = "$07"; //Iface
	G_IPv6RouteInfo[m][7] = "$00"; //index
	m++;
`?>

function uiOnload()
{
	var array_value = [];
	for (var i = 0; i < G_IPv6RouteInfo.length; i++)
	{
		array_value[i] = [G_IPv6RouteInfo[i][0],
					      G_IPv6RouteInfo[i][1],
					      G_IPv6RouteInfo[i][2],
					      G_IPv6RouteInfo[i][3],
					      G_IPv6RouteInfo[i][4],
					      G_IPv6RouteInfo[i][5],
					      G_IPv6RouteInfo[i][6]];
	}
	$T('td_routeinfo', array_value);
}

addListeners(uiOnload);
