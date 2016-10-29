var data_language = {
		innerHTML : {
			lang_status_help_menu : '状态帮助',
			lang_devinfo : '设备信息',
			lang_wiress_client : '无线客户端',
			lang_dhcp_client : 'DHCP客户端',
			lang_logs : '日志',
			lang_statistics : '统计',
			lang_routeinfo : '路由信息',
			lang_devinfo_subtitle : '设备信息',
			lang_devinfo_help : '<p>在此页面，您可以查看路由器的详细信息，例如：固件版本号、局域网IP地址等。</p><dl><dt>系统信息<dd><dl><dt>路由器名称<dd>路由器名称。<dt>时间日期<dd>路由器内部时间。<dt>固件版本<dd>显示现在使用的固件版本号。</dd></dl><BR><dt>因特网信息<dd>显示<a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg">基本设置 -> 因特网设置</a> 页面的广域网连接信息。此页面添加了连接状态信息，显示了ADSL是否连接同步。<dl><dt>因特网连接<dd>显示所有因特网连接信息。<dt>因特网连接状态<dd>显示所选择的连接状态。<dt>默认网关<dd>网关使用的IP地址。您可以手动配置网关或设置为自动获取。详细配置请查看<a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=route">高级设置 -> 路由</a>.<dt>首选和备用DNS服务器<dd><p>显示了首选和备用DNS服务器的IP地址。您可以手动设置DNS服务器或设置为自动获取。详细配置请查看 <a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=dns">高级设置 -> DNS</a>.</p><dt>上行线路速率(Kbps)<dd>显示每秒上传的速度。您需要连接到ISP，才可以显示上行线路速率。<dt>下行线路速率(Kbps)<dd>显示每秒下传的速度。您需要连接到ISP，才可以显示下行线路速率。</dd></dl><BR><dt>无线信息<dd>显示<a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wireless&var:subpage=wlbasic">基本设置> 无线设置</a>页面的详细配置信息。<dl><dt>Mac地址<dd>使用无线网络的终端的MAC地址<dt>状态<dd>显示路由器的无线功能是否开启<dt>(SSID):<dd>显示无线网络名称<dt>是否可视<dd>显示您的无线网络是否被其它无线设备可视。选择“可视”，您的无线网络名称广播给无线信号范围内所有的用户。如果您选择“隐藏”，您需要在客户端手动输入SSID，连接到网络。<dt>安全模式<dd>显示您当前设置的安全模式。</dd></dl><BR><dt>局域网信息<dd>显示路由器的局域网设置信息。<dl><dt>Mac地址<dd>显示路由器的MAC地址。<dt>IP地址<dd>显示路由器的IP地址<dt>子网掩码<dd>显示路由器的子网掩码。<dt>DHCP服务器<dd>显示路由器DHCP服务器是否启用。</dd></dl>',
			lang_wireless_subtitle : '无线客户端',
			lang_wireless_help1 : '<p>显示连接到无线路由器的站点（通过BSSID进行区分）。点击“刷新”按钮，刷新页面，以获取新的无线站点。</p>',
			lang_dhcpclient : 'DHCP客户端',
			lang_dhcpclient_help : '<p>显示连接路由器的计算机信息，如主机名称、MAC地址以及通过DHCP服务器动态分配的IP地址及IP地址的租赁时间。</p>',
			lang_log_subtitle : '日志',
			lang_log_help : '<p>如果开启日志功能，系统会记录关键事件，</p><p>您可以在 <a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=maintenance&var:page=syslog">管理 -> 系统日志</a>页面设置系统日志</p><p>您可以查看防火墙日志。</p>',
			lang_statistics_subtitle : '统计',
			lang_statistics_help : '<p>此页面显示所有LAN、WAN和无线包发送和接收的数据。</p><dl><dt>发送<dd>路由器发送数据包的个数。<dt>接收<dd>路由器接收数据包的个数。<dt>发送丢包<dd>发送过程中丢包的个数。丢包现象由错误、碰撞或路由器资源限制引起。<dt>接收丢包<dd>接收过程中丢包的数据。丢包现象由错误、碰撞或路由器资源限制引起。<dt>错误<dd>发送失败的数量导致了丢包。嘈杂的无线射率环境导致无线局域网的高错误率。<dt>ADSL统计<dd>显示了所有的ADSL数据信息，包括ADSL是否连接，上流速率、下流速率、型号、线路发码等。</dd></dl>',
			lang_routeinfo_subtitle : '路由信息',
			lang_routeinfo_help : '<p>显示路由信息，包括目的地的IP地址、网关、子网掩码以及其它路由信息。</p>'
		}

}