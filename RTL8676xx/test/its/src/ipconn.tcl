#
# 配置IPCONN连接
#


#定义全局变量 
set WAN_PATH "InternetGatewayDevice.WANDevice"
set WAN_INST "InternetGatewayDevice.WANDevice.1"

set WANCONN_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice"
set WANCONN_INST "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1"

set IPCONN_PATH "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection"
set IPCONN_INST "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1"

set NAME        "Name"
set ENABLE      "Enable"
set ADDRTYPE    "AddressingType"
set EXTIPADDR   "ExternalIPAddress"
set MASK        "SubnetMask"
set GATEWAY     "DefaultGateway"

set DeviceName  "X_TWSZ-COM_DeviceName"
set DNSSRV      "DNSServers"
set CONNSTATUS  "ConnectionStatus"
set LASTERROR   "LastConnectionError"

set INT        int
set UINT       unsignedInt
set STR        string
set BOOL       boolean

set i 1
set j 1

#
# 测试编号说明:
#   1.添加所有节点,在系统启动后需首先执行,且只能运行1次,因为要匹配添加实例的实例号
#   2.设置基于优先级的队列
#   3.设置基于流量控制的队列
#   4.设置基于IP五元组的分类规则
#   5.设置基于DSCP的分类规则
#   6.参数检查测试
#   7.删除参数测试a,此项测试必须最后进行,因为要匹配实例号
#
#   测试用例的编号未按文档进行手工编号，导致测试调试时较难查找出错的用例，后续得修改一下。

################################################################################



#===============================================================================
#             测试ADD消息
#===============================================================================
if {$TestCaseNo == 1} {
set i $TestCaseNo

#######输入合法性检查#############
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_ADD_NODE消息，非法路径的情况(路径为空,路径不存在,多条路径等等)"
set comment($i.$j) "#路径为空"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE \
    {\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j
set comment($i.$j) "#路径不存在"
set MsgList($i.$j) "{$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE} \
    {$IPCONN_PATH.xxxx\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j
set comment($i.$j) "#多条路径"
set MsgList($i.$j) "{$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE} \
    {$IPCONN_PATH=xxxxxx\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

#######正常情况#############
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_ADD_NODE消息，正常路径的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j)  "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^1$ ^0$
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_ADD_NODE消息，再次添加连接,只允许一条"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j


}


