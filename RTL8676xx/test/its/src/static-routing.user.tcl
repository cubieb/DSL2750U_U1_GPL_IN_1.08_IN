
#Static Routing 集成测试用例

#增加实例, 没有异常情况
if {$TestCaseNo == 1} {

#正常流程
set MsgList(1.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(1.1.1) ""

}

#增加WAN 连接测试
if {$TestCaseNo == 2} {

set MsgList(2.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}" 
set AckMatchList(2.1.1) ""

set MsgList(2.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}}" 
set AckMatchList(2.1.2) ""

set MsgList(2.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_ADD_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}}" 
set AckMatchList(2.1.3) ""
}

#连接WAN连接
if {$TestCaseNo == 3} {

set MsgList(3.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=ppp0\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.1) ""

set MsgList(3.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=ppp1\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.2) ""

set MsgList(3.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}
   {InterfaceName=ppp2\0}
   {RemoteIPAddress=172.16.2.10\0}}" 
set AckMatchList(3.1.3) ""
}

#SET消息 和 commit消息
if {$TestCaseNo == 4} {

#正常流程
#1. 增加3个实例和3个pppoe连接实例
set MsgList(4.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.1) ""

set MsgList(4.1.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.2) ""

set MsgList(4.1.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.\0}}" 
set AckMatchList(4.1.3) ""

set MsgList(4.1.4) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.4) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.5) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.5) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"

set MsgList(4.1.6) "{$MID_CMM  $MID_PPPOE  0x0b4a6f9c  $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(4.1.6) "{$MID_PPPOE  $MID_CMM  0x0b4a6f9c  $MSG_CMM_ADD_NODE_ACK} \
{{^0$} {^.*$} {^0$}}"


#2.设置正常流程
set MsgList(4.2.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   7 
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=192.168.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.GatewayIPAddress=172.16.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.ForwardingMetric=100\0}
}"
set AckMatchList(4.2.1) ""


#3.功能测试
#设置实例0 1有效, 2无效
set MsgList(4.3.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   6 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=192.168.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.GatewayIPAddress=172.16.2.1\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.ForwardingMetric=100\0}
}"
set AckMatchList(4.3.1) ""

set MsgList(4.3.2) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.2) ""
    
set MsgList(4.3.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   6 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Enable=0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.DestIPAddress=192.168.2.30\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.DestSubnetMask=255.255.255.0\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.GatewayIPAddress=\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=100\0}
}"
set AckMatchList(4.3.3) ""

set MsgList(4.3.4) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.4) ""

#实例1绑定接口改变
set MsgList(4.3.5) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.Interface=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
}"
set AckMatchList(4.3.5) ""

set MsgList(4.3.6) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.6) ""

#对无效实例2设置enable = 1
set MsgList(4.3.7) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.3.Enable=1\0}
}"
set AckMatchList(4.3.7) ""

set MsgList(4.3.8) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.8) ""

#对实例0设置enable=1
set MsgList(4.3.9) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=1\0}
}"
set AckMatchList(4.3.9) ""

set MsgList(4.3.10) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.10) ""

#对实例0设置局部enable=0
set MsgList(4.3.11) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=0\0}
}"
set AckMatchList(4.3.11) ""

set MsgList(4.3.12) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.12) ""

#设置默认路由为pppoe0
set MsgList(4.3.13) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
}"
set AckMatchList(4.3.13) ""

set MsgList(4.3.14) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.14) ""

#设置默认路由为pppoe1
set MsgList(4.3.15) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
}"
set AckMatchList(4.3.15) ""

set MsgList(4.3.16) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.16) ""

#设置默认路由为空
set MsgList(4.3.17) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=\0}
}"
set AckMatchList(4.3.17) ""

set MsgList(4.3.18) "{$MID_CMM $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(4.3.18) ""


#4.异常测试
#enable参数错误
set MsgList(4.4.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=2\0}
}"
set AckMatchList(4.4.1) ""

