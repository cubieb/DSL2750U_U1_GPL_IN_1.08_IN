
###########################集成测试脚本中需要的命令##############################

### CreateMsg命令根据消息头列表和消息体列表生成消息
proc CreateMsg {MsgList} {
    global ScriptPath int short
     
    ###载入模块ID和消息类型定义脚本
    source "$ScriptPath/lib/msg-head.tcl"
    
    set MsgHeadList [lindex $MsgList 0]
    set MsgBodyList [lindex $MsgList 1]
    
    set SrcMID  [binary format "$short" [lindex $MsgHeadList 0]]
    set DstMID  [binary format "$short" [lindex $MsgHeadList 1]]
    
    set MsgIDVal [format %u [lindex $MsgHeadList 2]] 
    set MsgID    [binary format "$int" $MsgIDVal]
    
    set TrueMsgTypeVal [lindex [lindex $MsgHeadList 3] 0]
    
    set FalseMsgTypeVal [lindex [lindex $MsgHeadList 3] 1]
    if {$FalseMsgTypeVal != ""} {
        set MsgType [binary format "$short" $FalseMsgTypeVal]
    } else {
        set MsgType [binary format "$short" $TrueMsgTypeVal]
    }
    
    set Reserve [binary format "$short" 0]
    
    #根据消息类型和消息体列表设置消息体
    switch $TrueMsgTypeVal\
        $MSG_CMM_SAVE_CFG {
            set MsgBody ""
        }\
        $MSG_CMM_RECOVER_CFG {
            set MsgBody ""
        }\
        $MSG_CMM_SET_VAL {
            set SetItemCountVal [lindex $MsgBodyList 0]
            if {$SetItemCountVal != ""} {
                set SetItemCount [binary format "$int" $SetItemCountVal]
            } else {
                set SetItemCount ""
            }
            
            set SetItem [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$SetItemCount$SetItem"
        }\
        $MSG_CMM_COMMIT {
            set MsgBody ""
        }\
        $MSG_CMM_CANCEL {
            set MsgBody ""
        }\
        $MSG_CMM_GET_NAME {
            set NextLevelVal [lindex $MsgBodyList 0]
            if {$NextLevelVal != ""} {
                set NextLevel [binary format "c" $NextLevelVal]
            } else {
                set NextLevel ""
            }
            
            set Path [lindex $MsgBodyList 1]
            set MsgBody "$NextLevel$Path"
        }\
        $MSG_CMM_GET_VAL {                
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"
        }\
        $MSG_CMM_ADD_NODE {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath
        }\
        $MSG_CMM_DEL_NODE {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath
        }\
        $MSG_CMM_SET_ATTR {
            set SetItemCountVal [lindex $MsgBodyList 0]
            if {$SetItemCountVal != ""} {
                set SetItemCount [binary format "$int" $SetItemCountVal]
            } else {
                set SetItemCount ""
            }
            
            set SetItem [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$SetItemCount$SetItem"            
        }\
        $MSG_CMM_GET_ATTR {
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"                 
        }\
        $MSG_CMM_GET_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_CLEAR_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_INFORM_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_REBOOT {
            set MsgBody ""
        }\
        $MSG_CMM_UPDATE {
            set MsgBody ""
        }\
        $MSG_PC_START_PROC {
            set NeedSteadyVal [lindex $MsgBodyList 0]
            if {$NeedSteadyVal != ""} {
                set NeedSteady [binary format "c" $NeedSteadyVal]
            } else {
                set NeedSteady ""
            }
            
            set ArgCountVal [lindex $MsgBodyList 1]
            if {$ArgCountVal != ""} {
                set ArgCount [binary format "c" $ArgCountVal]
            } else {
                set ArgCount ""
            }
            
            set ArgVal [join [lindex $MsgBodyList 2] {}]
            
            
            set MonCountVal [lindex $MsgBodyList 3]
            if {$MonCountVal != ""} {
                set MonCount [binary format "c" $MonCountVal]
            } else {
                set MonCount ""
            }
            
            set MonVal [join [lindex $MsgBodyList 4] {}]
            
            
            set MsgBody "$NeedSteady$ArgCount$ArgVal$MonCount$MonVal"
        }\
        $MSG_PC_SEND_SIGNAL {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }
                
            set SignalVal [lindex $MsgBodyList 2]
            if {$SignalVal != ""} {
                set Signal [binary format "c" $SignalVal]
            } else {
                set Signal ""
            }
            
            set NeedRestartVal [lindex $MsgBodyList 3]
            if {$NeedRestartVal != ""} {
                set NeedRestart [binary format "c" $NeedRestartVal]
            } else {
                set NeedRestart ""
            }
                
            set NeedSteadyVal [lindex $MsgBodyList 4]
            if {$NeedSteadyVal != ""} {
                set NeedSteady [binary format "c" $NeedSteadyVal]
            } else {
                set NeedSteady ""
            }
            
            set ArgCountVal [lindex $MsgBodyList 5]
            if {$ArgCountVal != ""} {
                set ArgCount [binary format "c" $ArgCountVal]
            } else {
                set ArgCount ""
            }
            
            set ArgVal [join [lindex $MsgBodyList 6] {}]
            
            set MonCountVal [lindex $MsgBodyList 7]
            if {$MonCountVal != ""} {
                set MonCount [binary format "c" $MonCountVal]
            } else {
                set MonCount ""
            }
            
            set MonVal [join [lindex $MsgBodyList 8] {}]
                       
            set MsgBody "$Name$InstNum$Signal$NeedRestart$NeedSteady$ArgCount$ArgVal$MonCount$MonVal"
        }\
        $MSG_PC_PROC_STATE {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }                                          
                  
            set ProcStateVal [lindex $MsgBodyList 2]
            if {$ProcStateVal != ""} {
                set ProcState [binary format "c" $ProcStateVal]
            } else {
                set ProcState ""
            }
            
            set ExitSignalVal [lindex $MsgBodyList 3]
            if {$ExitSignalVal != ""} {
                set ExitSignal [binary format "c" $ExitSignalVal]
            } else {
                set ExitSignal ""
            }            
            
            set LastSignalVal [lindex $MsgBodyList 4]
            if {$LastSignalVal != ""} {
                set LastSignal [binary format "c" $LastSignalVal]
            } else {
                set LastSignal ""
            }
            
            set LastItem [lindex $MsgBodyList 5]
            if {$LastItemVal != ""} {
                set LastItem [binary format "c" $LastItemVal]
            } else {
                set LastItem ""
            }           
            
            set MsgBody "$Name$InstNum$ProcState$ExitSignal$LastSignal$LastItem"
        }\
        $MSG_PC_PROC_OUTPUT {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }                                          
            
            set MatchCountVal [lindex $MsgBodyList 2]
            if {$MatchCountVal != ""} {
                set MatchCount [binary format "c" $MatchCountVal]
            } else {
                set MatchCount ""
            }
            
            set ItemSum [llength $MsgBodyList]
            set MatchItem ""
            
            for {set index 3} {$index < $ItemSum} {incr index} {
                set MatchItemStr [lindex $MsgBodyList $index]
                
                set MatchMonNoVal [lindex $MatchItemStr 0]
                if {$MatchMonNoVal != ""} {
                    set MatchMonNo [binary format "c" $MatchMonNoVal]
                } else {
                    set MatchMonNo ""
                }
                
                set MatchVal  [join [lindex $MatchItemStr 1] {}]
                set MatchItem "$MatchItem$MatchMonNo$MatchVal"
            }
           
            
            set MsgBody "$Name$InstNum$MatchCount$MatchItem"  
        }\
        $MSG_TR069_SET_PARKEY {
            set ParameterKey [join [lindex $MsgBodyList 0] {}] 
            set MsgBody "$ParameterKey"           
        }\
        $MSG_TR069_GET_EVENTS {
            set MsgBody "" 
        }\
        $MSG_TR069_CLEAR_EVENTS {
            set MsgBody ""
        }\
        $MSG_TR069_DOWNLOAD {
            set FileSizeVal [lindex $MsgBodyList 0]
            if {$FileSizeVal != ""} {
                set FileSize [binary format "$int" $FileSizeVal]
            } else {
                set FileSize ""
            }
            
            set DelaySecondsVal [lindex $MsgBodyList 1]
            if {$DelaySecondsVal != ""} {
                set DelaySeconds [binary format "$int" $DelaySecondsVal]
            } else {
                set DelaySeconds ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 2 end] {}]
            
            set MsgBody "$FileSize$DelaySeconds$ItemStr"
                        
        }\
        $MSG_TR069_UPLOADLOAD {
            set DelaySecondsVal [lindex $MsgBodyList 0]
            if {$DelaySecondsVal != ""} {
                set DelaySeconds [binary format "$int" $DelaySecondsVal]
            } else {
                set DelaySeconds ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$DelaySeconds$ItemStr"
        }\
        $MSG_TR069_GET_TRANSFERCOMPLETEINFO {
            set MsgBody ""
        }\
        $MSG_TR069_HTTPD_CONNECT {
            set MsgBody ""
        }\
        $MSG_TIMER_REGSTER {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId  [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set TypeVal [lindex $MsgBodyList 1]
            if {$TypeVal != ""} {
                set Type [binary format "c" $TypeVal]
            } else {
                set Type ""
            }
                
            set SpaceVal [lindex $MsgBodyList 2]
            if {$SpaceVal != ""} {
                set Space [binary format "c" $SpaceVal]
            } else {
                set Space ""
            }
            
            set YearVal [lindex $MsgBodyList 3]
            if {$YearVal != ""} {
                set Year  [binary format "c" $YearVal]
            } else {
                set Year ""
            }
            
            set MonthVal [lindex $MsgBodyList 4]
            if {$MonthVal != ""} {
                set Month [binary format "c" $MonthVal]
            } else {
                set Month ""
            }
            
            set DateVal [lindex $MsgBodyList 5]
            if {$DateVal != ""} {
                set Date [binary format "c" $DateVal]
            } else {
                set Date ""
            }
            
            set HourVal [lindex $MsgBodyList 6]
            if {$HourVal != ""} {
                set Hour [binary format "c" $HourVal]
            } else {
                set Hour ""
            }
            
            set MinuteVal [lindex $MsgBodyList 7]
            if {$MinuteVal != ""} {
                set Minute [binary format "c" $MinuteVal]
            } else {
                set Minute ""
            }
            
            set SecondVal [lindex $MsgBodyList 8]
            if {$SecondVal != ""} {
                set Second [binary format "c" $SecondVal]
            } else {
                set Second ""
            }
            
            
            set IntervalVal [lindex $MsgBodyList 9]
            if {$IntervalVal != ""} {
                set Interval [binary format "$int" $IntervalVal]
            } else {
                set Interval ""
            }
            
            set Name [join [lindex $MsgBodyList 10] {}]
            
            set MsgBody "$TimerId$Type$Space$Year$Month$Date$Hour$Minute$Second$Interval$Name"
        }\
        $MSG_TIMER_TIMEOUT {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set MsgBody "$TimerId"
        }\
        $MSG_TIMER_UNREGSTER {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set MsgBody "$TimerId"
        }\
        $MSG_MON_INTF_STATUS_INFORM {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_MON_INTF_STATUS_QUERY {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"             
        }\
        $MSG_MON_INTF_REGISTER {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"         
        }\
        $MSG_MON_INTF_UNREGISTER {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"
        }\
        $MSG_WAN_CONN_EST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
        
            set MsgBody "$ItemCount$Item"       
        }\
        $MSG_WAN_CONN_FIN {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
        
            set MsgBody "$ItemCount$Item"         
        }\
        $MSG_WAN_LINK_CHG {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"
        }\
        $MSG_LAN_IP_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set IpAddr   [lindex $MsgBodyList 1]
            set Mask        [lindex $MsgBodyList 2]
            
            set MsgBody "$LanDevNo$IpAddr$Mask"
        }\
        $MSG_LAN_MACLIST_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set MacList [lindex $MsgBodyList 1]
            
            set MsgBody "$LanDevNo$MacList"
        }\
        $MSG_LAN_DOMAIN_NAME_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set DomainName  [lindex $MsgBodyList 1]
            
            set MsgBody "$LanDevNo$DomainName"
        }\
        $MSG_GET_DHOST_LIST {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set MsgBody "$LanDevNo"
        }\
        $MSG_DNS_CHANGE {
            set DnsRelayEnVal [lindex $MsgBodyList 0]
            if {$DnsRelayEnVal != ""} {
                set DnsRelayEn [binary format "c" $DnsRelayEnVal]
            } else {
                set DnsRelayEn ""
            }
            
            set DNS            [lindex $MsgBodyList 1]
            
            set MsgBody "$DnsRelayEn$DNS"          
        }\
        $MSG_WLAN_ADD_DEVICE {
            set LanDevIdVal [lindex $MsgBodyList 0]
            if {$LanDevIdVal != ""} {
                set LanDevId [binary format "$int" $LanDevIdVal]
            } else {
                set LanDevId ""
            }
                        
            set IntfName    [lindex $MsgBodyList 1]

            set MsgBody "$LanDevId$IntfName"
        }\
        $MSG_WLAN_DEL_DEVICE {
            set LanDevIdVal [lindex $MsgBodyList 0]
            if {$LanDevIdVal != ""} {
                set LanDevId [binary format "$int" $LanDevIdVal]
            } else {
                set LanDevId ""
            }
                        
            set IntfName    [lindex $MsgBodyList 1]

            set MsgBody "$LanDevId$IntfName"
        }\
        $MSG_AUTH {
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"        
        }\
        $MSG_WAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_WAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_LAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_LAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_WLAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_WLAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_SNTP_TIME_UPDATE {
            set OldSecondVal [lindex $MsgBodyList 0]
            if {$OldSecondVal != ""} {
                set OldSecond [binary format "$int" $OldSecondVal]
            } else {
                set OldSecond ""
            }
            
            set NewSecondVal [lindex $MsgBodyList 1]
            if {$NewSecondVal != ""} {
                set NewSecond [binary format "$int" $NewSecondVal]
            } else {
                set NewSecond ""
            }
            
            set MsgBody "$OldSecond$NewSecond"
        }\
        $MSG_SNTP_TIME_CHANGED {
            set OldSecondVal [lindex $MsgBodyList 0]
            if {$OldSecondVal != ""} {
                set OldSecond [binary format "$int" $OldSecondVal]
            } else {
                set OldSecond ""
            }
            
            set NewSecondVal [lindex $MsgBodyList 1]
            if {$NewSecondVal != ""} {
                set NewSecond [binary format "$int" $NewSecondVal]
            } else {
                set NewSecond ""
            }
            
            set MsgBody "$OldSecond$NewSecond"            
        }\
        $MSG_NTPS_CHANGED {
            set TypeVal [lindex $MsgBodyList 0]
            if {$TypeVal != ""} {
                set Type [binary format "c" $TypeVal]
            } else {
                set Type ""
            }
            
            set EnabledVal [lindex $MsgBodyList 1]
            if {$EnabledVal != ""} {
                set Enabled [binary format "c" $EnabledVal]
            } else {
                set Enabled ""
            }
            
            set NTPServers [join [lrange $MsgBodyList 2 end] {}]
            
            set MsgBody "$Type$Enabled$NTPServers"
        }\
        $MSG_MSG4UDP_REGISTER {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"
        }\
        $MSG_MSG4UDP_UNREGISTER {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"            
        }\
        $MSG_MSG4UDP_MESSAGE {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"              
        }\
        $MSG_WAN_CONN_ENABLE_UPDATE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item" 
        }\
        $MSG_VLAN_ADD_BRIDGE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_DEL_BRIDGE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_BIND_PORT {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_UNBIND_PORT {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_CMM_INST_ADDED {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath    
        }\
        $MSG_CMM_INST_DELED {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath             
        }\
        default { 
            puts "Message Type error!"
            return -1 
        }
        
    set BodyLength [binary format "$int" [string length $MsgBody]]
    return "$SrcMID$DstMID$MsgID$MsgType$Reserve$BodyLength$MsgBody"
}

########### CreateMsg_n命令用于创建消息 ##############
###_n是CreateMsg的新版本
proc CreateMsg_n {MsgList} {
    global ScriptPath int short
     
    ###载入模块ID和消息类型定义脚本
    source "$ScriptPath/lib/msg-head.tcl"
    
    
         
    set MsgHeadList [lrange $MsgList 0 3]
    set MsgBodyList [lrange $MsgList 4 end]
    
    set SrcMID  [binary format "$short" [lindex $MsgHeadList 0]]
    set DstMID  [binary format "$short" [lindex $MsgHeadList 1]]
    
    set MsgIDVal [format %u [lindex $MsgHeadList 2]] 
    set MsgID    [binary format "$int" $MsgIDVal]

    set TrueMsgTypeVal [lindex [lindex $MsgHeadList 3] 0]
    
    set FalseMsgTypeVal [lindex [lindex $MsgHeadList 3] 1]
    if {$FalseMsgTypeVal != ""} {
        set MsgType [binary format "$short" $FalseMsgTypeVal]
    } else {
        set MsgType [binary format "$short" $TrueMsgTypeVal]
    }
    
    set Reserve [binary format "$short" 0]

    #根据消息类型和消息体列表设置消息体
    switch $TrueMsgTypeVal\
        $MSG_CMM_SAVE_CFG {
            set MsgBody ""
        }\
        $MSG_CMM_RECOVER_CFG {
            set MsgBody ""
        }\
        $MSG_CMM_SET_VAL {
            set SetItemCountVal [lindex $MsgBodyList 0]
            if {$SetItemCountVal != ""} {
                set SetItemCount [binary format "$int" $SetItemCountVal]
            } else {
                set SetItemCount ""
            }
            
            set SetItem [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$SetItemCount$SetItem"
        }\
        $MSG_CMM_COMMIT {
            set MsgBody ""
        }\
        $MSG_CMM_CANCEL {
            set MsgBody ""
        }\
        $MSG_CMM_GET_NAME {
            set NextLevelVal [lindex $MsgBodyList 0]
            if {$NextLevelVal != ""} {
                set NextLevel [binary format "c" $NextLevelVal]
            } else {
                set NextLevel ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$NextLevel$Path"
        }\
        $MSG_CMM_GET_VAL {                
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"
        }\
        $MSG_CMM_ADD_NODE {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath
        }\
        $MSG_CMM_DEL_NODE {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath
        }\
        $MSG_CMM_SET_ATTR {
            set SetItemCountVal [lindex $MsgBodyList 0]
            if {$SetItemCountVal != ""} {
                set SetItemCount [binary format "$int" $SetItemCountVal]
            } else {
                set SetItemCount ""
            }
            
            set SetItem [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$SetItemCount$SetItem"            
        }\
        $MSG_CMM_GET_ATTR {
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"                 
        }\
        $MSG_CMM_GET_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_CLEAR_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_INFORM_NOTI {
            set MsgBody ""
        }\
        $MSG_CMM_REBOOT {
            set MsgBody ""
        }\
        $MSG_CMM_UPDATE {
            set MsgBody ""
        }\
        $MSG_PC_START_PROC {
            set NeedSteadyVal [lindex $MsgBodyList 0]
            if {$NeedSteadyVal != ""} {
                set NeedSteady [binary format "c" $NeedSteadyVal]
            } else {
                set NeedSteady ""
            }
            
            set ArgCountVal [lindex $MsgBodyList 1]
            if {$ArgCountVal != ""} {
                set ArgCount [binary format "c" $ArgCountVal]
            } else {
                set ArgCount ""
            }
            
            set RemainBodyList [lrange $MsgBodyList 2 end]
            
            set MonCountIndex [lsearch -regexp $RemainBodyList {^[-0-9]+$}]
            if {$MonCountIndex == -1} {
                set RemainBody [join $RemainBodyList {}]
                set MsgBody "$NeedSteady$ArgCount$RemainBody"
            } elseif {$MonCountIndex == 0} {
                set MonCount [binary format "c" [lindex $RemainBodyList 0]]
                set MonVal [join [lrange $RemainBodyList 1 end] {}]
                set MsgBody "$NeedSteady$ArgCount$MonCount$MonVal"
            } else {
                set ArgVal [join [lrange $RemainBodyList 0 [expr {$MonCountIndex -1}]] {}]
                set MonCount [binary format "c" [lindex $RemainBodyList $MonCountIndex]]
                set MonVal [join [lrange $RemainBodyList [incr MonCountIndex] end] {}]
                set MsgBody "$NeedSteady$ArgCount$ArgVal$MonCount$MonVal"    
            }
        }\
        $MSG_PC_SEND_SIGNAL {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }
                      
            set SignalVal [lindex $MsgBodyList 2]
            if {$SignalVal != ""} {
                set Signal [binary format "c" $SignalVal]
            } else {
                set Signal ""
            }
            
            set NeedRestartVal [lindex $MsgBodyList 3]
            if {$NeedRestartVal != ""} {
                set NeedRestart [binary format "c" $NeedRestartVal]
            } else {
                set NeedRestart ""
            }
                
            set NeedSteadyVal [lindex $MsgBodyList 4]
            if {$NeedSteadyVal != ""} {
                set NeedSteady [binary format "c" $NeedSteadyVal]
            } else {
                set NeedSteady ""
            }
            
            set ArgCountVal [lindex $MsgBodyList 5]
            if {$ArgCountVal != ""} {
                set ArgCount [binary format "c" $ArgCountVal]
            } else {
                set ArgCount ""
            }
            
            set RemainBodyList [lrange $MsgBodyList 6 end]
            
            set MonCountIndex [lsearch -regexp $RemainBodyList {^[-0-9]+$}]
            if {$MonCountIndex == -1} {
                set RemainBody [join $RemainBodyList {}]
                set MsgBody "$Name$InstNum$Signal$NeedRestart$NeedSteady$ArgCount$RemainBody"
            } elseif {$MonCountIndex == 0} {
                set MonCount [binary format "c" [lindex $RemainBodyList 0]]
                set MonVal [join [lrange $RemainBodyList 1 end] {}]
                set MsgBody "$Name$InstNum$Signal$NeedRestart$NeedSteady$ArgCount$MonCount$MonVal"
            } else {
                set ArgVal [join [lrange $RemainBodyList 0 [expr {$MonCountIndex -1}]] {}]
                set MonCount [binary format "c" [lindex $RemainBodyList $MonCountIndex]]
                set MonVal [join [lrange $RemainBodyList [incr MonCountIndex] end] {}]
                set MsgBody "$Name$InstNum$Signal$NeedRestart$NeedSteady$ArgCount$ArgVal$MonCount$MonVal"    
            }
        }\
        $MSG_PC_PROC_STATE {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }                                          
                  
            set ProcStateVal [lindex $MsgBodyList 2]
            if {$ProcStateVal != ""} {
                set ProcState [binary format "c" $ProcStateVal]
            } else {
                set ProcState ""
            }
            
            set ExitSignalVal [lindex $MsgBodyList 3]
            if {$ExitSignalVal != ""} {
                set ExitSignal [binary format "c" $ExitSignalVal]
            } else {
                set ExitSignal ""
            }            
            
            set LastSignalVal [lindex $MsgBodyList 4]
            if {$LastSignalVal != ""} {
                set LastSignal [binary format "c" $LastSignalVal]
            } else {
                set LastSignal ""
            }
            
            set LastItem [lindex $MsgBodyList 5]
            if {$LastItemVal != ""} {
                set LastItem [binary format "c" $LastItemVal]
            } else {
                set LastItem ""
            }           
            
            set MsgBody "$Name$InstNum$ProcState$ExitSignal$LastSignal$LastItem"
        }\
        $MSG_PC_PROC_OUTPUT {
            set Name [lindex $MsgBodyList 0]
            set NameLen [string length $Name]
            if {$NameLen > 16} {
                puts "Name string length is too long"
                return -1
            } else {
                append Name [string repeat "\0" [expr {16 - $NameLen}]]
            }
            
            set InstNumVal [lindex $MsgBodyList 1]
            if {$InstNumVal != ""} {
                set InstNum [binary format "$int" $InstNumVal]
            } else {
                set InstNum ""
            }    
            
            set MatchCountVal [lindex $MsgBodyList 2]
            if {$MatchCountVal != ""} {
                set MatchCount [binary format "c" $MatchCountVal]
            } else {
                set MatchCount ""
            }
            
            set MatchItem     ""
            foreach {MatMonNoVal MatVal} [lrange $MsgBodyList 3 end] {
                if {$MatMonNoVal != ""} {
                    set MatMonNo [binary format "c" $MatMonNoVal]
                } else {
                    set MatMonNo ""
                }
                
                append MatchItem $MatMonNo $MatVal
            }
                        
            set MsgBody "$Name$InstNum$MatchCount$MatchItem"
        }\
        $MSG_TR069_SET_PARKEY {
            set ParameterKey [join [lrange $MsgBodyList 0 end] {}] 
            set MsgBody "$ParameterKey"           
        }\
        $MSG_TR069_GET_EVENTS {
            set MsgBody "" 
        }\
        $MSG_TR069_CLEAR_EVENTS {
            set MsgBody ""
        }\
        $MSG_TR069_DOWNLOAD {
            set FileSizeVal [lindex $MsgBodyList 0]
            if {$FileSizeVal != ""} {
                set FileSize [binary format "$int" $FileSizeVal]
            } else {
                set FileSize ""
            }
            
            set DelaySecondsVal [lindex $MsgBodyList 1]
            if {$DelaySecondsVal != ""} {
                set DelaySeconds [binary format "$int" $DelaySecondsVal]
            } else {
                set DelaySeconds ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 2 end] {}]
            
            set MsgBody "$FileSize$DelaySeconds$ItemStr"
                        
        }\
        $MSG_TR069_UPLOADLOAD {
            set DelaySecondsVal [lindex $MsgBodyList 0]
            if {$DelaySecondsVal != ""} {
                set DelaySeconds [binary format "$int" $DelaySecondsVal]
            } else {
                set DelaySeconds ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$DelaySeconds$ItemStr"
        }\
        $MSG_TR069_GET_TRANSFERCOMPLETEINFO {
            set MsgBody ""
        }\
        $MSG_TR069_HTTPD_CONNECT {
            set MsgBody ""
        }\
        $MSG_TIMER_REGSTER {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId  [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set TypeVal [lindex $MsgBodyList 1]
            if {$TypeVal != ""} {
                set Type [binary format "c" $TypeVal]
            } else {
                set Type ""
            }
                
            set SpaceVal [lindex $MsgBodyList 2]
            if {$SpaceVal != ""} {
                set Space [binary format "c" $SpaceVal]
            } else {
                set Space ""
            }
            
            set YearVal [lindex $MsgBodyList 3]
            if {$YearVal != ""} {
                set Year  [binary format "c" $YearVal]
            } else {
                set Year ""
            }
            
            set MonthVal [lindex $MsgBodyList 4]
            if {$MonthVal != ""} {
                set Month [binary format "c" $MonthVal]
            } else {
                set Month ""
            }
            
            set DateVal [lindex $MsgBodyList 5]
            if {$DateVal != ""} {
                set Date [binary format "c" $DateVal]
            } else {
                set Date ""
            }
            
            set HourVal [lindex $MsgBodyList 6]
            if {$HourVal != ""} {
                set Hour [binary format "c" $HourVal]
            } else {
                set Hour ""
            }
            
            set MinuteVal [lindex $MsgBodyList 7]
            if {$MinuteVal != ""} {
                set Minute [binary format "c" $MinuteVal]
            } else {
                set Minute ""
            }
            
            set SecondVal [lindex $MsgBodyList 8]
            if {$SecondVal != ""} {
                set Second [binary format "c" $SecondVal]
            } else {
                set Second ""
            }
            
            
            set IntervalVal [lindex $MsgBodyList 9]
            if {$IntervalVal != ""} {
                set Interval [binary format "$int" $IntervalVal]
            } else {
                set Interval ""
            }
            
            set Name [join [lrange $MsgBodyList 10 end] {}]
            
            set MsgBody "$TimerId$Type$Space$Year$Month$Date$Hour$Minute$Second$Interval$Name"
        }\
        $MSG_TIMER_TIMEOUT {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set MsgBody "$TimerId"
        }\
        $MSG_TIMER_UNREGSTER {
            set TimerIdVal [lindex $MsgBodyList 0]
            if {$TimerIdVal != ""} {
                set TimerId [binary format "$int" $TimerIdVal]
            } else {
                set TimerId ""
            }
            
            set MsgBody "$TimerId"
        }\
        $MSG_MON_INTF_STATUS_INFORM {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"            
        }\
        $MSG_MON_INTF_STATUS_QUERY {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"             
        }\
        $MSG_MON_INTF_REGISTER {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"         
        }\
        $MSG_MON_INTF_UNREGISTER {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
            
            set MsgBody "$ItemCount$Item"
        }\
        $MSG_WAN_CONN_EST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
        
            set MsgBody "$ItemCount$Item"       
        }\
        $MSG_WAN_CONN_FIN {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]
        
            set MsgBody "$ItemCount$Item"         
        }\
        $MSG_WAN_LINK_CHG {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"
        }\
        $MSG_LAN_IP_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set IpAddr   [lindex $MsgBodyList 1]
            set Mask     [lindex $MsgBodyList 2]
            
            set MsgBody "$LanDevNo$IpAddr$Mask"
        }\
        $MSG_LAN_MACLIST_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set MacList [lindex $MsgBodyList 1]
            
            set MsgBody "$LanDevNo$MacList"
        }\
        $MSG_LAN_DOMAIN_NAME_UPDATE {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set DomainName  [lindex $MsgBodyList 1]
            
            set MsgBody "$LanDevNo$DomainName"
        }\
        $MSG_GET_DHOST_LIST {
            set LanDevNoVal [lindex $MsgBodyList 0]
            if {$LanDevNoVal != ""} {
                set LanDevNo [binary format "$int" $LanDevNoVal]
            } else {
                set LanDevNo ""
            }
            
            set MsgBody "$LanDevNo"
        }\
        $MSG_DNS_CHANGE {
            set DnsRelayEnVal [lindex $MsgBodyList 0]
            if {$DnsRelayEnVal != ""} {
                set DnsRelayEn [binary format "c" $DnsRelayEnVal]
            } else {
                set DnsRelayEn ""
            }
            
            set DNS            [lindex $MsgBodyList 1]
            
            set MsgBody "$DnsRelayEn$DNS"          
        }\
        $MSG_WLAN_ADD_DEVICE {
            set LanDevIdVal [lindex $MsgBodyList 0]
            if {$LanDevIdVal != ""} {
                set LanDevId [binary format "$int" $LanDevIdVal]
            } else {
                set LanDevId ""
            }
                        
            set IntfName    [lindex $MsgBodyList 1]

            set MsgBody "$LanDevId$IntfName"
        }\
        $MSG_WLAN_DEL_DEVICE {
            set LanDevIdVal [lindex $MsgBodyList 0]
            if {$LanDevIdVal != ""} {
                set LanDevId [binary format "$int" $LanDevIdVal]
            } else {
                set LanDevId ""
            }
                        
            set IntfName    [lindex $MsgBodyList 1]

            set MsgBody "$LanDevId$IntfName"
        }\
        $MSG_AUTH {
            set PathCountVal [lindex $MsgBodyList 0]
            if {$PathCountVal != ""} {
                set PathCount [binary format "$int" $PathCountVal]
            } else {
                set PathCount ""
            }
            
            set Path [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$PathCount$Path"        
        }\
        $MSG_WAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"        
        }\
        $MSG_WAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item" 
        }\
        $MSG_LAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"        
        }\
        $MSG_LAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item" 
        }\
        $MSG_WLAN_DEL_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item"        
        }\
        $MSG_WLAN_ADD_INST {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item" 
        }\
        $MSG_CMM_CLEAR_CFG {
            set MsgBody ""
        }\
        $MSG_SNTP_TIME_UPDATE {
            set OldSecondVal [lindex $MsgBodyList 0]
            if {$OldSecondVal != ""} {
                set OldSecond [binary format "$int" $OldSecondVal]
            } else {
                set OldSecond ""
            }
            
            set NewSecondVal [lindex $MsgBodyList 1]
            if {$NewSecondVal != ""} {
                set NewSecond [binary format "$int" $NewSecondVal]
            } else {
                set NewSecond ""
            }
            
            set MsgBody "$OldSecond$NewSecond"
        }\
        $MSG_SNTP_TIME_CHANGED {
            set OldSecondVal [lindex $MsgBodyList 0]
            if {$OldSecondVal != ""} {
                set OldSecond [binary format "$int" $OldSecondVal]
            } else {
                set OldSecond ""
            }
            
            set NewSecondVal [lindex $MsgBodyList 1]
            if {$NewSecondVal != ""} {
                set NewSecond [binary format "$int" $NewSecondVal]
            } else {
                set NewSecond ""
            }
            
            set MsgBody "$OldSecond$NewSecond"            
        }\
        $MSG_NTPS_CHANGED {
            set TypeVal [lindex $MsgBodyList 0]
            if {$TypeVal != ""} {
                set Type [binary format "c" $TypeVal]
            } else {
                set Type ""
            }
            
            set EnabledVal [lindex $MsgBodyList 1]
            if {$EnabledVal != ""} {
                set Enabled [binary format "c" $EnabledVal]
            } else {
                set Enabled ""
            }
            
            set NTPServers [join [lrange $MsgBodyList 2 end] {}]
            
            set MsgBody "$Type$Enabled$NTPServers"
        }\
        $MSG_MSG4UDP_REGISTER {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"
        }\
        $MSG_MSG4UDP_UNREGISTER {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"            
        }\
        $MSG_MSG4UDP_MESSAGE {
            set Item [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody "$Item"              
        }\
        $MSG_WAN_CONN_ENABLE_UPDATE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set Item [join [lrange $MsgBodyList 1 end] {}]        
            
            set MsgBody "$ItemCount$Item" 
        }\
        $MSG_VLAN_ADD_BRIDGE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_DEL_BRIDGE {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_BIND_PORT {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_VLAN_UNBIND_PORT {
            set ItemCountVal [lindex $MsgBodyList 0]
            if {$ItemCountVal != ""} {
                set ItemCount [binary format "$int" $ItemCountVal]
            } else {
                set ItemCount ""
            }
            
            set ItemStr [join [lrange $MsgBodyList 1 end] {}]
            set MsgBody "$ItemCount$ItemStr"
        }\
        $MSG_CMM_INST_ADDED {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath    
        }\
        $MSG_CMM_INST_DELED {
            set ObjPath [join [lrange $MsgBodyList 0 end] {}]
            set MsgBody $ObjPath                
        }\
        default { 
            puts "Unknown msg type"
            return -1 
        }        
                
        set BodyLength [binary format "$int" [string length $MsgBody]]
        return "$SrcMID$DstMID$MsgID$MsgType$Reserve$BodyLength$MsgBody"
}

######## ParseAckMsg命令解析收到的消息 ########
####参数AckMsg表示接收到的消息，AckMatchList表示用来匹配响应消息的消息元素组成的列表
proc ParseAckMsg {AckMsg AckMatchList} {
    global ScriptPath int short
     
    ###载入模块ID和消息类型定义脚本
    source "$ScriptPath/lib/msg-head.tcl"
    
    if {[string length $AckMsg] < 16} {
        puts "ack msg is too short."
        return -1   
    }
    
    set AckMsgHead [string range $AckMsg 0 15]
    set AckMsgBody [string range $AckMsg 16 end]
    
    set HeadMatchList [lindex $AckMatchList 0]
    set BodyMatchList [lindex $AckMatchList 1] 
    
    #匹配源模块ID
    binary scan [string range $AckMsgHead 0 1] "$short" SrcMID
    if {$SrcMID < 0} {
        set SrcMID [expr {$SrcMID + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 0] $SrcMID]
    if {$MatchResult == 0} {
        #puts "ack msg head match fail:'SrcMID' field"
        return -2
    }
        
    #匹配目的模块ID
    binary scan [string range $AckMsgHead 2 3] "$short" DstMID
    if {$DstMID < 0} {
        set DstMID [expr {$DstMID + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 1] $DstMID]
    if {$MatchResult == 0} {
        #puts "ack msg head match fail:'DstMID' field"
        return -2
    }
    
    #匹配消息ID
    binary scan [string range $AckMsgHead 4 7] "$int" MsgID
    set MsgID [format "%#010x" $MsgID]
    set MatchResult [regexp [lindex $HeadMatchList 2] $MsgID]
    if {$MatchResult == 0} {
        puts "ack msg head match fail:'MsgID' field"
        return -1
    }
    
    #匹配消息类型
    binary scan [string range $AckMsgHead 8 9] "$short" MsgType
    if {$MsgType < 0} {
        set MsgType [expr {$MsgType + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 3] $MsgType]
    if {$MatchResult == 0} {
        puts "ack msg head match fail:'MsgType' field"
        return -1
    }
    
    #匹配保留字段
    binary scan [string range $AckMsgHead 10 11] "$short" Reserve
    if {$Reserve != 0} {
        puts "ack msg head match fail:'Reserve' field"
        return -1
    }
    
    #匹配消息体长度
    binary scan [string range $AckMsgHead 12 15] "$int" BodyLen
    if {$BodyLen != [string length $AckMsgBody]} {
        puts "ack msg head mathc fail:'BodyLength' field"
        return -1
    }
    
    #puts "AckMsgHead: SrcMID=$SrcMID, DstMID=$DstMID, MsgID=$MsgID, MsgType=$MsgType, Reserve=$Reserve, BodyLen=$BodyLen"
   
    #puts "ack msg head is Ok"  ;#消息头已经匹配
    
    if {[format %#x $MsgType] >= 0x8007 && [format %#x $MsgType] <= 0x8018} {
        #应答消息的类型属于CMM模块
        
        #匹配消息体Result字段
        binary scan [string range $AckMsgBody 0 3] "$int" Result
        set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
        if {$MatchResult == 0} {
            puts "ack msg body match fail:'Result' field"
            return -1
        }
        
        if {$Result == 0} {
            #正常应答消息的处理
            switch $MsgType\
                $MSG_CMM_SAVE_CFG_ACK {
                    if {$BodyLen == 4} {             
                        return 0 ;#匹配成功
                    } else {
                        puts "msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_RECOVER_CFG_ACK {
                    if {$BodyLen == 4} {                    
                        return 0 ;#匹配成功
                    } else {
                        puts "msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_SET_VAL_ACK {
                    if {$BodyLen == 8} {
                        #匹配Status字段
                        binary scan [string range $AckMsgBody 4 7] "$int" Status
                        set MatchResult [regexp [lindex $BodyMatchList 1] $Status]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'Status' field"
                            return -1
                        }
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_GET_NAME_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }    
                }\
                $MSG_CMM_GET_VAL_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }
                }\
                $MSG_CMM_ADD_NODE_ACK {
                    if {$BodyLen == 12} {
                        #匹配ObjNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ObjNo
                        if {$ObjNo < 0} {
                            set ObjNo [format %u $ObjNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ObjNo]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjNo' field"
                            return -1
                        }
                        
                        #匹配ObjStatus字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ObjStatus
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ObjStatus]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjStatus' field"
                            return -1
                        }
                        
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_DEL_NODE_ACK {
                    if {$BodyLen == 8} {
                        #匹配ObjStatus字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ObjStatus
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ObjStatus]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjStatus' field"
                            return -1
                        }
                        
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_SET_ATTR_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail: body length error"
                        return -1                       
                    }
                }\
                $MSG_CMM_GET_ATTR_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        #将错误信息加入日志
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }                    
                }\
                $MSG_CMM_GET_NOTI_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }                                    
                }\
                $MSG_CMM_CLEAR_NOTI_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail: body length error"
                        return -1                       
                    }               
                }\
                $MSG_CMM_UPDATE_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1                       
                    }
                }\
                default {
                    puts "Ack Msg Type error!"
                    return -1
                }
              
        } else {
            #错误应答消息的处理
            if {[format %#x $MsgType] == 0x8013} {
                if {$BodyLen == 4} {
                    return 0
                } else {
                    puts "ack msg body match fail:body length error"
                    return -1
                }
            }
            
            if {$BodyLen <12} {
                puts "ack msg body match fail:body length error"
                return -1
            } else {
                #匹配ErrNo字段
                binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                if {$ErrNo < 0} {
                    set ErrNo [format %u $ErrNo]
                }
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                if {$MatchResult ==0} {
                    puts "ack msg body match fail:'ErrNo' field"
                    return -1
                }
                        
                #匹配ErrItemCount字段
                binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                if {$MatchResult ==0} {
                    puts "ack msg body match fail:'ErrItemCount' field"
                    return -1
                }
                        
                #匹配ErrItem字段
                set ErrItemStr [string range $AckMsgBody 12 end]
                        
                for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j} {
                    set NulCharIndex [string first "\0" $ErrItemStr]
                            
                    set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                    set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ErrItem($i)' field"
                        return -1
                    }
                        
                    set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                }
                
                return 0
            }
        }

    } else {
        #应答消息的类型属于其它模块
        switch $MsgType\
            $MSG_PC_START_PROC_ACK {
                if {$BodyLen ==8} {
                    #匹配StartOK字段
                    binary scan [string range $AckMsgBody 0 3] "$int" StartOk
                    set MatchResult [regexp [lindex $BodyMatchList 0] $StartOk]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'StartOk' field"
                        return -1
                    }
                    
                    #匹配ProcId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" ProcId                    
                    if {$ProcId < 0} {
                        set ProcId [format %u $ProcId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $ProcId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'ProcId field'"
                        return -1
                    }
                    
                    return 0 ;#消息匹配成功                   
                } else {
                    puts "ack msg body match fail,body length error"
                    return -1
                }     
            }\
            $MSG_PC_SEND_SIGNAL_ACK {
                if {$BodyLen == 4} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'Result' field"
                        return -1
                    }                    
                    
                    return 0 ;#消息匹配成功
                } else {
                    puts "ack msg body match fail,body length error"
                    return -1
                }
            }\
            $MSG_TR069_GET_EVENTS_ACK {
                if {$BodyLen < 2} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配EventNum字段
                    binary scan [string range $AckMsgBody 0 1] "$short" EventNum
                    set MatchResult [regexp [lindex $BodyMatchList 0] $EventNum]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }                         
                    
                    #匹配EventContent字段
                    set EventContent [string range $AckMsgBody 2 end]
                    for {set i 1} {$i <= $EventNum} {incr i} {
                        set NulCharIndex [string first "\0" $EventContent]
                        set Event($i) [string range $EventContent 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $i] $Event($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'EventContent' field"
                            return -1
                        }
                        
                        set EventContent [string range $EventContent [incr NulCharIndex] end]                  
                    }
                    
                    return 0
                }                                
            }\
            $MSG_TR069_DOWNLOAD_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配Status字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Status
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Status]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Status' field"
                        return -1
                    }
                    
                    #匹配后面的字符串字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    for {set i 1} {$i <= 4} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                        switch $i \
                            1 {
                                set StartTime   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 1] $StartTime]
                                set field "StartTime"
                            }\
                            2 {
                                set CompleteTime [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult  [regexp [lindex $BodyMatchList 2] $CompleteTime]
                                set field "CompleteTime"
                            }\
                            3 {
                                set FaultCode   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 3] $FaultCode]
                                set field "FaultCode"                                
                            }\
                            4 {
                                set ErrorMsg    [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 4] $ErrorMsg]
                                set field "ErrorMsg"
                            }
                            
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'$field' field"
                            return -1
                        }
                            
                        set ItemStr  [string range $ItemStr [incr NulCharIndex] end]                                                                                      
                    }
                    
                    return 0
                }
            }\
            $MSG_TR069_UPLOADLOAD_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配Status字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Status
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Status]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Status' field"
                        return -1
                    }
                    
                    #匹配后面的字符串字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    for {set i 1} {$i <= 2} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                        switch $i \
                            1 {
                                set FaultCode   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 1] $FaultCode]
                                set field "FaultCode"                                
                            }\
                            2 {
                                set ErrorMsg    [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 2] $ErrorMsg]
                                set field "ErrorMsg"
                            }
                            
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'$field' field"
                            return -1
                        }
                            
                        set ItemStr  [string range $ItemStr [incr NulCharIndex] end]                                                                                      
                    }
                    
                    return 0
                }                                                    
            }\
            $MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK {
                set ItemStr [string range $AckMsgBody 0 end]
                for {set i 0} {$i < 4} {incr i} {
                    set NulCharIndex [string first "\0" $ItemStr]
                    switch $i \
                        0 {
                            set CommandKey  [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 0] $CommandKey]
                            set field "CommandKey"
                        }\
                        1 {
                            set FaultStruct [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 1] $FaultStruct]
                            set field "FaultStruct"                        
                        }\
                        2 {
                            set DownloadStartTime [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 2] $DownloadStartTime]
                            set field "DownloadStartTime"
                        }\
                        3 {
                            set DownloadCompleteTime [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 3] $DownloadCompleteTime]
                            set field "DownloadCompleteTime"
                        }
                    
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'$field' field"
                        return -1
                    }
                    
                    set ItemStr  [string range $ItemStr [incr NulCharIndex] end]

                }
                
                return 0
            }\
            $MSG_TIMER_REGSTER_ACK {
                if {$BodyLen ==8} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    } 
                    
                    #匹配TimerId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" TimerId
                    if {$TimerId < 0} {
                        set TimerId [format %u $TimerId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $TimerId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'TimerID' field"
                        return -1
                    }                     
                    
                    return 0
                } else {
                    puts "ack msg body match fail: body length error"
                    return -1
                }
            }\
            $MSG_TIMER_UNREGSTER_ACK {
                if {$BodyLen ==8} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    } 
                    
                    #匹配TimerId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" TimerId
                    if {$TimerId < 0} {
                        set TimerId [format %u $TimerId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $TimerId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'TimerID' field"
                        return -1
                    }                     
                    
                    return 0
                } else {
                    puts "ack msg body match fail: body length error"
                    return -1
                }    
            }\
            $MSG_MON_INTF_REGISTER_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail: body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult ==0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                    
                    if {$Result == 0} {
                        return 0
                    } else {
                        #匹配ErrNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                        if {$ErrNo < 0} {
                            set ErrNo [format %u $ErrNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrNo' field"
                            return -1
                        }
                        
                        #匹配ErrItemCount字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrItemCount' field"
                            return -1
                        }
                        
                        #匹配ErrItem字段
                        set ErrItemStr [string range $AckMsgBody 12 end]
                        
                        for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ErrItemStr]
                            
                            set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ErrItem($i)' field"
                                return -1
                            }
                        
                            set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                        }
                        
                        return 0
                    }          
                }            
            }\
            $MSG_MON_INTF_UNREGISTER_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail: body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult ==0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                    
                    if {$Result == 0} {
                        return 0
                    } else {
                        #匹配ErrNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                        if {$ErrNo < 0} {
                            set ErrNo [format %u $ErrNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrNo' field"
                            return -1
                        }
                        
                        #匹配ErrItemCount字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrItemCount' field"
                            return -1
                        }
                        
                        #匹配ErrItem字段
                        set ErrItemStr [string range $AckMsgBody 12 end]
                        
                        for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j } {
                            set NulCharIndex [string first "\0" $ErrItemStr]
                            
                            set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ErrItem($i)' field"
                                return -1
                            }
                        
                            set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                        }
                        return 0
                    }    
                        
                }     
            }\
            $MSG_GET_DHOST_LIST_ACK {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else { 
                    #匹配LanDevNo字段
                    binary scan [string range $AckMsgBody 0 3] "$int" LanDevNo
                    if {$LanDevNo < 0} {
                        set LanDevNo [format %u $LanDevNo]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $LanDevNo]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'LanDevNo' field"
                        return -1
                    }
                    
                    #匹配DHostCount字段
                    binary scan [string range $AckMsgBody 4 7]  "$int" DHostCount
                    set MatchResult [regexp [lindex $BodyMatchList 1] $DHostCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'DHostCount' field"
                        return -1
                    }
                    
                    #匹配DHostList字段
                    set DHostList [string range $AckMsgBody 8 end]
                    
                    for {set i 1;set j 2} {$i <= $DHostCount} {incr i;incr j} {
                        #匹配DHostRemainTime字段
                        binary scan [string range $DHostList 0 3] "$int" DHostRemainTime($i)
                        if {$DHostRemainTime($i) < 0} {
                            set DHostRemainTime($i) [format %u $DHostRemainTime($i)]
                        }
                        set MatchResult [regexp [lindex [lindex $BodyMatchList $j] 0] $DHostRemainTime($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'DHostRemainTime($i)' field"
                            return -1
                        }                       
                        
                        set DHostList [string range $DHostList 4 end]
                        set NulCharIndex [string first "\0" $DHostList]
                        #匹配DHostIpAddr字段
                        set DHostIpAddr($i) [string range $DHostList 0 $NulCharIndex]
                        set MatchResult [regexp [lindex [lindex $BodyMatchList $j] 1] $DHostIpAddr($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'DHostIpAddr($i)' field"
                            return -1
                        }
                        
                        set DHostList [string range [incr NulCharIndex] end]
                    }
                    
                    return 0
                }
            }\
            $MSG_MON_INTF_STATUS_INFORM {
                if {$BodyLen <4} {
                    puts "ack msg body match fail:body length error"
                } else {
                    #匹配ItemCount字段
                    binary scan [string range $AckMsgBody 0 3] "$int" ItemCount
                    set MatchResult [regexp [lindex $BodyMatchList 0] $ItemCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ItemCount' field"
                        return -1
                    }
                    
                    #匹配Item字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    
                    for {set i 1} {$i <= $ItemCount} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                            
                        set Item($i) [string range $ItemStr 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $i] $Item($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'Item($i)' field"
                            return -1
                        }
                        
                        set ItemStr [string range $ItemStr [incr NulCharIndex] end]               
                    }
                    
                    return 0
                }
            
            }\
            $MSG_AUTH_ACK {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                
                    #匹配ResItemCount字段
                    binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                    set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ResItemCount' field"
                        return -1
                    }                    
                    
                    #匹配ResItem字段
                    set ResItemStr [string range $AckMsgBody 8 end]
                    
                    for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                        set NulCharIndex [string first "\0" $ResItemStr]
                        set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItem($i)' field"
                            return -1
                        }
                        
                        set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                                                            
                    }
                    
                    return 0
                }
            }\
            $MSG_SNTP_TIME_UPDATE {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配OldSecond字段
                    binary scan [string range $AckMsgBody 0 3] "$int" OldSecond
                    if {$OldSecond < 0} {
                        set OldSecond [format %u $OldSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $OldSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'OldSecond' field"
                        return -1
                    }
                    
                    #匹配NewSecond字段
                    binary scan [string range $AckMsgBody 4 7] "$int" NewSecond
                    if {$NewSecond < 0} {
                        set NewSecond [format %u $NewSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 1] $NewSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'NewSecond' field"
                        return -1
                    }                    
                
                    return 0
                }
            }\
            $MSG_SNTP_TIME_CHANGED {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配OldSecond字段
                    binary scan [string range $AckMsgBody 0 3] "$int" OldSecond
                    if {$OldSecond < 0} {
                        set OldSecond [format %u $OldSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $OldSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'OldSecond' field"
                        return -1
                    }
                    
                    #匹配NewSecond字段
                    binary scan [string range $AckMsgBody 4 7] "$int" NewSecond
                    if {$NewSecond < 0} {
                        set NewSecond [format %u $NewSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 1] $NewSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'NewSecond' field"
                        return -1
                    }                    
                
                    return 0
                }            
            }\
            $MSG_MSG4UDP_REGISTER_ACK {
                return 0
            }\
            $MSG_MSG4UDP_UNREGISTER_ACK {
                return 0
            }\
            $MSG_MSG4UDP_MESSAGE {
                set ItemStr [string range $AckMsgBody 0 end]
                set MatchResult [regexp [lindex $BodyMatchList 0] $ItemStr]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'msg body string' field"
                    return -1
                }
                
                return 0
            }\
            $MSG_VLAN_ADD_BRIDGE_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配InfoString字段
                set InfoString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $InfoString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'InfoString' field"
                    return -1
                } 
                
                return 0
            }\
            $MSG_VLAN_DEL_BRIDGE_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0            
            }\
            $MSG_VLAN_BIND_PORT_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0            
            }\
            $MSG_VLAN_UNBIND_PORT_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0           
            }\
            default {
                puts "Ack MsgType error!"
                return -1               
            }
    }
}

