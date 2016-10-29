
#MSG_CMM_ADD_NODE消息测试
#增加实例, 增加两条实例
if {$TestCaseNo == 1} {

#正常流程
set MsgList(1.1.1) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}"
set AckMatchList(1.1.1) ""

set MsgList(1.1.2) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}" 
set AckMatchList(1.1.2) ""

}

#MSG_CMM_SET_VAL and MSG_CMM_COMMIT

if {$TestCaseNo == 2} {
#建立三个实例,其中1 2有效,3无效

set MsgList(2.1.1) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}" 
set AckMatchList(2.1.1) ""

set MsgList(2.1.2) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.1.URL=http\\\\:www.baidu.com\0}
}"
set AckMatchList(2.1.2) ""

set MsgList(2.1.3) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.1.3) ""

set MsgList(2.1.4) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}"
set AckMatchList(2.1.4) ""

set MsgList(2.1.5) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.hao123.com\0}
}"
set AckMatchList(2.1.5) ""

set MsgList(2.1.6) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.1.6) ""


set MsgList(2.1.7) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}" 
set AckMatchList(2.1.7) ""


#正常功能测试
#enable = 1
set MsgList(2.3.1) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=1\0}
}"
set AckMatchList(2.3.1) ""

set MsgList(2.3.2) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.3.2) ""


#enable = 0
set MsgList(2.3.3) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=0\0}
}"
set AckMatchList(2.3.3) ""

set MsgList(2.3.4) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.3.4) ""

#设置为白名单
set MsgList(2.3.5) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterMode=Allowed\0}
}"
set AckMatchList(2.3.5) ""

set MsgList(2.3.6) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.3.6) ""


#改变URL
set MsgList(2.3.7) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.sina.com\0}
}"
set AckMatchList(2.3.7) ""

set MsgList(2.3.8) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.3.8) ""


#异常测试
#enable值为空
set MsgList(2.4.1) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=\0}
}"
set AckMatchList(2.4.1) ""


#enable值错误
set MsgList(2.4.2) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=en\0}
}"
set AckMatchList(2.4.2) ""


#mode值为空
set MsgList(2.4.3) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterMode=\0}
}"
set AckMatchList(2.4.3) ""

#mode值错误
set MsgList(2.4.4) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterMode=RIPv\0}
}"
set AckMatchList(2.4.4) ""

#Url冲突
set MsgList(2.4.5) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.dayao.com\0}
}"
set AckMatchList(2.4.5) ""

set MsgList(2.9.1) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(2.9.1) ""

set MsgList(2.4.6) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.1.URL=http\\\\:www.dayao.com\0}
}"
set AckMatchList(2.4.6) ""

#URL过长
set MsgList(2.4.7) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.dayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayaodayao.com\0}
}"
set AckMatchList(2.4.7) ""


#URL出现不规则字符
set MsgList(2.4.8) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.da\我们都好\"\"yao.com\0}
}"
set AckMatchList(2.4.8) ""

}

#MSG_CMM_DEL_NODE

if {$TestCaseNo == 3} {

#建立三个实例,其中1 2有效,3无效

set MsgList(3.1.1) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}" 
set AckMatchList(3.1.1) ""

set MsgList(3.1.2) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.1.URL=http\\\\:www.baidu.com\0}
}"
set AckMatchList(3.1.2) ""

set MsgList(3.1.3) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(3.1.3) ""

set MsgList(3.1.4) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}"
set AckMatchList(3.1.4) ""

set MsgList(3.1.5) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.URL=http\\\\:www.hao123.com\0}
}"
set AckMatchList(3.1.5) ""

set MsgList(3.1.6) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(3.1.6) ""


set MsgList(3.1.7) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_ADD_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.\0}}" 
set AckMatchList(3.1.7) ""


#enable = 0,删除实例1
set MsgList(3.2.1) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=0\0}
}"
set AckMatchList(3.2.1) ""

set MsgList(3.2.2) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(3.2.2) ""

set MsgList(3.2.3) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.1.\0}}" 
set AckMatchList(3.2.3) ""


#enable = 1, 删除实例2
set MsgList(3.3.1) "{$MID_CMM  $MID_URLFILTER 0x00000013 $MSG_CMM_SET_VAL} \
{   1 
    {InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLFilterEnabled=1\0}
}"
set AckMatchList(3.3.1) ""

set MsgList(3.3.2) "{$MID_CMM $MID_URLFILTER 0x00000013 $MSG_CMM_COMMIT} {}"
set AckMatchList(3.3.2) ""

set MsgList(3.3.3) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.2.\0}}" 
set AckMatchList(3.3.3) ""

#删除无效实例
set MsgList(3.4.1) "{$MID_CMM  $MID_URLFILTER 0x00000001 $MSG_CMM_DEL_NODE} \
{{InternetGatewayDevice.X_TWSZ-COM_URLFilter.URLList.3.\0}}" 
set AckMatchList(3.4.1) ""

}

if {$TestCaseNo == 4} {
set MsgList(4.1.1) "{$MID_WEB  $MID_CMM 0x00000001 $MSG_CMM_SAVE_CFG} {}" 
set AckMatchList(4.1.1) ""
}

