#this tcl script includes test cases for module "pppoe"

####################################################################################
#set 消息
if {$TestCaseNo == 1} {

set MsgList(1.1.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(1.1.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


###1.1 正常流程
#设置参数
set MsgList(1.2.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{4 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.2.1.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.1.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.1.2) ""

#使能打开
set MsgList(1.2.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=1\0}}"
set AckMatchList(1.2.2.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.2.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.2.2) ""

#使能关闭
set MsgList(1.2.3.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=0\0}}"
set AckMatchList(1.2.3.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^0$} {^0$}}"

set MsgList(1.2.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_COMMIT} \
{}"
set AckMatchList(1.2.3.2) ""

###1.2 输入合法性检查

#改变节点个数大于实际个数
set MsgList(1.3.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{5 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.3.1.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9003$} {^9003$}}"

#改变节点个数小于实际个数
set MsgList(1.3.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{3 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=alex\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=PPPOE\0InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=Manual\0}}"
set AckMatchList(1.3.2.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9003$} {^9003$}}"

#消息体为空
set MsgList(1.3.3.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {\0}}"
set AckMatchList(1.3.3.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#路径错误,过短
set MsgList(1.3.4.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.=1\0}}"
set AckMatchList(1.3.4.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#路径错误,过多杂项1
set MsgList(1.3.5.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.Username=1\0}}"
set AckMatchList(1.3.5.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#路径错误,过多杂项2
set MsgList(1.3.6.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.ABC=1\0}}"
set AckMatchList(1.3.6.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#路径错误,有空格
set MsgList(1.3.7.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1. Username=1\0}}"
set AckMatchList(1.3.7.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#路径错误,实例不存在
set MsgList(1.3.8.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.99999.Username=1\0}}"
set AckMatchList(1.3.8.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9005$} {^9005$}}"

#ConnectionTrigger值为空
set MsgList(1.3.9.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=\0}}"
set AckMatchList(1.3.9.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#ConnectionTrigger值错误
set MsgList(1.3.10.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ConnectionTrigger=ManualManual\0}}"
set AckMatchList(1.3.10.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#username值为空
set MsgList(1.3.11.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=\0}}"
set AckMatchList(1.3.11.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#username超长
set MsgList(1.3.12.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=abcd[string repeat "abcd" 17]\0}}"
set AckMatchList(1.3.12.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#username不符合规格
set MsgList(1.3.13.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username=a@f!dd\0}}"
set AckMatchList(1.3.13.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Password值为空
set MsgList(1.3.14.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=\0}}"
set AckMatchList(1.3.14.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Password超长
set MsgList(1.3.15.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=abcd[string repeat "abcd" 17]\0}}"
set AckMatchList(1.3.15.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Password不符合规格
set MsgList(1.3.16.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password=a@f!dd\0}}"
set AckMatchList(1.3.16.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Name值过长
set MsgList(1.3.17.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=abc[string repeat "abc" 100]\0}}"
set AckMatchList(1.3.17.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Name不符合规格
set MsgList(1.3.18.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Name=a@f!dd\0}}"
set AckMatchList(1.3.18.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU过小
set MsgList(1.3.19.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=0\0}}"
set AckMatchList(1.3.19.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU过大
set MsgList(1.3.20.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=1541\0}}"
set AckMatchList(1.3.20.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#MRU不符合规格
set MsgList(1.3.21.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize=a33\0}}"
set AckMatchList(1.3.21.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#路径错误
set MsgList(1.3.22.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize.[string repeat "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MaxMRUSize." 40]\0}}"
set AckMatchList(1.3.22.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#PPPAuthenticationProtocol为空
set MsgList(1.3.23.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PPPAuthenticationProtocol=\0}}"
set AckMatchList(1.3.23.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#PPPAuthenticationProtocol错误
set MsgList(1.3.24.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.PPPAuthenticationProtocol=unknow\0}}"
set AckMatchList(1.3.24.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#X_TWSZ-COM_ConnectionAction为空
set MsgList(1.3.25.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_ConnectionAction=\0}}"
set AckMatchList(1.3.25.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#X_TWSZ-COM_ConnectionAction错误
set MsgList(1.3.26.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.X_TWSZ-COM_ConnectionAction=unknow\0}}"
set AckMatchList(1.3.26.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Enable为空
set MsgList(1.3.27.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=\0}}"
set AckMatchList(1.3.27.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"

#Enable错误
set MsgList(1.3.28.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_CMM_SET_VAL} \
{1 {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Enable=unknow\0}}"
set AckMatchList(1.3.28.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_SET_VAL_ACK$}} \
{{^9007$} {^9007$}}"


}


if {$TestCaseNo == 2} {

set MsgList(2.1.1) "{$MID_CMM  $MID_WAN  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.\0}}"
set AckMatchList(2.1.1) "{$MID_WAN  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#正常情况
set MsgList(2.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#超过连接数
set MsgList(2.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(2.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.7) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.7) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"
set MsgList(2.3.8) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.3.8) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

#异常情况
#对象路径为空
set MsgList(2.4.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{}"
set AckMatchList(2.4.1) "{{^$MID_PPPOE$} {^$MID_CMM$} {^0x0b4a6f9c$} {^$MSG_CMM_ADD_NODE_ACK$}} \
{{^67371524$} {^67371524$} {^1$} {^ =67371524.*\0$}}"

#对象路径错误
set MsgList(2.4.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnectio.\0}}"
set AckMatchList(2.4.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevi.=67371524.*\0$}}"