##############################################################
proc ParseAckMsg_n {AckMsg AckMatchList} {
    global ScriptPath int short
     
    ###载入模块ID和消息类型定义脚本
    source "$ScriptPath/lib/msg-head.tcl"
              
    if {[string length $AckMsg] < 16} {
        puts "ack msg is too short."
        return -1   
    }

    set AckMsgHead [string range $AckMsg 0 15]
    set AckMsgBody [string range $AckMsg 16 end]
    
    set HeadMatchList [lrange $AckMatchList 0 3]
    set BodyMatchList [lrange $AckMatchList 4 end]

    #匹配源模块ID
    binary scan [string range $AckMsgHead 0 1] "$short" SrcMID
    if {$SrcMID < 0} {
        set SrcMID [expr {$SrcMID + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 0] $SrcMID]
    if {$MatchResult == 0} {
        #puts "ack msg head match fail:'SrcMID' field"
        return -2
    }

    #匹配目的模块ID
    binary scan [string range $AckMsgHead 2 3] "$short" DstMID
    if {$DstMID < 0} {
        set DstMID [expr {$DstMID + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 1] $DstMID]
    if {$MatchResult == 0} {
        #puts "ack msg head match fail:'DstMID' field"
        return -2
    }
    
   #匹配消息ID
    binary scan [string range $AckMsgHead 4 7] "$int" MsgID
    if {$MsgID < 0} {
        set MsgID [format %u $MsgID]
    }
    set MsgID [format "%#010x" $MsgID]
    set MatchResult [regexp [lindex $HeadMatchList 2] $MsgID]
    if {$MatchResult == 0} {
        puts "ack msg head match fail:'MsgID' field"
        return -1
    }

    #匹配消息类型
    binary scan [string range $AckMsgHead 8 9] "$short" MsgType
    if {$MsgType < 0} {
        set MsgType [expr {$MsgType + 65536}]
    }
    set MatchResult [regexp [lindex $HeadMatchList 3] $MsgType]
    if {$MatchResult == 0} {
        puts "ack msg head match fail:'MsgType' field"
        return -1
    }
    
    #匹配保留字段
    binary scan [string range $AckMsgHead 10 11] "$short" Reserve
    if {$Reserve != 0} {
        puts "ack msg head match fail:'Reserve' field"
        return -1
    }
    
    #匹配消息体长度
    binary scan [string range $AckMsgHead 12 15] "$int" BodyLen
    if {$BodyLen != [string length $AckMsgBody]} {
        puts "ack msg head mathc fail:'BodyLength' field"
        return -1
    }
    
    #puts "ack msg head is Ok"  ;#消息头已经匹配
    
    if {[format %#x $MsgType] >= 0x8007 && [format %#x $MsgType] <= 0x8018} {
        #应答消息的类型属于CMM模块
        
        #匹配消息体Result字段
        binary scan [string range $AckMsgBody 0 3] "$int" Result
        set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
        if {$MatchResult == 0} {
            puts "ack msg body match fail:'Result' field"
            return -1
        }  

        if {$Result == 0} {
            #正常应答消息的处理
            switch $MsgType\
                $MSG_CMM_SAVE_CFG_ACK {
                    if {$BodyLen == 4} {             
                        return 0 ;#匹配成功
                    } else {
                        puts "msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_RECOVER_CFG_ACK {
                    if {$BodyLen == 4} {                    
                        return 0 ;#匹配成功
                    } else {
                        puts "msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_SET_VAL_ACK {
                    if {$BodyLen == 8} {
                        #匹配Status字段
                        binary scan [string range $AckMsgBody 4 7] "$int" Status
                        set MatchResult [regexp [lindex $BodyMatchList 1] $Status]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'Status' field"
                            return -1
                        } else {
                            return 0
                        }
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_GET_NAME_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]                        
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }    
                }\
                $MSG_CMM_GET_VAL_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }
                }\
                $MSG_CMM_ADD_NODE_ACK {
                    if {$BodyLen == 12} {
                        #匹配ObjNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ObjNo
                        if {$ObjNo < 0} {
                            set ObjNo [format %u $ObjNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ObjNo]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjNo' field"
                            return -1
                        }
                        
                        #匹配ObjStatus字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ObjStatus
                        if {$ObjStatus < 0} {
                            set ObjStatus [format %u $ObjStatus]
                        }                        
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ObjStatus]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjStatus' field"
                            return -1
                        }
                        
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_DEL_NODE_ACK {
                    if {$BodyLen == 8} {
                        #匹配ObjStatus字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ObjStatus
                        if {$ObjStatus < 0} {
                            set ObjStatus [format %u $ObjStatus]
                        }
                        
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ObjStatus]                         
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ObjStatus' field"
                            return -1
                        }
                        
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1
                    }
                }\
                $MSG_CMM_SET_ATTR_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail: body length error"
                        return -1                       
                    }
                }\
                $MSG_CMM_GET_ATTR_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        #将错误信息加入日志
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }                    
                }\
                $MSG_CMM_GET_NOTI_ACK {
                    if {$BodyLen < 8} {
                        puts "ack msg body match fail:body length error"
                        return -1
                    } else {
                        #匹配ResItemCount字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]                        
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItemCount' field"
                            return -1
                        }
                        
                        #匹配ResItem字段
                        set ResItemStr [string range $AckMsgBody 8 end]
                        for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ResItemStr]
                            set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ResItem($i)' field"
                                return -1
                            }
                        
                            set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                  
                        }
                    
                        return 0
                    }                                    
                }\
                $MSG_CMM_CLEAR_NOTI_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail: body length error"
                        return -1                       
                    }               
                }\
                $MSG_CMM_UPDATE_ACK {
                    if {$BodyLen == 4} {
                        return 0
                    } else {
                        puts "ack msg body match fail:body length error"
                        return -1                       
                    }
                }\
                default {
                    puts "Ack Msg Type error!"
                    return -1
                }
              
        } else {
            #错误应答消息的处理
            if {[format %#x $MsgType] == 0x8013} {
                if {$BodyLen == 4} {
                    return 0
                } else {
                    puts "ack msg body match fail:body length error"
                    return -1
                }
            }
            
            if {$BodyLen <12} {
                puts "ack msg body match fail:body length error"
                return -1
            } else {
                #匹配ErrNo字段
                binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                if {$ErrNo < 0} {
                    set ErrNo [format %u $ErrNo]
                }
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                if {$MatchResult ==0} {
                    puts "ack msg body match fail:'ErrNo' field"
                    return -1
                }
                        
                #匹配ErrItemCount字段
                binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                if {$MatchResult ==0} {
                    puts "ack msg body match fail:'ErrItemCount' field"
                    return -1
                }
                        
                #匹配ErrItem字段
                set ErrItemStr [string range $AckMsgBody 12 end]
                        
                for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j} {
                    set NulCharIndex [string first "\0" $ErrItemStr]
                            
                    set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                    set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ErrItem($i)' field"
                        return -1
                    }
                        
                    set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                }
                
                return 0
            }
        }
    
    } else {
        #应答消息的类型属于其它模块
        switch $MsgType\
            $MSG_PC_START_PROC_ACK {
                if {$BodyLen ==8} {
                    #匹配StartOK字段
                    binary scan [string range $AckMsgBody 0 3] "$int" StartOk
                    if {$StartOk < 0} {
                        set StartOk [format %u $StartOk]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $StartOk]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'StartOk' field"
                        return -1
                    }
                    
                    #匹配ProcId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" ProcId                    
                    if {$ProcId < 0} {
                        set ProcId [format %u $ProcId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $ProcId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'ProcId field'"
                        return -1
                    }
                    
                    return 0 ;#消息匹配成功                   
                } else {
                    puts "ack msg body match fail,body length error"
                    return -1
                }     
            }\
            $MSG_PC_SEND_SIGNAL_ACK {
                if {$BodyLen == 4} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {                        
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail,'Result' field"
                        return -1
                    } else {                   
                        return 0 ;#消息匹配成功
                    }
                } else {
                    puts "ack msg body match fail,body length error"
                    return -1
                }
            }\
            $MSG_TR069_GET_EVENTS_ACK {
                if {$BodyLen < 2} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配EventNum字段
                    binary scan [string range $AckMsgBody 0 1] "$short" EventNum
                    set MatchResult [regexp [lindex $BodyMatchList 0] $EventNum]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }                         
                    
                    #匹配EventContent字段
                    set EventContent [string range $AckMsgBody 2 end]
                    for {set i 1} {$i <= $EventNum} {incr i} {
                        set NulCharIndex [string first "\0" $EventContent]
                        set Event($i) [string range $EventContent 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $i] $Event($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'EventContent' field"
                            return -1
                        }
                        
                        set EventContent [string range $EventContent [incr NulCharIndex] end]                  
                    }
                    
                    return 0
                }                                
            }\
            $MSG_TR069_DOWNLOAD_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配Status字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Status
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Status]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Status' field"
                        return -1
                    }
                    
                    #匹配后面的字符串字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    for {set i 1} {$i <= 4} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                        switch $i \
                            1 {
                                set StartTime   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 1] $StartTime]
                                set field "StartTime"
                            }\
                            2 {
                                set CompleteTime [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult  [regexp [lindex $BodyMatchList 2] $CompleteTime]
                                set field "CompleteTime"
                            }\
                            3 {
                                set FaultCode   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 3] $FaultCode]
                                set field "FaultCode"                                
                            }\
                            4 {
                                set ErrorMsg    [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 4] $ErrorMsg]
                                set field "ErrorMsg"
                            }
                            
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'$field' field"
                            return -1
                        }
                            
                        set ItemStr  [string range $ItemStr [incr NulCharIndex] end]                                                                                      
                    }
                    
                    return 0
                }
            }\
            $MSG_TR069_UPLOADLOAD_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail, body length error"
                    return -1
                } else {
                    #匹配Status字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Status
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Status]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Status' field"
                        return -1
                    }
                    
                    #匹配后面的字符串字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    for {set i 1} {$i <= 2} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                        switch $i \
                            1 {
                                set FaultCode   [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 1] $FaultCode]
                                set field "FaultCode"                                
                            }\
                            2 {
                                set ErrorMsg    [string range $ItemStr 0 $NulCharIndex]
                                set MatchResult [regexp [lindex $BodyMatchList 2] $ErrorMsg]
                                set field "ErrorMsg"
                            }
                            
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'$field' field"
                            return -1
                        }
                            
                        set ItemStr  [string range $ItemStr [incr NulCharIndex] end]                                                                                      
                    }
                    
                    return 0
                }                                                    
            }\
            $MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK {
                set ItemStr [string range $AckMsgBody 0 end]
                for {set i 0} {$i < 4} {incr i} {
                    set NulCharIndex [string first "\0" $ItemStr]
                    switch $i \
                        0 {
                            set CommandKey  [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 0] $CommandKey]
                            set field "CommandKey"
                        }\
                        1 {
                            set FaultStruct [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 1] $FaultStruct]
                            set field "FaultStruct"                        
                        }\
                        2 {
                            set DownloadStartTime [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 2] $DownloadStartTime]
                            set field "DownloadStartTime"
                        }\
                        3 {
                            set DownloadCompleteTime [string range $ItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList 3] $DownloadCompleteTime]
                            set field "DownloadCompleteTime"
                        }
                    
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'$field' field"
                        return -1
                    }
                    
                    set ItemStr  [string range $ItemStr [incr NulCharIndex] end]

                }
                
                return 0
            }\
            $MSG_TIMER_REGSTER_ACK {
                if {$BodyLen ==8} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    } 
                    
                    #匹配TimerId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" TimerId
                    if {$TimerId < 0} {
                        set TimerId [format %u $TimerId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $TimerId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'TimerID' field"
                        return -1
                    }                     
                    
                    return 0
                } else {
                    puts "ack msg body match fail: body length error"
                    return -1
                }
            }\
            $MSG_TIMER_UNREGSTER_ACK {
                if {$BodyLen ==8} {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    } 
                    
                    #匹配TimerId字段
                    binary scan [string range $AckMsgBody 4 7] "$int" TimerId
                    if {$TimerId < 0} {
                        set TimerId [format %u $TimerId]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 1] $TimerId]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'TimerID' field"
                        return -1
                    }                     
                    
                    return 0
                } else {
                    puts "ack msg body match fail: body length error"
                    return -1
                }    
            }\
            $MSG_MON_INTF_REGISTER_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail: body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult ==0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                    
                    if {$Result == 0} {
                        if {$BodyLen == 4} {                        
                            return 0
                        } else {
                            puts "ack msg body match fail: body length error"
                            return -1
                        }
                    } else {
                        #匹配ErrNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                        if {$ErrNo < 0} {
                            set ErrNo [format %u $ErrNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrNo' field"
                            return -1
                        }
                        
                        #匹配ErrItemCount字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrItemCount' field"
                            return -1
                        }
                        
                        #匹配ErrItem字段
                        set ErrItemStr [string range $AckMsgBody 12 end]
                        
                        for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j} {
                            set NulCharIndex [string first "\0" $ErrItemStr]
                            
                            set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ErrItem($i)' field"
                                return -1
                            }
                        
                            set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                        }
                        
                        return 0
                    }          
                }            
            }\
            $MSG_MON_INTF_UNREGISTER_ACK {
                if {$BodyLen < 4} {
                    puts "ack msg body match fail: body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult ==0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                    
                    if {$Result == 0} {
                        if {$BodyLen == 4} {
                            return 0
                        } else {
                            puts "ack msg body match fail: body length error"
                            return -1
                        }
                    } else {
                        #匹配ErrNo字段
                        binary scan [string range $AckMsgBody 4 7] "$int" ErrNo
                        if {$ErrNo < 0} {
                            set ErrNo [format %u $ErrNo]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList 1] $ErrNo]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrNo' field"
                            return -1
                        }
                        
                        #匹配ErrItemCount字段
                        binary scan [string range $AckMsgBody 8 11] "$int" ErrItemCount
                        set MatchResult [regexp [lindex $BodyMatchList 2] $ErrItemCount]
                        if {$MatchResult ==0} {
                            puts "ack msg body match fail:'ErrItemCount' field"
                            return -1
                        }
                        
                        #匹配ErrItem字段
                        set ErrItemStr [string range $AckMsgBody 12 end]
                        
                        for {set i 1;set j 3} {$i <= $ErrItemCount} {incr i;incr j } {
                            set NulCharIndex [string first "\0" $ErrItemStr]
                            
                            set ErrItem($i) [string range $ErrItemStr 0 $NulCharIndex]
                            set MatchResult [regexp [lindex $BodyMatchList $j] $ErrItem($i)]
                            if {$MatchResult == 0} {
                                puts "ack msg body match fail:'ErrItem($i)' field"
                                return -1
                            }
                        
                            set ErrItemStr [string range $ErrItemStr [incr NulCharIndex] end]               
                        }
                        return 0
                    }    
                        
                }     
            }\
            $MSG_GET_DHOST_LIST_ACK {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else { 
                    #匹配LanDevNo字段
                    binary scan [string range $AckMsgBody 0 3] "$int" LanDevNo
                    if {$LanDevNo < 0} {
                        set LanDevNo [format %u $LanDevNo]
                    }
                    set MatchResult [regexp [lindex $BodyMatchList 0] $LanDevNo]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'LanDevNo' field"
                        return -1
                    }
                    
                    #匹配DHostCount字段
                    binary scan [string range $AckMsgBody 4 7]  "$int" DHostCount
                    set MatchResult [regexp [lindex $BodyMatchList 1] $DHostCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'DHostCount' field"
                        return -1
                    }
                    
                    #匹配DHostList字段
                    set DHostList [string range $AckMsgBody 8 end]
                    
                    for {set i 1;set j 2} {$i <= $DHostCount} {incr i;incr j} {
                        #匹配DHostRemainTime字段
                        binary scan [string range $DHostList 0 3] "$int" DHostRemainTime($i)
                        if {$DHostRemainTime($i) < 0} {
                            set DHostRemainTime($i) [format %u $DHostRemainTime($i)]
                        }
                        set MatchResult [regexp [lindex $BodyMatchList $j] $DHostRemainTime($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'DHostRemainTime($i)' field"
                            return -1
                        }                       
                        
                        set DHostList [string range $DHostList 4 end]
                        set NulCharIndex [string first "\0" $DHostList]
                        
                        #匹配DHostIpAddr字段
                        set DHostIpAddr($i) [string range $DHostList 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList [incr j]] $DHostIpAddr($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'DHostIpAddr($i)' field"
                            return -1
                        }
                        
                        set DHostList [string range [incr NulCharIndex] end]
                    }
                    
                    return 0
                }
            }\
            $MSG_MON_INTF_STATUS_INFORM {
                if {$BodyLen <4} {
                    puts "ack msg body match fail:body length error"
                } else {
                    #匹配ItemCount字段
                    binary scan [string range $AckMsgBody 0 3] "$int" ItemCount
                    set MatchResult [regexp [lindex $BodyMatchList 0] $ItemCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ItemCount' field"
                        return -1
                    }
                    
                    #匹配Item字段
                    set ItemStr [string range $AckMsgBody 4 end]
                    
                    for {set i 1} {$i <= $ItemCount} {incr i} {
                        set NulCharIndex [string first "\0" $ItemStr]
                            
                        set Item($i) [string range $ItemStr 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $i] $Item($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'Item($i)' field"
                            return -1
                        }
                        
                        set ItemStr [string range $ItemStr [incr NulCharIndex] end]               
                    }
                    
                    return 0
                }
            
            }\
            $MSG_AUTH_ACK {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配Result字段
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'Result' field"
                        return -1
                    }
                
                    #匹配ResItemCount字段
                    binary scan [string range $AckMsgBody 4 7] "$int" ResItemCount
                    set MatchResult [regexp [lindex $BodyMatchList 1] $ResItemCount]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'ResItemCount' field"
                        return -1
                    }                    
                    
                    #匹配ResItem字段
                    set ResItemStr [string range $AckMsgBody 8 end]
                    
                    for {set i 1;set j 2} {$i <= $ResItemCount} {incr i;incr j} {
                        set NulCharIndex [string first "\0" $ResItemStr]
                        set ResItem($i) [string range $ResItemStr 0 $NulCharIndex]
                        set MatchResult [regexp [lindex $BodyMatchList $j] $ResItem($i)]
                        if {$MatchResult == 0} {
                            puts "ack msg body match fail:'ResItem($i)' field"
                            return -1
                        }
                        
                        set ResItemStr [string range $ResItemStr [incr NulCharIndex] end]                                                            
                    }
                    
                    return 0
                }            
            }\
            $MSG_CMM_CLEAR_CFG_ACK {
                if {$BodyLen != 4} {
                    puts "ack msg body match fail:body length error"
                    return -1                   
                } else {
                    binary scan [string range $AckMsgBody 0 3] "$int" Result
                    if {$Result < 0} {
                        set Result [format %u $Result]
                    }
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail:'Result' field"
                    return -1
                }               
                
                return 0
            }\
            $MSG_SNTP_TIME_UPDATE {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配OldSecond字段
                    binary scan [string range $AckMsgBody 0 3] "$int" OldSecond
                    if {$OldSecond < 0} {
                        set OldSecond [format %u $OldSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $OldSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'OldSecond' field"
                        return -1
                    }
                    
                    #匹配NewSecond字段
                    binary scan [string range $AckMsgBody 4 7] "$int" NewSecond
                    if {$NewSecond < 0} {
                        set NewSecond [format %u $NewSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 1] $NewSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'NewSecond' field"
                        return -1
                    }                    
                
                    return 0
                }
            }\
            $MSG_SNTP_TIME_CHANGED {
                if {$BodyLen < 8} {
                    puts "ack msg body match fail:body length error"
                    return -1
                } else {
                    #匹配OldSecond字段
                    binary scan [string range $AckMsgBody 0 3] "$int" OldSecond
                    if {$OldSecond < 0} {
                        set OldSecond [format %u $OldSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 0] $OldSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'OldSecond' field"
                        return -1
                    }
                    
                    #匹配NewSecond字段
                    binary scan [string range $AckMsgBody 4 7] "$int" NewSecond
                    if {$NewSecond < 0} {
                        set NewSecond [format %u $NewSecond]
                    }
                    
                    set MatchResult [regexp [lindex $BodyMatchList 1] $NewSecond]
                    if {$MatchResult == 0} {
                        puts "ack msg body match fail:'NewSecond' field"
                        return -1
                    }                    
                
                    return 0
                }            
            }\
            $MSG_MSG4UDP_REGISTER_ACK {
                return 0
            }\
            $MSG_MSG4UDP_UNREGISTER_ACK {
                return 0
            }\
            $MSG_MSG4UDP_MESSAGE_ACK {
                return 0
            }\
            $MSG_VLAN_ADD_BRIDGE_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配InfoString字段
                set InfoString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $InfoString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'InfoString' field"
                    return -1
                } 
                
                return 0
            }\
            $MSG_VLAN_DEL_BRIDGE_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0            
            }\
            $MSG_VLAN_BIND_PORT_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0            
            }\
            $MSG_VLAN_UNBIND_PORT_ACK {
                #匹配Result字段
                binary scan [string range $AckMsgBody 0 3] "$int" Result
                if {$Result < 0} {
                    set Result [format %u $Result]
                }
                
                set MatchResult [regexp [lindex $BodyMatchList 0] $Result]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'Result' field"
                    return -1
                }
                
                #匹配ErrorString字段
                set ErrorString [string range $AckMsgBody 4 end]
                set MatchResult [regexp [lindex $BodyMatchList 1] $ErrorString]
                if {$MatchResult == 0} {
                    puts "ack msg body match fail: 'ErrorString' field"
                    return -1
                } 
                
                return 0           
            }\
            default {
                puts "Unknown Ack MsgType"
                return -1               
            }            
    }
}

