var data_language = {

	innerHTML : {
		lang_IncomingIPFiltering : '进站IP过滤',
		lang_dsc1 : '该页面用于创建进站IP过滤规则，通过为过滤规则定义一个“过滤器名”来标识一条规则。规则至少需选一个以下过滤条件。点击“应用”按钮保存、生效过滤规则。',
		lang_dsc2 : '默认情况下,当防火墙启用时，所有WAN侧的输入流将通过，但通过设置过滤规则，相应IP流将被阻塞。',
		lang_ActiveInboundFilter : '生效进站过滤器',
		lang_Name : '名称',
		lang_VPIVCI : 'VPI/VCI',
		lang_wwProtocol : '协议',
		lang_urceAddress : '源IP地址',
		lang_DeAddres : '目的IP地址',
		lang_DePort : '目的端口',
		lang_ourcePort : '源端口',
		lang_SchedulRule : '时间表规则',
		lang_IncomingIPFiltering1 : '进站IP流过滤',
		lang_FilterName : '过滤器名：',
		lang_Protocol : '协议：',
		lang_SourceIPType : '源IP类型：',
		lang_SourceIPAddress : '源IP地址：',
		lang_SourceSubnetMask : '源子网掩码：',
		lang_StartSourceIPAd : '源IP开始地址：',
		lang_EndSourceIPAd : '源ip结束地址：',
		lang_SourcePortType : '源端口类型：',
		lang_SourcePort : '源端口：',
		lang_DestinationIPType : '目的IP类型：',
		lang_DestinationIP : '目的IP地址：',
		lang_DestinationSubnet : '目的子网掩码：',
		lang_StartDestination : '目的IP开始地址：',
		lang_EndDestination : '目的IP结束地址：',
		lang_DestinationPortType : '目的端口类型：',
		lang_DestinationPort : '目的端口：',
		lang_Schedule1 : '时间表：',
		lang_ViewAvailable : '查看时间表',
		lang_WANInterfacesConfigured : 'WAN接口（仅应用于配置为路由模式并且防火墙功能启用）',
	    lang_WANInterfaces : 'WAN接口:',
		lang_help : '帮助...',
		lang_helpdsc1 : '给每条规则定义一个名字很重要。',
		lang_helpdsc2 : '源IP地址为WAN侧地址，目的IP地址为LAN侧IP地址。',
		lang_helpdsc3 : '每条规则允许WAN侧访问。',
		lang_helpdsc4 : '当启用规则时，需选择一条时间表规则。如果没有您需要的时间表规则，请跳转到<a 置 <a style="color:#E29861;" href="/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=schedules"> 高级 -> 时间表</a>页面设置时间表规则。',
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
		ancel : '取消'
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