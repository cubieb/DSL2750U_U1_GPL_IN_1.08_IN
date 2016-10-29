var data_language = {

	innerHTML : {
		lang_BlockMACAddress 	: 'MAC地址过滤',
		lang_dsc1 				: 'MAC(媒体访问控制)地址选项用于对基于网络适配器的MAC地址的网络访问进行控制。一个MAC地址是由网络适配器厂商分配的一个唯一的ID。这个特性可以配置成允许或者禁止网络/因特网访问。',
		lang_note 				: '注意：如果您选择的是“允许列表上的计算机访问网络”，强烈建议将您当前电脑的mac地址也加到规则里面。否则当您配置完规则之后，您当前的电脑也将无法访问路由器！',
		lang_BlockMAC 			: 'MAC地址过滤规则',
		lang_note_mac			: '&nbsp&nbsp您当前电脑的mac地址：',
		lang_block_mode 		: '配置下面的MAC过滤:',
		lang_Num 				: '序号',
		lang_Mac 				: 'MAC地址',
		lang_Dhcp 				: 'DHCP客户端列表',
		lang_Schedule1 			: '调度进程'
	},
	
	value : {
		Apply : '应用',
		AddSchedule : '添加新调度',
		refresh : '取消'
	}, 
	
	option_text : {
		SELECT_block_mode : ['关闭MAC过滤','打开MAC过滤，允许列表上的计算机访问网络','打开MAC过滤，禁止列表上的计算机访问网络']
	},
	
	dynamic : {
		pc_list : '计算机列表',
		always : '所有时间有效',
		never : '任何时间不生效',
		confirmMac : '您确定不将当前电脑的mac地址加到规则里吗？',
		selectRule1 : '选择的第',
		selectRule2 : '条mac字段是空的!',
		cpmacerror : '请选择正确的MAC地址'
	}
}