#===============================================================================
#             测试SET消息
#===============================================================================
if {$TestCaseNo == 2} {
set i $TestCaseNo

#######输入合法性检查#############

#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息,路径异常公共模块已经检查,不再检查
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，路径不全（非叶子节点）的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001 $MSG_CMM_SET_VAL 
#    1 
#    {$IPCONN_PATH.1.=1\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，实例号不存在的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001 $MSG_CMM_SET_VAL 
#    1 
#    {$IPCONN_PATH.2.$ADDRTYPE=DHCP\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#return

#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，NATEnabled取值非法(情况1)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1 
#    {$IPCONN_INST.NATEnabled=On\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，NATEnabled取值非法(情况2)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#1 
#{$IPCONN_INST.NATEnabled=1024\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9005\0$}
#"
#incr j


set comment($i.$j) "测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置值为空的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1 
    {$IPCONN_INST.AddressingType=\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，AddressingType取值非法的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
1 
{$IPCONN_INST.AddressingType=Undefined\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，Name取值超长(超过256字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1 
    {$IPCONN_INST.Name=[string repeat "ipconn1" 100]\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，AddressingType取值非法的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.AddressingType=Auto\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，ExternalIPAddress取值非法(多一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，ExternalIPAddress取值非法(少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，ExternalIPAddress取值非法(中间少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10..10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，ExternalIPAddress取值非法(多一个.)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.10.10.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，ExternalIPAddress取值非法(超过255)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.ExternalIPAddress=10.10.256.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，SubnetMask取值非法(多一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.255.0.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，SubnetMask取值非法(少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，SubnetMask取值非法(中间少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255..0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，SubnetMask取值非法(多一个.)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.254.0.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，SubnetMask取值非法(超过255)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.SubnetMask=255.255.256.0\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DefaultGateway取值非法(多一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DefaultGateway取值非法(少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DefaultGateway取值非法(中间少一个字节)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10..10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DefaultGateway取值非法(多一个.)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.10.10.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DefaultGateway取值非法(超过255)的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    1
    {$IPCONN_INST.DefaultGateway=10.10.256.10\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}
"
incr j

#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(超过64字符)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#1 
#{$IPCONN_INST.DNSServers=[string repeat "10.10.10.10" 50]\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(多一个字节)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10.10.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(少一个字节)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(中间少一个字节)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10..10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(多一个.)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.10.10.\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(超过256)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.256.10\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j
#
#set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，DNSServers取值非法(分隔符不是,)的情况"
#set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
#    1
#    {$IPCONN_INST.DNSServers=10.10.20.10;10.10.20.20\0}
#"
#set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
#    {.*} {.*} {^1$} {.*9007\0$}
#"
#incr j

}

#===============================================================================
#             测试SET消息
#===============================================================================
if {$TestCaseNo == 3} {
set i $TestCaseNo

#######正常情况#############

if {1} {
###测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置Static IP方式连接,并提交
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置Static IP方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=0\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_COMMIT消息，提交配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置DHCPC方式连接,并提交
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置DHCPC方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    3 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_COMMIT消息，提交配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置Static IP方式连接,并取消
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置Static IP方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf003\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_CANCEL消息，取消配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_CANCEL"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
##测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置DHCPC方式连接,并取消
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置DHCPC方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    2 
    {$IPCONN_INST.Name=TestInf004\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_CANCEL消息，取消配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_CANCEL"
set AckMatchList($i.$j) ""
incr j
}

}



#==============================================================================
#             测试MSG_WAN_LINK_CHG建链消息
#==============================================================================
if {$TestCaseNo == 4} {
set i $TestCaseNo


###输入合法性检查###
if {1} {
set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，通知项个数多于实际项数的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，通知项个数少于实际项数的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少Path通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项个数多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0}
    {Name=eth1\0} 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=\0} {Status=Up\0} {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项值错误的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99999.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少NAME通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0} 
    {Name=eth0\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项值错误的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth99\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少STATUS通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项值非法的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j
}

#############正常情况###############

if {1} {
#测试WAN向IPCONN模块发送WAN_LINK_CHG消息, Static IP方式触发连接
set comment($i.$j) "#配置IP方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set comment($i.$j) "#提交配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息, Static IP方式触发连接"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}

if {1} {
###测试WAN向IPCONN模块发送WAN_LINK_CHG消息, DHCPC方式触发连接
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    3 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息, DHCPC方式触发连接"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Up\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}

}



#==============================================================================
#             测试MSG_WAN_LINK_CHG断链消息
#==============================================================================
if {$TestCaseNo == 5} {
set i $TestCaseNo


####输入合法性检查
if {1} {
set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，通知项个数多于实际项数的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，通知项个数少于实际项数的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少Path通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项个数多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0}\
    {Name=eth1\0} 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，Path通知项值错误的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.99999.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少NAME通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Down\0} 
    {Name=eth0\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，NAME通知项值错误的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth99\0} 
    {Status=Down\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，缺少STATUS通知项的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    2
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项多于1个的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    4
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} {Name=eth1\0} 
    {Status=Down\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项值为空的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息，STATUS通知项值非法的情况"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Name=eth1\0} 
    {Status=Up\0}
"
set AckMatchList($i.$j) ""
incr j
return
}


#############正常情况###############

###测试WAN向IPCONN模块发送WAN_LINK_CHG消息, Static IP方式断开连接
if {1} {
if {1} {
    set comment($i.$j) "#配置IP方式连接"
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
        7 
        {$IPCONN_INST.Enable=1\0}
        {$IPCONN_INST.Name=TestInf001\0}
        {$IPCONN_INST.AddressingType=Static\0}
        {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
        {$IPCONN_INST.SubnetMask=255.0.0.0\0}
        {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
        {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
    "
    set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {^0$} {^0$}
    "
    incr j
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#建立连接"
    set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
        3
        {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
        {Status=Up\0} 
        {Name=eth1\0}
    "
    set AckMatchList($i.$j) ""
    incr j
}

set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息, Static IP方式断开连接"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
}


###测试WAN向IPCONN模块发送WAN_LINK_CHG消息, DHCPC方式断开连接
if {1} {
if {1} {
    set comment($i.$j) "#配置DHCP方式连接"
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
        3 
        {$IPCONN_INST.Enable=1\0}
        {$IPCONN_INST.Name=TestInf002\0}
        {$IPCONN_INST.AddressingType=DHCP\0}
    "
    set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {^0$} {^0$}
    "
    incr j
    set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#建立连接"
    set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
        3
        {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
        {Status=Up\0} 
        {Name=eth1\0}
    "
    set AckMatchList($i.$j) ""
    incr j
}


set comment($i.$j) "#测试WAN向IPCONN模块发送WAN_LINK_CHG消息, DHCPC方式断开连接"
set MsgList($i.$j) "$MID_WAN  $MID_GRP_WAN_LINK_UPDATE  0x00000001  $MSG_WAN_LINK_CHG
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.\0} 
    {Status=Down\0} 
    {Name=eth1\0}
"
set AckMatchList($i.$j) ""
incr j
return
}

}


#==============================================================================
#             测试MSG_PC_PROC_OUTPUT消息
#==============================================================================
if {$TestCaseNo == 6} {
set i $TestCaseNo

###输入合法性检查
set comment($i.$j) "#测试PC向IPCONN模块发送MSG_PC_PROC_OUTPUT消息，没有匹配值的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_PC_PROC_OUTPUT
{ipconn.1.1\0}
1 
{5}
{1  10.28.100.23\0} 
{2  255.255.0.0\0} 
{3  10.28.100.1\0} 
{4  10.28.100.1\0} 
{5  {}}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试PC向IPCONN模块发送MSG_PC_PROC_OUTPUT消息"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_PC_PROC_OUTPUT
    {ipconn.1.1\0}
    1 
    5 
    {1  10.28.100.23\0} 
    {2  255.255.0.0\0} 
    {3  10.28.100.1\0} 
    {4  10.28.100.1\0} 
    {5  {Lease obtained, entering bound state\0}}
"
set AckMatchList($i.$j) ""
incr j

}




#===============================================================================
#             测试UPDATE消息
#===============================================================================
if {$TestCaseNo == 7} {
set i $TestCaseNo

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_UPDATE消息，更新配置"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_UPDATE"
set AckMatchList($i.$j) ""
incr j

}


#===============================================================================
#             测试DELETE消息
#===============================================================================
if {$TestCaseNo == 8} {

#######输入合法性检查#############
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_DEL_NODE消息，非法路径的情况(路径为空,路径不存在等等)"
set comment($i.$j) "#路径为空"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.xx.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#路径超长"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.[string repeat "aaaaa" 100]\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#路径不存在"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN 0x00000001 $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.2.\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

set comment($i.$j) "#路径缺少."
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN 0x00000001 $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.1\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}
"
incr j

#######正常情况#############
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_DEL_NODE消息，正常路径的情况"
set MsgList($i.$j) "$MID_WEB $MID_CMM 0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.1.\0}
"
set AckMatchList($i.$j) ""
incr j

set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_DEL_NODE消息，删除WANConn下所有实例"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_DEL_NODE
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j) ""
incr j

}

#===============================================================================
#             测试保存消息
#===============================================================================
if {$TestCaseNo == 9} {
set i $TestCaseNo


set comment($i.$j) "#保存配置"
set MsgList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001  $MSG_CMM_SAVE_CFG"
set AckMatchList($i.$j) ""
incr j
return

if {1} {
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_ADD_NODE消息，正常路径的情况"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_ADD_NODE 
    {$IPCONN_PATH.\0}
"
set AckMatchList($i.$j)  "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^1$ ^0$
"
incr j
}

if {1} {
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置Static IP方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf001\0}
    {$IPCONN_INST.AddressingType=Static\0}
    {$IPCONN_INST.ExternalIPAddress=202.10.44.50\0}
    {$IPCONN_INST.SubnetMask=255.0.0.0\0}
    {$IPCONN_INST.DefaultGateway=202.10.0.1\0}
    {$IPCONN_INST.DNSServers=202.10.0.253,202.10.0.254\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}
return

if {0} {
set comment($i.$j) "#测试CMM向IPCONN模块发送MSG_CMM_SET_VAL消息，设置DHCP方式连接"
set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_SET_VAL
    7 
    {$IPCONN_INST.Enable=1\0}
    {$IPCONN_INST.Name=TestInf002\0}
    {$IPCONN_INST.AddressingType=DHCP\0}
"
set AckMatchList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
{^0$} {^0$}
"
incr j

set MsgList($i.$j) "$MID_CMM  $MID_IPCONN  0x00000001  $MSG_CMM_COMMIT"
set AckMatchList($i.$j) ""
incr j
}

if {0} {
set comment($i.$j) "##向WAN模块发送MSG_MON_INTF_STATUS_INFORM消息"
set MsgList($i.$j) "$MID_ETHLAN  $MID_WAN  0x00000001  $MSG_MON_INTF_STATUS_INFORM
    3
    {Path=InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig.Enable\0}
    {Name=eth1\0}
    {Status=Connected\0}
"
set AckMatchList($i.$j) ""
incr j
}
return

}
