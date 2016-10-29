
G_Error = "<?error found?>"; 
var G_Conns_DSL = [];
var G_Conns_Eth = [];
var Eth_num_ajax = 0;
var DSL_num_ajax = 0;

<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
	`	
	<?if gt $11 0
	`	
		<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
		`	
			G_Conns_DSL[DSL_num_ajax] = [ "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00."
									];
			DSL_num_ajax++;
		`?>
	`?>
	<?if gt $12 0
	`	
		<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
		`	
			G_Conns_DSL[DSL_num_ajax] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00."
									];
			DSL_num_ajax++;
		`?>
	`?>
`?>
<?if eq 1 $var:mod_ETHWAN
`	
	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`	
		<?if gt $11 0
		`	
			<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
			`	
				G_Conns_Eth[Eth_num_ajax] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00."
										];
				Eth_num_ajax++;
			`?>
		`?>
		<?if gt $12 0
		`	
			<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
			`	
				G_Conns_Eth[Eth_num_ajax] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00."
										];
				Eth_num_ajax++;
			`?>
		`?>
	`?>
`?>
