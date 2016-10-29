#this tcl script includes test cases for module "pppoe"

####################################################################################
### ITP.MOD.IPT.001 获取NAT状态
if {$TestCaseNo == 1} {
### 1.1 输入合法性检查
set MsgList(1.1.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {\0}}"
set AckMatchList(1.1.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {abcd[string repeat "abcd" 256]\0}}"
set AckMatchList(1.1.1.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}}"
set AckMatchList(1.1.1.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.4) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled.NATEnabled\0}}"
set AckMatchList(1.1.1.4) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled.ABC\0}}"
set AckMatchList(1.1.1.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.6) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1. NATEnabled\0}}"
set AckMatchList(1.1.1.6) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ABCEnabled\0}}"
set AckMatchList(1.1.1.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

set MsgList(1.1.1.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.NATEnabled\0}}"
set AckMatchList(1.1.1.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

# 正常情况
set MsgList(1.1.2.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled\0}}"
set AckMatchList(1.1.2.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{0 1 {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0$}}"

set MsgList(1.1.2.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled\0}}"
set AckMatchList(1.1.2.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{0 1 {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0$}}"

}

####################################################################################
### ITP.MOD.IPT.002 设置NAT状态
if {$TestCaseNo == 2} {
### 2.1 输入合法性检查
set MsgList(2.1.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {=1\0}}"
set AckMatchList(2.1.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {abcd[string repeat "abcd" 256]=1\0}}"
set AckMatchList(2.1.1.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.=1\0}}"
set AckMatchList(2.1.1.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.4) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled.NATEnabled=1\0}}"
set AckMatchList(2.1.1.4) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled.ABC=1\0}}"
set AckMatchList(2.1.1.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.6) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1. NATEnabled=1\0}}"
set AckMatchList(2.1.1.6) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ABCEnabled=1\0}}"
set AckMatchList(2.1.1.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.NATEnabled=1\0}}"
set AckMatchList(2.1.1.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.9) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.65535.NATEnabled=1\0}}"
set AckMatchList(2.1.1.9) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(2.1.1.10) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=\0}}"
set AckMatchList(2.1.1.10) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(2.1.1.10) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=\0}}"
set AckMatchList(2.1.1.10) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(2.1.1.11) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0n\0}}"
set AckMatchList(2.1.1.11) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(2.1.1.12) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1024\0}}"
set AckMatchList(2.1.1.12) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(2.1.1.13) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=yes\0}}"
set AckMatchList(2.1.1.13) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(2.1.1.14) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=256\0}}"
set AckMatchList(2.1.1.14) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

# 正常情况
set MsgList(2.1.2.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0}}"
set AckMatchList(2.1.2.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(2.1.2.2) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(2.1.2.2) ""

set MsgList(2.1.2.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.2.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.2.4) ""

set MsgList(2.1.3.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0}}"
set AckMatchList(2.1.3.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.3.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.3.6) ""

set MsgList(2.1.3.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.3.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.3.8) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_CANCEL} \
{}"
set AckMatchList(2.1.3.8) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息
set MsgList(2.1.4.7) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(2.1.4.7) ""

set MsgList(2.1.4.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.4.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.4.9) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.4.9) ""

