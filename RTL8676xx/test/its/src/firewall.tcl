##
## 配置Firewall
##

##定义全局变量 
set FW_PATH "InternetGatewayDevice.X_TWSZ-COM_Firewall"

set i 1
set j 1




####################################################################################
### 12. 设置安全级别状态(SecurityLevel)
if {$TestCaseNo == 12} {
    set i $TestCaseNo

    ### 12.1 输入合法性检查

    set comment($i.$j) "#值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#SecurityLevel值非法:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=0n\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#SecurityLevel值非法:4"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=4\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#SecurityLevel值非法:1a"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=1a\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#SecurityLevel值非法:超长"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=[string repeat "1" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    #### 12.2 正常情况 ####

    set comment($i.$j) "#set SecurityLevel=0"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=0\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set SecurityLevel=1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set SecurityLevel=2"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=2\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set SecurityLevel=3"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=3\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.SecurityLevel=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_CANCEL}"
    set AckMatchList($i.$j) ""
    incr j

}


####################################################################################
### 13. 设置IP过滤状态(IPFilterEnable)
if {$TestCaseNo == 13} {
    set i $TestCaseNo

    ### 13.1 输入合法性检查

    set comment($i.$j) "#值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#bool值非法:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#bool值非法:1024"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=1024\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    
    set comment($i.$j) "#bool值非法:超长"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=[string repeat "1" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ### 13.2 正常情况

    set comment($i.$j) "#set IPFilterEnable 1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j
    return

    set comment($i.$j) "#set IPFilterEnable 0"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=0\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set IPFilterEnable 1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.IPFilterEnable=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j
    
    set comment($i.$j) "#取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_CANCEL} \
    {}"
    set AckMatchList($i.$j) ""
    incr j

}


####################################################################################
### 14. 设置CurrentLANFilterTable
if {$TestCaseNo == 14} {
    set i $TestCaseNo

    ### 14.1 输入合法性检查

    set comment($i.$j) "#值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentLANFilterTable值非法:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentLANFilterTable值非法:4"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=4\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentLANFilterTable值非法:1a"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=1a\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    # 14.2 正常情况

    set comment($i.$j) "#set CurrentLANFilterTable=1, 白名单"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j


    set comment($i.$j) "#set CurrentLANFilterTable=2, 黑名单"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=2\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set CurrentLANFilterTable=1, 然后取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentLANFilterTable=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_CANCEL} \
    {}"
    set AckMatchList($i.$j) ""
    incr j
    
    set comment($i.$j) "#get CurrentLANFilterTable"
    set MsgList($i.$j) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
    {1 {$FW_PATH.CurrentLANFilterTable\0}}"
    set AckMatchList($i.$j) ""
    incr j

}


####################################################################################
### 15. 设置CurrentWANFilterTable
if {$TestCaseNo == 15} {
    set i $TestCaseNo

    ### 15.1 输入合法性检查

    set comment($i.$j) "#值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentWANFilterTable值非法:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentWANFilterTable值非法:5"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=5\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#CurrentWANFilterTable值非法:1a"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=1a\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    # 15.2 正常情况

    set comment($i.$j) "#set CurrentWANFilterTable=3, 白名单"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=3\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set CurrentWANFilterTable=4, 黑名单"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=4\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set CurrentWANFilterTable=3, 白名单,然后取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.CurrentWANFilterTable=3\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_CANCEL} \
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#get CurrentWANFilterTable"
    set MsgList($i.$j) "{$MID_WEB $MID_CMM 0x00000001 $MSG_CMM_GET_VAL} \
    {1 {$FW_PATH.CurrentWANFilterTable\0}}"
    set AckMatchList($i.$j) ""
    incr j
    return
}


####################################################################################
### 21. 添加规则
if {$TestCaseNo == 21} {
    set i $TestCaseNo

    ###21.1输入检查
if {1} {
    set comment($i.$j) "#对象路径为空"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径错误"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanBlackFilter1.xxxxx.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径有多余部分"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanBlackFilter.1.DevName.xxxx\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径未以.结束"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanBlackFilter\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j
}

    ###21.2 正常情况
    set comment($i.$j) "#添加WanBlackFilter"
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanBlackFilter.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^1$ ^0$"
    incr j
}