#enable参数为空
set MsgList(4.4.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.Enable=\0}
}"
set AckMatchList(4.4.2) ""

#dest IP参数错误1
set MsgList(4.4.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.\0}
}"
set AckMatchList(4.4.3) ""

#dest IP参数错误2
set MsgList(4.4.4) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=256.16.3.3\0}
}"
set AckMatchList(4.4.4) ""

#dest IP参数错误3
set MsgList(4.4.5) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.1.\0}
}"
set AckMatchList(4.4.5) ""

#dest IP参数错误4
set MsgList(4.4.6) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172.16.3.2a\0}
}"
set AckMatchList(4.4.6) ""

#dest IP参数错误5
set MsgList(4.4.7) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=1111.16.3.\0}
}"
set AckMatchList(4.4.7) ""

#dest IP参数错误6
set MsgList(4.4.8) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172..16.3.2\0}
}"
set AckMatchList(4.4.8) ""

#dest IP参数错误7
set MsgList(4.4.9) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=172..16.3\0}
}"
set AckMatchList(4.4.9) ""

#dest IP参数为空
set MsgList(4.4.10) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.1.DestIPAddress=\0}
}"
set AckMatchList(4.4.10) ""


#错误的接口路径, 为空
set MsgList(4.4.11) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=\0}
}"
set AckMatchList(4.4.11) ""

#错误的接口路径, 没有此路径
set MsgList(4.4.12) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.5.\0}
}"
set AckMatchList(4.4.12) ""

#ForwardingMetric错误1
set MsgList(4.4.13) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=-1\0}
}"
set AckMatchList(4.4.13) ""

#ForwardingMetric错误2
set MsgList(4.4.14) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=1000\0}
}"
set AckMatchList(4.4.14) ""

#ForwardingMetric错误, 为空
set MsgList(4.4.15) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.Layer3Forwarding.Forwarding.2.ForwardingMetric=\0}
}"
set AckMatchList(4.4.15) ""


}






#增加LAN 连接测试
if {$TestCaseNo == 5} {

set MsgList(5.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0}}"
set AckMatchList(5.1.1) ""

set MsgList(5.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0}}" 
set AckMatchList(5.1.2) ""

set MsgList(5.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_ADD_INST} \
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.3.\0}}"
set AckMatchList(5.1.3) ""

}



#断开WAN连接
if {$TestCaseNo == 6} {

set MsgList(6.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.1) ""

set MsgList(6.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.2) ""

set MsgList(6.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{3 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}
   {InterfaceName=\0}
   {RemoteIPAddress=\0}}" 
set AckMatchList(6.1.3) ""
}


#删除WAN连接
if {$TestCaseNo == 7} {

set MsgList(7.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}" 
set AckMatchList(7.1.1) ""

set MsgList(7.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}}" 
set AckMatchList(7.1.2) ""

set MsgList(7.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_WAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.3.\0}}" 
set AckMatchList(7.1.3) ""

}

#删除LAN 连接测试
if {$TestCaseNo == 8} {

set MsgList(8.1.1) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{2 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0}
   {InterfaceName=eth0\0}}" 
set AckMatchList(8.1.1) ""

set MsgList(8.1.2) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0}
   {InterfaceName=eth2\0}}" 
set AckMatchList(8.1.2) ""

set MsgList(8.1.3) "{$MID_PPPOE  $MID_STATIC_ROUTING 0x00000001 $MSG_LAN_DEL_INST} 
{1 {Path=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.3.\0}
   {InterfaceName=eth3\0}}" 
set AckMatchList(8.1.3) ""

}


#删除实例测试
if {$TestCaseNo == 9} {

#正常流程
set MsgList(9.1.1) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.1.\0}}" 
set AckMatchList(9.1.1) ""

set MsgList(9.1.2) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.2.\0}}" 
set AckMatchList(9.1.2) ""

set MsgList(9.1.3) "{$MID_CMM  $MID_STATIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.Layer3Forwarding.Forwarding.3.\0}}" 
set AckMatchList(9.1.3) ""

}