#对象路径有多余部分
set MsgList(2.4.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.DevName.xxxx\0}}"
set AckMatchList(2.4.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.DevName.xxxx=67371524.*\0$}}"

#对象路径未以.结束
set MsgList(2.4.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection\0}}"
set AckMatchList(2.4.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.1.WANConnectionDevice=67371524.*\0$}}"

#对象路径在配置树不存在
set MsgList(2.4.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE}\
{{InternetGatewayDevice.WANDevice.222.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(2.4.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^67371524$} {^67371524$} {^1$} {^InternetGatewayDevice.WANDevice.222.WANConnectionDevice.=67371524.*\0$}}"

}

if {$TestCaseNo == 3} {

###正常功能
set MsgList(3.1.1.1) "$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT \
 {pppd\0} 1   6   1 {Connect: ppp0\0}  \
                  2 {local  IP address 172.16.2.23\0} \
                  3 {remote IP address 10.2.10.5\0} \
                  4 {primary   DNS address 10.28.100.2\0} \
                  5 {secondary DNS address 10.28.100.7\0} \
                  6 {PPP session is iDex1 12346\0}"
set AckMatchList(3.1.1.1) ""

#实例号不正确
set MsgList(3.2.1.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppd\0} 9   1 {1 {Connect: ppp0\0}}}"
set AckMatchList(3.2.1.1) ""

#模块名不正确
set MsgList(3.2.2.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppdd\0} 1   1 {1 {Connect: ppp0\0}}}"
set AckMatchList(3.2.2.1) ""

#内容为空
set MsgList(3.2.6.1) "{$MID_CMM  $MID_PPPOE 0x0b4a6f9c $MSG_PC_PROC_OUTPUT} \
{{pppd\0} 1  1 {1 \0}}"
set AckMatchList(3.2.6.1) ""

}


if {$TestCaseNo == 4} {

#建立5条实例
set MsgList(4.1.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.4.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.4.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.5.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.5.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#正常流程测试
#up测试
set MsgList(4.2.1.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.2.1.1) ""
#down测试
set MsgList(4.2.2.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Down\0}}"
set AckMatchList(4.2.2.1) ""

#异常流程测试,输入合法性检查
#项数多于实际数目
set MsgList(4.3.1.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.1.1) ""

#项数小于实际数目
set MsgList(4.3.2.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.2.1) ""

#没有Path字段
set MsgList(4.3.3.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.3.1) ""

#重复Path字段
set MsgList(4.3.4.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.4.1) ""

#Path字段为空
set MsgList(4.3.5.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=\0Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.5.1) ""

#错误的Path字段0
set MsgList(4.3.6.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=xxx.yyy.\0Name=eth0\0Status=Up\0}}"
set AckMatchList(4.3.6.1) ""

#Path字段错误5
set MsgList(4.3.7.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1. \0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.7.1) ""

#没有name字段
set MsgList(4.3.8.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Status=Up\0}}"
set AckMatchList(4.3.8.1) ""

#重复name字段
set MsgList(4.3.9.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth0\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.9.1) ""

#0Name字段为空
set MsgList(4.3.10.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=\0Status=Up\0}}"
set AckMatchList(4.3.10.1) ""

#Path字段错误4
set MsgList(4.3.11.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.Username\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.11.1) ""

#没有Status字段
set MsgList(4.3.12.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0}}"
set AckMatchList(4.3.12.1) ""

#重复Status字段
set MsgList(4.3.13.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{4 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=Up\0Status=Down\0}}"
set AckMatchList(4.3.13.1) ""

#Status字段为空
set MsgList(4.3.14.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=\0}}"
set AckMatchList(4.3.14.1) ""

#Status字段错误
set MsgList(4.3.15.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0Name=eth1\0Status=CONN\0}}"
set AckMatchList(4.3.15.1) ""

#Path字段错误1
set MsgList(4.3.16.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.16.1) ""

#Path字段错误2
set MsgList(4.3.17.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.17.1) ""

#Path字段错误3
set MsgList(4.3.18.1) "{$MID_WAN  $MID_PPPOE 0x0b4a6f9c $MSG_WAN_LINK_CHG} \
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99.\0Name=eth1\0Status=Up\0}}"
set AckMatchList(4.3.18.1) ""

}

#2. 删除实例测试  MSG_CMM_DEL_NODE
if {$TestCaseNo == 5} {

#建立3条实例
set MsgList(5.1.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(5.1.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(5.1.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(5.1.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#异常情况

#对象路径为空
set MsgList(5.2.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{\0}}"
set AckMatchList(5.2.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径错误
set MsgList(5.2.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.xxx.\0}}"
set AckMatchList(5.2.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径有多余部分
set MsgList(5.2.3) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username.xxxx\0}}"
set AckMatchList(5.2.3) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径未以.结束1
set MsgList(5.2.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1\0}}"
set AckMatchList(5.2.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径未以.结束2
set MsgList(5.2.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1\0}}"
set AckMatchList(5.2.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径不存在1
set MsgList(5.2.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.99.\0}}"
set AckMatchList(5.2.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#对象路径不存在2
set MsgList(5.2.7) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99.\0}}"
set AckMatchList(5.2.7) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{{^9005$} {^9005$}}"

#正常情况
#删除一条实例
set MsgList(5.3.1) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}"
set AckMatchList(5.3.1) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

#删除所有实例
set MsgList(5.3.2) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0}}"
set AckMatchList(5.3.2) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_DEL_NODE_ACK} \
{0 0}"

}









