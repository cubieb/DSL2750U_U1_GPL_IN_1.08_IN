#this tcl script includes test cases for module "ethlan"

####################################################################################
##1 查询ETHLAN统计
if {$TestCaseNo == 1} {
###1.1 输入合法性检查

set MsgList(1.1.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesSent.xxxxx\0}}"
set AckMatchList(1.1.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.2.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats. BytesSent\0}}"
set AckMatchList(1.1.2.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.3.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.xxxxx\0}}"
set AckMatchList(1.1.3.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.4.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesSent.xxxxx\0}}"
set AckMatchList(1.1.4.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.5.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{5 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesReceived\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsReceived\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.xxxxx\0}}"
set AckMatchList(1.1.5.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


###1.2 正常流程
set MsgList(1.2.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{4 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesReceived\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsReceived\0}}"
set AckMatchList(1.2.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^4$} }"


###1.3 处理性能
set MsgList(1.3.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{4 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.BytesReceived\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsSent\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.PacketsReceived\0}}"
set AckMatchList(1.3.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^4$} {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Stats.}}"

}

######################################################################################
##2 查询ETHLAN配置
if {$TestCaseNo == "all" || $TestCaseNo == 2} {
###2.1 输入合法性检查
set MsgList(2.1.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.xxxxx\0}}"
set AckMatchList(2.1.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$}}"


set MsgList(2.1.2.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName.xxxx\0}}"
set AckMatchList(2.1.2.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.3.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {Xxxxx.LANInterfaceName\0}}"
set AckMatchList(2.1.3.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.4.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName=\0}}"
set AckMatchList(2.1.4.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.5.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {=\0}}"
set AckMatchList(2.1.5.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.6.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{8 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Status\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.xxxxx\0}}"
set AckMatchList(2.1.6.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


###2.2 正常流程
set MsgList(2.2.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{7 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Status\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode\0}}"
set AckMatchList(2.2.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^7$}}"

###2.3 处理性能
set MsgList(2.3.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{7 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Status\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode\0}}"
set AckMatchList(2.3.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^7$}}"
}

#######################################################################################
##3 配置ETHLAN
if {$TestCaseNo == "all" || $TestCaseNo == 3} {
###输入合法性检查

set MsgList(3.1.1.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.xxxxx=1\0}}"
set AckMatchList(3.1.1.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"


set MsgList(3.1.2.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable .xxxx=1\0}}"
set AckMatchList(3.1.2.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"


set MsgList(3.1.3.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {Xxxxx.Enable=1\0}}"
set AckMatchList(3.1.3.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$}}"


set MsgList(3.1.4.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable=\0}}"
set AckMatchList(3.1.4.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"


set MsgList(3.1.5.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {=1\0}}"
set AckMatchList(3.1.5.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"

set MsgList(3.1.6.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable=\0}}"
set AckMatchList(3.1.6.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"

set MsgList(3.1.7.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.LANInterfaceName=eth0\0}}"
set AckMatchList(3.1.7.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"

set MsgList(3.1.8.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable=1024\0}}"
set AckMatchList(3.1.8.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"


set MsgList(3.1.9.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled=On\0}}"
set AckMatchList(3.1.9.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"


set MsgList(3.1.10.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate=1024\0}}"
set AckMatchList(3.1.10.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^67511087$} }"


set MsgList(3.1.11.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode=1024\0}}"
set AckMatchList(3.1.11.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^67576623$} }"



###3.1 正常流程
set MsgList(3.2.1.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{4 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable=1\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled=1\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate=1000\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode=Full\0}}"
set AckMatchList(3.2.1.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(3.2.2.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.2.2.1) ""

###3.2 处理性能
set MsgList(3.3.1.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{4 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.Enable=1\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled=1\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MaxBitRate=1000\0InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.DuplexMode=Full\0}}"
set AckMatchList(3.3.1.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"
}


#######################################################################################
##4 LAN MAC LIST变更通知
if {$TestCaseNo == "all" || $TestCaseNo == 4} {
###4.1 输入合法性检查
set MsgList(4.1.1.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{777 {00:19:E0:25:A6:80, 00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.1.1) ""

set MsgList(4.1.2.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {\0}}"
set AckMatchList(4.1.2.1) ""

set MsgList(4.1.3.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:19:E0:25:A6,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.3.1) ""

set MsgList(4.1.4.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:19:E0:25:A6:80:90,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.4.1) ""

set MsgList(4.1.5.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:GH:E0:25:A6:80,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.5.1) ""

set MsgList(4.1.6.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {ABC:19:E0:25:A6:80, 0:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.6.1) ""

set MsgList(4.1.7.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:19:E0:25:A6:80,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.1.7.1) ""

###4.2 正常流程
set MsgList(4.2.1.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled=1\0}}"
set AckMatchList(4.2.1.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(4.2.2.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.2.2.1) ""

set MsgList(4.2.3.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:19:E0:25:A6:80,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.2.3.1) ""

set MsgList(4.2.4.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddressControlEnabled=0\0}}"
set AckMatchList(4.2.4.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(4.2.5.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_COMMIT} \
{}"
set AckMatchList(4.2.5.1) ""

set MsgList(4.2.6.1) "{$MID_LAN  $MID_ETHLAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{1 {00:19:E0:25:A6:80,00:19:B9:47:2A:A0\0}}"
set AckMatchList(4.2.6.1) ""


###4.3 处理性能
}

#######################################################################################
##5 UPDATE消息
if {$TestCaseNo == "all" || $TestCaseNo == 5} {
###5.1 正常功能测试

set MsgList(5.1.1.1) "{$MID_CMM  $MID_ETHLAN 0x0b4a6f9c $MSG_CMM_UPDATE} \
{}"
set AckMatchList(5.1.1.1) "{{^$MID_ETHLAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_UPDATE_ACK$}} {}"

}



















