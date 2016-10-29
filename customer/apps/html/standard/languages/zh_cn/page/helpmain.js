var data_language = {
	innerHTML : {
	'lang_management_help'      : '管理帮助',
	'lang_system_management'	: '系统管理',
	'lang_firmware_update'	            : '固件升级',
	'lang_access_control'       : '访问控制',
	'lang_user_management'		: '用户密码',
	'lang_service'					: '服务',
	'lang_IP_acl'			: 'IP过滤',
	'lang_dgn'					: '诊断',
	'lang_log'					: '系统日志',
	'lang_system_subtitle' : '系统管理',
	'lang_system_help' : '<dl><dd>在此页面，您可以对路由器配置进行管理、重启设备、备份配置文件、下载配置文件，及将系统配置恢复到出厂默认值。选择恢复到出厂默认值，将会清除所有的设置，包括创建的所有规则。<dt>重启<dd>重启路由器，以使设置生效。<dt>备份设置<dd>将配置文件备份到您的计算机。当前配置修改时，可以使用备份配置登陆路由器。在进行固件升级前，请备份配置文件。<dt>升级设置<dd>将之前保存的配置文件进行升级。<dt>恢复默认设置<dd>将系统配置恢复到出厂默认值。选择恢复默认值，路由器的所有配置都将丢失。在进行此操作前，如果您需要保存当前路由器的配置，请选择“备份设置”。</dd></dl>',
	'lang_upgrade_subtitle' : '固件升级',
	'lang_upgrade_help' : '<dl><dd>在此页面，您可以升级最新版本固件，以完善设备功能，提高性能。<p>请参照以下步骤进行固件升级：</p><ol><li>点击 <span class=button_ref>浏览</span> 按钮，在您的计算机上选择升级文件。<li>选择正确的升级文件后，点击下面的“升级固件”按钮，开始升级固件。此操作过程需要一分钟或更长时间。<li>路由器重启。此操作过程需要一分钟或更长时间。</ol><dd>注意：一些固件升级操作可能导致配置恢复到出厂默认值。在进行升级操作时，请确保您已经在<a href="/cgi-bin/webproc?getpage=html/index.html&var:menu=maintenance&var:page=system">维护 -> 系统</a>页面，选择“备份设置”。</dd></dl>',
	'lang_access_subtitle' : '访问控制',
	'lang_access_help'   : '<dl><dt>用户密码<dd><p>帐户密码用于设置登陆Web管理页面的密码。强烈建议您修改密码，以确保您的路由器的安全。</p><p>您可以通过“admin”、“support”、“user”三个帐号登陆路由器，进行管理。</p><dl><dt>admin密码<dd>输入admin的密码。使用“admin”登陆路由器，可以对Web界面进行全面管理。<dt>support密码<dd>仅限于ISP技术人员使用“support”登陆路由器，进行维护并运行诊断测试。<dt>user密码<dd>使用“user”登陆路由器，可以查看配置信息和数据，也可以升级路由器固件。</dd></dl></dd></dl><dl><dt>服务<dd>在此页面，您可以通过FTP、HTTP、ICMP、SNMP、SSH、TELNET和TFTP访问路由器。<dl><dt>局域网<dd>从局域网访问路由器。<dt>广域网<dd>从ISP访问路由器。</dd></dl></dd></dl><dl><dt>IP过滤<dd>选择开启访问控制模式，只有在访问控制表中的IP地址才可以访问路由器。此功能与<b>服务功能</b>同时使用。</dd></dl>',
	'lang_dgn_subtitle' : '诊断',
	'lang_dng_help' : '<p>在此页面，您可以运行诊断测试，以检查您的路由器连接状态。有如下三种连接的测试结果。</p><ul><li>本地网络连接。<li>DSL服务供应商连接。<li>因特网服务供应商连接。</ul>',
	'lang_log_subtitle' : '系统日志',
	'lang_log_help' : '<p>在此页面，您可以选择开启日志功能，系统会根据选择的日志等级开始记录事件。</p><dl><dt>日志级别<dd>根据设置的等级记录日志。<dt>显示级别<dd>在日志页面，根据设置的等级记显示日志。<p>如果在“模式”栏选择“远程”或“两者”，记录的事件将会发送给远程系统日志服务器特定的IP地址和UDP端口。如果在“模式”栏选择“本地”或“两者”，事件将会记录在本地记忆里。根据实际情况，选择相应的模式，点击“应用”按钮，以设置系统日志。</p><dt>查看系统日志<dd>点击此按钮，您可以查看设置的等级的系统日志。</dd></dl>'
	}
}