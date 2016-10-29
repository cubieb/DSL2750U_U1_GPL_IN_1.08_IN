#
# 配置SNMPD
#

#定义全局变量 
set SNMP_PATH "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent"

set L1 0
set L2 0
set L3 0

incr L1

####################################################################################
##1.1 设置SNMP
####################################################################################
incr L2
set L3 0

if {0 && $TestCaseNo == 1} {

################1.1.1 输入合法性检查############

	incr L3
	set L4 0
	
	set comment($L1.$L2.$L3.$L4) "# SetItemCount多于实际个数"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL 
	3 
    $SNMP_PATH.Enable=1\0
	$SNMP_PATH.ROCommunity=public\0
" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# SetItemCount少于实际个数"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable=1\0
	   $SNMP_PATH.ROCommunity=public\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 路径为空"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 =1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 路径超长"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable.$SNMP_PATH.Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 非叶子节点"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 路径多余"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable.X_TWSZ-COM_Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 路径包含空格"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH. X_TWSZ-COM_Enable=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 设置只读节点Status"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Status=1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# Enable布尔值非法"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.Enable=100\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
    {.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# ROCommunity超长"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.ROCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# RWCommunity超长"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.RWCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapVersion小于1"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapVersion=0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapVersion大于3"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapVersion=4\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHost主机名含非法字符"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=gj#com\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHost主机名超长"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.gj.com.\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP地址非法"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.256.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP地址全0"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=0.0.0.0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP地址全1"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=255.255.255.255\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP多一个字节"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.1.1.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapHostIP少一个字节"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapHost=192.168.1\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# TrapCommunity超长"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{1 $SNMP_PATH.TrapCommunity=1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 一组设置，其中第一项非法"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=100\0 
	   $SNMP_PATH.RWCommunity=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.TrapHost=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9007\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 一组设置，其中中间一项非法"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=1\0 
	   $SNMP_PATH.RWCommunity.ABC=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.TrapHost=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 一组设置，其中最后一项非法"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL} 
	{4 $SNMP_PATH.Enable=1\0 
	   $SNMP_PATH.RWCommunity=public\0 
	   $SNMP_PATH.TrapCommunity=public\0 
	   $SNMP_PATH.=192.168.1.12\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{.*} {.*} {^1$} {.*9005\0$}"
}

if {$TestCaseNo == 1} {

####################### 1.1.2正常功能测试####################
	incr L3
	set L4 0
	
	set comment($L1.$L2.$L3.$L4) "# 配置SNMP并启用"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL}
	{6 $SNMP_PATH.Enable=1\0
	   $SNMP_PATH.RWCommunity=public\0
	   $SNMP_PATH.RWCommunity=private\0
	   $SNMP_PATH.TrapVersion=2\0
	   $SNMP_PATH.TrapHost=192.168.1.12\0
	   $SNMP_PATH.TrapCommunity=public\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{^0$} {^0$}" 
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 提交"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_COMMIT" 
	set AckMatchList($L1.$L2.$L3.$L4) ""
    incr L4

	set comment($L1.$L2.$L3.$L4) "# 配置Enable=0"
	set MsgList($L1.$L2.$L3.$L4) "{$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_SET_VAL}
	{1 $SNMP_PATH.Enable=0\0}" 
	set AckMatchList($L1.$L2.$L3.$L4) "$MID_SNMPC $MID_CMM 0x00000001 $MSG_CMM_SET_VAL_ACK 
	{^0$} {^0$}" 
    incr L4
	
	set comment($L1.$L2.$L3.$L4) "# 提交"
	set MsgList($L1.$L2.$L3.$L4) "$MID_CMM  $MID_SNMPC 0x00000001 $MSG_CMM_COMMIT" 
	set AckMatchList($L1.$L2.$L3.$L4) ""
    incr L4
}