### SocketClient命令用于打开客户端套接口字，并带有超时机制 ###
proc SocketClient {host port timeout} {
    global connected
    after $timeout {set connected timeout}
    set sock [socket -async $host $port]
    fileevent $sock writable {set connected ok}
    vwait connected
    fileevent $sock writable {} ;#删除已注册的命令
    
    if {$connected == "timeout"} {
        return -code error timeout
    } else {
        puts "connect ok"
        return $sock
    }
}

###### SendMsg命令用于向套接字中发送消息 ######
proc SendMsg {Sock Msg} {
    fileevent $Sock writable {}
    puts -nonewline $Sock $Msg
    flush $Sock
    
}
######## RecvMsg命令用于从套接字中读取消息 ######
proc RecvMsg {Sock} {
    set AckMsg [read $Sock]
    
    if {[eof $Sock]} {
        puts "connect done!"
        catch {close $Sock}
        #return -1
        return -code error
    } 
    
    return $AckMsg
}

######## InitMsgQueue 命令用于初始化接收消息队列 ##########
proc InitMsgQueue {QueueLen} {
    global MsgQueue MsgQueLen ReadIndex WriteIndex 
    
    set ReadIndex     0
    set WriteIndex    0     
    set MsgQueLen     $QueueLen
    
    for {set i 0} {$i < $MsgQueLen} {incr i} {
        set MsgQueue($i) 0
    }
}

