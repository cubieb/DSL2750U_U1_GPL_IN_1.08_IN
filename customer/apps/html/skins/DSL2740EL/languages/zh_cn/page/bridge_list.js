var data_language = {

	innerHTML : {
		lang_bridgefilter : '桥过滤',
		lang_dsc1 : '仅在桥接模式下，桥过滤对ATM PVCs 有效。“允许”表示除匹配以下表中指定规则的二层帧外的所有数据帧均允许通过。“阻止”表示除匹配以下表中指定规则的二层帧外的所有数据帧均不允许通过。',
		lang_dsc2 : '通过指定下面至少一个条件创建过滤器来标识MAC层帧。如果指定多个条件，所有的条件将生效，点击“应用”按钮保存并生效过滤器。',
		lang_dsc3 : '警告：改变全局策略将导致所指定的规则自动被移除。您需要为新的策略创建新的规则。',
		lang_BridgeFilteringGlobal : '桥过滤总体策略',
		lang_ALLOW1 : '允许',
		lang_allpacketsbut : '所有的帧但是',
		lang_DENY1 : '阻止',
		lang_thosematching1 : '与选定规则匹配的MAC帧。',
		lang_DENY2 : '阻止',
		lang_allpackets2 : '阻止所有的帧但是',
		lang_ALLOW2 : '允许',
		lang_thosematching : '与选定规则匹配的MAC帧。',
		lang_displaylist : '列表',
		lang_protocol : '协议',
		lang_AddBridgeFilte : '添加桥过滤器',
		lang_ProtocolType : '协议类型：',
		lang_DestinationMACAddr : '目的MAC地址：',
		lang_ourceMACAddr : '源MAC地址：',
		lang_FrameDirect : '帧流向：',
		lang_Timeschedule : '时间规则：',
		lang_ViewAvailable : '查看时间表',
		lang_Wannterface : 'WAN接口：',
		lang_help : '帮助...',
		lang_helpdsc : '根据您选择的总体策略，创建一条MAC地址规则，以阻止或允许网络访问。',
		lang_DMAC : '目的MAC',
		lang_SMAC : '源MAC',
		lang_DIR : '方向',
		lang_TIME : '时间规则',
		lang_help_more : '更多...'
	},
	
	value : {
		lang_Apply : '应用',
		lang_uiPageRefresh : '取消',
		lang_Add : '添加',
		lang_Edit : '编辑',
		lang_Delete : '删除',
		lang_Apply2 : '应用',
		lang_Cancel2 : '取消'
	},
	 option_text:{
     protocol : ['点击选择以下选项'],
	 direction : ['WAN到LAN','LAN到WAN']
    },
	
	dynamic : 
	{
		always: '总是',
		never:   '从不',
		select_all_wan: '选择所有接口'
	}

}