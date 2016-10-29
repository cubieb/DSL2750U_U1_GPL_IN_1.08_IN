#this tcl script includes test cases for module "LAN"

set MajorSum 4

####################################################################################



################################ 设置LAN ip接口 ###############################

if {$TestCaseNo == "all" || $TestCaseNo == 1} {

#----------------------------- 输入合法性检查 ----------------------------
### 未知消息类型
set MsgList(1.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 {$MSG_CMM_SET_VAL $MSG_CMM_SET_VAL_ACK}} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.1.1) ""


### 无效路径
set MsgList(1.1.2.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress111=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.2.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^9005$} {^9005$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress111=00009005.*\0$}}"


### 节点名超长
set MsgList(1.1.3.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress[string repeat "IPInterfaceIPAddress" 30]=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.3.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^9005$} {^9005$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress.*=00009005.*\0$}}"


### 节点值超长
set MsgList(1.1.4.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66[string repeat "11:22:33:44:55:66" 30]\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.4.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^9007$} {^9007$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=00009007.*\0$}}"


### 节点值不存在
set MsgList(1.1.5.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.5.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^9007$} {^9007$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=00009007.*\0$}}"


### 非法MAC地址
set MsgList(1.1.6.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.6.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^98575151$} {^98575151$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=15049007.*\0$}}"


### 非法接口状态
set MsgList(1.1.7.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=2\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.7.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^66462511$} {^66462511$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=10149007.*\0$}}"


### IP地址是网络地址
set MsgList(1.1.8.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.1.0\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.8.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^98444079$} {^98444079$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=15029007.*\0$}}"


### IP地址是广播地址
set MsgList(1.1.9.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.1.255\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.9.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^98444079$} {^98444079$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=15029007.*\0$}}"

### IP地址冲突
set MsgList(1.1.10.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.1.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.1.10.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^.*$} {^.*$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.*=15069007.*\0$}}"


#----------------------------- 配置没有改变时的正常流程 ----------------------------
### 初始配置
set MsgList(1.2.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.2.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.1.2) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_COMMIT} {}"
set AckMatchList(1.2.1.2) ""


### 取消配置
set MsgList(1.2.1.3) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.2.1.3) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.1.4) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_CANCEL} {}"
set AckMatchList(1.2.1.4) ""


### 提交配置
set MsgList(1.2.1.5) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:76\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.3.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.2.1.5) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.1.6) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_COMMIT} {}"
set AckMatchList(1.2.1.6) ""


#----------------------------- 配置改变时的正常流程 ----------------------------
### 初始配置
set MsgList(1.2.2.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(1.2.2.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.2.2) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_COMMIT} {}"
set AckMatchList(1.2.2.2) ""


### 取消配置
set MsgList(1.2.2.3) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:11:11:11:11:11\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.1.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.0.0\0} 
}"
set AckMatchList(1.2.2.3) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.2.4) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_CANCEL} {}"
set AckMatchList(1.2.2.4) ""


### 提交配置
set MsgList(1.2.2.5) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:11:11:11:11:11\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.1.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.0.0\0} 
}"
set AckMatchList(1.2.2.5) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(1.2.2.6) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_COMMIT} {}"
set AckMatchList(1.2.2.6) ""

}



################################ 获取LAN 配置信息 ###############################

if {$TestCaseNo == "all" || $TestCaseNo == 2} {

### 初始配置
set MsgList(2.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_SET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0} 
}"
set AckMatchList(2.1.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_SET_VAL_ACK$}} {{^0$} {^0$}}"

set MsgList(2.1.1.2) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.1.1.2) ""


### 正常情况
set MsgList(2.1.1.3) "{$MID_WEB $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask\0}
}"
set AckMatchList(2.1.1.3) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^0$} {^4$} 
	{^string.* InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses=11:22:33:44:55:66\0$} 
    {^boolean InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable=1\0$} 
    {^string InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress=192.168.2.1\0$} 
	{^string InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask=255.255.255.0\0$} 
}"


### 未知消息类型
set MsgList(2.1.2.1) "{$MID_CMM $MID_LAN 0x00000001 {$MSG_CMM_GET_VAL $MSG_CMM_VAL}} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask\0}\
}"
set AckMatchList(2.1.2.1) ""


### 非法路径
set MsgList(2.1.3.1) "{$MID_WEB $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} {4
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses111\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress\0} 
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask\0}
}"
set AckMatchList(2.1.3.1) "{{^$MID_CMM$} {^$MID_WEB$} {^0x00000001$} {^$MSG_CMM_GET_VAL_ACK$}} {{^.*$} {^.*$} {^1$}
	{^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.AllowedMACAddresses111=.*\0$} 
}"

}