####################################################################################
###22 添加规则,4个过滤表
if {$TestCaseNo == 23} {
    set i $TestCaseNo
    
    if {1} {
    set comment($i.$j) "#保存配置"
    set MsgList($i.$j) "$MID_IPCONN $MID_CMM 0x00000001  $MSG_CMM_SAVE_CFG"
    set AckMatchList($i.$j) ""
    incr j
    return
    }

    set comment($i.$j) "# Add WanBlackFilter Rule "
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanBlackFilter.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^2$ ^0$"
    incr j

    set comment($i.$j) "# Add WanWhiteFilter Rule "
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.WanWhiteFilter.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^2$ ^0$"
    incr j

    set comment($i.$j) "# Add LanBlackFilter Rule "
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.LanBlackFilter.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^2$ ^0$"
    incr j

    set comment($i.$j) "# Add LanWhiteFilter Rule "
    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
    {$FW_PATH.LanWhiteFilter.\0}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
    ^0$ ^2$ ^0$"
    incr j
}

####################################################################################
###24 往LanBlackFilter表中添加20条规则
if {$TestCaseNo == 24} {
    set i $TestCaseNo

    for {set j 1} {$j <= 21} {incr j} {
        set comment($i.$j) "# Add LanBlackFilter Rule $j"
        set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_ADD_NODE 
        {$FW_PATH.LanBlackFilter.\0}"
        if {$j < 20} {
            set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
                ^0$ ^${j}$ ^0$"
        } else { 
            # 超出容量
            set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_ADD_NODE_ACK 
                {.*} {.*} {^1$} {.*9007\0$}"
        }
    }

}

####################################################################################
### 31. 删除WanBlackFilter
if {$TestCaseNo == 31} {
    set i $TestCaseNo

    ###31.1输入检查
if {1} {
    set comment($i.$j) "#对象路径为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径错误"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter1.xxxxx.\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径有多余部分"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter.1.a\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径没有以.结束"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter.1\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#对象路径没有实例号"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j

    set comment($i.$j) "#实例idx没有对应实例"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter.1000.\0}}"
    set AckMatchList($i.$j) "$MID_FIREWALL  $MID_CMM  0x00000001  $MSG_CMM_DEL_NODE_ACK 
    {.*} {.*} {^1$} {.*9005\0$}"
    incr j
}
    ###31.2正常逻辑

    set comment($i.$j) "#删除添加WanBlackFilter"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_DEL_NODE} 
    {{$FW_PATH.WanBlackFilter.1.\0}}"
    set AckMatchList($i.$j) ""
    incr j

}

####################################################################################
### 41. 设置LanBlackFilter表数据
if {$TestCaseNo == 41} {
    set i $TestCaseNo

    ### 41.1 输入合法性检查
if {1} {
    ###set Enable
    set comment($i.$j) "#enable值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#enable值非法（字符): On"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#enable值非法(数字,边界值): -1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=-1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    
    set comment($i.$j) "#enable值非法(数字,边界值): 2"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=2\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    
    set comment($i.$j) "#enable值非法(数字,边界值): 1024"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=1024\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ###set Protocol
    set comment($i.$j) "#Protocol枚举值错误:ARP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=ARP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#Protocol枚举值错误:IGMP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=IGMP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ###set SrcIP
    set comment($i.$j) "#IP地址包含字母"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcIP=a.1.2.3\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#IP地址包含大于255的数字"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcIP=256.1.2.3\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#IP地址少字节"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcIP=127.1.2\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#IP地址多字节"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcIP=127.1.2.3.4\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ###set SrcPort
    set comment($i.$j) "#Port大于65536"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcPort=70000\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#Port非数字"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcPort=70a\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    
    set comment($i.$j) "#Port端口范围错误"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcPort=80:70\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ###set SrcMask
    set comment($i.$j) "#Mask不为连续1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcMask=255.129.0.0\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#Mask不符合IP格式"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcMask=255.129.0.0.a\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    
    ###set Description
    set comment($i.$j) "#Description超过长度"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Description=[string repeat "a" 257]\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
    return
}

    ### 41.2 正确输入
if {0} {
    set comment($i.$j) "#正常情况,enable值为0"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=0\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j
    
    set comment($i.$j) "#正常情况,enable值为1"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Enable=1\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Protocol为UDP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=UDP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Protocol为TCP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=TCP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Protocol为TCP/UDP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=TCP/UDP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,IP=172.16.3.96"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcIP=172.16.3.96\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Port=80"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcPort=80\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j
    
    set comment($i.$j) "#正常情况,Port=80:90"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcPort=80\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Mask=255.255.255.0"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.SrcMask=255.255.255.0\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#正常情况,Protocol为ICMP"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$FW_PATH.LanBlackFilter.1.Protocol=ICMP\0}}"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    return
}