set MsgList(2.1.5.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(2.1.5.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

#测试PPPOE向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(2.1.5.2) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(2.1.5.2) ""

set MsgList(2.1.5.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.5.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.5.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.5.4) ""

set MsgList(2.1.6.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(2.1.6.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.6.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.6.6) ""

set MsgList(2.1.6.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.6.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.6.8) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_CANCEL} \
{}"
set AckMatchList(2.1.6.8) ""

#测试PPPOE向WAN模块发送MSG_WAN_CONN_FIN消息
set MsgList(2.1.7.7) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(2.1.7.7) ""

set MsgList(2.1.7.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(2.1.7.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(2.1.7.9) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(2.1.7.9) ""

}

if {$TestCaseNo == 3} {
set MsgList(3.3.1.1) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(3.3.1.1) ""

set MsgList(3.4.1.2) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(3.4.1.2) ""

set MsgList(3.4.2.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0}}"
set AckMatchList(3.4.2.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(3.4.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.4.2.4) ""

set MsgList(3.4.3.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(3.4.3.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(3.4.3.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.4.3.6) ""

set MsgList(3.4.3.7) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(3.4.3.7) ""

set MsgList(3.4.3.8) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(3.4.3.8) ""

set MsgList(3.4.3.9) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
set AckMatchList(3.4.3.9) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(3.4.3.10) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.4.3.10) ""

set MsgList(3.4.3.11) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(3.4.3.11) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(3.4.3.12) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.4.3.12) ""

set MsgList(3.4.3.7) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(3.4.3.7) ""

set MsgList(3.4.3.8) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(3.4.3.8) ""

set MsgList(3.5.3.1) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(3.5.3.1) ""

set MsgList(3.6.3.1) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(3.6.3.1) ""

}

if {$TestCaseNo == 4} {
set MsgList(4.3.1.1) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(4.3.1.1) ""

set MsgList(4.3.1.2) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(4.3.1.2) ""

set MsgList(4.3.1.3) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(4.3.1.3) ""

set MsgList(4.3.1.4) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(4.3.1.4) ""

set MsgList(4.4.2.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0}}"
set AckMatchList(4.4.2.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(4.4.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.4.2.4) ""

set MsgList(4.4.3.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(4.4.3.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(4.4.3.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.4.3.6) ""

set MsgList(4.4.3.7) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(4.4.3.7) ""

set MsgList(4.4.3.8) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(4.4.3.8) ""

set MsgList(4.4.3.9) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
set AckMatchList(4.4.3.9) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(4.4.3.10) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.4.3.10) ""

set MsgList(4.4.3.11) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(4.4.3.11) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(4.4.3.12) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.4.3.12) ""

set MsgList(4.4.3.7) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(4.4.3.7) ""

set MsgList(4.4.3.8) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(4.4.3.8) ""

set MsgList(4.5.3.1) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(4.5.3.1) ""

set MsgList(4.6.3.1) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(3.6.3.1) ""

}

# if {$TestCaseNo == 2} {
#正常情况
#测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息
# set MsgList(2.2.1) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
# set AckMatchList(2.2.1) ""

# set MsgList(2.2.2) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp1\0}}"
# set AckMatchList(2.2.2) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息
# set MsgList(2.2.3) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
# set AckMatchList(2.2.3) ""

# set MsgList(2.2.4) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp1\0}}"
# set AckMatchList(2.2.4) ""

# }

# if {$TestCaseNo == 3} {
#正常情况
#测试CMM向WAN模块发送MSG_CMM_ADD_NODE消息
# set MsgList(3.2.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}"
# set AckMatchList(3.2.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2 0}"

# set MsgList(3.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.\0}}"
# set AckMatchList(3.2.3) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 3 0}"

# set MsgList(3.2.5) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}"
# set AckMatchList(3.2.5) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2 0}"

# }

# if {$TestCaseNo == 4} {
#正常情况
#测试CMM向WAN模块发送MSG_CMM_DEL_NODE消息
# {{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.\0}}"

# set MsgList(4.2.3) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.2.\0}}"
# set AckMatchList(4.2.3) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"
# }

if {$TestCaseNo == 5} {
#正常情况
set MsgList(5.1.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{\0}}"
set AckMatchList(5.1.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(5.1.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.ABCE\0}}"
set AckMatchList(5.1.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(5.1.3) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.ABCD.\0}}"
set AckMatchList(5.1.3) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(5.1.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.ABCD.\0}}"
set AckMatchList(5.1.4) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(5.1.5) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping\0}}"
set AckMatchList(5.1.5) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(5.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.\0}}"
set AckMatchList(5.2.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2}"

