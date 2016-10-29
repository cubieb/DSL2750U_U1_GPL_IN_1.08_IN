#####################################
#DNS集成测试用例
#####################################


set i 1
set j 1


##测试命令
if {$TestCaseNo == 0 || $TestCaseNo == 9} {

    #设置 X_TWSZ-COM_DNSRelayEnable=0, 开启/关闭dns代理功能
	set MsgList(9.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=1\0}" 
	set AckMatchList(9.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} }"
    
    #提交
	set MsgList(9.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(9.1.2.1) ""

}

###1.1 参数检查

##1 DNS设置命令处理过程
if {$TestCaseNo == 0 || $TestCaseNo == 1} {

###1.1 参数检查

    #2 对象路径为空
    set MsgList(1.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 \0}" 
    set AckMatchList(1.1.2.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}" 

    #3 设置路径后缺少等号
    set MsgList(1.1.3.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable\0}" 
    set AckMatchList(1.1.3.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #4 给节点设置超长值
    set MsgList(1.1.4.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1  InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.4.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #5 错误的超长路径
    set MsgList(1.1.5.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa=1\0}" 
    set AckMatchList(1.1.5.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"
    
    #6 不存在的节点路径
    set MsgList(1.1.6.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnableABC=1\0}" 
    set AckMatchList(1.1.6.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"
   
    #7 给非叶子节点设置值
    set MsgList(1.1.7.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement=1\0}" 
    set AckMatchList(1.1.7.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9005$} {^9005$} {^1$} {^.*$}}"


    #9 错误的 X_TWSZ-COM_DNSRelayEnable 值
    set MsgList(1.1.9.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=x\0}" 
    set AckMatchList(1.1.9.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #9 空的的 X_TWSZ-COM_DNSRelayEnable 值
    set MsgList(1.1.9.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=\0}" 
    set AckMatchList(1.1.9.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #9 超长的 X_TWSZ-COM_DNSRelayEnable 值
    set MsgList(1.1.9.3) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.9.3) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #10 错误的 WANPPPConnection.1.DNSOverrideAllowed 值
    set MsgList(1.1.10.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=x\0}" 
    set AckMatchList(1.1.10.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^66462511$} {^66462511$} {^1$} {^.*$}}"

    #11 错误的 WANIPConnection.1.DNSServers 值
    set MsgList(1.1.11.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=das\0}" 
    set AckMatchList(1.1.11.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #11 超长的 WANIPConnection.1.DNSServers 值
    set MsgList(1.1.11.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0}" 
    set AckMatchList(1.1.11.2) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"
    
    #12 错误的 WANPPPConnection.1.DNSServers 值
    set MsgList(1.1.12.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=188.146.888.999\0}" 
    set AckMatchList(1.1.12.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #13  错误的WANIPConnection.1.X_TWSZ-COM_UsrDNSServers值
    set MsgList(1.1.13.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers=188.146.888.999\0}" 
    set AckMatchList(1.1.13.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #14 错误的 WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers 值
    set MsgList(1.1.14.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
    {1 InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=da\0}" 
    set AckMatchList(1.1.14.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
    {{^9007$} {^9007$} {^1$} {^.*$}}"

    #15 错误的消息类型
    set MsgList(1.1.15.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_MON_INTF_STATUS_QUERY} \
    {1 \0}"
    set AckMatchList(1.1.15.1) ""

}

###1.2 正常功能
if {$TestCaseNo == 0 || $TestCaseNo == 2} {

    #设置 X_TWSZ-COM_DNSRelayEnable=0, 关闭dns代理功能
	set MsgList(2.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=0\0}" 
	set AckMatchList(2.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} }"   
    #提交
	set MsgList(2.1.2.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(2.1.2.1) ""
    #回滚
	set MsgList(2.1.3.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.3.1) ""


    #设置 X_TWSZ-COM_DNSRelayEnable=1, 开启dns代理功能
	set MsgList(2.1.4.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{1 InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable=1\0}" 
	set AckMatchList(2.1.4.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #提交
	set MsgList(2.1.5.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(2.1.5.1) ""
    #回滚
	set MsgList(2.1.6.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.6.1) ""


####开始设置 WANIPConnection
    #设置 WANIPConnection 下的节点 DNSOverrideAllowed , DNSServers 和 X_TWSZ-COM_UsrDNSServers 
    #使用自定义dns服务器
	set MsgList(2.1.7.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSOverrideAllowed=1\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(2.1.7.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #提交
	set MsgList(2.1.8.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}" 
	set AckMatchList(2.1.8.1) ""
    #回滚
	set MsgList(2.1.9.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.9.1) ""
    #获取设置后的值
    set MsgList(2.1.9.2) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(2.1.9.2) ""


####开始设置 WANPPPConnection
    #设置 WANPPPConnection 下的节点 DNSOverrideAllowed , DNSServers 和 X_TWSZ-COM_UsrDNSServers
    #使用自动获取的dns
	set MsgList(2.1.10.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=0\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(2.1.10.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #提交
	set MsgList(2.1.11.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(2.1.11.1) ""
    #回滚
	set MsgList(2.1.12.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9e $MSG_CMM_CANCEL} {}"
	set AckMatchList(2.1.12.1) ""

    #获取设置后的值
    set MsgList(2.1.12.2) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(2.1.12.2) ""



}


#########2 广播消息##############
if {$TestCaseNo == 0 || $TestCaseNo == 3} {

###2.1 参数检测

    #设置 WANPPPConnection.1 的初始值
	set MsgList(3.1.1.1) "{$MID_CMM  $MID_DNS 0x0b4a6f9c $MSG_CMM_SET_VAL} \
	{3 \
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed=0\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers=10.10.10.10\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers=10.10.10.20\0}" 
	set AckMatchList(3.1.1.1) "{{^$MID_DNS$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
	{{^0$} {^0$} }"
    #提交
	set MsgList(3.1.1.2) "{$MID_CMM  $MID_DNS 0x0b4a6f9d $MSG_CMM_COMMIT} {}"
	set AckMatchList(3.1.1.2) ""

    #获取广播处理之前的新值
    set MsgList(3.1.1.3) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(3.1.1.3) ""

    #空的消息体
	set MsgList(3.1.1.4) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \0}" 
	set AckMatchList(3.1.1.4) ""

    #消息体中参数名错误
    set MsgList(3.1.2.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Pathx=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.2.1) ""

    #消息体中重复的参数
    set MsgList(3.1.3.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.3.1) ""

    #消息体中缺少参数
    set MsgList(3.1.4.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{1 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}" 
	set AckMatchList(3.1.4.1) ""

    #消息体中参数Path值错误
    set MsgList(3.1.5.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{2 \
    Path=xx\0\
    DNSServers=10.10.10.30\0}" 
	set AckMatchList(3.1.5.1) ""

    #消息体中参数DNSServers值错误
    set MsgList(3.1.5.1) "{$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST} \
	{1 \
    Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0\
    DNSServers=xxx\0}" 
	set AckMatchList(3.1.5.1) ""

    #获取广播处理之后的新值,和原来的值一样,说明处理失败
    set MsgList(3.1.6.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
	{4 \
    InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSOverrideAllowed\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DNSServers\0\
    InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_UsrDNSServers\0}" 
	set AckMatchList(3.1.6.1) ""

}


###2.2 正常流程

if {$TestCaseNo == 4} {
set i $TestCaseNo

if {0} {
set MsgList($i.$j) "$MID_PPPOE  $MID_GRP_WAN_CONN_UPDATE 0x0b4a6f9c $MSG_WAN_CONN_EST 
2 
{Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
{DNSServers=10.10.10.30\0}
" 
set AckMatchList($i.$j) ""
incr j
return
}

###发送主机名更新广播
if {1} {
set comment($i.$j) "##向DNS模块发送主机名更新消息"
set MsgList($i.$j) "$MID_DHCPS  $MID_DNS  0x00000001  $MSG_LAN_DOMAIN_NAME_UPDATE
{}
"
set AckMatchList($i.$j) ""
incr j
return
}
    

}