###########delete msg process###################
if {$TestCaseNo == "all" || $TestCaseNo == 3} {

### 删除非主lan device
set MsgList(3.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.2.\0} 
}"
set AckMatchList(3.1.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} {{^0$} {^0$}}"

### 删除主lan device
set MsgList(3.1.2.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.\0} 
}"
set AckMatchList(3.1.2.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} 
	{{^.*$} {^.*$} {^1$}  {^InternetGatewayDevice.LANDevice.1.=15079007\0$} 
}"

}

###########update msg process###################

if {$TestCaseNo == "all" || $TestCaseNo == 4} {

### 正常情况
set MsgList(4.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_UPDATE} {}"
set AckMatchList(4.1.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_UPDATE_ACK$}} {{^0$} }"

}

###########add msg process###################
if {$TestCaseNo == "all" || $TestCaseNo == 5} {

### 正常情况
set MsgList(5.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_ADD_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.\0} 
}"
set AckMatchList(5.1.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_ADD_NODE_ACK$}}  
	{{^0$} {^3$} {^0$}}"

set MsgList(5.1.2.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_ADD_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.\0} 
}"
set AckMatchList(5.1.2.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_ADD_NODE_ACK$}}  
	{{^0$} {^4$} {^0$}}"

##超出最大四个接口的限制
set MsgList(5.1.3.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_ADD_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.\0} 
}"
set AckMatchList(5.1.3.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_ADD_NODE_ACK$}} {{^.*$} {^.*$} {^1$}
	{^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.=.*\0$} 
}"

}


###########delete msg process###################
if {$TestCaseNo == "all" || $TestCaseNo == 6} {

### 路径错误
set MsgList(6.1.1.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.5.\0} 
}"
set AckMatchList(6.1.1.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} 
	{{^95167277$} {^95167277$} {^1$} {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.5.=14529005\0$} 
}"

### 删除主lan device非主interface
set MsgList(6.1.2.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0} 
}"
set AckMatchList(6.1.2.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} {{^0$} {^0$}}"

##删除主lan device主interface
set MsgList(6.1.3.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0} 
}"
set AckMatchList(6.1.3.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} 
	{{^98771759$} {^98771759$} {^1$}  {^InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.=15079007\0$} 
}"

### 删除非主lan device非主interface
set MsgList(6.1.4.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.2.\0} 
}"
set AckMatchList(6.1.4.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} {{^0$} {^0$}}"

##删除非主lan device主interface
set MsgList(6.1.5.1) "{$MID_CMM $MID_LAN 0x00000001 $MSG_CMM_DEL_NODE} {
	{InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.\0} 
}"
set AckMatchList(6.1.5.1) "{{^$MID_LAN$} {^$MID_CMM$} {^0x00000001$} {^$MSG_CMM_DEL_NODE_ACK$}} {{^0$} {^0$}}"

}

##添加桥设备
if {$TestCaseNo == "all" || $TestCaseNo == 8} {

##桥名称为空
set MsgList(8.1.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_ADD_BRIDGE} {1
{\0}
}"
set AckMatchList(8.1.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_ADD_BRIDGE_ACK$}} {{^2$} 
	{Result=1\0}
	{InfoString=BridgeName is empty\0}
}"

##桥名称不正确
set MsgList(8.1.2.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_ADD_BRIDGE } {1
 {BridgeName=abc\0}
 }"
 
set AckMatchList(8.1.2.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_ADD_BRIDGE_ACK$}} {{^2$} 
	{Result=1\0}
	{InfoString=BridgeName is empty\0}
}"

##正常流程
set MsgList(8.2.1.1) "$MID_VLAN  $MID_LAN  0x00000001  $MSG_VLAN_ADD_BRIDGE  \
                      1  BridgeName=br1\0
                     "
set AckMatchList(8.2.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_ADD_BRIDGE_ACK$}} {{^2$} 
    {Result=0\0}
    {InfoString=^InternetGatewayDevice.LANDevice.*\0$}}"

}

##删除桥设备
if {$TestCaseNo == "all" || $TestCaseNo == 9} {

##LANDevice路径超长
set MsgList(9.1.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_DEL_BRIDGE} {1
{LANDevicePath=InternetGatewayDevice.abc.2.[string repeat "IPInterfaceIPAddress" 300]\0}
}"
set AckMatchList(9.1.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_DEL_BRIDGE_ACK$}} {{^2$} 
	{Result=1\0}
	{InfoString=LANDevicePath is incorrect\0}
}"

