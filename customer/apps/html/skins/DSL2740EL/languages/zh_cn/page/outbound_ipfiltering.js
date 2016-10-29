var data_language = {

	innerHTML : {
		lang_OutcomingIPFiltering : '出站IP过滤',
		lang_dsc1 : '该页面用于创建进站IP过滤规则，通过为过滤规则定义一个“过滤器名”来标识一条规则。规则至少需选一个以下过滤条件。点击“应用”按钮保存、生效过滤规则。',
		lang_dsc2 : '警告：改变全局策略将导致配置的规则自动被删除，您需要为新的策略创建新的规则。',
		lang_dsc3 : '默认情况下,所有LAN侧的输出流未阻塞，但通过设置过滤规则，对应IP流将阻塞。',
		lang_ActiveInboundFilter : '生效出站过滤器',
		lang_Name : '名称',
		lang_Protocol7 : '协议',
		lang_SourceAddress : '源IP地址',
		lang_DestAddress : '目的IP地址',
		lang_DestPort : '目的端口',
		lang_SourcePort1 : '源端口：',
		lang_Scheduleule : '时间表规则',
		lang_outcomingPFiltering : '出站IP流过滤',
		lang_FilterName : '过滤器名：',
		lang_Protocol : '协议：',
		lang_SourceIPType : '源IP类型：',
		lang_SourceIPAddress : '源IP地址：',
		lang_Sourceubnetask : '源子网掩码：',
		lang_StartourceIPAddr : '源IP开始地址：',
		lang_EndSourceddress : '源ip结束地址：',
		lang_SourcePortype : '源端口类型：',
		lang_SourcePort : '源端口',
		lang_DestinationIPTyp : '目的IP类型：',
		lang_DestinationIPAd : '目的IP地址：',
		lang_DestinationSubnetM : '目的子网掩码：',
		lang_StartDestinationIA : '目的IP开始地址：',
		lang_EndDestinationIPA : '目的IP结束地址：',
		lang_DestinationPortTyp : '目的端口类型：',
		lang_DestinationPort : '目的端口：',
		lang_Schedule1 : '时间表：',
		lang_ViewAvailableSche : '查看时间表',
		lang_help : '帮助...',
		lang_helpdsc1 : '给每条规则定义一个名字很重要。',
		lang_helpdsc2 : '源IP地址为LAN侧地址，目的IP地址为WAN侧地址。',
		lang_helpdsc3 : '每条规则阻塞LAN侧输出流。',
		lang_helpdsc4 : '当规则生效时，为该规则选择时间表规则。如果没有您需要的时间表规则，您可以跳转到<a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=schedules" style="color:#E29861;">高级 -> 时间表 </a>页面设置时间表规则。',
		lang_helpdsc5 : '点击“应用”按钮保存您完成的规则设置。',
		lang_helpdsc6 : '在规则表中选中某条规则，并点击“删除”按钮永久删除规则。',
		lang_nihao : '（端口 或 端口：端口）',
		lang_nihao1 : '（端口 或 端口：端口）',
		lang_help_more : '更多...'
	},
	
	value : {
		addEntry : '添加',
		editEntry : '编辑',
		removeEntry : '删除',
		submit : '应用',
		CancelEdit : '取消'
	},
	option_text:{
     SELECT_protocol : ['任意','TCP&UDP','TCP'],
	 SELECT_SrcIP : ['任意','单IP','网络IP','IP范围'],
	 SELECT_SrcPortType : ['任意','单端口','端口范围'],
	 SELECT_DestIP : ['任意','单IP','网络IP','IP范围'],
	 SELECT_DestPortType : ['任意','单端口','端口范围']
	},
	dynamic : 
	{
		always: '总是',
		never:   '从不'
	}
}