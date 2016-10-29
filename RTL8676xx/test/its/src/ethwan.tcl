#this tcl script includes test cases for module "ethlan"

####################################################################################
##1 查询ETHWAN统计
if {$TestCaseNo == 1} {
###1.1 输入合法性检查

set MsgList(1.1.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesSent.xxxxx\0}}"
set AckMatchList(1.1.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.2.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats. BytesSent\0}}"
set AckMatchList(1.1.2.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.3.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.xxxxx\0}}"
set AckMatchList(1.1.3.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.4.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesSent.xxxxx\0}}"
set AckMatchList(1.1.4.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"

set MsgList(1.1.5.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{5 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesSent\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesReceived\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.PacketsSent\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.PacketsReceived\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.xxxxx\0}}"
set AckMatchList(1.1.5.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


###1.2 正常流程
set MsgList(1.2.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{4 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesSent\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.BytesReceived\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.PacketsSent\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Stats.PacketsReceived\0}}"

set AckMatchList(1.2.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^4$} }"

}

######################################################################################
##2 查询ETHWAN配置
if {$TestCaseNo == "all" || $TestCaseNo == 2} {
###2.1 输入合法性检查
set MsgList(2.1.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.xxxxx\0}}"
set AckMatchList(2.1.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$}}"


set MsgList(2.1.2.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName.xxxx\0}}"
set AckMatchList(2.1.2.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.3.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {Xxxxx.WANInterfaceName\0}}"
set AckMatchList(2.1.3.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.4.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANInterfaceName=\0}}"
set AckMatchList(2.1.4.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.5.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{1 {=\0}}"
set AckMatchList(2.1.5.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


set MsgList(2.1.6.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{7 {InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Status\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MACAddress\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MaxBitRate\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.DuplexMode\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.xxxxx\0}}"
set AckMatchList(2.1.6.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^95167277$} }"


###2.2 正常流程
set MsgList(2.2.1.1) "{$MID_WEB  $MID_CMM 0x0b4a6f9c $MSG_CMM_GET_VAL} \
{6 {InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Status\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MACAddress\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MaxBitRate\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.DuplexMode\0}}"
set AckMatchList(2.2.1.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x0b4a6f9c$} {^$MSG_CMM_GET_VAL_ACK$}} \
{{^0$} {^6$}}"

}

#######################################################################################
##3 配置ETHWAN
if {$TestCaseNo == "all" || $TestCaseNo == 3} {
###输入合法性检查

set MsgList(3.1.1.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.xxxxx=1\0}}"
set AckMatchList(3.1.1.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"


set MsgList(3.1.2.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable .xxxx=1\0}}"
set AckMatchList(3.1.2.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"


set MsgList(3.1.3.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {Xxxxx.Enable=1\0}}"
set AckMatchList(3.1.3.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$}}"


set MsgList(3.1.4.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable=\0}}"
set AckMatchList(3.1.4.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"


set MsgList(3.1.5.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {=1\0}}"
set AckMatchList(3.1.5.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"

set MsgList(3.1.6.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable=\0}}"
set AckMatchList(3.1.6.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"

set MsgList(3.1.7.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.X_TWSZ-COM_DeviceName=eth0\0}}"
set AckMatchList(3.1.7.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} }"

set MsgList(3.1.8.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable=1024\0}}"
set AckMatchList(3.1.8.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^66462511$} }"


set MsgList(3.1.9.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MaxBitRate=1024\0}}"
set AckMatchList(3.1.9.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^67511087$} }"


set MsgList(3.1.10.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.DuplexMode=1024\0}}"
set AckMatchList(3.1.10.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^67576623$} }"



###3.1 正常流程
set MsgList(3.2.1.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{3 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable=1\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MaxBitRate=1000\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.DuplexMode=Full\0}}"
set AckMatchList(3.2.1.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(3.2.2.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.2.2.1) ""



set MsgList(3.2.3.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{3 {InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable=1\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.MaxBitRate=1000\0InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.DuplexMode=Auto\0}}"
set AckMatchList(3.2.3.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(3.2.4.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_COMMIT} \
{}"
set AckMatchList(3.2.4.1) ""



}



#######################################################################################
##4 LAN MAC LIST变更通知
if {$TestCaseNo == "all" || $TestCaseNo == 4} {
###4.1 输入合法性检查
set MsgList(4.1.1.1) "{$MID_LAN  $MID_ETHWAN 0x0b4a6f9c $MSG_LAN_MACLIST_UPDATE} \
{}"
set AckMatchList(4.1.1.1) ""

}

#######################################################################################
##5 UPDATE消息
if {$TestCaseNo == "all" || $TestCaseNo == 5} {
###5.1 正常功能测试

set MsgList(5.1.1.1) "{$MID_CMM  $MID_ETHWAN 0x0b4a6f9c $MSG_CMM_UPDATE} \
{}"
set AckMatchList(5.1.1.1) "{{^$MID_ETHWAN$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_UPDATE_ACK$}} {}"

}


if {$TestCaseNo == "all" || $TestCaseNo == 6} {
###5.1 正常功能测试

set MsgList(6.1.1.1) "{$MID_MON  $MID_ETHWAN 0x0b4a6f9c $MSG_MON_INTF_STATUS_INFORM} \
{3 {PATH=InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable\0NAME=eth1\0STATUS=CONNECTED\0}}"
set AckMatchList(6.1.1.1) "{{^$MID_ETHWAN$} {^$MID_WAN$} {^0x00000408$} {^$MSG_MON_INTF_STATUS_INFORM$}} {}"

}




















