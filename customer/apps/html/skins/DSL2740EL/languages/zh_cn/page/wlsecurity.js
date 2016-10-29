var data_language = {
	innerHTML : {
		lang_config: '无线安全配置',
		lang_config1 : '无线安全模式',
		lang_introduction : '使用本页设置您路由器的无线安全配置。请注意：本页的修改需要对应到您无线终端的修改。',
		lang_introduction1 : '使用无线安全特性可以保护您的隐私。路由器支持三种加密模式：WEP、WPA和WPA2，WEP是原始的无线加密标准，WPA和WPA2提供一种更高级的加密。',
		lang_note : 'WPS:WPS连接，必须选中WPA-PSK/WPA2-PSK 加密模式，并且广播SSID。',
		lang_note1 : '请注意：你需要重复设置相同的设置到您的无线终端。',
		lang_mode : '加密模式：',
		lang_WEP_introduction : '如果您选择WEP加密模式，路由器将<b>仅仅</b>工作在<b>传统无线模式（802.11B/G）</b>。',
		lang_WEP_introduction1 : 'WEP是一种无线加密标准。使用WEP，您必须在路由器和无线终端上输入相同的密钥。选中64比特时，您需要输入10个16进制数字；选中128比特时，您需要输入26个16进制数字。一个16进制数字包括数字0到9，字母A到F。使用WEP最安全的验证方式是使用“共享密钥”。',
		lang_WEP_introduction2 : '您也可以在WEP密钥框中输入任何字符串，字符串通过ASCII码将会被自动转换成16进制密钥。64比特密钥必须输入5个字符；128比特密钥则需要输入13个字符。',
		lang_WEP_length : 'WEP密钥长度：',
		lang_WEP_choice : '选择WEP密钥：',
		lang_WEP_key1 : 'WEP密钥1：',
		lang_WEP_key2 : 'WEP密钥2：',
		lang_WEP_key3 : 'WEP密钥3：',
		lang_WEP_key4 : 'WEP密钥4：',
		lang_WEP_auth : '认证：',
		lang_WPA_introduction : '使用<b>WPA或WPA2</b>以实现高安全性与高兼容性的一种这种模式为传统的客户端提供WPA加密，当检测到客户端具有更高的安全模式时，这种模式具有提供WPA2的能力。使用<b>仅使用WPA2</b>能提供最高的安全性。',
		lang_WPA_introduction1 : '为了达到比较好的加密效果，请选择<b>仅使用WPA2</b>加密模式。',
		lang_WPA_introduction2 : 'WPA-PSK不请求认证服务器，WPA请求一个外部的RADIUS服务器。',
		lang_WPA_introduction3 : '当使用WPA加密时，路由器通过远程RADIUS服务器使用EAP(802.x)认证客户端。',
		lang_WPA_mode : 'WPA模式：',
		lang_WPS_enabled : '使用：',
		lang_time_interval : '密钥更新间隔：',
		lang_shard_key : '共享密钥',
		lang_shard_key1 : '共享密钥：',
		lang_radius_IP : 'RADIUS服务器IP地址：',
		lang_radius_port : 'RADIUS服务器端口：',
		lang_radius_secret : 'RADIUS服务器共享密钥：',
		lang_WPAEncryption : 'WPA加密模式：',
		lang_help_hint : '帮助...',
		lang_help1 : '在使用无线加密之前，请确保写下已经配置的<b>密钥</b>。每台想连接到路由器的无线终端都必须输入该密钥。',
		lang_WirelessSSID : '无线SSID',
		lang_SelectSSID : '选择SSID：',
		lang_more : '更多...'
	},
	
	value : {
		Apply : '应用',
		Cancel : '取消'
	},
	
	option_text:{
		SELECT_Mode : ['不加密','WEP','自动（WPA或WPA2）','仅使用WPA2','仅使用WPA'],
		SELECT_WEPIndex : ['64比特（10个16进制或5个字符）','128比特（26个16进制或13个字符）'],
		SELECT_Auth : ['公开','共享密钥']
	},
	dynamic : {
		atuo : '自动（WPA或者WPA2）'
	}

}

