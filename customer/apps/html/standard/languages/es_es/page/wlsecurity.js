var data_language = {
	  innerHTML : {
		lang_config            : 'Seguridad inalámbrica',
		lang_config1           : 'Modo de seguridad Inalámbrica',
		lang_introduction      : 'En esta página, puede configurar los parámetros de seguridad inalámbrica del enrutador. Tenga en cuenta que los cambios realizados en esta página también se deben duplicar para sus clientes en móviles y PC.',
		lang_introduction1     : 'Para proteger su privacidad, puede configurar las características de seguridad inalámbrica. El dispositivo es compatible con 3 modos de seguridad inalámbricos, incluyendo: WEP, WPA, y WPA2. WEP es el estándar original de encriptación inalámbrica. WPA y WPA2 proporcionan mayores niveles de seguridad.',
		lang_note1             : 'Recuerde que su SSID y la clave de seguridad tendrán que configurar los mismos valores en sus dispositivos móviles y PC.',
		lang_mode              : 'Modo de seguridad :',
		lang_WEP_introduction  : 'Si selecciona WEP, el dispositivo opera SOLO en el legado de modo inalámbrico (802.11B/G).',
		lang_WEP_introduction1 : 'WEP es el estándar de cifrado inalámbrico. Para usarlo, se debe introducir la misma clave(s) en el enrutador y las estaciones inalámbricas. Una clave de 64 bits se compone de 10 dígitos hexadecimales y una clave de 128 bits se compone de 26 dígitos hexadecimales. Un dígito hexadecimal es un número del 0 al 9 o una letra de la A a la F. Para uso más seguro de WEP, establezca el tipo de autenticación " Clave compartida ".',
		lang_WEP_introduction2 : 'Usted también puede entrar en cualquier cadena de texto en un cuadro de clave WEP, en cuyo caso se convierte en una clave hexadecimal utilizando los valores ASCII de los caracteres. Un máximo de 5 caracteres de texto puede ser introducido para claves de 64 bits, y un máximo de 13 caracteres para claves de 128 bits',
		lang_WEP_length        : 'Longitud de clave WEP :',
		lang_WEP_choice        : 'Elija clave WEP :',
		lang_WEP_key1          : 'WEP Key1 :',
		lang_WEP_key2          : 'WEP Key2 :',
		lang_WEP_key3          : 'WEP Key3 :',
		lang_WEP_key4          : 'WEP Key4 :',
		lang_WEP_auth          : 'Autenticación :',
		lang_WPA_introduction  : 'Seleccione WPA o WPA2 para lograr un equilibrio de seguridad fuerte y mejor compatibilidad. Este modo utiliza WPA para clientes heredados, mientras que mantienede una mayor seguridad para las estaciones que son capaces para WPA2. El sistema de cifrado más fuerte que el cliente admite se utiliza. Para mayor seguridad, seleccione Sólo WPA2. Este modo utiliza AES (CCMP) de cifrado y a las estaciones existentes no se les permite acceder con seguridad WPA. Para la máxima compatibilidad, seleccione WPA solamente. Este modo utiliza cifrado TKIP. Algunos dispositivos de juego y el legado trabajan sólo en este modo.',
		lang_WPA_introduction1 : 'Para lograr un mejor rendimiento inalámbrico, seleccione sólo WPA2 (que utiliza el cifrado AES).',
		lang_WPA_introduction2 : 'WPA-PSK no requiere un servidor de autenticación. La opción WPA requiere un servidor RADIUS externo.',
		lang_WPA_introduction3 : 'Cuando la empresa WPA está habilitada, el enrutador utiliza EAP (802.1x) para autenticar los clientes a través de un servidor RADIUS remoto.',
		lang_WPA_mode          : 'Modo WPA :',
		lang_time_interval     : 'Intervalo de actualización de la clave de Grupo :',
		lang_shard_key         : 'Clave precompartida',
		lang_shard_key1        : 'Clave precompartida :',
		lang_radius_IP         : 'Dirección IP del servidor RADIUS :',
		lang_radius_port       : 'Puerto del servidor RADIUS :',
		lang_radius_secret     : 'Secreto Compartido del Servidor RADIUS :',
		lang_help_hint         : 'Consejos Útiles ...',
		lang_help1             : 'Si habilita la seguridad inalámbrica, asegúrese de que usted recuerde la clave de cifrado configurado. Es necesario introducir la información en cualquier dispositivo inalámbrico que se conecta a la red inalámbrica.',
		lang_WirelessSSID      : 'SSID inalámbrico',
		lang_SelectSSID        : 'Seleccionar SSID :',
		lang_more              : 'Más...',
		//lang_secure_mode       : 'Cifrado WPA :',
		lang_wpa               : 'WPA Encriptación :'
	},
	
	value : {
		Apply                  : 'Aplicar',
		Cancel                 : 'Cancelar'
	},
	
	option_text:{ 
		SELECT_WPAMode         : ['WPA-PSK','WPA-Empresa'],
		SELECT_EncryptionModes : ['TKIP','AES','TKIP+AES'],
		SELECT_Mode            : ['None','WEP','Auto(WPA o WPA2)','solo WPA2','solo WPA'],
		SELECT_WEPIndex        : ['64 bits(10 dígitos hexadecimales o 5carácteres)', '128 Bits (26 dígitos hexadecimales o 13 caractéres)'],
		SELECT_Auth            : ['Abrir','Compartir clave']
	},
	dynamic : {
		auto                   : 'Auto(WPA o WPA2)'
	}

}