if {0} {
    set comment($i.$j) "#多个表项同时设置并提交"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {10 
    {$FW_PATH.LanBlackFilter.1.Enable=1\0}
    {$FW_PATH.LanBlackFilter.1.SrcIP=2.2.2.3\0}
    {$FW_PATH.LanBlackFilter.1.SrcPort=3601:4000\0}
    {$FW_PATH.LanBlackFilter.1.SrcMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.1.DestIP=3.3.3.3\0}
    {$FW_PATH.LanBlackFilter.1.DestPort=0:1234\0}
    {$FW_PATH.LanBlackFilter.1.DstMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.1.Protocol=TCP\0}
    {$FW_PATH.LanBlackFilter.1.DevName=InternetGatewayDevice.LANDevice.1\0}
    {$FW_PATH.LanBlackFilter.1.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j
    return
}

if {1} {
    set comment($i.$j) "#多个表项同时设置并取消"
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {9 
    {$FW_PATH.LanBlackFilter.1.Enable=1\0}
    {$FW_PATH.LanBlackFilter.1.SrcIP=2.2.2.4\0}
    {$FW_PATH.LanBlackFilter.1.SrcPort=3601\0}
    {$FW_PATH.LanBlackFilter.1.SrcMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.1.DestIP=3.3.3.4\0}
    {$FW_PATH.LanBlackFilter.1.DestPort=1234\0}
    {$FW_PATH.LanBlackFilter.1.DstMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.1.Protocol=TCP\0}
    {$FW_PATH.LanBlackFilter.1.DevName=InternetGatewayDevice.LANDevice.1\0}
    {$FW_PATH.LanBlackFilter.1.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_CANCEL} \
    {}"
    set AckMatchList($i.$j) ""
    incr j
}

}

###42 规则设置正常情况
if {$TestCaseNo == 42} {
    set i $TestCaseNo
    
    set comment($i.$j) "#测试IPCONN向FIREWALL模块发送MSG_WAN_CONN_EST消息"
    set MsgList($i.$j) "{$MID_IPCONN  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_EST} 
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} 
    {ExternalIPAddress=10.10.10.10\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=eth1\0}
    }"
    set AckMatchList($i.$j) ""
    incr j

    ### WanBlackFilter
    set comment($i.$j) "# Set WanBlackFilter Rule "
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {9 
    {$FW_PATH.WanBlackFilter.2.SrcIP=5.5.5.2\0}
    {$FW_PATH.WanBlackFilter.2.SrcPort=5002\0}
    {$FW_PATH.WanBlackFilter.2.SrcMask=255.255.0.0\0}
    {$FW_PATH.WanBlackFilter.2.DestIP=3.3.3.2\0}
    {$FW_PATH.WanBlackFilter.2.DestPort=3002\0}
    {$FW_PATH.WanBlackFilter.2.DstMask=255.255.0.0\0}
    {$FW_PATH.WanBlackFilter.2.Protocol=TCP\0}
    {$FW_PATH.WanBlackFilter.2.DevName=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0}
    {$FW_PATH.WanBlackFilter.2.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    ### WanWhiteFilte
    set comment($i.$j) "# Set WanWhiteFilte Rule "
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {9 
    {$FW_PATH.WanWhiteFilter.2.SrcIP=5.5.5.1\0}
    {$FW_PATH.WanWhiteFilter.2.SrcPort=5001\0}
    {$FW_PATH.WanWhiteFilter.2.SrcMask=255.255.0.0\0}
    {$FW_PATH.WanWhiteFilter.2.DestIP=3.3.3.1\0}
    {$FW_PATH.WanWhiteFilter.2.DestPort=3001\0}
    {$FW_PATH.WanWhiteFilter.2.DstMask=255.255.0.0\0}
    {$FW_PATH.WanWhiteFilter.2.Protocol=TCP\0}
    {$FW_PATH.WanWhiteFilter.2.DevName=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1\0}
    {$FW_PATH.WanWhiteFilter.2.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    ### LanBlackFilter 
    set comment($i.$j) "# Set LanBlackFilter Rule "
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {9 
    {$FW_PATH.LanBlackFilter.2.SrcIP=5.5.5.4\0}
    {$FW_PATH.LanBlackFilter.2.SrcPort=5004\0}
    {$FW_PATH.LanBlackFilter.2.SrcMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.2.DestIP=3.3.3.4\0}
    {$FW_PATH.LanBlackFilter.2.DestPort=3004\0}
    {$FW_PATH.LanBlackFilter.2.DstMask=255.255.0.0\0}
    {$FW_PATH.LanBlackFilter.2.Protocol=TCP\0}
    {$FW_PATH.LanBlackFilter.2.DevName=InternetGatewayDevice.LANDevice.1\0}
    {$FW_PATH.LanBlackFilter.2.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    # LanWhiteFilter 
    set comment($i.$j) "# Set LanWhiteFilter Rule "
    set MsgList($i.$j) "{$MID_CMM  $MID_FIREWALL 0x00000001 $MSG_CMM_SET_VAL} 
    {9 
    {$FW_PATH.LanWhiteFilter.2.SrcIP=5.5.5.3\0}
    {$FW_PATH.LanWhiteFilter.2.SrcPort=5003\0}
    {$FW_PATH.LanWhiteFilter.2.SrcMask=255.255.0.0\0}
    {$FW_PATH.LanWhiteFilter.2.DestIP=3.3.3.3\0}
    {$FW_PATH.LanWhiteFilter.2.DestPort=3003\0}
    {$FW_PATH.LanWhiteFilter.2.DstMask=255.255.0.0\0}
    {$FW_PATH.LanWhiteFilter.2.Protocol=TCP\0}
    {$FW_PATH.LanWhiteFilter.2.DevName=InternetGatewayDevice.LANDevice.1\0}
    {$FW_PATH.LanWhiteFilter.2.Description=testport\0}
    }"
    set AckMatchList($i.$j) "^$MID_FIREWALL$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_FIREWALL  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j
}
 



###51 连接通知消息
if {$TestCaseNo == 51} {
    set i $TestCaseNo

    ### 51.1 输入合法性检查
if {0} {
    set comment($i.$j) "#错误的MSG_WAN_CONN_EST消息:Path错误"
    set MsgList($i.$j) "{$MID_IPCONN  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_EST} 
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.x.\0} 
    {ExternalIPAddress=10.10.10.10\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=eth1\0}
    }"

    set AckMatchList($i.$j) ""
    incr j

    return
}

    ### 51.2 正常情况

if {1} {
    set comment($i.$j) "#测试IPCONN向FIREWALL模块发送MSG_WAN_CONN_EST消息"
    set MsgList($i.$j) "{$MID_IPCONN  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_EST} 
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} 
    {ExternalIPAddress=10.10.10.10\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=eth1\0}
    }"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#测试IPCONN向FIREWALL模块发送MSG_WAN_CONN_FIN消息"
    set MsgList($i.$j) "{$MID_IPCONN  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_FIN}
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.\0} 
    {ExternalIPAddress=10.10.10.10\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=eth1\0}
    }"
    set AckMatchList($i.$j) ""
    incr j

    return
}

if {1} {
    set comment($i.$j) "#测试PPPOE向FIREWALL模块发送MSG_WAN_CONN_EST消息"
    set MsgList($i.$j) "{$MID_PPPOE  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_EST} 
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} 
    {ExternalIPAddress=10.10.10.20\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=ppp1\0}
    }"
    set AckMatchList($i.$j) ""
    incr j
    
    set comment($i.$j) "#测试PPPOE向FIREWALL模块发送MSG_WAN_CONN_FIN消息"
    set MsgList($i.$j) "{$MID_PPPOE  $MID_FIREWALL  0x00000001  $MSG_WAN_CONN_FIN} 
    {4 
    {Path=InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.\0} 
    {ExternalIPAddress=10.10.10.20\0} 
    {SubnetMask=255.0.0.0\0} 
    {InterfaceName=ppp1\0}
    }"
    set AckMatchList($i.$j) ""
    incr j
    return
}

}




 