###### WriteMsgQueue命令用于读取消息并将消息存入消息队列 #######
proc WriteMsgQueue {sock} {
    global MsgQueue MsgQueLen WriteIndex ReadFlag
   
    set AckMsg [RecvMsg $sock]
    
    if  {$WriteIndex < $MsgQueLen} {
        set MsgQueue($WriteIndex) $AckMsg
        incr WriteIndex
    } else {
        set WriteIndex 0
        set MsgQueue($WriteIndex) $AckMsg
        incr WriteIndex
    }
    
    set ReadFlag 1
}

###### ReadMsgQueue命令用于从消息队列中读出消息 #######
proc ReadMsgQueue {} {   
    global MsgQueue MsgQueLen WriteIndex ReadIndex
    
    if {$WriteIndex != $ReadIndex} {
        if {$ReadIndex < $MsgQueLen} {
            set AckMsg $MsgQueue($ReadIndex)
            incr ReadIndex
        } else {
            set ReadIndex 0
            set AckMsg $MsgQueue($ReadIndex)
            incr ReadIndex
        }
        
        return $AckMsg
    } else {
        return 0
    }
}

######### CreateLog 命令用于创建测试日志 ##########
proc CreateLog {module TestCaseType MsgIndex} {
    global ScriptPath
    
    set LogFile "$ScriptPath/log/test_$module.log"
    set LogFileId [open $LogFile a+]
    fconfigure $LogFileId -translation crlf
    
    if {[file size $LogFile] == 0} {
        set str1 "###################################################################"
        set str2 "#                                                                 #"
        set str3 "此文件包含$module 模块的测试日志"
        set PrefixSum [expr {([string length $str1]-[string length $str3]-2)/2}]
        
        puts $LogFileId $str1
        puts $LogFileId $str2
        puts $LogFileId [string replace $str2 $PrefixSum [expr {$PrefixSum + [string length $str3]-1}] $str3]
        puts $LogFileId $str2
        puts $LogFileId "$str1\n"
    }
       
    set time [clock format [clock seconds] -format "%Y-%m-%d %A %H:%M:%S"]
    puts $LogFileId "\n===================================================="
    if {$TestCaseType == 1} {
        puts $LogFileId "     使用定制的单模块集成测试用例 $MsgIndex 进行测试"
    } elseif {$TestCaseType == 2} {
        puts $LogFileId "     使用定制的功能集成测试用例 $MsgIndex 进行测试"
    } else {
        puts $LogFileId "     使用用户自定义集成测试用例 $MsgIndex 进行测试"
    }    
    
    puts $LogFileId "        时间：$time"
    puts $LogFileId "===================================================="
    flush $LogFileId
    
    return $LogFileId
}
   