##LANDevice路径不正确
set MsgList(9.1.2.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_DEL_BRIDGE} {1
{LANDevicePath=InternetGatewayDevice.abc.2.\0}
}"
set AckMatchList(9.1.2.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_DEL_BRIDGE_ACK$}} {{^2$} 
	{Result=1\0}
	{InfoString=LANDevicePath is incorrect\0}
}"

##LANDevice路径为空
set MsgList(9.1.3.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_DEL_BRIDGE} {1
{LANDevicePath=\0}
}"
set AckMatchList(9.1.3.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_DEL_BRIDGE_ACK$}} {{^2$} 
	{Result=1\0}
	{InfoString=LANDevicePath is incorrect\0}
}"


##正常流程
set MsgList(9.1.4.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_DEL_BRIDGE} {1
{LANDevicePath=InternetGatewayDevice.LANDevice.2.\0}
}"
set AckMatchList(9.1.4.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_DEL_BRIDGE_ACK$}} {{^2$} 
	{Result=0\0}
	{InfoString=LANDevicePath is incorrect\0}
}"

}


##添加桥绑定接口
if {$TestCaseNo == "all" || $TestCaseNo == 10} {

##LANDevice路径为空
set MsgList(10.1.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_BIND_PORT} 
{2
{LANDevicePath=\0}
{ToBindPort=\0}
}"
set AckMatchList(10.1.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_BIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath or ToBindPort is empty\0}
}"



##LANDevice路径不正确
set MsgList(10.1.2.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_BIND_PORT} 
{2
{LANDevicePath=InternetGatewayDevice.abc.2.\0}
{ToBindPort=eth0.2\0}
}"
set AckMatchList(10.1.2.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_BIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath is incorrect\0}
}"

##LANDevice路径超长
set MsgList(10.1.3.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_BIND_PORT} 
{2
{LANDevicePath=[string repeat "IPInterfaceIPAddress" 300]\0}
{ToBindPort=[string repeat "IPInterfaceIPAddress" 300]\0}
}"
set AckMatchList(10.1.3.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_BIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath or ToBindPort is empty\0}
}"

##正常流程
set MsgList(10.2.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_BIND_PORT} 
{2
{LANDevicePath=InternetGatewayDevice.LANDevice.1.\0}
{ToBindPort=eth0.1,eth0.2,eth0.3,eth0.4\0}
}"
set AckMatchList(10.2.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_BIND_PORT_ACK$}} 
{{^2$} 
    {Result=0\0}
    {InfoString=SUCCESS\0}
}"

}

##删除桥绑定接口
if {$TestCaseNo == "all" || $TestCaseNo == 11} {


##LANDevice路径为空
set MsgList(11.1.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_UNBIND_PORT} 
{2
{LANDevicePath=\0}
{ToBindPort=\0}
}"
set AckMatchList(11.1.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_UNBIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath or ToUnBindPort is empty\0}
}"



##LANDevice路径不正确
set MsgList(11.1.2.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_UNBIND_PORT} 
{2
{LANDevicePath=InternetGatewayDevice.abc.2.\0}
{ToUnBindPort=eth0.2\0}
}"
set AckMatchList(11.1.2.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_UNBIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath is incorrect\0}
}"

##LANDevice路径超长
set MsgList(11.1.3.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_UNBIND_PORT} 
{2
{LANDevicePath=[string repeat "IPInterfaceIPAddress" 300]\0}
{ToUnBindPort=[string repeat "IPInterfaceIPAddress" 300]\0}
}"
set AckMatchList(11.1.3.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_UNBIND_PORT_ACK$}} 
{{^2$} 
    {Result=1\0}
    {InfoString=LANDevicePath or ToBindPort is empty\0}
}"

##正常流程
set MsgList(11.2.1.1) "{$MID_VLAN $MID_LAN 0x00000001 $MSG_VLAN_UNBIND_PORT} 
{2
{LANDevicePath=InternetGatewayDevice.LANDevice.2.\0}
{ToUnBindPort=eth0.1,eth0.2,eth0.3,eth0.4\0}
}"
set AckMatchList(11.2.1.1) "{{^$MID_LAN$} {^$MID_VLAN$} {^0x00000001$} {^$MSG_VLAN_UNBIND_PORT_ACK$}} 
{{^2$} 
    {Result=0\0}
    {InfoString=SUCCESS\0}
}"

}

if {$TestCaseNo == "all" || $TestCaseNo == 12} {
set MsgList(12.1.1.1) "$MID_WEB $MID_CMM 0x00000001 $MSG_CMM_SAVE_CFG "
set AckMatchList(12.1.1.1) ""

}