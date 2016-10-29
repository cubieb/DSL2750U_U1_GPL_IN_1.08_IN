var data_language = {
	
	innerHTML : {
		lang_syslog : 'Registro del Sistema',
		lang_dsc1 : 'Si el registro está habilitado, el sistema comienza a registrar todos los eventos seleccionados. Si el modo está ajustado en "remoto", los registros se envían a la dirección IP y el puerto UDP de un servidor syslog remoto. Si el modo está establecido en "Local", los registros se registran en el equipo local. Si se establece "Ambos", los registros se guardan en el equipo local y el servidor syslog remoto.',
		lang_dsc2 : 'Seleccione los valores apropiados y haga clic en "Aplicar" para guardar la configuración de registro del sistema.',
		lang_log_note : 'Nota: Esto no funciona correctamente si el tiempo en el módem no está configurado correctamente. En este caso, por favor, defina el tiempo del módem en "Configuración/hora y fecha".',
		lang_SlogCfg : 'Registro del Sistema -- Configuración',
		lang_Enable : 'Habilitar registro',
		inputmode : 'Modo :',
		serveripaddr : 'Dirección IP del servidor :',
		severudpport : 'Puerto UDP del Servidor :',
		lang_helph1 : 'Consejos Útiles ...',
		lang_helpP : 'En esta página, usted puede activar, configurar y ver el registro del sistema del enrutador. Las actividades de registro del sistema de registro en el enrutador. Sólo un número limitado de entradas de registro se pueden guardar, dependiendo de los detalles de registro y la memoria del enrutador. Si usted tiene un servidor syslog externo, puede configurar el registro exterior y todas las entradas de registro se envían al servidor remoto.',
		lang_helpH : 'Más...'
	},
	value : {
		INPUT_cancel : 'Aplicar',
		INPUT_cance2 : 'Cancelar',
		INPUT_apply : 'Ver Registro del Sistema',
		INPUT_FIREWALL_apply : 'Ver registro de Firewall'
	},
	option_text:{
		INPUT_logmode : ['Local','Remoto',' Ambos']
	}
}