####### WriteFile命令用于将数据写入到文件中 ########
proc WriteFile {FileId Msg} {
    
    puts -nonewline $FileId $Msg
    
    set MsgLen [string length $Msg]
    
    if {[expr {$MsgLen % 16}] != 0} {
        set FillStr [string repeat [binary format "H2" ff] [expr {16 - $MsgLen%16}]]
        puts -nonewline $FileId $FillStr
    }
    
}

####### ParseName 命令用于根据编码返回该编码对应的模块名称或消息类型名称  #########
proc ParseName {code IsModNo} {
    global ScriptPath
    
    ###载入模块ID和消息类型定义脚本
    source "$ScriptPath/lib/msg-head.tcl"
    
    if {$IsModNo == 1} {
        switch $code\
            $MID_PC {
                set name PC
            }\
            $MID_SNMPA {
                set name SNMPA
            }\
            $MID_AUTH {
                set name AUTH
            }\
            $MID_IGMP_PROXY {
                set name IGMP_PROXY
            }\
            $MID_CMM {
                set name CMM
            }\
            $MID_LAN {
                set name LAN
            }\
            $MID_IPT {
                set name IPT
            }\
            $MID_ACL {
                set name ACL
            }\
            $MID_LOG {
                set name LOG
            }\
            $MID_ETHLAN {
                set name ETHLAN
            }\
            $MID_ETHWAN {
                set name ETHWAN
            }\
            $MID_PPPOE {
                set name PPPOE
            }\
            $MID_WLAN {
                set name WLAN
            }\
            $MID_TR069BE {
                set name TR069BE
            }\
            $MID_DGN {
                set name DGN
            }\
            $MID_DHCPR {
                set name DHCPR
            }\
            $MID_DHCPS {
                set name DHCPS
            }\
            $MID_TIMER {
                set name TIMER
            }\
            $MID_IPCONN {
                set name IPCONN
            }\
            $MID_FIREWALL {
                set name FIREWALL
            }\
            $MID_SNMPC {
                set name SNMPC
            }\
            $MID_QOS {
                set name QOS
            }\
            $MID_STATIC_ROUTING {
                set name STATIC_ROUTING
            }\
            $MID_VDSL {
                set name VDSL
            }\
            $MID_DNS {
                set name DNS
            }\
            $MID_ALG {
                set name ALG
            }\
            $MID_WAN {
                set name WAN
            }\
            $MID_DYNAMIC_ROUTING {
                set name DYNAMIC_ROUTING
            }\
            $MID_SNTP {
                set name SNTP
            }\
            $MID_VLAN {
                set name VLAN
            }\
            $MID_USB_MASS {
                set name USB_MASS
            }\
            $MID_FTPD {
                set name FTPD
            }\
            $MID_NATPRIO {
                set name NATPRIO
            }\
            $MID_LSVLAN {
                set name MID_LSVLAN
            }\
            $MID_PORTMAPPING {
                set name MID_PORTMAPPING
            }\
            $MID_URLFILTER {
                set name URLFILTER
            }\
            $MID_ATM {
                set name ATM
            }\
            $MID_DSL {
                set name DSL
            }\
            $MID_DDNS {
                set name DDNS
            }\
            $MID_PROUTE {
                set name ROUTE
            }\
            $MID_CFG {
                set name CFG
            }\
            $MID_SUPP {
                set name SUPP
            }\
            $MID_MACFILTER {
                set name MACFILTER
            }\
            $MID_TRACERT {
                set name TRACERT
            }\
            $MID_IPPD {
                set name IPPD
            }\
            $MID_WEBP {
                set name WEBP
            }\
            $MID_BRIDGE_FILTER {
                set name BRIDGE_FILTER
            }\
            $MID_SCHED {
                set name SCHED
            }\
            $MID_WEB {
                set name WEB
            }\
            $MID_LNB {
                set name LNB
            }\
            $MID_CLI {
                set name CLI
            }\
            $MID_TR069FE {
                set name TR069FE
            }\
            $MID_TR069_HTTPD {
                set name TR069_HTTPD
            }\
            $MID_ELM {
                set name ELM
            }\
            $MID_UPG {
                set name UPG
            }\
            $MID_VDSLD {
                set name VDSLD
            }\
            $MID_TM {
                set name TM
            }\
            $MID_MON {
                set name MON
            }\
            $MID_UPGCGI {
                set name UPGCGI
            }\
            $MID_FTPUPG {
                set name FTPUPG
            }\
            $MID_PTI {
                set name PTI
            }\
            $MID_MSG4UDP {
                set name MSG4UDP
            }\
            $MID_GRP_MACLIST_UPDATE {
                set name GRP_MACLIST_UPDATE
            }\
            $MID_GRP_MON_LINK_UPDATE {
                set name GRP_MON_LINK_UPDATE
            }\
            $MID_GRP_WAN_LINK_UPDATE {
                set name GRP_WAN_LINK_UPDATE
            }\
            $MID_GRP_WAN_CONN_UPDATE {
                set name GRP_WAN_CONN_UPDATE
            }\
            $MID_GRP_DNS_UPDATE {
                set name GRP_DNS_UPDATE
            }\
            $MID_GRP_WAN_DEL_INST {
                set name GRP_WAN_DEL_INST
            }\
            $MID_GRP_WAN_ADD_INST {
                set name GRP_WAN_ADD_INST
            }\
            $MID_GRP_LAN_DEL_INST {
                set name GRP_LAN_DEL_INST
            }\
            $MID_GRP_LAN_ADD_INST {
                set name GRP_LAN_ADD_INST
            }\
            $MID_GRP_WLAN_DEL_INST {
                set name GRP_WLAN_DEL_INST
            }\
            $MID_GRP_WLAN_ADD_INST {
                set name GRP_WLAN_ADD_INST
            }\
            default {
                set name UNKNOWN_MOD
            }
            
            return $name    
    } else {
        switch $code\
            $MSG_PC_START_PROC {
                set name MSG_PC_START_PROC
            }\
            $MSG_PC_SEND_SIGNAL {
                set name MSG_PC_SEND_SIGNAL
            }\
            $MSG_PC_PROC_STATE {
                set name MSG_PC_PROC_STATE
            }\
            $MSG_PC_PROC_OUTPUT {
                set name MSG_PC_PROC_OUTPUT
            }\
            $MSG_PC_EXIT {
                set name MSG_PC_EXIT
            }\
            $MSG_CMM_GET_VAL {
                set name MSG_CMM_GET_VAL
            }\
            $MSG_CMM_SET_VAL {
                set name MSG_CMM_SET_VAL
            }\
            $MSG_CMM_ADD_NODE {
                set name MSG_CMM_ADD_NODE
            }\
            $MSG_CMM_DEL_NODE {
                set name MSG_CMM_DEL_NODE
            }\
            $MSG_CMM_GET_NAME {
                set name MSG_CMM_GET_NAME
            }\
            $MSG_CMM_GET_ATTR {
                set name MSG_CMM_GET_ATTR
            }\
            $MSG_CMM_SET_ATTR {
                set name MSG_CMM_SET_ATTR
            }\
            $MSG_CMM_INFORM_NOTI {
                set name MSG_CMM_INFORM_NOTI
            }\
            $MSG_CMM_GET_NOTI {
                set name MSG_CMM_GET_NOTI
            }\
            $MSG_CMM_CLEAR_NOTI {
                set name MSG_CMM_CLEAR_NOTI
            }\
            $MSG_CMM_COMMIT {
                set name MSG_CMM_COMMIT
            }\
            $MSG_CMM_CANCEL {
                set name MSG_CMM_CANCEL
            }\
            $MSG_CMM_UPDATE {
                set name MSG_CMM_UPDATE
            }\
            $MSG_CMM_SAVE_CFG {
                set name MSG_CMM_SAVE_CFG
            }\
            $MSG_CMM_RECOVER_CFG {
                set name MSG_CMM_RECOVER_CFG 
            }\
            $MSG_CMM_UPGRADE {
                set name MSG_CMM_UPGRADE
            }\
            $MSG_CMM_BACKUP {
                set name MSG_CMM_BACKUP
            }\
            $MSG_CMM_REBOOT {
                set name MSG_CMM_REBOOT
            }\
            $MSG_TIMER_REGSTER {
                set name MSG_TIMER_REGSTER
            }\
            $MSG_TIMER_UNREGSTER {
                set name MSG_TIMER_UNREGSTER
            }\
            $MSG_TIMER_TIMEOUT {
                set name MSG_TIMER_TIMEOUT
            }\
            $MSG_WAN_INTERFACE_STATE {
                set name MSG_WAN_INTERFACE_STATE
            }\
            $MSG_WAN_CONN_EST {
                set name MSG_WAN_CONN_EST
            }\
            $MSG_WAN_CONN_FIN {
                set name MSG_WAN_CONN_FIN
            }\
            $MSG_WAN_LINK_CHG {
                set name MSG_WAN_LINK_CHG
            }\
            $MSG_LAN_IP_UPDATE {
                set name MSG_LAN_IP_UPDATE
            }\
            $MSG_LAN_MACLIST_UPDATE {
                set name MSG_LAN_MACLIST_UPDATE
            }\
            $MSG_LAN_DOMAIN_NAME_UPDATE {
                set name MSG_LAN_DOMAIN_NAME_UPDATE
            }\
            $MSG_GET_DHOST_LIST {
                set name MSG_GET_DHOST_LIST
            }\
            $MSG_DNS_CHANGE {
                set name MSG_DNS_CHANGE
            }\
            $MSG_WLAN_ADD_DEVICE {
                set name MSG_WLAN_ADD_DEVICE
            }\
            $MSG_WLAN_DEL_DEVICE {
                set name MSG_WLAN_DEL_DEVICE
            }\
            $MSG_TR069_SET_PARKEY {
                set name MSG_TR069_SET_PARKEY
            }\
            $MSG_TR069_GET_EVENTS {
                set name MSG_TR069_GET_EVENTS
            }\
            $MSG_TR069_CLEAR_EVENTS {
                set name MSG_TR069_CLEAR_EVENTS
            }\
            $MSG_TR069_DOWNLOAD {
                set name MSG_TR069_DOWNLOAD
            }\
            $MSG_TR069_UPLOADLOAD {
                set name MSG_TR069_UPLOADLOAD
            }\
            $MSG_TR069_REBOOT {
                set name MSG_TR069_REBOOT
            }\
            $MSG_TR069_FACTORYRESET {
                set name MSG_TR069_FACTORYRESET
            }\
            $MSG_TR069_SCHEDULEINFORM {
                set name MSG_TR069_SCHEDULEINFORM
            }\
            $MSG_TR069_GET_TRANSFERCOMPLETEINFO {
                set name MSG_TR069_GET_TRANSFERCOMPLETEINFO
            }\
            $MSG_LBT_WLAN {
                set name MSG_LBT_WLAN
            }\
            $MSG_LBT_WPS {
                set name MSG_LBT_WPS
            }\
            $MSG_TR069_HTTPD_CONNECT {
                set name MSG_TR069_HTTPD_CONNECT
            }\
            $MSG_TR069_HTTPD_CREATE_UPFILE {
                set name MSG_TR069_HTTPD_CREATE_UPFILE
            }\
            $MSG_ETHNET_LINK_STATE {
                set name MSG_ETHNET_LINK_STATE
            }\
            $MSG_ETHWAN_INTF_NAME {
                set name MSG_ETHWAN_INTF_NAME
            }\
            $MSG_MON_INTF_REGISTER {
                set name MSG_MON_INTF_REGISTER
            }\
            $MSG_MON_INTF_UNREGISTER {
                set name MSG_MON_INTF_UNREGISTER
            }\
            $MSG_MON_INTF_STATUS_QUERY {
                set name MSG_MON_INTF_STATUS_QUERY
            }\
             $MSG_MON_INTF_STATUS_INFORM {
                set name MSG_MON_INTF_STATUS_INFORM
            }\
            $MSG_AUTH {
                set name MSG_AUTH
            }\
            $MSG_WAN_DEL_INST {
                set name MSG_WAN_DEL_INST
            }\
            $MSG_WAN_ADD_INST {
                set name MSG_WAN_ADD_INST
            }\
            $MSG_LAN_DEL_INST {
                set name MSG_LAN_DEL_INST
            }\
            $MSG_LAN_ADD_INST {
                set name MSG_LAN_ADD_INST
            }\
            $MSG_WLAN_DEL_INST {
                set name MSG_WLAN_DEL_INST
            }\
            $MSG_WLAN_ADD_INST {
                set name MSG_WLAN_ADD_INST
            }\
            $MSG_CMD {
                set name MSG_CMD
            }\
            $MSG_RESP {
                set name MSG_RESP
            }\
            $MSG_RESP_FRAGMENT {
                set name MSG_RESP_FRAGMENT
            }\
            $MSG_RESP_ERR {
                set name MSG_RESP_ERR
            }\
            $MSG_CMM_CLEAR_CFG {
                set name MSG_CMM_CLEAR_CFG
            }\
            $MSG_BUTTON {
                set name MSG_BUTTON
            }\
            $MSG_SNTP_TIME_UPDATE {
                set name MSG_SNTP_TIME_UPDATE
            }\
            $MSG_SNTP_TIME_CHANGED {
                set name MSG_SNTP_TIME_CHANGED
            }\
            $MSG_NTPS_CHANGED {
                set name MSG_NTPS_CHANGED
            }\
            $MSG_MSG4UDP_REGISTER {
                set name MSG_MSG4UDP_REGISTER
            }\
            $MSG_MSG4UDP_UNREGISTER {
                set name MSG_MSG4UDP_UNREGISTER
            }\
            $MSG_MSG4UDP_MESSAGE {
                set name MSG_MSG4UDP_MESSAGE
            }\
            $MSG_WAN_CONN_ENABLE_UPDATE {
                set name MSG_WAN_CONN_ENABLE_UPDATE
            }\
            $MSG_VLAN_ADD_BRIDGE {
                set name MSG_VLAN_ADD_BRIDGE
            }\
            $MSG_VLAN_DEL_BRIDGE {
                set name MSG_VLAN_DEL_BRIDGE
            }\
            $MSG_VLAN_BIND_PORT {
                set name MSG_VLAN_BIND_PORT
            }\
            $MSG_VLAN_UNBIND_PORT {
                set name MSG_VLAN_UNBIND_PORT
            }\
            $MSG_CMM_INST_ADDED {
                set name MSG_CMM_INST_ADDED
            }\
            $MSG_CMM_INST_DELED {
                set name MSG_CMM_INST_DELED
            }\
            $MSG_WAN_CONN_SET_CHNG {
                set name MSG_WAN_CONN_SET_CHNG
            }\
            $MSG_PC_START_PROC_ACK {
                set name MSG_PC_START_PROC_ACK
            }\
            $MSG_PC_SEND_SIGNAL_ACK {
                set name MSG_PC_SEND_SIGNAL_ACK
            }\
            $MSG_PC_PROC_STATE_ACK {
                set name MSG_PC_PROC_STATE_ACK
            }\
            $MSG_PC_PROC_OUTPUT_ACK {
                set name MSG_PC_PROC_OUTPUT_ACK
            }\
            $MSG_PC_EXIT_ACK {
                set name MSG_PC_EXIT_ACK
            }\
            $MSG_CMM_GET_VAL_ACK {
                set name MSG_CMM_GET_VAL_ACK
            }\
            $MSG_CMM_SET_VAL_ACK {
                set name MSG_CMM_SET_VAL_ACK
            }\
            $MSG_CMM_ADD_NODE_ACK {
                set name MSG_CMM_ADD_NODE_ACK
            }\
            $MSG_CMM_DEL_NODE_ACK {
                set name MSG_CMM_DEL_NODE_ACK
            }\
            $MSG_CMM_GET_NAME_ACK {
                set name MSG_CMM_GET_NAME_ACK
            }\
            $MSG_CMM_GET_ATTR_ACK {
                set name MSG_CMM_GET_ATTR_ACK
            }\
            $MSG_CMM_SET_ATTR_ACK {
                set name MSG_CMM_SET_ATTR_ACK
            }\
            $MSG_CMM_INFORM_NOTI_ACK {
                set name MSG_CMM_INFORM_NOTI_ACK
            }\
            $MSG_CMM_GET_NOTI_ACK {
                set name MSG_CMM_GET_NOTI_ACK
            }\
            $MSG_CMM_CLEAR_NOTI_ACK {
                set name MSG_CMM_CLEAR_NOTI_ACK
            }\
            $MSG_CMM_COMMIT_ACK {
                set name MSG_CMM_COMMIT_ACK
            }\
            $MSG_CMM_CANCEL_ACK {
                set name MSG_CMM_CANCEL_ACK
            }\
            $MSG_CMM_UPDATE_ACK {
                set name MSG_CMM_UPDATE_ACK
            }\
            $MSG_CMM_SAVE_CFG_ACK {
                set name MSG_CMM_SAVE_CFG_ACK
            }\
            $MSG_CMM_RECOVER_CFG_ACK {
                set name MSG_CMM_RECOVER_CFG_ACK 
            }\
            $MSG_CMM_UPGRADE_ACK {
                set name MSG_CMM_UPGRADE_ACK
            }\
            $MSG_CMM_BACKUP_ACK {
                set name MSG_CMM_BACKUP_ACK
            }\
            $MSG_CMM_REBOOT_ACK {
                set name MSG_CMM_REBOOT_ACK
            }\
            $MSG_TIMER_REGSTER_ACK {
                set name MSG_TIMER_REGSTER_ACK
            }\
            $MSG_TIMER_UNREGSTER_ACK {
                set name MSG_TIMER_UNREGSTER_ACK
            }\
            $MSG_TIMER_TIMEOUT_ACK {
                set name MSG_TIMER_TIMEOUT_ACK
            }\
            $MSG_WAN_INTERFACE_STATE_ACK {
                set name MSG_WAN_INTERFACE_STATE_ACK
            }\
            $MSG_WAN_CONN_EST_ACK {
                set name MSG_WAN_CONN_EST_ACK
            }\
            $MSG_WAN_CONN_FIN_ACK {
                set name MSG_WAN_CONN_FIN_ACK
            }\
            $MSG_WAN_LINK_CHG_ACK {
                set name MSG_WAN_LINK_CHG_ACK
            }\
            $MSG_LAN_IP_UPDATE_ACK {
                set name MSG_LAN_IP_UPDATE_ACK
            }\
            $MSG_LAN_MACLIST_UPDATE_ACK {
                set name MSG_LAN_MACLIST_UPDATE_ACK
            }\
            $MSG_LAN_DOMAIN_NAME_UPDATE_ACK {
                set name MSG_LAN_DOMAIN_NAME_UPDATE_ACK
            }\
            $MSG_GET_DHOST_LIST_ACK {
                set name MSG_GET_DHOST_LIST_ACK
            }\
            $MSG_DNS_CHANGE_ACK {
                set name MSG_DNS_CHANGE_ACK
            }\
            $MSG_WLAN_ADD_DEVICE_ACK {
                set name MSG_WLAN_ADD_DEVICE_ACK
            }\
            $MSG_WLAN_DEL_DEVICE_ACK {
                set name MSG_WLAN_DEL_DEVICE_ACK
            }\
            $MSG_TR069_SET_PARKEY_ACK {
                set name MSG_TR069_SET_PARKEY_ACK
            }\
            $MSG_TR069_GET_EVENTS_ACK {
                set name MSG_TR069_GET_EVENTS_ACK
            }\
            $MSG_TR069_CLEAR_EVENTS_ACK {
                set name MSG_TR069_CLEAR_EVENTS_ACK
            }\
            $MSG_TR069_DOWNLOAD_ACK {
                set name MSG_TR069_DOWNLOAD_ACK
            }\
            $MSG_TR069_UPLOADLOAD_ACK {
                set name MSG_TR069_UPLOADLOAD_ACK
            }\
            $MSG_TR069_REBOOT_ACK {
                set name MSG_TR069_REBOOT_ACK
            }\
            $MSG_TR069_FACTORYRESET_ACK {
                set name MSG_TR069_FACTORYRESET_ACK
            }\
            $MSG_TR069_SCHEDULEINFORM_ACK {
                set name MSG_TR069_SCHEDULEINFORM_ACK
            }\
            $MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK {
                set name MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK
            }\
            $MSG_LBT_WLAN_ACK {
                set name MSG_LBT_WLAN_ACK
            }\
            $MSG_LBT_WPS_ACK {
                set name MSG_LBT_WPS_ACK
            }\
            $MSG_TR069_HTTPD_CONNECT_ACK {
                set name MSG_TR069_HTTPD_CONNECT_ACK
            }\
            $MSG_TR069_HTTPD_CREATE_UPFILE_ACK {
                set name MSG_TR069_HTTPD_CREATE_UPFILE_ACK
            }\
            $MSG_ETHNET_LINK_STATE_ACK {
                set name MSG_ETHNET_LINK_STATE_ACK
            }\
            $MSG_ETHWAN_INTF_NAME_ACK {
                set name MSG_ETHWAN_INTF_NAME_ACK
            }\
            $MSG_MON_INTF_REGISTER_ACK {
                set name MSG_MON_INTF_REGISTER_ACK
            }\
            $MSG_MON_INTF_UNREGISTER_ACK {
                set name MSG_MON_INTF_UNREGISTER_ACK
            }\
            $MSG_MON_INTF_STATUS_QUERY_ACK {
                set name MSG_MON_INTF_STATUS_QUERY_ACK
            }\
             $MSG_MON_INTF_STATUS_INFORM_ACK {
                set name MSG_MON_INTF_STATUS_INFORM_ACK
            }\
            $MSG_AUTH_ACK {
                set name MSG_AUTH_ACK
            }\
            $MSG_WAN_DEL_INST_ACK {
                set name MSG_WAN_DEL_INST_ACK
            }\
            $MSG_WAN_ADD_INST_ACK {
                set name MSG_WAN_ADD_INST_ACK
            }\
            $MSG_LAN_DEL_INST_ACK {
                set name MSG_LAN_DEL_INST_ACK
            }\
            $MSG_LAN_ADD_INST_ACK {
                set name MSG_LAN_ADD_INST_ACK
            }\
            $MSG_WLAN_DEL_INST_ACK {
                set name MSG_WLAN_DEL_INST_ACK
            }\
            $MSG_WLAN_ADD_INST_ACK {
                set name MSG_WLAN_ADD_INST_ACK
            }\
            $MSG_CMD_ACK {
                set name MSG_CMD_ACK
            }\
            $MSG_RESP_ACK {
                set name MSG_RESP_ACK
            }\
            $MSG_RESP_FRAGMENT_ACK {
                set name MSG_RESP_FRAGMENT_ACK
            }\
            $MSG_RESP_ERR_ACK {
                set name MSG_RESP_ERR_ACK
            }\
            $MSG_CMM_CLEAR_CFG_ACK {
                set name MSG_CMM_CLEAR_CFG_ACK
            }\
            $MSG_VLAN_ADD_BRIDGE_ACK {
                set name MSG_VLAN_ADD_BRIDGE_ACK
            }\
            $MSG_VLAN_DEL_BRIDGE_ACK {
                set name MSG_VLAN_DEL_BRIDGE_ACK
            }\
            $MSG_VLAN_BIND_PORT_ACK {
                set name MSG_VLAN_BIND_PORT_ACK
            }\
            $MSG_VLAN_UNBIND_PORT_ACK {
                set name MSG_VLAN_UNBIND_PORT_ACK
            }\
            $MSG_CMM_INST_DELED_ACK {
                set name MSG_CMM_INST_DELED_ACK
            }\
            default {
                set name UNKNOWN_MSG
            }
            
        return $name
    }

}

