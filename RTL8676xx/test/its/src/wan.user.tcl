
#1. 增加实例测试  MSG_CMM_ADD_NODE

#1.1 异常测试
if {$TestCaseNo == 1} {
#对象路径为空
set MsgList(1.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{}"
set AckMatchList(1.1.1) "{{^$MID_WAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_ADD_NODE_ACK$}} \
{{^67371524$} {^67371524$} {^1$} {^ =67371524.*\0$}}"

#对象路径错误
set MsgList(1.1.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevi.\0}}"
set AckMatchList(1.1.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevi.=67371524.*\0$}}"

#对象路径有多余部分
set MsgList(1.1.3) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx\0}}"
set AckMatchList(1.1.3) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx=67371524.*\0$}}"

#对象路径未以.结束
set MsgList(1.1.4) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice\0}}"
set AckMatchList(1.1.4) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice=67371524.*\0$}}"

#对象路径不是自己模块
set MsgList(1.1.5) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}"
set AckMatchList(1.1.5) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.=67371524.*\0$}}"

#对象路径在配置树不存在
set MsgList(1.1.6) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE}\
{{InternetGatewayDevice.WANDevice.222.WANConnectionDevice.\0}}"
set AckMatchList(1.1.6) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.222.WANConnectionDevice.=67371524.*\0$}}"

#正常情况
set MsgList(1.2.1) "{$MID_WEB  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(1.2.1) "{$MID_CMM  $MID_WEB  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2}"

set MsgList(1.2.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(1.2.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

}

#2. 删除实例测试  MSG_CMM_DEL_NODE
if {$TestCaseNo == 2} {

#异常情况

#对象路径为空
set MsgList(2.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{\0}}"
set AckMatchList(2.1.1) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径错误
set MsgList(2.1.2) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.xxxxx.\0}}"
set AckMatchList(2.1.2) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径有多余部分
set MsgList(2.1.3) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx\0}}"
set AckMatchList(2.1.3) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径未以.结束
set MsgList(2.1.4) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0}}"
set AckMatchList(2.1.4) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径不是自己模块
set MsgList(2.1.5) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}}"
set AckMatchList(2.1.5) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#正常情况
set MsgList(2.2.1) "{$MID_WEB  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.31.\0}}"
set AckMatchList(2.2.1) "{$MID_CMM  $MID_WEB  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

}

#8. 物理连接变化通知测试 MSG_MON_INTF_STATUS_INFORM
if {$TestCaseNo == 3} {

#异常测试
#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，通知项个数为0的情况
set MsgList(3.1.1) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{0 {}}"
set AckMatchList(3.1.1) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，通知项个数小于实际的通知项个数的情况
set MsgList(3.1.2) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{1 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.2) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，通知项个数大于实际的通知项个数的情况
set MsgList(3.1.3) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.3) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，通知项值不能被识别的情况
set MsgList(3.1.4) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Down\0}}"
set AckMatchList(3.1.4) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项不存在的情况
set MsgList(3.1.5) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.5) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项多于1个的情况
set MsgList(3.1.6) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Path=InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.\0\
Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.6) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项值为空的情况
set MsgList(3.1.7) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.7) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项值超长的情况
set MsgList(3.1.8) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.[string repeat "Enabled" 100].\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.8) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项值多余的情况
set MsgList(3.1.9) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.9) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Path通知项值未以"."结束的情况
set MsgList(3.1.10) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.10) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Name通知项不存在的情况
set MsgList(3.1.11) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Status=Connected\0}}"
set AckMatchList(3.1.11) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Name通知项多于一个的情况
set MsgList(3.1.12) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth0\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.1.12) ""

#zhujianwen 13
#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Name通知项值为空的情况
set MsgList(3.1.13) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=\0Status=Connected\0}}"
set AckMatchList(3.1.13) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Name通知项值非法的情况
#set MsgList(3.1.14) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
#{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=fth0\0Status=Connected\0}}"
#set AckMatchList(3.1.14) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，STATUS通知项不存在的情况
set MsgList(3.1.15) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0}}"
set AckMatchList(3.1.15) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，STATUS通知项多于一个的情况
set MsgList(3.1.16) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{4 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0Status=Disconnected\0}}"
set AckMatchList(3.1.16) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，STATUS通知项值为空的情况
set MsgList(3.1.17) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=\0}}"
set AckMatchList(3.1.17) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，STATUS通知项值非法的情况
set MsgList(3.1.18) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=EST\0}}"
set AckMatchList(3.1.18) ""