set MsgList(5.2.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.\0}}"
set AckMatchList(5.2.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2}"

}

if {$TestCaseNo == 6} {
#正常情况
set MsgList(6.1.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{\0}}"
set AckMatchList(6.1.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(6.1.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.ABCE\0}}"
set AckMatchList(6.1.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(6.1.3) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.a\0}}"
set AckMatchList(6.1.3) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(6.1.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1\0}}"
set AckMatchList(6.1.4) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(6.1.5) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping\0}}"
set AckMatchList(6.1.5) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

set MsgList(6.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.\0}}"
set AckMatchList(6.2.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0}"

set MsgList(6.2.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.1.\0}}"
set AckMatchList(6.2.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0}"

set MsgList(6.3.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.\0}}"
set AckMatchList(6.3.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 1}"

set MsgList(6.3.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.\0}}"
set AckMatchList(6.3.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 1}"

set MsgList(6.3.3) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp0\0}}"
set AckMatchList(6.3.3) ""

set MsgList(6.3.4) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(6.3.4) ""

set MsgList(6.4.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.\0}}"
set AckMatchList(6.4.1) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0}"

set MsgList(6.4.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.1.\0}}"
set AckMatchList(6.4.2) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0}"

}

# if {$TestCaseNo == 6} {
#正常情况
# set MsgList(6.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}}"
# set AckMatchList(6.2.4) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{0 2 0}"
# }

####################################################################################
### ITP.MOD.IPT.002 设置NAT状态
if {$TestCaseNo == 8} {
### 8.1 输入合法性检查
set MsgList(8.1.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {=1\0}}"
set AckMatchList(8.1.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {abcd[string repeat "abcd" 256]=1\0}}"
set AckMatchList(8.1.1.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.=1\0}}"
set AckMatchList(8.1.1.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.4) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.NATEnabled=1\0}}"
set AckMatchList(8.1.1.4) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.ABC=1\0}}"
set AckMatchList(8.1.1.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.6) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1. NATEnabled=1\0}}"
set AckMatchList(8.1.1.6) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ABCEnabled=1\0}}"
set AckMatchList(8.1.1.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.NATEnabled=1\0}}"
set AckMatchList(8.1.1.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.9) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.65535.NATEnabled=1\0}}"
set AckMatchList(8.1.1.9) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

set MsgList(8.1.1.10) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=\0}}"
set AckMatchList(8.1.1.10) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(8.1.1.10) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=\0}}"
set AckMatchList(8.1.1.10) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(8.1.1.11) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=0n\0}}"
set AckMatchList(8.1.1.11) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(8.1.1.12) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=1024\0}}"
set AckMatchList(8.1.1.12) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(8.1.1.13) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=yes\0}}"
set AckMatchList(8.1.1.13) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

set MsgList(8.1.1.14) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=256\0}}"
set AckMatchList(8.1.1.14) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

# 正常情况
set MsgList(8.1.2.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=0\0}}"
set AckMatchList(8.1.2.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.2.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=1\0}}"
set AckMatchList(8.1.2.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.2.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.2.4) ""

set MsgList(8.1.3.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=0\0}}"
set AckMatchList(8.1.3.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.3.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.3.6) ""

set MsgList(8.1.3.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=1\0}}"
set AckMatchList(8.1.3.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.3.8) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_CANCEL} \
{}"
set AckMatchList(8.1.3.8) ""

set MsgList(8.1.4.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1=1\0}}"
set AckMatchList(8.1.4.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.4.9) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.4.9) ""

set MsgList(8.1.5.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(8.1.5.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.5.3) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(8.1.5.3) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.5.4) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.5.4) ""

set MsgList(8.1.6.5) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
set AckMatchList(8.1.6.5) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.6.6) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.6.6) ""

set MsgList(8.1.6.7) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
set AckMatchList(8.1.6.7) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.6.8) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_CANCEL} \
{}"
set AckMatchList(8.1.6.8) ""