####### WriteLog 命令用于将发送或接收的消息转换成测试者易于查看的方式并写入日志 ######
proc WriteLog {FileId Msg MsgType MsgCount} {
    global int short
    
    if {$MsgType == 1} {
        #puts $FileId "Tcl ---> TM ($MsgCount)"
        puts  "Tcl ---> TM ($MsgCount)"
    } elseif {$MsgType == 2} {
        #puts $FileId "Tcl <--- TM ($MsgCount: Ack)"
        puts "Tcl <--- TM ($MsgCount: Ack)"
    } else {
        #puts $FileId "Tcl <--- TM ($MsgCount: Unconcerned)"
        puts "Tcl <--- TM ($MsgCount: Unconcerned)"
    }
    
    #puts $FileId "MsgHead:"
    puts "MsgHead:"
    binary scan [string range $Msg 0 1] "$short" SrcMID
    if {$SrcMID < 0} {
        set SrcMID [expr {$SrcMID + 65536}]
    }    
    #set SrcMID [format %#06x $SrcMID]
    set SrcMIDStr [ParseName $SrcMID 1]
    if {$SrcMIDStr == "UNKNOWN_MOD"} {
        set SrcMID "UNKNOWN_MOD([format %#06x $SrcMID])"
    } else {
        set SrcMID $SrcMIDStr
    }
    
    binary scan [string range $Msg 2 3] "$short" DstMID
    if {$DstMID < 0} {
        set SrcMID [expr {$DstMID + 65536}]
    } 
    #set DstMID [format %#06x $DstMID]
    set DstMIDStr [ParseName $DstMID 1]
    if {$DstMIDStr == "UNKNOWN_MOD"} {
        set DstMID "UNKNOWN_MOD([format %#06x $DstMID])"
    } else {
        set DstMID $DstMIDStr
    }
    
    
    binary scan [string range $Msg 4 7] "$int" MsgID
    if {$MsgID < 0} {
        set MsgID [format %u $MsgID]
    } 
    set MsgID [format %#010x $MsgID]
       
    binary scan [string range $Msg 8 9] "$short" MsgType
    if {$MsgType < 0} {
        set MsgType [expr {$MsgType + 65536}]
    }
    #set MsgType [format %#06x $MsgType]
    set MsgTypeStr [ParseName $MsgType 0]
    if {$MsgTypeStr == "UNKNOWN_MSG"} {
        set MsgType "UNKNOWN_MSG([format %#06x $MsgType])"
    } else {
        set MsgType $MsgTypeStr
    }
    
    binary scan [string range $Msg 12 15] "$int" BodyLength
    
    #puts $FileId "SrcMID:$SrcMID, DstMID:$DstMID, MsgID:$MsgID, MsgType:$MsgType, BodyLength:$BodyLength"
    puts "SrcMID:$SrcMID, DstMID:$DstMID, MsgID:$MsgID, MsgType:$MsgType, BodyLength:$BodyLength"
    
    #puts $FileId "MsgBody:"    
    puts "MsgBody:"
    set MsgBody [string range $Msg 16 end]
    set BodyLen [string length $MsgBody]
    
    if {$MsgBody != ""} {
        set BodyHexLen [expr {2*$BodyLen}]
        binary scan $MsgBody "H$BodyHexLen" MsgBodyHex
        
        ###将MsgBody中的非可视字符转换为可视字符    
        for {set i 0} {$i < $BodyLen} {incr i} {
            set char [string index $MsgBody $i]
            binary scan $char "c" ch_ascii
            if {$ch_ascii < 33 || $ch_ascii > 126} {
                append MsgBodyNew "."
            } else {
                append MsgBodyNew $char
            }                           
        }        
        set MsgBody $MsgBodyNew
    
        for {set i 0} {$i < $BodyHexLen} {incr i 2} {
            if {[expr {($i+2)%32}] == 0} {
                append MsgBodyHex_w "[string range $MsgBodyHex $i [expr {$i+1}]]"
            } else {
                append MsgBodyHex_w "[string range $MsgBodyHex $i [expr {$i+1}]] "  
            }
        }
        
        set SpaceFillNum [expr {47 - ([string length $MsgBodyHex_w]%47)}]
        append MsgBodyHex_w [string repeat " " $SpaceFillNum]
        
        for {} {$BodyLen > 0} {incr BodyLen -16} {
            #puts -nonewline $FileId  "[string range $MsgBodyHex_w 0 46] ; "
            puts -nonewline "[string range $MsgBodyHex_w 0 46] ; "
            
            set MsgBodyHex_w [string range $MsgBodyHex_w 47 end]
        
            #puts $FileId "[string range $MsgBody 0 15]"
            puts "[string range $MsgBody 0 15]"
            
            set MsgBody [string range $MsgBody 16 end]
        }
    }
    #puts $FileId ""
    puts ""
    #flush $FileId
}

######### CreateMsgBin 命令用于产生保存消息二进制数据的文件 #########
proc CreateMsgBin {module} {
    global ScriptPath
    
    set BinFile "$ScriptPath/test_log/test_$module.bin"
    set BinFileId [open $BinFile a+]
    fconfigure $BinFileId -encoding binary -translation binary
    
    set TimeStr [clock format [clock seconds] -format "%m-%d %H:%M:%S"]
    set TimeBin [binary format "a[string length $TimeStr]" $TimeStr]
    puts -nonewline $BinFileId "$TimeBin\0\0"
    
    return $BinFileId
}

###### IndexCompare 命令是lsort命令中用到的比较命令，用于对索引进行比较 #####
proc IndexCompare {a b} {
    set a_list [split $a .]
    set b_list [split $b .]
    set ItemCount_a [llength $a_list]
    set ItemCount_b [llength $b_list]
    
    if {$ItemCount_a != $ItemCount_b} {
        puts "error! the style of index for 'MsgList' must be same."
        return -code error
    }
    
    for {set i 0} {$i < $ItemCount_a} {incr i} {
        set a_item [lindex $a_list $i]
        set b_item [lindex $b_list $i]
        
        if {$a_item < $b_item} {
            return -1
        } elseif {$a_item > $b_item} {
            return 1
        } else {
            continue
        }
    }
    
    return 0
}

###################################################################################################
proc StartTest {module TestCaseType MsgIndex} {
    global ScriptPath host port timeout LogFileId ReadFlag    
    
    set IndexLevelList [split $MsgIndex .]
    
    foreach IndexLevel $IndexLevelList {
        if {![string is integer $IndexLevel]} {
            puts "error! each index level for msg must be a integer and be comparted by '.'"
            return
        }
    }
    
    source "$ScriptPath/lib/msg-head.tcl"
    
    set TestCaseNo [lindex $IndexLevelList 0]
    
    if {$TestCaseType == 1} {
        source "$ScriptPath/src/$module.tcl"
        puts "开始执行单模块集成定制测试用例 $MsgIndex"
    } elseif {$TestCaseType == 2} {  
        source "$ScriptPath/src/$module.func.tcl"
        puts "开始执行功能集成定制测试用例 $MsgIndex"
    } else {
        source "$ScriptPath/src/$module.user.tcl"
        puts "开始执行用户自定义集成测试用例 $MsgIndex"
    }
    
    set IndexList [array names MsgList]
    if {$IndexList == ""} {
        puts "error!test case set $TestCaseNo doesn't exist."
        return -1    
    }
    
    set NewIndexList ""   
    foreach Index $IndexList {
        if {[string match $MsgIndex.* $Index] || [string match $MsgIndex $Index]} {
            lappend NewIndexList $Index
        }     
    }
    
    if {$NewIndexList == ""} {
        puts "error! msg with index prefix $MsgIndex doesn't exist."
        return -1
    }
        
    set NewIndexList [lsort -command IndexCompare $NewIndexList] ;#对新索引列表中的索引进行排序
    
    ;#打开套接字
    set sock [SocketClient $host $port $timeout]
    fconfigure $sock -encoding binary -translation binary -blocking 0 ;#-buffering none
    
    set AckTimeout     5000    ;#应答超时，单位为ms
    set ProcMsgCount   0
    
    InitMsgQueue  20                                     ;#初始化消息队列
    fileevent $sock readable [list WriteMsgQueue $sock]  ;#注册消息接收处理命令
    
    foreach index $NewIndexList {        
        #读取消息注释变量
        if {[info exists comment($index)]} {
            puts "#####################################################################################"
            puts "\033\[0;34m$comment($index)\033\[0m"
            puts "#####################################################################################"
        }
        
        #读取消息变量
        if {$MsgList($index) != ""} {
            ###产生消息时产生的错误要捕捉
            if {[llength $MsgList($index)] == 2} {
                set Msg [CreateMsg $MsgList($index)]
            } elseif {[llength $MsgList($index)] >= 4} {
                set Msg [CreateMsg_n $MsgList($index)]
            } else {
                puts "\033\[0;31mitem logic sequence error in MsgList($index)\033\[0m"
                return -1
            }
            
            if {$Msg == -1} {
                return -1
            }
                                    
            WriteLog $LogFileId $Msg 1 $index                   ;#将生成的消息写入日志            
            fileevent $sock writable [list SendMsg $sock $Msg]  ;#发送消息
        } 

        #读取匹配变量
        if {$AckMatchList($index) != ""} {
            #响应匹配变量不为空（有匹配内容）
            while {1} {
                #从消息队列中读取消息
                set AckMsg [ReadMsgQueue]
                            
                if {$AckMsg == 0} {
                    #消息队列中暂时没有消息
                    after $AckTimeout [list set ReadFlag 0]
                    vwait ReadFlag
                    if {$ReadFlag == 1} {
                        #已经收到消息
                        set AckMsg [ReadMsgQueue]
                        after cancel [list set ReadFlag 0]
                    } else {
                        #没有收到响应消息
                        puts "AckMsg($index) timeout"
                        puts "\033\[0;31m测试用例$TestCaseNo 的第$index 条应答消息匹配失败\033\[0m"
                        return -1
                    }
                }
                
                if {[llength $AckMatchList($index)] == 2} {
                    set ParseResult [ParseAckMsg $AckMsg $AckMatchList($index)]
                } elseif {[llength $AckMatchList($index)] >= 4} {
                    set ParseResult [ParseAckMsg_n $AckMsg $AckMatchList($index)]
                } else {
                    puts "\033\[0;31mitem logic sequence error in AckMatchList($index)\033\[0m"
                    return -1
                }
                       
                if {$ParseResult == 0} {
                    #匹配成功
                    WriteLog $LogFileId $AckMsg  2 $index
                    puts "\033\[0;32m测试用例$TestCaseNo 的第$index 条应答消息匹配成功\n\n\033\[0m"
                    break
                } elseif {$ParseResult == -1} {
                    #匹配失败
                    WriteLog $LogFileId $AckMsg 2 $index
                    puts "\033\[0;31m测试用例$TestCaseNo 的第$index 条应答消息匹配失败\n\n\033\[0m"
                    break
                } else {
                    #返回值为-2表示接收到的是一个无关消息，应继续匹配
                    incr ProcMsgCount
                    WriteLog $LogFileId $AckMsg 3 $ProcMsgCount
                    puts "received a unconcerned msg"
                    continue
                }
            }

        } else {
            #响应匹配变量为空（无匹配内容）
            puts "测试用例$TestCaseNo 的第$index 条消息无需匹配\n\n"
            while {1} {
                #从消息队列中读取消息
                set AckMsg [ReadMsgQueue]
                if {$AckMsg == 0} {
                    #消息队列中暂时没有消息
                    after $AckTimeout [list set ReadFlag 0]
                    vwait ReadFlag
                    if {$ReadFlag == 1} {
                        #收到响应消息
                        set AckMsg [ReadMsgQueue]
                        after cancel [list set ReadFlag 0]
                        
                        incr ProcMsgCount
                        WriteLog $LogFileId $AckMsg 3 $ProcMsgCount
                        puts "received a unconcerned msg"
                                    
                        continue
                    } else {
                        #没有收到响应消息
                        #puts "测试用例$TestCaseNo 的第$index 条应答消息匹配成功"
                        after cancel [list set ReadFlag 0]
                        break
                    }
                } else {
                    #消息队列中有消息
                    incr ProcMsgCount
                    WriteLog $LogFileId $AckMsg 3 $ProcMsgCount
                    puts "received a unconcerned msg"
                    continue
                }
            }
        }
    }   
}

##################################################################################################
proc CreateMonLog {} {
    global ScriptPath
    
    set LogFile "$ScriptPath/log/MonMsg.log"
    set LogFileId [open $LogFile a+]
    fconfigure $LogFileId -translation crlf
    
    if {[file size $LogFile] == 0} {
        set str1 "###################################################################"
        set str2 "#                                                                 #"
        set str3 "           此文件包含从TM模块接收到的消息日志"
        set PrefixSum [expr {([string length $str1]-[string length $str3]-2)/2}]
        
        puts $LogFileId $str1
        puts $LogFileId $str2
        puts $LogFileId [string replace $str2 $PrefixSum [expr {$PrefixSum + [string length $str3]-1}] $str3]
        puts $LogFileId $str2
        puts $LogFileId "$str1\n"
    }
       
    set time [clock format [clock seconds] -format "%Y-%m-%d %A %H:%M:%S"]
    puts $LogFileId "\n===================================================="
    
    puts $LogFileId "        时间：$time"
    puts $LogFileId "===================================================="
    flush $LogFileId
    
    return $LogFileId

}
#################################################################################################
proc MonMsg {} {
    global ScriptPath host port timeout LogFileId ReadFlag
    
    ;#打开套接字
    set sock [SocketClient $host $port $timeout]
    fconfigure $sock -encoding binary -translation binary -blocking 0

    set ProcMsgCount   0
    InitMsgQueue      30                                 ;#初始化消息队列
    fileevent $sock readable [list WriteMsgQueue $sock]  ;#注册消息接收处理命令
    
    while (1) {
        #从消息队列中读取消息
        set AckMsg [ReadMsgQueue]
        if {$AckMsg == 0} {
            #消息队列中暂时没有消息
            vwait ReadFlag
            set AckMsg [ReadMsgQueue]  
        }
        
        puts "Reveived a msg from TM"
        incr ProcMsgCount
        WriteLog $LogFileId $AckMsg 3 $ProcMsgCount        
    
    }

}

#################################################################################################
proc ParseMod {ModArg} {
    switch  $ModArg\
        "ALG" {
            puts "match ALG module"
            set module alg
        }\
        "CMM" {
            puts "match CMM module"
            set module cmm
        }\
        "PC" {
            puts "match PC module"
            set module pc
        }\
        "TR069" {
            puts "match TR069 module"
            set module tr069
        }\
        "TIMER" {
            puts "match TIMER module"
            set module timer
        }\
        "ETHLAN" {
            puts "match ETHLAN module"
            set module ethlan
        }\
        "ETHWAN" {
            puts "match ETHWAN module"
            set module ethwan
        }\
        "MON" {
            puts "match MON module"
            set module mon
        }\
        "IPCONN" {
            puts "match IPCONN module"
            set module ipconn
        }\
        "IPT" {
            puts "match IPT module"
            set module ipt
        }\
        "ACL" {
            puts "match ACL module"
            set module acl
        }\
        "LOGGER" {
            puts "match LOGGER module"
            set module logger
        }\
        "PING" {
            puts "match PING module"
            set module ping
        }\
        "DHCPC" {
            puts "match DHCPC module"
            set module dhcpc
        }\
        "PPPOE" {
            puts "match PPPOE module"
            set module pppoe
        }\
        "WAN" {
            puts "match WAN module"
            set module wan
        }\
        "LAN" {
            puts "match LAN module"
            set module lan
        }\
        "DHCPS" {
            puts "match DHCPS module"
            set module dhcps
        }\
        "DHCPR" {
            puts "match DHCPR module"
            set module dhcpr
        }\
        "DNS" {
            puts "match DNS module"
            set module dns
        }\
        "WLAN" {
            puts "match WLAN module"
            set module wlan
        }\
        "WEB" {
            puts "match WEB module"
            set module web
        }\
        "MGNT" {
            puts "match MGNT module"
            set module mgnt
        }\
        "NAPT" {
            puts "match NAPT module"
            set module napt
        }\
        "NAT" {
            puts "match NAT module"
            set module nat
        }\
        "AUTH" {
            puts "match AUTH module"
            set module auth
        }\
        "FIREWALL" {
            puts "match FIREWALL module"
            set module firewall
        }\
        "SNMPC" {
            puts "match SNMPC module"
            set module snmpc
        }\
        "SNMPA" {
            puts "match SNMPA module"
            set module snmpa
        }\
        "QOS" {
            puts "match QOS module"
            set module qos
        }\
        "IGMP" {
            puts "match IGMP-PROXY module"
            set module igmp
        }\
        "STATIC-ROUTING" {
            puts "match STATIC-ROUTING module"
            set module static-routing
        }\
        "DYNAMIC-ROUTING" {
            puts "match DYNAMIC-ROUTING module"
            set module dynamic-routing
        }\
        "SNTP" {
            puts "match SNTP module"
            set module sntp
        }\
        "VLAN" {
            puts "match VLAN module"
            set module vlan
        }\
        "USB" {
            puts "match USB module"
            set module usb        
        }\
        "FTPD" {
            puts "match FTPD module"
            set module ftpd
        }\
        "LBT" {
            puts "match LBT module"
            set module lbt
        }\
        "TEST" {
            puts "match TEST module"
            set module test
        }\
        default {
            puts "unknown module name! the first arg should be:"
            puts "ALG,CMM,PC,TR069,TIMER,ETHLAN,ETHWAN,MON,IPCONN,IPT,PING,PPPOE,WAN,LAN,DHCPS,DNS,WEB,WLAN"
            return -1
        }
        
        return $module
}

