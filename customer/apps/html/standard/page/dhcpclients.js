//lan clients --
var LanHosts = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice. "Hosts.HostNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.LANDevice.$20.Hosts.Host. "HostName MACAddress IPAddress LeaseTimeRemaining VendorClassID AddressSource"
             `	<?if eq `DHCP` `<?echo $26?>` 	
		`	LanHosts[m] = [];
				LanHosts[m][0] = "<?echo $21?>";
				LanHosts[m][1] = "<?echo $22?>";
				LanHosts[m][2] = "<?echo $23?>";
				LanHosts[m][3] = "<?echo $24?>";
			++m;
			`?>
		`?>
	`?>
`?>

function uiOnload(){	
	$T('td_lanClients',LanHosts);
}

addListeners(uiOnload);
