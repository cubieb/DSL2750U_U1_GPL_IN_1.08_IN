
#MSG_CMM_ADD_NODE消息测试
#增加实例
if {$TestCaseNo == 1} {

#正常流程
set MsgList(1.1.1) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}"
set AckMatchList(1.1.1) ""

set MsgList(1.1.2) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}" 
set AckMatchList(1.1.2) ""

}

#MSG_CMM_SET_VAL and MSG_CMM_COMMIT
#pppoe ipconn增加3个实例

if {$TestCaseNo == 2} {

set MsgList(2.1.1) "{$MID_CMM  $MID_PPPOE 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}" 
set AckMatchList(2.1.1) ""

set MsgList(2.1.2) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}" 
set AckMatchList(2.1.2) ""

set MsgList(2.1.3) "{$MID_CMM  $MID_PPPOE 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}" 
set AckMatchList(2.1.3) ""

set MsgList(2.1.4) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.\0}}" 
set AckMatchList(2.1.4) ""

set MsgList(2.1.5) "{$MID_CMM  $MID_IPCONN 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}" 
set AckMatchList(2.1.5) ""

set MsgList(2.1.6) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.\0}}" 
set AckMatchList(2.1.6) ""

#正常功能测试
set MsgList(2.2.1) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.RouteProtocolRx=RIPv1\0}
}"
set AckMatchList(2.2.1) ""

set MsgList(2.2.2) "{$MID_CMM $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.2.2) ""


set MsgList(2.2.3) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.RouteProtocolRx=Off\0}
}"
set AckMatchList(2.2.3) ""

set MsgList(2.2.4) "{$MID_CMM $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.2.4) ""


set MsgList(2.2.5) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.RouteProtocolRx=RIPv2\0}
}"
set AckMatchList(2.2.5) ""

set MsgList(2.2.6) "{$MID_CMM $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.2.6) ""

#异常测试
#值为空
set MsgList(2.3.1) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.RouteProtocolRx=\0}
}"
set AckMatchList(2.3.1) ""

#值错误
set MsgList(2.3.2) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.RouteProtocolRx=RIPv\0}
}"
set AckMatchList(2.3.2) ""

#值为空
set MsgList(2.3.3) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.RouteProtocolRx=\0}
}"
set AckMatchList(2.3.3) ""

#值错误
set MsgList(2.3.4) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.RouteProtocolRx=RIPv\0}
}"
set AckMatchList(2.3.4) ""

}

#MSG_WAN_CONN_EST
if {$TestCaseNo == 3} {

set MsgList(3.1.1) "{$MID_PPPOE  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=ppp0\0}}" 
set AckMatchList(3.1.1) ""

set MsgList(3.2.1) "{$MID_PPPOE  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=ppp0\0}}" 
set AckMatchList(3.2.1) ""

set MsgList(3.3.1) "{$MID_IPCONN  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_EST} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}
   {InterfaceName=eth0\0}}" 
set AckMatchList(3.3.1) ""

}


#MSG_WAN_CONN_FIN
if {$TestCaseNo == 4} {

set MsgList(4.1.1) "{$MID_PPPOE  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}
   {InterfaceName=ppp0\0}}" 
set AckMatchList(4.1.1) ""

set MsgList(4.2.1) "{$MID_PPPOE  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.2.\0}
   {InterfaceName=ppp0\0}}" 
set AckMatchList(4.2.1) ""

set MsgList(4.3.1) "{$MID_IPCONN  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_WAN_CONN_FIN} 
{2 {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}
   {InterfaceName=eth0\0}}" 
set AckMatchList(4.3.1) ""

}


#MSG_CMM_DEL_NODE

if {$TestCaseNo == 5} {

set MsgList(5.1.1) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0}}" 
set AckMatchList(5.1.1) ""

set MsgList(5.2.1) "{$MID_CMM  $MID_DYNAMIC_ROUTING 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0}}" 
set AckMatchList(5.2.1) ""

}


