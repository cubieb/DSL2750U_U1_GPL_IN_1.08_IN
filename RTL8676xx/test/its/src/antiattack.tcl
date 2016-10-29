##
## 配置Anti Attack
##

##定义全局变量 
set ANTI_PATH "InternetGatewayDevice.X_TWSZ-COM_AntiAttack"

set i 1
set j 1




####################################################################################
### 11. 设置DOS ENABLE状态(AntiDosEnable)
if {$TestCaseNo == 11} {
    set i $TestCaseNo

    ########### 11.1 输入合法性检查######

    set comment($i.$j) "#值为空"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#bool值非法:On"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=0n\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#bool值非法:1024"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=1024\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    set comment($i.$j) "#bool值非法:超长"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=[string repeat "1" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j

    ## 11.2 正常情况

    set comment($i.$j) "#set AntiDosEnable=1"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL
    5 
    {$ANTI_PATH.AntiDos.AntiDosEnable=1\0}
    {$ANTI_PATH.AntiDos.SynCookieEnable=0\0}
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=0\0}
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=0\0}
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=0\0}
    "
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#set AntiDosEnable=0"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL 
    5
    {$ANTI_PATH.AntiDos.AntiDosEnable=0\0}
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0}
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0}
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0}
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0}
    "
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set comment($i.$j) "#提交"
    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

}




####################################################################################
### 16. 设置AntiDos相关子项
if {$TestCaseNo == 16} {
    set i $TestCaseNo

    # 15.2 正常情况

    set comment($i.$j) "#开启antidos总开关,开启子功能开关"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=1\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=1\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j


    set comment($i.$j) "#开启antidos总开关,关闭子功能开关"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=1\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=0\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=0\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=0\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=0\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j

    set comment($i.$j) "#关闭antidos总开关,开启子功能开关"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {17 \
    {$ANTI_PATH.AntiDosEnable=0\0} \
    {$ANTI_PATH.AntiDos.SynCookieEnable=1\0} \
    {$ANTI_PATH.AntiDos.IcmpEchoIgnrBcastEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiFraggleEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiEchoCargenEnable=1\0} \
    {$ANTI_PATH.AntiDos.AntiIPLandEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetSynRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetFinRstEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetPshEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnAckSetUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanUnsetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanSetAllEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetSynRstAckFinUrgEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinEnable=1\0} \
    {$ANTI_PATH.AntiScan.AntiScanForAllSetFinUrgPshEnable=1\0} \
    }"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {^0$} {^0$}"
    incr j

    set MsgList($i.$j) "$MID_CMM  $MID_ANTIATTACK  0x00000001  $MSG_CMM_COMMIT
    {}"
    set AckMatchList($i.$j) ""
    incr j
}

####################################################################################
### 17. 数据异常
if {$TestCaseNo == 17} {
    set i $TestCaseNo

    set comment($i.$j) "#AntiDosEnable 数据长度异常"
    set MsgList($i.$j) "{$MID_CMM  $MID_ANTIATTACK 0x00000001 $MSG_CMM_SET_VAL} 
    {1 {$ANTI_PATH.AntiDosEnable=[string repeat "ab" 1024]\0}}"
    set AckMatchList($i.$j) "^$MID_ANTIATTACK$ ^$MID_CMM$ ^0x00000001$ ^$MSG_CMM_SET_VAL_ACK$  
    {.*} {.*} {^1$} {.*9007\0$}"
    incr j
}