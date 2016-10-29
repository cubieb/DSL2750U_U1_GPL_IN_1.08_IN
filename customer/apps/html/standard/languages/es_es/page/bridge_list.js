var data_language = {
		innerHTML : {
		lang_bridgefilter : 'Puente de filtrado',
		lang_dsc1 : 'El Puente de filtrado sólo es eficaz en ATM PVCs configurado en modo Puente. PERMITIR significa que todos los marcos de la capa MAC se pueden transmitir. NEGAR significa que todos los marcos de la capa MAC, excepto aquellos que se ajusten a una norma de la siguiente lista no se pueden transmitir.',
		lang_dsc2 : 'Especifique al menos una condición para crear un filtro para identificar los marcos de capa MAC. Si especifica una serie de condiciones, todas ellas toman efecto de forma simultánea. Haga clic en "Aplicar" para guardar el filtro y habilitarlo.',
		lang_dsc3 : 'ADVERTENCIA: El cambio de una política global a otra elimina automáticamente todas las reglas existentes. Usted tendrá que crear nuevas reglas para la nueva política.',
		lang_BridgeFilteringGlobal : 'Puente de filtrado de Política Global:',
		lang_ALLOW1 : 'PERMITIR',
		lang_allpacketsbut : 'todos los paquetes, excepto',
		lang_DENY1 : 'NEGAR',
		lang_thosematching1 : 'Las direcciones MAC que coincidan con una regla en la lista',
		lang_DENY2 : 'NEGAR',
		lang_allpackets2 : 'todos los paquetes, excepto',
		lang_ALLOW2 : 'PERMITIR',
		lang_thosematching : 'Las direcciones MAC que coincidan con una regla en la lista',
		lang_displaylist : 'mostrar lista',
		lang_protocol : 'protocolo',
		lang_TIME     : 'HORA',
		lang_AddBridgeFilte : 'Añadir Filtro de Puente',
		lang_ProtocolType : 'Tipo de protocolo:',
		lang_DestinationMACAddr : 'Dirección MAC de destino:',
		lang_ourceMACAddr : 'Fuente de direcciones MAC:',
		lang_userPriority : 'Prioridad de usuario:',
		//lang_vlanID      :'vlanID:',
		lang_FrameDirect : 'Marco de Dirección:',
		lang_Timeschedule : 'Itinerario:',
		lang_ViewAvailable : 'Ver horarios disponibles',
		lang_Wannterface : 'Interfaz Wan:',
		lang_help : 'Consejos Útiles ...',
		lang_helpdsc : 'Crear una lista de direcciones MAC a la que desea permitir o denegar el acceso a la red en función de la política global actual.',
		lang_DMAC : 'DMAC',
		lang_SMAC : 'SMAC',
		lang_DIR : 'DIR',
		lang_help_more : 'Más...'
	},
	
	value : {
		lang_Apply : 'Aplicar',
		lang_uiPageRefresh : 'Cancelar',
		lang_Add : 'Agregar',
		lang_Edit : 'Editar',
		lang_Delete : 'Borrar',
		lang_Apply2 : 'Aplicar',
		lang_Cancel2 : 'Cancelar'
	},
	option_text:{
		protocol : ['(Haga clic para Seleccionar)']
	},
    
	dynamic : 
	{
		always: 'Siempre',
		never:   'Nunca',
		select_all_wan: 'Seleccione todas las interfaces'
	}
}