set MsgList(8.1.7.8) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{8 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingEnabled=1\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingLeaseDuration=3600\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.RemoteHost=10.10.0.0/255.255.0.0\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.ExternalPort=0\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalPort=1234\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingProtocol=TCP\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient=10.10.10.10\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingDescription=test port\0}}"
set AckMatchList(8.1.7.8) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.7.9) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.7.9) ""

set MsgList(8.1.7.10) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{8 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingEnabled=1\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingLeaseDuration=3600\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.RemoteHost=10.10.0.0/255.255.0.0\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.ExternalPort=0\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalPort=1234\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingProtocol=TCP\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient=10.10.10.10\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingDescription=test port\0} }"
set AckMatchList(8.1.7.10) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.7.12) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{8 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingEnabled=1\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingLeaseDuration=3600\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.RemoteHost=\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.ExternalPort=0\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalPort=1234\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingProtocol=TCP\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient=10.10.10.10\0} {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.PortMappingDescription=test port\0} }"
set AckMatchList(8.1.7.12) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{0 0 }"

set MsgList(8.1.7.13) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(8.1.7.13) ""

}
 
##9 UPDATE消息
if {$TestCaseNo == 9} {
###9.1 正常功能测试
set MsgList(9.1.1.1) "{$MID_CMM $MID_IPT 0x0b4a6f9c $MSG_CMM_UPDATE} {}"
set AckMatchList(9.1.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_UPDATE_ACK$}} {{^0$} }"
}


##8 UPDATE消息
# if {$TestCaseNo == 8} {
###8.2 正常流程
# set MsgList(8.2.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0}}"
# set AckMatchList(8.2.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

# set MsgList(8.2.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
# set AckMatchList(8.2.2.1) ""

###正常流程
# set MsgList(8.3.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled\0}}"
# set AckMatchList(8.3.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{ {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=0\0$}}"

# set MsgList(8.4.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0}}"
# set AckMatchList(8.4.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

# set MsgList(8.4.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
# set AckMatchList(8.4.2.1) ""

###正常流程
# set MsgList(8.5.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled\0}}"
# set AckMatchList(8.5.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{ {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=0\0$}}"

# }