#正常情况
#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Connected的情况
set MsgList(3.2.1) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Connected\0}}"
set AckMatchList(3.2.1) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Disconnected的情况
set MsgList(3.2.2) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=Disconnected\0}}"
set AckMatchList(3.2.2) ""

#测试MON向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息，Unknow的情况
set MsgList(3.2.3) "{$MID_MON  $MID_WAN  0x0b4a6f9c  $MSG_MON_INTF_STATUS_INFORM} \
{3 {Path=InternetGatewayDevice.WANDevice.1.\0Name=eth1\0Status=UNKNOW\0}}"
set AckMatchList(3.2.3) ""

}

#9. 新建连接通知 MSG_WAN_CONN_EST
if {$TestCaseNo == 4} {

#输入合法性检查
#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，通知项个数多于实际项数的情况
set MsgList(4.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.1) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，通知项个数少于实际项数的情况
set MsgList(4.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.2) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，消息包含不能识别的通知项的情况
set MsgList(4.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{1 {Nat=Enable\0}}"
set AckMatchList(4.1.3) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，Path通知项不存在的情况
set MsgList(4.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{2 {ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.4) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，Path通知项值为空的情况
set MsgList(4.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.5) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息，Path通知项值非法的情况
set MsgList(4.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=ITU.CPE.IPCONN.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.1.6) ""

#正常情况
#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(4.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.2.1) ""

#测试PPPOE向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(4.2.2) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(4.2.2) ""
}

#10. 新建连接通知 MSG_WAN_CONN_FIN
if {$TestCaseNo == 5} {

#输入合法性检查
#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，通知项个数多于实际项数的情况
set MsgList(5.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.1) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，通知项个数少于实际项数的情况
set MsgList(5.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.2) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，消息包含不能识别的通知项的情况
set MsgList(5.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{1 {Nat=Enable\0}}"
set AckMatchList(5.1.3) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，Path通知项不存在的情况
set MsgList(5.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{2 {ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.4) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，Path通知项值为空的情况
set MsgList(5.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.5) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息，Path通知项值非法的情况
set MsgList(5.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=ITU.CPE.IPCONN.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.1.6) ""

#正常情况
#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(5.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.1) ""

#测试PPPOE向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(5.2.2) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.2) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息
set MsgList(5.2.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.3) ""

#测试PPPOE向WAN模块发送MSG_WAN_CONN_FIN消息
set MsgList(5.2.4) "{$MID_PPPOE  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0ExternalIPAddress=10.10.10.10\0SubnetMask=255.0.0.0\0}}"
set AckMatchList(5.2.4) ""

}

#11. 新建连接通知 MSG_WAN_CONN_ENABLE_UPDATE
if {$TestCaseNo == 6} {

#通知项个数多于实际项数的情况
set MsgList(6.1.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.1) ""

#通知项个数小于实际项数的情况
set MsgList(6.1.2) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.2) ""

#通知项个数不足的情况
set MsgList(6.1.3) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0}}"
set AckMatchList(6.1.3) ""

#路径为空的情况
set MsgList(6.1.4) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=\0Enable=1\0}}"
set AckMatchList(6.1.4) ""

#路径非法
set MsgList(6.1.5) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.fas.fasf\0Enable=1\0}}"
set AckMatchList(6.1.5) ""

#路径错误的情况
set MsgList(6.1.6) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.\0Enable=1\0}}"
set AckMatchList(6.1.6) ""

#没有path的情况
set MsgList(6.1.7) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{1 {Enable=1\0}}"
set AckMatchList(6.1.7) ""

#enable错误
set MsgList(6.1.8) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=fasdf\0}}"
set AckMatchList(6.1.8) ""

#没有等号的情况
set MsgList(6.1.9) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{3 {PathInternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0Status=Conneted\0}}"
set AckMatchList(6.1.9) ""

#正常情况
set MsgList(6.2.1) "{$MID_IPCONN  $MID_WAN  0x0b4a6f9c  $MSG_WAN_CONN_ENABLE_UPDATE} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Enable=1\0Status=Conneted\0}}"
set AckMatchList(6.2.1) ""

}


