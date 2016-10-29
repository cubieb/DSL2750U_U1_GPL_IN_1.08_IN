#!/usr/bin/tclsh
########################################################
#               测试人员可修改以下的参数               #
########################################################

#set EndianType  "little"    ;#用于小端字节序主机测试
set EndianType "big"         ;#用于大端字节序主机测试

set host      "192.168.1.1"   ;#服务器端的ip地址
set port      "7130"          ;#服务器端的端口
set timeout   "1000"          ;#连接超时值(ms)

########################################################
set ScriptPath [file dirname [info script]]

if {$EndianType == "little"} {
    set int i
    set short s
} else {
    set int I
    set short S
}

###载入模块ID和消息类型定义脚本
#source "$ScriptPath/lib/msg-head.tcl"

###载入命令定义脚本
source "$ScriptPath/lib/command-set.tcl"
##################################################################################
if {$argc == 0} {
    set LogFileId [CreateMonLog]
    MonMsg
    close $LogFileId
    
} elseif {$argc == 3} {
    set ModArg [string toupper [lindex $argv 0] 0 end]
    
    set module [ParseMod $ModArg]
    if {$module == -1} {
        return
    }
            
    if {[lindex $argv 1] == "-c"} {
        set TestCaseType  1                  ;#测试用例类型1代表定制的单模块集成测试用例
    } elseif {[lindex $argv 1] == "-f"} {
        set TestCaseType  2                  ;#测试用例类型2代表定制的功能集成测试用例
    } elseif {[lindex $argv 1] == "-u"} {
        set TestCaseType  3                  ;#测试用例类型3代表用户自定义的集成测试用例          
    } else {
        puts "error! the second argument must be '-c'、'-u' or '-f'"
        return 
    }
        
    set MsgIndex [string trimleft [lindex $argv 2] "0"]                                             
    set LogFileId [CreateLog $module $TestCaseType $MsgIndex]                
    set result [StartTest $module $TestCaseType $MsgIndex]
        
    if {$result == -1} {
        close $LogFileId
        return
    }
        
    close $LogFileId
} else {
    puts "error! the num of argument should be 0 or 3"
    return
}