##9 UPDATE消息
# if {$TestCaseNo == 9} {
# ###9.2 正常流程
# set MsgList(9.2.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0}}"
# set AckMatchList(9.2.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
# {{^0$} {^0$}}"
# 
# set MsgList(9.2.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
# {}"
# set AckMatchList(9.2.2.1) ""
# 
# ###正常流程
# set MsgList(9.3.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled\0}}"
# set AckMatchList(9.3.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
# { {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.NATEnabled=1\0$}}"
# 
# set MsgList(9.4.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0}}"
# set AckMatchList(9.4.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
# {{^0$} {^0$}}"
# 
# set MsgList(9.4.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
# {}"
# set AckMatchList(9.4.2.1) ""
# 
# ###正常流程
# set MsgList(9.5.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled\0}}"
# set AckMatchList(9.5.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
# { {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.NATEnabled=1\0$}}"
# 
# }
# 
# ##10 UPDATE消息
# if {$TestCaseNo == 10} {
# ###10.2 正常流程
# ###正常流程
# set MsgList(10.3.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANIPConnection.1.PortMapping.2.NATEnabled\0}}"
# set AckMatchList(10.3.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
# { {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANIPConnection.1.PortMapping.2.NATEnabled=0\0$}}"
# 
# }
# 
# if {$TestCaseNo == 11} {
# #正常情况
# #测试CMM向WAN模块发送MSG_CMM_DEL_NODE消息
# set MsgList(11.2.3) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
# {{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.4.\0}}"
# set AckMatchList(11.2.3) "{$MID_IPT  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
# {0 0}"
# }
# 
# if {$TestCaseNo == 12} {
# ###8.2 正常流程
# # {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.2.PortMapping.1.NATEnabled=0\0}}"
# 
# set MsgList(12.2.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.2.PortMappingEnabled=0\0}}"
# set AckMatchList(12.2.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
# {{^0$} {^0$}}"
# 
# set MsgList(12.2.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
# {}"
# set AckMatchList(12.2.2.1) ""
# 
# }
# 
# ##13 UPDATE消息
# if {$TestCaseNo == 13} {
# ###13.2 正常流程
# set MsgList(13.2.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.1.InternalClient=192.168.1.105/255.255.255.255\0}}"
# set AckMatchList(13.2.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
# {{^0$} {^0$}}"
# 
# set MsgList(13.2.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
# {}"
# set AckMatchList(13.2.2.1) ""
# 
# ###正常流程
# set MsgList(13.3.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.1.InternalClient\0}}"
# set AckMatchList(13.3.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
# { {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PortMapping.1.InternalClient=192.168.1.105/255.255.255.255\0$}}"
# 
# set MsgList(13.4.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient=192.168.1.105/255.255.255.255\0}}"
# set AckMatchList(13.4.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
# {{^0$} {^0$}}"
# 
# set MsgList(13.4.2.1) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
# {}"
# set AckMatchList(13.4.2.1) ""
# 
# ###正常流程
# set MsgList(13.5.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient\0}}"
# set AckMatchList(13.5.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
# { {^0$} {^1$} {^boolean InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.1.InternalClient=1\0$}}"
# 
# # }
# 
if {$TestCaseNo == 14} {
### 14.2 正常流程
# {1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.2.PortMapping.1.NATEnabled=0\0}}"
# 
# PortMappingEnabled
# PortMappingLeaseDuration
# RemoteHost
# ExternalPort
# InternalPort
# PortMappingProtocol
# InternalClient
# PortMappingDescription
# PortMappingWeight

set MsgList(14.2.1.1) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{9 \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.PortMappingEnabled=0\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.PortMappingLeaseDuration=0\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.RemoteHost=10.10.10.10/255.255.255.255\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.ExternalPort=10:10000\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.InternalPort=80\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.PortMappingProtocol=tcp\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.InternalClient=192.168.1.2/255.255.255.255\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.PortMappingDescription=WANPPPConnection.2.PortMapping.1.\0} \
{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.PortMapping.1.PortMappingWeight=33554432\0} \
}"
set AckMatchList(14.2.1.1) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(14.2.1.2) "{$MID_CMM  $MID_IPT  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(14.2.1.2) ""

}
#

if {$TestCaseNo == 15} {
set MsgList(15.1.1.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.PortMapping.\0}}"
set AckMatchList(15.1.1.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

}

if {$TestCaseNo == 16} {
set MsgList(16.1.1.2) "{$MID_CMM  $MID_IPT 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.2.NATEnabled\0}}"
set AckMatchList(16.1.1.2) "{{^$MID_IPT$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{}"

}

if {$TestCaseNo == 17} {
# 正常情况
# 测试IPCONN向WAN模块发送MSG_WAN_CONN_EST消息
set MsgList(17.2.1) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
set AckMatchList(17.2.1) ""

set MsgList(17.2.2) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_EST} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp1\0}}"
set AckMatchList(17.2.2) ""

#测试IPCONN向WAN模块发送MSG_WAN_CONN_FIN消息
# set MsgList(17.2.3) "{$MID_IPCONN  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=eth1\0}}"
# set AckMatchList(17.2.3) ""

# set MsgList(17.2.4) "{$MID_PPPOE  $MID_IPT  0x0b4a6f9c  $MSG_WAN_CONN_FIN} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} {ExternalIPAddress=10.10.10.10\0} {SubnetMask=255.0.0.0\0} {InterfaceName=ppp1\0}}"
# set AckMatchList(17.2.4) ""

# }


