#include <stdio.h>
#include <stdlib.h>
#include "tbserror.h"

typedef struct __error_info
{
  int errnum;
  const char *const msg;
}error_info;


/*
三点要素:
1. 对应到TR069
2. 英文解释
3. 中文解释

*/


static const error_info error_table[] =
{
    {TR069_ERRNO_UNSUP_FUNC,      "Method not supported"},
    {TR069_ERRNO_REJECT,          "Request denied"},
    {TR069_ERRNO_CPE_ERR,         "Internal error"},
    {TR069_ERRNO_INVALID_PARAM,   "Invalid arguments"},
    {TR069_ERRNO_NO_RESOURCE,     "Resources exceeded"},
    {TR069_ERRNO_INVALID_NAME,    "Invalid parameter name"},
    {TR069_ERRNO_INVALID_TYPE,    "Invalid parameter type"},
    {TR069_ERRNO_INVALID_VAL,     "Invalid parameter value"},
    {TR069_ERRNO_WRITE_FORBID,    "Attempt to set a non-writable parameter"},
    {TR069_ERRNO_NOTI_REQ_REJ,    "Notification request rejected"},
    {TR069_ERRNO_DOWNLOAD_FAIL,   "unable to connect to the server"},
    {TR069_ERRNO_UPLOAD_FAIL,     "unable to connect to the server"},
    {TR069_ERRNO_FS_AUTHFAIL,     "File transfer server authentication failure"},
    {TR069_ERRNO_FS_NOTSUPP,      "Unsupported protocol for file transfer"},
    {TR069_ERRNO_INVALID_IGMP,      "Download failure: unable to join multicast group"},
    {TR069_ERRNO_INVALID_URL,     "Download failure: unable to contact file server"},
    
    {TBS_FAILED,                  "failure"},
    {TBS_MSG_SEND_ERR,            "message sending failed"},
    {TBS_OUT_OF_MEM,              "memory not enough"},
    {TBS_PARAM_ERR,               "Invalid parameter"},
    {TBS_OUT_OF_SRC,              "Resources not enough"},
    {TBS_TIME_OUT,                "timeout"},
    {TBS_COLLISION,               "collision"},
    {TBS_NULL_PTR,                "NULL pointer"},
    {TBS_LIST_NOT_EMPTY,          "List is not empty"},
    {TBS_LIST_EMPTY,              "List is empty"},
    {TBS_INVALID_INDEX,           "Invalid index"},
    {TBS_INDEX_OVERFLOW,          "Index overflow"},
    {TBS_CORRUPT_STRUCT,          "Corrupt struct"},
    {TBS_NO_INSTANCE,             "Do not found the instance"},
    {TBS_MODULE_BUSY,             "Setting up. Wait a moument"},

};

#define num_error_names  (sizeof(error_table)/sizeof(error_info))

/*=========================================================================*/
/*  函数名称: tbsstrerr                                                    */
/*  函数功能: 查找错误码对应的字符串并返回                                 */
/*  输  入  : errnum: 错误码，应为负数                                     */
/*  创  建  : huangce / 2008-03-19                                             */
/*=========================================================================*/

const char *tbsstrerr (int errnum)
{
    int i;
    const char *msg = "";
    static char buf[32];

    for(i = 0; i < num_error_names; i ++)
    {
        if(error_table[i].errnum == errnum)
        {
            msg = error_table[i].msg;
            break;
        }
    }
    if(i == num_error_names)
    {
        sprintf (buf, "Error %d", errnum);
        msg = buf;
    }
    return msg;
}


int tbserr2tr069(int tbserrno)
{
    int i;
    for(i = 0; i < num_error_names; i ++)
    {
        //TBS中错误码是负数
        if((error_table[i].errnum == tbserrno) && (error_table[i].errnum < 0))
        {
            //TR069中自定义的错误码范围为9800 - 9899
            return 9900 + error_table[i].errnum;
            break;
        }
    }
    return tbserrno;
}



const ST_TBS_ERR_TABLE g_astTbsErrTable[] =
{
    {ERR_NOT_SUPPORT_FUNCTION,    "Not support this function,now", "暂时不支持这种方法"},
    {ERR_IP_INVALID_FORMAT,       "Invalid IP format", "IP格式错误"},
    {ERR_PORT_INVALID_FORMAT,     "Invalid port format", "端口格式错误"},
    {ERR_PORT_TOO_SMALL,          "Port cant be zero or negative,should be in 1-65535",
                                  "端口值不能为0或负数, 应该为1-65535"},
    {ERR_PORT_TOO_BIG,            "Port should be in 1-65535",  "端口值无效，应该在1-65535"},
    {ERR_PRIORITY_INVALID_VALUE,  "Invalid priority value",  "优先级必须在1-8之间"},
    {ERR_PRIORITY_TOO_SMALL,      "Priority too small,should be in 1-8",  "优先级必须在1-8之间"},
    {ERR_PRIORITY_TOO_BIG,        "Priority too big,should be in 1-8",  "优先级必须在1-8之间"},
    {ERR_MASK_INVALID_FORMAT,     "Net mask invalid format",  "子网掩码格式错误"},
    {ERR_MASK_EMPTY,              "net mask can not be emty",  "子网掩码不能为空"},
    {ERR_MAKS_ALL_ZERO,           "net mask can not be zero",  "子网掩码不能为全0"},
    {ERR_MASK_INVALID_VALUE,      "the left part of net mask must be continuous binary 1",  "子网掩码左边应该是连续的1"},
    {ERR_ENABLE_EMPTY,            "enable should not be zero", "enable 不能为空"},
    {ERR_ENABLE_INVALID_VALUE,    "bool val must be \"0\" or \"1\"", "必须为\"0\" 或者\"1\""},
    {ERR_INT_INVALID_VALUE,       "must be int value",  "请输入整数"},
    {ERR_UINT_INVALID_VALUE,      "must be unsigned int", "请输入正整数"},
    {ERR_MAC_INVALID_VALUE,       "mac must be \"xx:xx:xx:xx:xx:xx\" format and consist of hexadecimal number without all 0",  "mac地址应该是不全为0的xx:xx:xx:xx:xx:xx形式的十六进制数。"},
    {ERR_IPLIST_TOO_MANY,         "ip number more than max limit",  "IP个数过多"},
    {ERR_IPLIST_INVALID_FORMAT,   "valid ip list format",  "IP 列表格式错误"},
    {ERR_MACLIST_INVALID_FORMAT,  "valid mac list format",  "MAC 列表格式错误"},
    {ERR_MACLIST_TOO_MANY,        "too many mac",  "mac 个数过多"},
    {ERR_INSTANCE_TOO_MANY,       "too many instance,can not add new one",  "已达实例上限，不能再添加"},
    {ERR_INSTANCE_IMMUTABLE,      "this instance can not be delete",  "本配置不允许删除"},
    {ERR_INNER_MSG_INVALID,       "inner msg format error",  "内部消息错误，请联系开发商"},
    {ERR_INNER_MSG_REPEATED_NAME, "inner msg has repeated name",  "内部消息错，有同名类型，请联系开发商"},
    {ERR_INNER_CACHE_INVALID,     "inner cache error",  "内部结构出错，请联系开发商"},
    {ERR_IP_MASK_NOT_MATCH,       "IP address does not match with the subnet mask ",  "IP地址和子网掩码不匹配"},
    {ERR_IP_IS_HOST_ADDR,         "IP address can not be host address ",  "不能为本地地址"},
    {ERR_IP_IS_CAST_ADDR,         "IP address can not be broadcast address ",  "不能为广播地址"},
    {ERR_IP_ADDR_START_MINOR_END, "IP end address minor start address ",  "结束地址不能小于开始地址"},
    {ERR_IP_ADDR_POOL_OVERLAP,    "IP address pool overlaps ",  "IP地址池冲突"},
    {ERR_STR_TOO_LONG,            "String is too long ",  "字符串过长"},
    {ERR_CAN_NOT_EMPTY,           "can not be empty ",  "不能为空"},
    {ERR_GET_NODE_VALUE_FAIL,     "get node value failed ",  "获取节点值失败"},
    {ERR_SET_NODE_VALUE_FAIL,     "Set node value failed ",  "设置节点值失败"},
    {ERR_INVALID_DOMAIN_NAME,     "Invalid domain name ",  "域名格式不对"},
    {ERR_OUT_OF_RANGE,            "the value out of range", "值超出指定的范围"},
    {ERR_IPLIST_HAS_REPEAT,       "IP list has to repeat", "列表中有重复IP地址"},
    {ERR_IP_IS_WRONG_NET_SECTION,         "wrong net section", "IP地址网段错误"},
    {ERR_IP_IS_CAST_OR_NET_ADDR,         "IP address can not be broadcast address or net address",  "不能为广播地址或网络地址"},
    {ERR_ENABLE_DHCPS_CONFLICT_DHCPR, "DHCP couldnt perform as server and relay simultaneously", "DHCP不能同时工作于Server和Relay"},
    {ERR_PORT_RANGE_INVALID,      "invalid portrange (min > max)", "无效的端口范围(结束端口>开始端口)"},
    {ERR_INVALID_VAL,           "invalid value", "无效值"},
    {ERR_LIST_HAS_REPEAT,       "The list are duplicates", "列表中有重复项"},
    {ERR_PACKET_RANGE_INVALID,      "invalid packetlength (min > max)", "无效的包长范围(开始包长>结束包长)"},
		

    {ERR_MSG_MID_INVALID,       "ERR_MSG_MID_INVALID","ERR_MSG_MID_INVALID"},
    {ERR_MSG_SOCKET,            "ERR_MSG_SOCKET","ERR_MSG_SOCKET"},
    {ERR_MSG_PROC_NOT_FOUND,    "ERR_MSG_PROC_NOT_FOUND","ERR_MSG_PROC_NOT_FOUND"},
    {ERR_MSG_SEND_FAIL,         "ERR_MSG_SEND_FAIL","ERR_MSG_SEND_FAIL"},
    {ERR_MSG_DSTMID_UNREGED,    "ERR_MSG_DSTMID_UNREGED","ERR_MSG_DSTMID_UNREGED"},
    {ERR_MSG_NOT_FULL,          "ERR_MSG_NOT_FULL","ERR_MSG_NOT_FULL"},
    {ERR_MSG_PART_NOEFFECT,     "ERR_MSG_PART_NOEFFECT","ERR_MSG_PART_NOEFFECT"},
    {ERR_MSG_PART_INVALID,      "ERR_MSG_PART_INVALID","ERR_MSG_PART_INVALID"},
    {ERR_MSG_TIMEOUT,           "ERR_MSG_TIMEOUT","ERR_MSG_TIMEOUT"},
    {ERR_MSG_PART_LIST_FULL,    "ERR_MSG_PART_LIST_FULL","ERR_MSG_PART_LIST_FULL"},

    {ERR_MALLOC_FAILED,         "ERR_MALLOC_FAILED","ERR_MALLOC_FAILED"},
    {ERR_BUF_NOT_ENOUGH,        "ERR_BUF_NOT_ENOUGH","ERR_BUF_NOT_ENOUGH"},
    {ERR_INTERNAL_ERR,          "ERR_INTERNAL_ERR","ERR_INTERNAL_ERR"},
    {ERR_PARA_INVALID,          "ERR_PARA_INVALID","ERR_PARA_INVALID"},
    {ERR_FILE_OPEN,             "ERR_FILE_OPEN","ERR_FILE_OPEN"},
    {ERR_FILE_READ,             "ERR_FILE_READ","ERR_FILE_READ"},
    {ERR_FILE_WRITE,            "ERR_FILE_WRITE","ERR_FILE_WRITE"},
    {ERR_FILE_CHKSUM,           "ERR_FILE_CHKSUM","ERR_FILE_CHKSUM"},
    {ERR_BUSY,                  "ERR_BUSY","ERR_BUSY"},
    {ERR_CREATE_MSG_FAIL,       "ERR_CREATE_MSG_FAIL","ERR_CREATE_MSG_FAIL"},
    {ERR_MSG_EXT_MID_LIST_FULL, "ERR_MSG_EXT_MID_LIST_FULL","ERR_MSG_EXT_MID_LIST_FULL"},

    {ERR_CFG_ATTR_STAT,         "Stat data not support set noti",             "统计数据不支持设置noti属性"},
    {ERR_CFG_PATH_NOT_EXSITED,  "Path not exist",                           "路径不存在"},
    {ERR_CFG_INVALID_OBJ_VAL,   "Attemp to set val to obj",                   "试图对对象路径设置值"},
    {ERR_CFG_INVALID_STRING,    "Invalid string val",                         "非法字符串"},
    {ERR_CFG_INVALID_INT_0,     "0 must expressed by \"0\", \"+0\"\"-0\"\"00\" are all invalid", "0只能用\"0\"表示, \"+0\"\"-0\"\"00\"皆视为非法"},
    {ERR_CFG_INVALID_INT_01,    "non-zero int must begin with \"+\" or non-zero number", "非0整型数必须以符号或者正数值开头"},
    {ERR_CFG_INVALID_INT_A,     "include invalid char",                       "含有非法字符"},
    {ERR_CFG_INVALID_UINT_0,    "0 must expressed by \"0\", \"+0\"\"-0\"\"00\" are all invalid", "0只能用\"0\"表示, \"+0\"\"-0\"\"00\"皆视为非法"},
    {ERR_CFG_INVALID_UINT_01,   "non-zero unit must begin non-zero number",   "非0无符整型数必须以符号或者正数值开头"},
    {ERR_CFG_INVALID_UINT_A,    "include invalid char",                       "含有非法字符"},
    {ERR_CFG_INVALID_BOOL,      "bool val must be \"0\" or \"1\"",                "必须为\"0\" 或者\"1\""},
    {ERR_CFG_INVALID_TIME,      "dateTime val must be like \"0000-00-02T03:04:05\"", "必须形如 \"0000-00-02T03:04:05\""},
    {ERR_CFG_INVALID_HEX,       "Hex val must be number or A-F a-f",          "十六进制数必须是数字或者a-f A-F"},
    {ERR_CFG_NOT_WRITABLE,      "The path not writable",                      "该路径不可写"},
    {ERR_CFG_NOT_IN_ACCESSLIST, "Not in accesslist",                          "不在可访问列表中"},
    {ERR_CFG_PATH_NOT_INST,     "The path is not a valid inst",               "该路径不是一个合法的对象实例"},
    {ERR_CFG_NOT_STANDARD,      "The path is not a standard path",            "该路径不是一个标准的路径"},
    {ERR_CFG_INVALID_TYPE,      "The node type is invalid",                   "节点类型非法"},
    {ERR_CFG_ITEM_OPT_FAIL,     "App config flash opt failed",                "Flash配置数据操作失败"},
    {ERR_CFG_WILDCARD_QUE_FULL, "Wildcard queue has been full",               "通配路径队列已满"},
    {ERR_CFG_INVALID_PATH,      "Invalid path",                               "非法路径名"},
    {ERR_CFG_OBJ_INST_FULL,     "The object instance count has reached the max allowed count",    "对象实例个数已经达到允许的最大限制"},
    {ERR_CFG_REJECT_ATTR_SET,     "This node reject to set of active notication",    "此节点不能设置为主动上报"},

    {ERR_CMM_NOMID,            "Not support the opt to this path",           "当前不支持该路径进行操作"},

    {ERR_DGN_INVALID_STATE,    "invalid state (must be \"Requested\")",        "非法状态值(必须是\"Requested\")"},
    {ERR_DGN_INVALID_INTF,     "invalid intf (not exsited)",                 "非法接口(不存在)"},
    {ERR_DGN_INVALID_HOST_IP_PART, "invalid host (not support partly ip)",  "非法主机(当前不支持部分ip地址)"},
    {ERR_DGN_INVALID_HOST_IP_OCT,  "invalid host (not support oct format ip)", "非法主机(当前不支持八进制的ip地址)"},
    {ERR_DGN_INVALID_HOST_BEGIN,   "invalid host (must begin with number,char or \"_\")", "非法主机(主机名必须以数字字母或者\"_\"开始)"},
    {ERR_DGN_INVALID_TIMES,    "invalid Repeate times",                      "非法重复次数"},
    {ERR_DGN_INVALID_TO,       "invalid timeout",                            "非法超时时间"},
    {ERR_DGN_INVALID_SIZE,     "invalid data size",                          "非法数据包大小"},
    {ERR_DGN_INVALID_DSCP,     "invalid DSCP",                               "非法DSCP值"},
    {ERR_DGN_NOHOST,          "not set host",                               "当前还未设置主机"},
    {ERR_DGN_INVALID_HOST_A,   "invalid host (must not contain char except \".\", \"_\", \"-\", \"@\", a-z A-Z 0-9)",    "非法主机(主机名不能包含\".\", \"_\", \"-\", \"@\", 字母和数字之外的字符)"},
    {ERR_DGN_INVALID_HOST_LEN, "invalid host (too long, exceeding 256)",    "主机名太长(超过256个字符)"},

    /* AUTH */
    {ERR_AUTH_CANNOT_CHANGE_USER,   "cannot modify user name","不允许修改用户名","No puede modificar nombre del usuario"},
    {ERR_AUTH_CANNOT_CHANGE_LEVEL,  "cannot modify user name","不允许修改用户级别","No puede modificar el nivel de usuario"},
    {ERR_AUTH_CANNOT_REMOVE_ADMIN,  "cannot remove admin","不允许删除admin用户","No puede quitar el admin"},
    {ERR_AUTH_HAVE_SAME_USER,       "have same user","已有相同用户名","Tenga mismo usuario"},
    {ERR_AUTH_PASSWORD_TOO_LONG,    "password length must less than 64","密码长度必须小于64", "Longitud de la contrasena debe menos de 64"},
    {ERR_AUTH_USERNAME_TOO_LONG,    "user name length must less than 32","用户名长度必须小于32","La longitud del username debe menos de 32"},
    {ERR_AUTH_SESSION_FULL,         "Too many users log on, Please wait a minute","登录用户过多","Demasiados usuarios abren una sesion, satisfacen espera al minuto"},
    {ERR_AUTH_WRONG_USER,           "Username wrong","错误的用户名","Mal del username"},
    {ERR_AUTH_WRONG_PASSWORD,       "Password wrong","错误的密码","Contrasena incorrecta"},
    {ERR_AUTH_WRONG_USER_OR_PWD,    "Username or Password wrong","错误的用户名或密码","Contrasena incorrecta"},
    {ERR_AUTH_SESSION_TIMEOUT,      "Session is timeout","会话超时","Tiempo de inactividad excedido"},
    {ERR_AUTH_SESSION_ILLEGAL,      "Inadequate access","访问权限不足","Acceso inadecuado"},
    {ERR_AUTH_LOGIN_FREQUENT,       "Please login in one minute","请一分钟后再登录","Por favor conexion en un minuto"},
    {ERR_AUTH_LOGOUT_SUCCESS,       "Logout success","注销成功","Se ha desconectado del equipo"},
    {ERR_AUTH_OLDPASSWORD_INVALID,"Old password is wrong!","旧密码错误","La vieja contrasena es incorrecta"},
#ifdef CONFIG_APPS_LOGIC_FIRSTLOGIN_CHECK
    {ERR_AUTH_LOGIN_FIRSTUSE,    "first login!","第一次登录","Primera conexion"},
    {ERR_AUTH_WRONG_PASSWD,    "Please input a new password different from the default one","新密码不能与默认密码相同"},
#endif
    /* WAN */
    {ERR_WAN_INVALID_INNER_MSG, "inner msg error",                            "内部消息错误"},
    {ERR_WAN_INVALID_VAL,       "get invalid value",                          "获取值无效"},
    {ERR_WAN_INVALID_SVC_LIST,   "Invalid Service List",                          "非法的服务类型"},
    {ERR_WAN_INVALID_CONN_TYPE,  "Invalid Connection Type",                       "非法的连接类型"},
    {ERR_WAN_INVALID_LAN_INTF,   "Invalid Lan Intf",                              "非法的lan口绑定"},
    {ERR_WAN_LAN_INTF_REPEAT,     "The lan intf mutil bind",                       "lan口重复绑定"},
    {ERR_WAN_TOO_MORE_BRIDGE,    "Not allowed to Create more than one bridge on the same vlan", "一个wan vlan上不允许创建多个桥连接"},
    {ERR_WAN_BRIDGE_ROUTE_MIX,   "Not allowed to mix bridge and route on the same vlan",        "一个wan vlan上不允许创建桥和路由连接共存的连接的情况"},
    {ERR_WAN_TR069_BRIDGE,       "The TR069 service type is not allowed to config on a bridge conn",  "TR069服务类型的wan连接不能不是桥类型"},
    {ERR_WAN_TR069_LAN,          "Not allowed to bind lan to a TR069 wan conn",   "TR069服务类型的wan连接不允许绑定lan口"},
    {ERR_WAN_NOT_EXIST,          "please select a wan connection",   "请选择一条wan连接"},
    {ERR_WAN_INVALID_VLAN_ID,    "Invalid Vlan ID", ""},
    {ERR_WAN_NOREOA_LINK_EXIST,  "For one PVC, only one IPOA or PPPOA can be configured", ""},
    {ERR_WAN_EOA_LINK_EXIST,  "For one PVC, MER, PPPOE and Bridging are exclusive with IPOA and PPPOA", ""},
    {ERR_WAN_TOO_MANY_PVC,    "CPE can support up to 8 PVCs", ""},
    {ERR_WAN_CANNOT_CHNG_LINK_TYPE, "For one PVC, more than one MER, PPPOE or Bridging connetion exist, can not change protocol to IPOA or PPPOA", ""},
    {ERR_WAN_ETH_LINK_CONFLICT, "For one VLAN,only 1 IPOE, 1 Bridging and 4 PPPOE connections can be configured at the same time", "一个VLAN，只能同时配置一条桥和一条MER连接"},
    {ERR_WAN_DSL_LINK_CONFLICT, "For one (PVC,VLAN), only 1 MER, 1 Bridging and 4 PPPOE connections can be configured at the same time", "一个（PVC,VLAN），只能同时配置一条桥和一条MER连接"},
    {ERR_WAN_TOO_MANY_ENABLED_DSL_PPP, "CPE can support up to 4 enabled PPPOA and PPPOE connections", ""},
    {ERR_WAN_TOO_MANY_ENABLED_ETH_PPP, "CPE can support up to 4 enabled PPPOE connections", ""},
    {ERR_DSL_TOO_MANY_IP_INST, "For one (PVC,VLAN), only 1 MER, 1 Bridging connections can be configured at the same time", "一个（PVC,VLAN），只能同时配置一条桥和一条MER连接"},
    {ERR_ETH_TOO_MANY_IP_INST, "For one VLAN, only 1 IPOE, 1 Bridging connections can be configured at the same time", "一个VLAN，只能同时配置一条桥和一条MER连接"},
    
    /* LAN */
    {ERR_LAN_INVALID_IP_FORMAT, "Invalid IP format",             "非法IP地址格式"},
    {ERR_LAN_INVALID_LAN_IP, "Invalid LAN IP",             "非法的LAN IP地址"},
    {ERR_LAN_MACLIST_TOO_LONG, "Mac addreess list too long",             "mac地址列表过长或个数过多"},
    {ERR_LAN_INVALID_MACLIST, "Invalid Mac address",             "错误的mac地址"},
    {ERR_LAN_IP_MASK_CONFLICT, "Net Mask confilct with IP",             "IP地址和子网掩码冲突"},
    {ERR_LAN_IP_CONFLICT, "Repeated IP",             "IP地址与其它IP设置重复"},
    {ERR_LAN_LAST_INSTANCE, "This is first IP config,cannot delete",             "第一个IP设置不允许删除"},
    {ERR_LAN_TOO_MANY_INSTANCE, "Too many LAN IP config",             "LAN IP设置已经有达到上限，无法再添加"},
    {ERR_LAN_SAME_REGION_CONFLICT, "same net region with other lan device,will bring route error",             "LAN IP设置与其它LAN设置网段相同，会导致VLAN出错"},
    {ERR_LAN_NOT_EXIST, "please select a lan interface",             "请选择一条lan连接"},
    {ERR_LAN_INVALID_IPV6_FORMAT, "Invalid IPv6 address format", "非法IPv6地址格式"},

    /* DNS */
    {ERR_DNS_VALID_DNS_ADDR,        "invalid DNS address",            "DNS地址格式错误"},
    {ERR_DNS_HAS_REPEAT,            "DNS address repeat",             "DNS地址有重复"},
    {ERR_DNS_CUSTOM_BE_EMPTY,       "When allowed to custom DNS, DNS addresses can not be empty",
                                    "当允许自定义DNS时, DNS地址不能为空"},

    /* ETHLAN & ETHWAN */
    {ERR_ETH_INVALID_BITRATE,    "invalid max bit rate",     "非法比特率"},
    {ERR_ETH_INVALID_DUPLEXMODE, "invalid duplex mode",     "非法单双工模式"},

    /* LSVLAN */
    {ERR_LSVLAN_INVALID_BRIDGENAME,  "invalid bridge name",  "非法桥名称"},
    {ERR_LSVLAN_INVALID_VLANID,      "invalid Vlan ID",      "非法VLAN ID"},
    {ERR_LSVLAN_INVALID_FILTERBRREF, "invalid Filter Bridge Reference",  "非法的桥引用过滤规则"},
    {ERR_LSVLAN_INVALID_FILTERIF,    "invalid Filter Interface",  "非法接口引用过滤规则"},
    {ERR_LSVLAN_DEL_FIRST_BRIDGE,    "the first bridge should not be deleted",  "第一个桥不能删除"},
    {ERR_LSVLAN_INSTANCE_IN_USE,     "the instance which is in use should not be deleted", "正在使用实例不能删除"},
    {ERR_LSVLAN_BRNAME_CONFLICT,     "Bridge name conflict", "桥名称冲突"},
    {ERR_LSVLAN_BRVLAN_CONFLICT,     "Vlan id conflict", "VLAN ID冲突"},
    {ERR_LSVLAN_IF_CONFLICT,         "One interface must belong to only one LAN Group", "一个接口只能属于一个LAN Group"},
    {ERR_LSVLAN_WLAN_IF_TAGGED,      "VAP interface should not be tagged mode", "VAP接口不能是tagged模式"},

    /*LPVLAN*/
    {ERR_LPVLAN_INVALID_VLAN_ID,     "invalid Vlan ID", "非法VLAN ID"},
    {ERR_LPVLAN_INVALID_VLAN_ID_LIST,"invalid Vlan ID list", "非法VLAN ID列表"},
    {ERR_LPVLAN_TOO_MANY_VLAN,       "too many Vlan ID in list", "列表中VLAN ID过多"},
    

    /* IPCONN */
    {ERR_IPCONN_NAME_TOO_LONG,      "the length of connection name can not over 256",
                                    "连接名称长度不能超过256"},
    {ERR_IPCONN_NAME_INVALID,       "invalid connection name (only contains the following characters: [-_.@0-9a-zA-Z] ) ",
                                    "无效的连接名(只允许如下字符: [-_.@0-9a-zA-Z])"},
    {ERR_IPCONN_ADDR_TYPE_INVALID,  "invalid address type value",
                                    "无效的地址类型"},
    {ERR_IPCONN_ACTIVE_CONN_OVER,   "the number of active connections to the ceiling",
                                    "当前活动连接个数已到上限"},
    {ERR_IPCONN_WAN_CONN_OVER,      "the number of WAN connections to the ceiling",
                                    "当前WAN连接个数已到上限"},
    {ERR_IPCONN_WAN_IPCONN_OVER,    "Each WAN Device allow to create just a IP Connection",
                                    "一个WAN设备只允许建立一条IP连接"},
    {ERR_IPCONN_IP_GATEWAY_SAME,    "Static IP address and default gateway address can not be the same",
                                    "静态IP地址和默认网关地址不能一样"},
    {ERR_IPCONN_NAME_EXISTS,        "Connection Name has exists",
                                    "连接名不能重复"},
    {ERR_IPCONN_CONNTYPE_INVALID,   "invalid connection type",
                                    "无效的连接类型"},
    {ERR_PPPOE_ACNAME_INVALID,   "invalid AC name",
                                    "无效的AC名"},
    {ERR_PPPOE_SERVICENAME_INVALID,   "invalid service name",
                                    "无效的服务名"},
    {ERR_IPCONN_IP_GATEWAY_NOT_MATCH,    "Static IP address and default gateway address is not in the same network",
                                    "静态IP地址和默认网关地址不在同一网段"},

    /* DHCPS */
    {ERR_DHCPS_INVALID_CLASS_ID, "Unknown class id ", "未知客户类型"},
    {ERR_DHCPS_EXIST_CLASS_ID, "Already exist class id ", "已存在的客户类型"},
    {ERR_DHCPS_IFIP_ADDRPOOL_UNMATCH, "Interface address and dhcp address pool not match", "接口地址与地址池不匹配"},
    {ERR_DHCPS_LEASE_TIME, "Lease time must be more than or equal to -1", "租约时间必须大于或等于-1"},
    {ERR_DHCPS_CPEIP_CONFLICT_DHCP_POLL_IPADDR, "CPE IP conflicts with DHCP pool IP addr", "本机IP和DHCP地址池冲突"},

    /* PPPOE */
    {ERR_PPPOE_NO_RESOURSE, "no resourse",  "没有资源可用"},
    {ERR_PPPOE_INVALID_VAL,  "invalid value",  "无效值"},
    {ERR_PPPOE_INVALID_PATH,	"invalid path", "无效路径"},
    {ERR_PPPOE_EXCEED_CONNECTION_NUM,	"exceed connection number", "超出连接数"},
    {ERR_PPPOE_INVALID_INSTANCE,	"invalid instance", "无效实例"},
    {ERR_PPPOE_ENABLE_INVALID,	"invalid enable", "无效enable"},
    {ERR_PPPOE_USERNAME_INVALID,	"invalid  username", "无效用户名"},
    {ERR_PPPOE_USERNAME_OVERLENTH,	"username  over  length", "用户名超长"},
    {ERR_PPPOE_USERNAME_EMPTY,	"username empty", "用户名为空"},
    {ERR_PPPOE_PASSWD_INVALID,	"invalid passwd", "无效密码"},
    {ERR_PPPOE_PASSWD_OVERLENTH,	"passwd over length", "密码超长"},
    {ERR_PPPOE_PASSWD_EMPTY,	"passwd empty", "密码为空"},
    {ERR_PPPOE_CONNETNAME_INVALID,	"connected name invalid", "无效的连接名称"},
    {ERR_PPPOE_CONNETNAME_OVERLENTH,	"connected name over length", "连接名称超长"},
    {ERR_PPPOE_AUTH_INVALID,	"invalid auth", "无效的鉴权方式"},
    {ERR_PPPOE_MRU_INVALID,	"MRU invalid", "无效的MRU"},
    {ERR_PPPOE_TRIGGER_INVALID,	"invalid trigger", "无效的触发方式"},
    {ERR_PPPOE_CONNACTION_INVALID,	"connect button invalid", "手动触发方式的按钮无效"},
    {ERR_PPPOE_LCPECHO_INVALID,	"LCPECHO invalid, should be 5 ~ 65535", "无效的LCPECHO，应该为 5 ~ 65535"},
    {ERR_PPPOE_LCPECHO_RETRY_INVALID,	"LCPECHO retry invalid, should be 0 ~ 65535", "无效的LCPECHO重试次数，应该为 0 ~ 65535"},
    {ERR_PPPOE_ACTIVE_CONN_OVER,	"exceed active connect", "超出可用的连接数"},
    {ERR_PPPOE_INVALID_INNER_MSG,	"inner message invalid", "内部消息错误"},
    {ERR_PPPOE_INVALID_MATCH_INDEX,	"the index of message invalid", "消息索引错误"},
    {ERR_PPPOE_NO_PPPSESSION, "no pppsession in flash", "没有pppsession 存在flash里"},
    {ERR_PPPOE_NO_PPPSESSION_IDX, "invalid pppsession idx", "pppsession idx 错误"},
    {ERR_PPPOE_ERROR_PPPSESSION_VAL, "pppsession  value error", "pppsession 错误值"},
    {ERR_PPPOE_NAME_REPEATED,  "WAN connected name repeated",  "WAN连接名称重复"},
    {ERR_PPPOE_IDLEDISCONNTIME_INVALID,  "invalid idle disconnect time",  "无效的闲置超时时间"},
    {ERR_PPPOE_APN_INVALID,                 "invalid apn",  "无效的APN"},
    {ERR_PPPOE_APN_EMPTY,	                "apn empty",  "APN为空"},
    {ERR_PPPOE_APN_OVERLENTH,	            "apn over  length",  "APN超长"},
    {ERR_PPPOE_DIALNUMBER_INVALID,          "invalid dialnumber",  "无效的拨号号码"},
    {ERR_PPPOE_DIALNUMBER_EMPTY,            "dialnumber empty",  "拨号号码为空"},
    {ERR_PPPOE_DIALNUMBER_OVERLENTH,	    "dialnumber over length",  "拨号号码超长"},
    {ERR_PPPOE_MTU_INVALID,	                "MTU invalid", "无效的MTU"},
    {ERR_PPPOE_TOO_MANY_PPPOU_INST,         "CPE can support up to 1 PPPoU connections", "CPE只允许一条3G连接"},
    {ERR_PPPOE_IDLEDELAYTIME_INVALID,	    "Backup delay time invalid, should be 0~600", "无效的备份延迟时间，应该为0~600，并且要能被15整除"},
    {ERR_PPPOE_IDLEDELAYTIME_EMPTY,	        "Backup delay time empty", "备份延迟时间为空"},
    {ERR_PPPOE_IDLEDELAYTIME_DIVIDE,	    "Backup delay time  must be divisible by 15", "备份延迟时间必须能被15整除"},

    /* QOS */
#ifdef CONFIG_APPS_LOGIC_QOS_IMQ    
    {ERR_QOS_INVALID_COMMITTED_RATE,    "Invalid Bandwidth value, should be 100-102400Kbps",
                                        "无效的带宽值,应该为100-102400Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "Invalid peak rate value, should be 100-102400Kbps",
                                        "无效的峰值速率值,应该为100-102400Kbps"},
    {ERR_QOS_INVALID_BURST_SIZE,        "Invalid burst size, should be 5-100bytes", "无效的突发数据量,应该为5-100bytes"},
    {ERR_QOS_INVALID_QUE_LEN,           "Queue length should be 10-150packets or 15000-225000bytes",
                                        "队列长度应该为10-150packets或15000-225000bytes"},
    {ERR_QOS_INVALID_QUE_WEIGHT,        "Invalid queue weight value,should be 0-100", "无效的队列权重值,应该为0-100"},
    {ERR_QOS_INVALID_QUE_CAR,           "Invalid queue bandwidth value, should be 0-102400Kbps", "无效的队列带宽值,应该为0-102400Kbps"},
    {ERR_QOS_INVALID_CLASS_IF,          "Invalid ingress interface", "无效的输入接口"},
    {ERR_QOS_INVALID_VLAN_ID,           "Invalid vlan id, shoud be 1-4094", "无效的VLAN ID,应该为1-4094"},
    {ERR_QOS_INVALID_IP_LEN,            "Invalid ip length value,should be 20-1500", "无效的IP数据长度,应该为20-1500"},
    {ERR_QOS_INVALID_CLASS_QUE,         "Invalid index of queue instance", "无效的队列实例索引"},
    {ERR_QOS_QUEUE_NOT_EXIST,           "The queue instance does not exist", "队列实例不存在"},
    {ERR_QOS_QUEUE_CAR_CONFLICT,        "The bandwidth sum of all enabled queues shouled be less than or equal to the global bandwidth value",
                                        "所有已启用队列的带宽值之和应该小于或等于总带宽值"},
    {ERR_QOS_SHAPE_RATE_CONFLICT,       "Committed rate should be less than or equal to peak rate", "承诺速率应该小于等于峰值速率"},
    {ERR_QOS_QUEUE_CAR_NOLIMIT_CONFLICT,"The bandwidth sum of all enabled queues shouled be less than or equal to peak rate",
                                        "所有已启用队列的带宽值之和应该小于或等于峰值速率"},
    {ERR_QOS_QUEUE_LEN_CONFLICT,        "Queue length should be 20-150packets or 30000-225000bytes",
                                        "队列长度应该为20-150packets或30000-225000bytes"},
    {ERR_QOS_QUEUE_WEIGHT_CONFLICT,     "The weight sum of all enabled queues should be less than or equal to 100%",
                                        "所有已启用队列的权重之和应该小于或等于100%"},
    {ERR_QOS_QUEUE_PRIO_CONFLICT,       "Two enabled queues binding the same SP interface should not have the same priority",
                                        "两个绑定到同一SP接口的已启用队列的优先级不能相同"},
    {ERR_QOS_IP_LENGTH_CONFLICT,        "The minimum ip length should be less than or equal to the maximum ip length",
                                        "IP长度最小值应该小于或等于IP长度最大值"},
    {ERR_QOS_PORT_CONFLICT,             "The minimum port value should be less than or equal to the maximum port value",
                                        "端口最小值应该小于或等于端口最大值"},
    {ERR_QOS_APP_NAME_CONFLICT,         "The configuration with same app name has existed", "具有相同业务名称的配置已经存在了"},
    {ERR_QOS_INVALID_IP_ADDR,           "", "IP地址格式不正确"},
    {ERR_QOS_INVALID_PREFIX_LEN,        "", "无效的IPv6地址前缀长度,应该为0-128"},
    {ERR_QOS_SRCIP_CONFLICT,            "", "源IP地址格式不正确"},
    {ERR_QOS_DSTIP_CONFLICT,            "", "目的IP地址格式不正确"},
    {ERR_QOS_INVALID_FLOW_LABEL,        "", "无效的Flow Label值,应该为0-1048575"},
    {ERR_QOS_INVALID_TRAFFIC_CLASS,     "", "无效的Traffic Class值,应该为0-255"},
#else
    {ERR_QOS_INVALID_IF_PATH,           "Invalid egress interface" , "无效的输出接口"},
#if 0
    {ERR_QOS_INVALID_COMMITTED_RATE,    "To ethernet interface,rate value should be 100-100000Kbps and to VAP interface it should be 100-30000Kbps",
                                        "对于以太网接口,速率值应该为100-100000Kbps,对于VAP接口应该为100-30000Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "To ethernet interface,rate value should be 100-100000Kbps and to VAP interface it should be 100-30000Kbps",
                                        "对于以太网接口,速率值应该为100-100000Kbps,对于VAP接口应该为100-30000Kbps"},
    {ERR_QOS_INVALID_COMMITTED_RATE,    "Invalid committed rate value, should be 100-100000Kbps",
                                        "无效的承诺速率值，应该为100-100000Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "Invalid peak rate value, should be 100-100000Kbps",
                                        "无效的峰值速率值，应该为100-100000Kbps"},
    {ERR_QOS_INVALID_BURST_SIZE,        "Invalid burst size, should be 2-100", "无效的突发数据量,应该为2-100"},
#endif    
    {ERR_QOS_INVALID_QUE_SCH_ALGORITHM, "Invalid queue scheduler Algorithm, should be DWRR or SP",
                                        "无效的队列调度方式，应该为DWRR或SP"},
    {ERR_QOS_INVALID_QUE_IF,            "Invalid index of interface instance", "无效的接口实例索引"},
    {ERR_QOS_QUE_IF_NOT_EXIST,          "The interface instance does not exist", "接口实例不存在"},
    {ERR_QOS_INVALID_QUE_LEN,           "Queue length should be 20-150packets or 30000-225000bytes",
                                        "队列长度应该为20-150packets或30000-225000bytes"},
    {ERR_QOS_INVALID_QUE_LEN_TYPE,      "Invalid queue length type, should be 0(packets) or 1(bytes)",
                                        "无效的队列长度类型，应该为0(packets)或1(bytes)"},
    {ERR_QOS_INVALID_QUE_WEIGHT,        "Invalid queue weight value,should be 1-99", "无效的队列权重值,应该为1-99"},
    {ERR_QOS_INVALID_DROP_ALGORITHM,    "Invalid drop algorithm, should be DT(drop tail)", "无效的丢弃算法,应该为DT(尾丢弃)"},
    {ERR_QOS_INVALID_CLASS_TYPE,        "Invalid classify type, should be 0(L2) or 1(L3)", "无效的分类类型,应该为0(L2)或1(L3)"},
    {ERR_QOS_INVALID_CLASS_IF,          "Invalid ingress interface", "无效的输入接口"},
    {ERR_QOS_INVALID_ETH_TYPE,          "Invalid ethertype field valid", "无效的以太网类型字段值"},
    {ERR_QOS_INVALID_ETH_PRIO,          "Invalid ethernet priority value, shoud be 0-7", "无效的以太网优先级,应该为0-7"},
    {ERR_QOS_INVALID_VLAN_ID,           "Invalid vlan id, shoud be 1-4094", "无效的VLAN ID,应该为1-4094"},
    {ERR_QOS_INVALID_IP_LEN,            "Invalid ip length value,should be 20-1500", "无效的IP数据长度,应该为20-1500"},
    {ERR_QOS_INVALID_DSCP,              "Invalid DSCP value, should be PHB format", "无效的DSCP值,应该为PHB格式"},
    {ERR_QOS_INVALID_PROTOCOL,          "Invalid protocol value, should be 0(tcp&udp),1(icmp),6(tcp),17(udp)",
                                        "无效的协议值，应该为0(tcp&udp), 1(icmp), 6(tcp), 17(udp)"},
    {ERR_QOS_INVALID_CLASS_QUE,         "Invalid index of queue instance", "无效的队列实例索引"},
    {ERR_QOS_CLASS_QUE_NOT_EXIST,       "The queue instance does not exist", "队列实例不存在"},
    {ERR_QOS_SHAPE_RATE_CONFLICT,       "Committed rate should be less than or equal to peak rate", "承诺速率应该小于等于峰值速率"},
    {ERR_QOS_QUEUE_LEN_CONFLICT,        "Queue length should be 20-150packets or 30000-225000bytes",
                                        "队列长度应该为20-150packets或30000-225000bytes"},
    {ERR_QOS_QUEUE_WEIGHT_CONFLICT,     "The weight sum of all enabled queue binding the same DWRR interface should be less than or equal to 100%",
                                        "绑定到同一个DWRR接口的所有已启用队列的权重和应该小于或等于100%"},
    {ERR_QOS_QUEUE_PRIO_CONFLICT,       "Two enabled queues binding the same SP interface should not have the same priority",
                                        "两个绑定到同一SP接口的已启用队列的优先级不能相同"},
    {ERR_QOS_IP_LENGTH_CONFLICT,        "The minimum ip length should be less than or equal to the maximum ip length",
                                        "IP长度最小值应该小于或等于IP长度最大值"},
    {ERR_QOS_PORT_CONFLICT,             "The minimum port value should be less than or equal to the maximum port value",
                                        "端口最小值应该小于或等于端口最大值"},
#endif
#if 0
    /* DYNAMIC ROUTE */
    {ERR_DYNAMIC_INVALID_VAL, "value invalid",  "值无效"},
    {ERR_DYNAMIC_INVALID_CONFIG_FILE, "invalid configurate file",  "配置文件无效"},
    {ERR_DYNAMIC_VERSION_INVALID, "RIP version invalid",  "无效的rip版本"},
    {ERR_DYNAMIC_INVALID_PC_MSG, "PC inner message invalid",  "无效的PC内部消息"},
    {ERR_DYNAMIC_INVALID_WAN, "WAN device invalid",  "WAN设备无效"},
    {ERR_DYNAMIC_INVALID_INNER_PARA,  "invalid inner parament ",  "无效的内部参数"},
#endif

    /* DYNAMIC ROUTE */
    {ERR_DROUTE_INVALID_VAL, "value invalid",  "值无效"},
    {ERR_DROUTE_INVALID_CONFIG_FILE, "invalid configurate file",  "配置文件无效"},
    {ERR_DROUTE_VERSION_INVALID, "RIP version invalid",  "无效的rip版本"},
    {ERR_DROUTE_INVALID_PC_MSG, "PC inner message invalid",  "无效的PC内部消息"},
    {ERR_DROUTE_INVALID_WAN, "WAN device invalid",  "WAN设备无效"},
    {ERR_DROUTE_INVALID_INNER_PARA,  "invalid inner parament ",  "无效的内部参数"},

    {ERR_STATIC_INVALID_INSTANCE      , "invalid instance",  "无效实例"},
    {ERR_STATIC_INTERFACE             , "invalid interface ",  "无效的连接设备"},
    {ERR_STATIC_NO_RESOURSE           , "has no resourse",  "没有资源"},
    {ERR_STATIC_INVALID_VAL           , "value invalid",  "值无效"},
    {ERR_STATIC_ENABLE_INVALID        , "enable invalid",  "使能参数无效"},
    {ERR_STATIC_INVALID_PATH          , "invalid path",  "无效路径"},
    {ERR_STATIC_INVALID_DEFAULT_PARA  , "invalid default route parament",  "无效默认路由参数"},
    {ERR_STATIC_INVALID_ROUTE_PARA    , "invalid route para",  "无效路由参数"},
    {ERR_STATIC_INVALID_PARA          , "invalid parament",  "无效参数"},
    {ERR_STATIC_DEL_INSTANCE          , "del instance error",  "删除实例错误"},
    {ERR_STATIC_INVALID_METRIC        , "invalid metric",  "无效metric"},
    {ERR_STATIC_INVALID_IP            , "invalid IP",  "无效IP"},
    {ERR_STATIC_IP_EMPTY              , "IP empty",  "IP 地址空"},
    {ERR_STATIC_INVALID_MASK          , "invalid netmask",  "无效子网码"},
    {ERR_STATIC_MASK_EMPTY            , "netmask empty",  "子网码空"},
    {ERR_STATIC_INVALID_GATEWAY       , "invalid gateway",  "无效网关"},
    {ERR_STATIC_METRIC_OUT_LIMITED    , "metric is out of limited number 0~128",  "metric参数超过范围0~128"},
    {ERR_STATIC_INVALID_LOGFILE       , "invalid log file",  "无效的log文件"},

   /* V6 Static routing */
//    {ERR_V6STATIC_ENABLE_INVALID        , "enable invalid",  "使能参数无效"},
//    {ERR_V6STATIC_BAD_PREFIX        , "Invalid Prefix! Valid range is 3-128",  "Prefix不正确，正确的取值范围是3-128"},    
//    {ERR_V6STATIC_BAD_METRIC        , "Invalid Metric!",  "无效的metric参数"},  

    /* SNMPC */
    {ERR_SNMPC_INVALID_VERSION,         "invalid SNMP version", "无效的SNMP版本号"},

    /* Firewall */
    {ERR_FW_RULES_OVER,                 "The number of rules can not be over 20",
                                        "允许添加的规则不能超过20条"},
    {ERR_FW_RULE_EXISTS,                "this rule has exists in filter table",
                                        "已有重复的规则存在"},
    {ERR_FW_SRC_IP_EMPTY,                "Source IP Address can not be empty !",
                                        "源IP地址池不能为空"},
    {ERR_FW_DEST_IP_EMPTY,                "Destination IP Address can not be empty !",
                                        "目的IP地址池不能为空"},

    /* SNTP */
    {ERR_SNTP_INVALID_UP_INTERVAL,      "Invalid update interval value, shoud be 1-24(hours)",
                                        "无效的更新时间间隔值,应该为1-24(小时)"},
    {ERR_SNTP_INVALID_RETRY_INTERVAL,   "Invalid retry interval value, should be 1-60(minutes)",
                                        "无效的重试时间间隔,应该为1-60(分钟)"},
    {ERR_SNTP_INVALID_NTP_SERVER,       "The Ip address or domain name of NTP server is invalid",
                                        "NTP服务器的IP地址或域名是无效的"},
    {ERR_SNTP_INVALID_DATA_TIME,        "Invalid time format",   "时间格式不合法"},
    {ERR_SNTP_NULL_NTPSERVER_STR,       "Two NTP servers should not be null at the same time",
                                        "两个NTP服务器不能同时为空"},
    {ERR_SNTP_NTPSERVERS_CONFLICT,      "Two NTP servers should not be same value",
                                        "两个NTP服务器不能相同"},
    {ERR_SNTP_DST_CONFLICT,             "The conflict between the start and the end of daylight saving time exist",
                                        "夏令时开始和结束时间之间存在冲突"},

    /*tr069be*/
    {ERR_TR069BE_INVALID_URL,  "invalid URL", "非法的URL"},
    {ERR_TR069BE_INVALID_UPLOAD_URL, "unable to connect to the server", "连接服务器失败"},
    {ERR_TR069BE_INVALID_DOWNLOAD_URL, "unable to connect to the server", "连接服务器失败"},
    {ERR_USERNAME,  "invalid username", "非法的用户名"},
    {ERR_PASSWORD,  "invalid password", "非法的密码"},
    {ERROR_TR069_INVALID_CAFILE, "Invalid CAfile", "非法的证书"},

    {ERR_URLFILTER_INVALID_INNER_PARA  ,  "inner parament error",   "内部参数传递错误"},
    {ERR_URLFILTER_INVALID_VAL         ,  "invalid value",   "值无效"},
    {ERR_URLFILTER_ENABLE_EMPTY        ,  "value enable empty",   "值enable为空"},
    {ERR_URLFILTER_ENABLE_INVALID      ,  "value enable wrong",   "值enable错误"},
    {ERR_URLFILTER_INVALID_CONFIG_FILE ,  "invalid config file",   "无效配置文件"},
    {ERR_URLFILTER_MODE_EMPTY          ,  "value mode empty",   "值模式为空"},
    {ERR_URLFILTER_MODE_INVALID        ,  "value mode wrong",   "值模式错误"},
    {ERR_URLFILTER_URL_CONFLICT        ,  "URL conflict",   "URL 冲突"},
    {ERR_URLFILTER_URLNUMBER_OVER      ,  "URL Number is over", "创建的url数目超出范围"},
    {ERR_URLFILTER_URL_LENGTH_OVER     ,  "URL length is over", "URL过长"},
    {ERR_URLFILTER_URL_ERROR           ,  "URL value is error",  "URL值错误"},
    {ERR_URLFILTER_INVALID_FILTERMODE           ,  "Invalid filter mode",  "无效的URL过滤模式"},
    {ERR_URLFILTER_INVALID_TIME           ,  "Invalid time",  "无效时间"},
    {ERR_URLFILTER_INVALID_DAY           ,  "Invalid day",  "无效的日期"},
    {ERR_URLFILTER_TIME_CONFLICT           ,  "Start time is larger then end time",  "开始时间大于结束时间"},
    {ERR_URLFILTER_MAC_CONFLICT           ,  "MAC is exist already",  "MAC地址已经存在"},

    /* IPT */
    {ERR_IPT_CONFLICT_DMZ,            "DMZ host ip should not be null", "DMZ主机地址不可以为空"},

    /* WLAN */
    {ERR_WLAN_NO_RESOURSE,            "no resourse", "资源缺乏"},
    {ERR_WLAN_INVALID_VAL,            "invalid value",   "值无效"},
    {ERR_WLAN_INVALID_PATH,            "invalid path",  "无效路径"},
    {ERR_WLAN_VAL_EMPTY,            "value empty",   "值为空"},
    {ERR_WLAN_EXCEED_VAP_NUM,             "The number of Vap can not exceed 5",
                                                                "允许添加的VAP不能超过5个"},
    {ERR_WLAN_INVALID_INSTANCE,            "invalid instance", "无效实例"},
    {ERR_WLAN_INVALID_VAPIDX,            "invalid Vap index", "无效的Vap序号"},
    {ERR_WLAN_NAME_REPEATED,            "name repeated", "名重复"},
    {ERR_WLAN_INVALID_SESSION,   "wps session has already built", "已经启动了WPS协商"},
	{ERR_WLAN_IN_OOB,   "It is already in OOB status", "已经处于OOB状态"},
	{ERR_WLAN_11N_TIKP_ERROR,   "802.11n mode and TIKP encryption can not be configurated together!", "11n 模式不能和TKIP加密模式共存!"},

    /* PORTMAPPING */
    {ERR_PTMP_INVALID_DESCRIPTION,    "Invalid characters inclued in description", "描述信息中含有非法字符"},
    {ERR_PTMP_CONFLICT_PORTRANGE,     "The minimum port value should be less than or equal to the maximum port value",
                                      "端口最小值应该小于或等于端口最大值"},
    {ERR_PTMP_CONFLICT_TUPLE,         "Two instances conflict for the conflicting tuple composed of RemoteHost, ExternalPort and Protocol ",
                                      "两个配置实例的三元组(远端主机, 外部端口和协议)发生冲突"},
	{ERR_PTMP_CONFLICT_PORTRANGE_GENERAL,"Port have been used by ACL,Please change other port",
	                                  "端口已经被服务控制列表占用,请使用其它端口!"},
	{ERR_PTMP_HOST_NAME_NOT_EXIST,    "The Host Name does not exist, please check the DHCP client table!",
	                                  "主机名不存在，请检查DHCP客户端列表!"},


	/* USB3G */
	{ERR_USB3G_INVALID_PIN_LENGTH,                "The length of PIN code must be 4",
                                        "PIN码的长度必须为4"},
    {ERR_USB3G_INVALID_PUK_LENGTH,                "The length of PUK code must be 8",
                                        "PUK码的长度必须为8"},                                    
	{ERR_USB3G_INVALID_PINPUKCODE,            "The PIN/PUK code must be decimal",
                                        "PIN/PUK码必须由十进制数字组成"},

    /* ACL */
    {ERR_ACL_RULES_OVER,                "The number of rules can not be over 10",
                                        "允许添加的服务不能超过10条"},
    {ERR_ACL_RULE_EXISTS,               "This open port has been occupied",
                                        "此开放端口已经被占用"},
    {ERR_ACL_PORT_BE_ZERO,              "When protocol is ICMP, the port value must be zero",
                                        "当协议为ICMP时,端口值必须为0"},
    {ERR_ACL_PORT_CANOT_ZERO,           "When protocol is not ICMP, The port value can not be zero",
                                        "当协议不为ICMP时,端口值不能为0"},
    {ERR_ACL_PORT_IN_USE,               "This port has been used",
                                        "此端口已经被使用"},

    /* LOGGER */
    {ERR_LOGGER_UPLOAD_FAILED,          "Log file upload failed",  "日志文件上传失败"},
    {ERR_LOGGER_INVALID_TFTP_SERVER,    "Invalid TFTP server",  "非法的TFTP服务器"},
    {ERR_LOGGER_CLEAR_FAILED,           "Log file clear failed",  "日志文件清除失败"},
    {ERR_LOGGER_EMPTY_TFTP_SERVER,      "Server address cannot be empty","服务器地址不能为空"},
    {ERR_LOGGER_INVALID_SERVER,         "Invalid server","非法的服务器"},

    /* ATM */
    {ERR_ATM_PVC,                       "VPI/VCI is invalid", "PVC配置不合法，请以VPI/VCI的格式填写"},
    {ERR_ATM_VPI,                       "Invalid VPI value (0~255)", "VPI值非法(0~255)"},
    {ERR_ATM_VCI,                       "Invalid VCI value (32~65535)", "VCI值非法(32~65535)"},
    {ERR_ATM_UNKNOWN_ENCAP,             "Unkown encapsulation", "未知封装类型"},
    {ERR_ATM_UNKNOWN_QOS_TYPE,          "Unkown Qos type", "未知Qos类型"},
    {ERR_ATM_PCR,                       "Invalid PCR value (0~7100)", "峰值信元速率/信元延迟变化率非法(0~7100)"},
    {ERR_ATM_MBS,                       "Invalid MBR value (0~1000000)", "最大突发尺寸非法(0~1000000)"},
    {ERR_ATM_SCR,                       "Invalid SCR value (0~7099)", "平均信元速率非法"},
    {ERR_ATM_MIN_PCR,                   "Wrong MCR value (0~pcr)", "最小信元速率非法"},
    {ERR_ATM_VLAN_ID,                   "Wrong vlan id (0~4095)", "VLAN ID非法(0~4095)"},
    {ERR_ATM_VLAN_PRIORITY,             "Wrong vlan priority(0~7)", "VLAN优先级非法(0~7)"},
    {ERR_ATM_VLAN_ID_CONFLICT,          "Same vlan id", "VLAN ID不能重复"},

    /* UPNP */
    {ERR_UPNP_SRCIP_EXIST,              "This IP address is already in the list","该IP地址已在列表中"},

    /* DDNS */
    {ERR_DDNS_INVALID_DDNSPROVIDER,     "Invalid DDNS provider","无效的动态DNS服务商"},
    {ERR_DDNS_INVALID_HOSTNAME,         "Invalid hostname","无效的注册域名"},
    {ERR_DDNS_HOSTNAME_LENGTH_OVER,     "Hostname length is over","注册域名超长"},
    {ERR_DDNS_INVALID_WAN_CONN_SID,     "Invalid WAN connection ID","无效的WAN连接"},
    {ERR_DDNS_INVALID_USERNAME,         "Invalid username","无效的用户名"},
    {ERR_DDNS_USERNAME_LENGTH_OVER,     "Username length is over","用户名超长"},
    {ERR_DDNS_INVALID_PASSWORD,         "Invalid password","无效的密码"},
    {ERR_DDNS_PASSWORD_LENGTH_OVER,     "Password length is over","密码超长"},
    {ERR_DDNS_INVALID_EMAIL,            "Invalid email address","无效的邮箱地址"},
    {ERR_DDNS_EMAIL_LENGTH_OVER,        "Email address length is over","邮箱地址超长"},
    {ERR_DDNS_INVALID_KEY,              "Invalid key","无效的密钥"},
    {ERR_DDNS_KEY_LENGTH_OVER,          "Key length is over","密钥超长"},
    {ERR_DDNS_HOSTNAME_CONFLICT,        "Hostname exist already","注册域名已经存在"},
    {ERR_DDNS_INVALID_DDNS_HOST,         "Invalid DDNS host","无效的DDNS 服务器"},
    {ERR_DDNS_DDNS_HOST_LENGTH_OVER,     "DDNS host length is over","DDNS服务器主机标识超长"},
    {ERR_DDNS_PRIVODER_CONFLICT,        "www.oray.cn exist already","www.oray.cn已经存在"},

    /* SUPP */
    {ERR_SUPP_USERNAME_OR_PASSWORD_BE_EMPTY, "username or password empty", "用户名或密码为空"},

    /* MACFILTER */
    {ERR_MACFILTER_INVALID_USERNAME,  "Invalid username","无效用户名"},
    {ERR_MACFILTER_INVALID_FILTERMODE,  "Invalid filter mode","无效的过滤模式"},
    {ERR_MACFILTER_INVALID_TIME,        "Invalid time","无效的时间"},
    {ERR_MACFILTER_INVALID_DAY,         "Invalid day","无效的日期"},
    {ERR_MACFILTER_TIME_CONFLICT,       "Start time is larger then end time","开始时间大于结束时间"},
    {ERR_MACFILTER_MAC_CONFLICT,        "MAC is exist already","MAC地址已经存在"},
    {ERR_MACFILTER_INVALID_INNER_PARA,  "Invalid inner parameter","无效内部参数"},

    /*RADVD*/
    {ERR_HEX_FORMAT_WRONG,          "invalid hex number","不是有效的十六进制数"},
    {ERR_GLOBALID_OUT_OF_RANGE, "invalid global id,must be 40bit hex number","global id必须是十六进制数"},
    {ERR_CONFIG_TYPE_INVALID_VALUE, "invalid config type,must be Static or Delegated","配置类型必须是 Static或Delegated"},
    {ERR_ROUTE_LIFETIME_OUT_OF_RANGE, "lifetime range is 0~65535","生命周期为0~65535"},
    {ERR_ROUTE_PREFERENCE_INVALID_VALUE, "route preference must be low,medium or high","优先级必须是设置为low,medium or high"},
    {ERR_SUBNET_ID_OUT_OF_RANGE, "SubnetID range is 0~255","子网范围为0~255"},
    {ERR_INVALID_PREFIX_LEN, "Invalid prefix length, it should be 0~128","无效的地址前缀长度，范围为0~128"},        
    {ERR_INVALID_PREFIX, "Invalid prefix","无效的地址前缀"},   


    /* TRACEROUT */
    {ERR_TRACERT_NOSTART,               "Traceroute is not started","进程未启动"},
    {ERR_TRACERT_BUSY,                  "Traceroute is running","进程正在运行"},

    /* URLFILTER */
    {ERR_URLFILTER_INVALID_FILTERMODE,  "Invalid filter mode","无效的过滤模式"},
    {ERR_URLFILTER_INVALID_TIME,        "Invalid time","无效的时间"},
    {ERR_URLFILTER_INVALID_DAY,         "Invalid day","无效的日期"},
    {ERR_URLFILTER_TIME_CONFLICT,       "Start time is larger than end time","开始时间大于结束时间"},
    {ERR_URLFILTER_INVALID_INNER_PARA,  "Invalid inner parameter","无效内部参数"},

    /* SCHEDULE */
    {ERR_SCHEDULE_INVALID_NAME,             "Name is invalid.", "名字不能为空且不能包含中文及特殊字符"},
    {ERR_SCHEDULE_INVALID_ENABLEALLDAYS,    "Day select is invalid.","日期选择不合法"},
    {ERR_SCHEDULE_INVALID_STARTTIME,         "Invalid time,must be 00:00-23:59","时间不合法，必须是00:00-23:59"},
    {ERR_SCHEDULE_INVALID_ENDTIME,          "End time is invalid.","结束时间不合法"},
    {ERR_SCHEDULE_INVALID_SPICIFIEDDAY,     "No days are specified.","未指定日期"},
    {ERR_SCHEDULE_INVALID_SCHEDULEDAYS,     "Day select is invalid.","日期选择不合法"},
    {ERR_SCHEDULE_INVALID_ENABLEALLTIME,    "Start time is invalid.","开始时间不合法"},
    {ERR_SCHEDULE_TIME_CONFLICT,    "Start time is larger than End time.","开始时间大于结束时间"},
	{ERR_SCHEDULE_TIME_DISABLED,    "Before you create rules, please enable NTP service", "建立规则之前，请先启用NTP服务"},

    /* IGMP */
    {ERR_IGMP_INVALID_VERSION,              "IGMP version is invalid.","IGMP版本不合法"},

    /* Diagnostics */
    {ERR_DG_INVALID_STATE,              "Diagnostics state is invalid.",    "诊断状态值不合法"},
    {ERR_DG_INVALID_INTERFACE,          "Interface is invalid.",            "接口不合法"},
    {ERR_DG_PVC_TOO_MUCH,               "Too much PVC pairs in search list.", "搜索列表中PVC对数太多"},


    //default gateway
    {ERR_DEGW_EMPTY_IP,              "IP address is empty","IP地址为空"},
    {ERR_DEGW_INVALID_IP,              "IP address is invalid","IP地址无效"},
    {ERR_DEGW_EMPTY_USERINIF,              "User interface is empty","用户接口为空"},
    {ERR_DEGW_INVALID_USERINIF,              "User interface is invalid","用户接口无效"},
    {ERR_DEGW_EMPTY_SELECT,              "Mode is empty","模式为空"},
    {ERR_DEGW_INVALID_SELECT,              "Mode must be one of AutoDeftGW,GWIP andUserInIf","模式必须为AutoDeftGW,GWIP或UserInIf"},

    //IP ACL
    {ERR_IP_ACL_EMPTY_IP,              "IP address is empty","IP地址为空"},
    {ERR_IP_ACL_INVALID_IP,              "IP address is invalid","IP地址无效"},
    {ERR_IP_ACL_IP_ZERO,              "You can use this function unless there is a IP at least","最少有一条记录才能使用这个功能"},

    /* IPTUNNEL */
    {ERR_IPTUNNEL_NO_RESOURSE,                "no resourse.", "没有资源可用"},
    {ERR_IPTUNNEL_VAL_EMPTY,                  "value empty.", "值为空"},
    {ERR_IPTUNNEL_NAME_OVERLENGTH,            "name over maxlength.", "值超过最大长度"},
    {ERR_IPTUNNEL_VALID_STRING,               "name is valid string.", "值是无效字符串"},
    {ERR_IPTUNNEL_EXIST_NAME,                 "Already exist tunnel name.", "已存在的名称"},
    {ERR_IPTUNNEL_EXIST_WAN_IF,               "Wan connection has been used.", "已使用的Wan连接"},

    /* GRE */
    {ERR_GRE_NO_RESOURSE,                "no resourse.", "没有资源可用"},
    {ERR_GRE_INVALID_MASK_LENGTH,        "Pritvate Tunnel Mask Len should be 1~32", "通道掩码大小需为1～32"},
    {ERR_GRE_INVALID_IPV4_ADDRESS,            "invalid ipv4 address.", "非法ipv4地址"},
    {ERR_GRE_VALID_STRING,               "name is valid string.", "值是无效字符串"},
    {ERR_GRE_EXIST_NAME,                 "Already exist tunnel name.", "已存在的名称"},
    {ERR_GRE_VAL_EMPTY,                  "value empty.", "值为空"},
    {ERR_GRE_NAME_OVERLENGTH,            "name over maxlength.", "值超过最大长度"},
	{ERR_GRE_WAN_IP_CONFLICT,            "Tunnel already exists with the same remote IP and WAN interface", "已存在隧道有相同的远端IP和WAN接口"},

    /* v6sroute */
    {ERR_V6SROUTE_BAD_ADDR,                   "Invalid IP ! Please input a right IPv6 address!", "v6地址错误，请填入正确的v6地址"},
    {ERR_V6SROUTE_REP_ADDR,                   "Ipv6 address conflicts.", "v6地址冲突"},
    
	/* DHCPv6 */
	{ERR_DHCP6S_RANGE_ERROR, "IPv6 Interface ID range error", "IPv6接口ID范围错误"},

    /* IPSEC*/
    {ERR_IPSEC_INVALID_NAME,                   "Name is invalid.", "名字不合法"},
    {ERR_IPSEC_INVALID_TUNNELMODE,             "Tunnel Mode is invalid.", "通道模式不合法"},
    {ERR_IPSEC_INVALID_IP,                     "IP is invalid.", "IP地址不合法"},
    {ERR_IPSEC_IP_EMPTY,                       "IP is empty.", "IP地址为空"},
    {ERR_IPSEC_INVALID_IPMASK,                     "IP mask is invalid.", "子网掩码不合法"},
    {ERR_IPSEC_IPMASK_EMPTY,                       "IP mask is empty.", "子网掩码为空"},
    {ERR_IPSEC_IP_MASK_CONFLICT,               "IP address and netmask conflict",  "IP地址与子网掩码冲突"},
    {ERR_IPSEC_INVALID_KEYEXCHMETHOD,          "Key Exchange Method is invalid.", "密钥扩展模式不合法"},
    {ERR_IPSEC_INVALID_SHARDKEY,               "Shard key is invalid.", "共享密钥不合法"},
    {ERR_IPSEC_INVALID_MODE,             "Mode is invalid.", "模式不合法"},
    {ERR_IPSEC_INVALID_ENCRYALG,         "Encryption Algorithm is invalid.", "加密算法不合法"},
    {ERR_IPSEC_INVALID_INTEGALG,         "Integrity Alogorithm is invalid.", "验证算法不合法"},
    {ERR_IPSEC_INVALID_DHGROUP,         "Diffie-Hellman Group Key Exchange is invalid.", "Diffie-Hellman Group Key不合法"},
    {ERR_IPSEC_INVALID_KEYLIFETIME,         "Key Life Time is invalid.", "密钥有效时间不合法"},
    {ERR_IPSEC_INVALID_ENCRYKEY,         "Encryption Key is invalid.", "密钥不合法"},
    {ERR_IPSEC_ENCRY_CONFLICT,           "Encryption Algorithm and Encryption Key conflict",  "加密算法与加密密钥不匹配"},
    {ERR_IPSEC_INVALID_INTERFACE,         "WAN Interface is invalid.", "WAN接口路径不合法"},
    {ERR_IPSEC_INVALID_SPI,              "SPI is invalid.", "SPI不合法"},
    {0, NULL, NULL}
};



/*============================================================================
                      Debug Utils
=============================================================================*/
/*
*	函数名称: 	tbsGetMsgTypeName
*	函数功能: 	调试辅助函数，将数值形式MsgType转换为字符串可读形式。
*	输入参数: 	usMsgType - MsgType数值
*	返回值  :	对应的MsgType字符串名字。如果不在函数识别范围中，返回
*				UNKNOWN_MSGTYPE(ddd)格式。
*	创建    : 	匡素文 / 2008-11-10
*/
const char* tbsGetMsgTypeName(unsigned short usMsgType)
{
	#define MSG_TYPE_NAME(name) case name: return #name

	switch (usMsgType)
    {

        MSG_TYPE_NAME(MSG_POLL_EXIT);
        MSG_TYPE_NAME(MSG_POLL_EXIT_ACK);

        MSG_TYPE_NAME(MSG_PC_START_PROC);
        MSG_TYPE_NAME(MSG_PC_START_PROC_ACK);
        MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL);
        MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL_ACK);
        MSG_TYPE_NAME(MSG_PC_PROC_STATE);
        MSG_TYPE_NAME(MSG_PC_PROC_STATE_ACK);
        MSG_TYPE_NAME(MSG_PC_PROC_OUTPUT);
        MSG_TYPE_NAME(MSG_PC_PROC_OUTPUT_ACK);
        MSG_TYPE_NAME(MSG_PC_EXIT);
        MSG_TYPE_NAME(MSG_PC_EXIT_ACK);

        MSG_TYPE_NAME(MSG_CMM_GET_VAL);
        MSG_TYPE_NAME(MSG_CMM_GET_VAL_ACK);
        MSG_TYPE_NAME(MSG_CMM_SET_VAL);
        MSG_TYPE_NAME(MSG_CMM_SET_VAL_ACK);
        MSG_TYPE_NAME(MSG_CMM_ADD_NODE);
        MSG_TYPE_NAME(MSG_CMM_ADD_NODE_ACK);
        MSG_TYPE_NAME(MSG_CMM_DEL_NODE);
        MSG_TYPE_NAME(MSG_CMM_DEL_NODE_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_NAME);
        MSG_TYPE_NAME(MSG_CMM_GET_NAME_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_ATTR);
        MSG_TYPE_NAME(MSG_CMM_GET_ATTR_ACK);
        MSG_TYPE_NAME(MSG_CMM_SET_ATTR);
        MSG_TYPE_NAME(MSG_CMM_SET_ATTR_ACK);
        MSG_TYPE_NAME(MSG_CMM_INFORM_NOTI);
        MSG_TYPE_NAME(MSG_CMM_INFORM_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_NOTI);
        MSG_TYPE_NAME(MSG_CMM_GET_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_NOTI);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_COMMIT);
        MSG_TYPE_NAME(MSG_CMM_COMMIT_ACK);
        MSG_TYPE_NAME(MSG_CMM_CANCEL);
        MSG_TYPE_NAME(MSG_CMM_CANCEL_ACK);
        MSG_TYPE_NAME(MSG_CMM_UPDATE);
        MSG_TYPE_NAME(MSG_CMM_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_CMM_SAVE_CFG);
        MSG_TYPE_NAME(MSG_CMM_SAVE_CFG_ACK);
        MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG);
        MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG_ACK);
        MSG_TYPE_NAME(MSG_CMM_UPGRADE);
        MSG_TYPE_NAME(MSG_CMM_UPGRADE_ACK);
        MSG_TYPE_NAME(MSG_CMM_BACKUP);
        MSG_TYPE_NAME(MSG_CMM_BACKUP_ACK);
        MSG_TYPE_NAME(MSG_CMM_REBOOT);
        MSG_TYPE_NAME(MSG_CMM_REBOOT_ACK);

        MSG_TYPE_NAME(MSG_TIMER_REGSTER);
        MSG_TYPE_NAME(MSG_TIMER_REGSTER_ACK);
        MSG_TYPE_NAME(MSG_TIMER_UNREGSTER);
        MSG_TYPE_NAME(MSG_TIMER_UNREGSTER_ACK);
        MSG_TYPE_NAME(MSG_TIMER_TIMEOUT);
        MSG_TYPE_NAME(MSG_TIMER_TIMEOUT_ACK);

        MSG_TYPE_NAME(MSG_WAN_INTERFACE_STATE);
        MSG_TYPE_NAME(MSG_WAN_INTERFACE_STATE_ACK);
        MSG_TYPE_NAME(MSG_WAN_CONN_EST);
        MSG_TYPE_NAME(MSG_WAN_CONN_EST_ACK);
        MSG_TYPE_NAME(MSG_WAN_CONN_FIN);
        MSG_TYPE_NAME(MSG_WAN_CONN_FIN_ACK);
        MSG_TYPE_NAME(MSG_WAN_LINK_CHG);
        MSG_TYPE_NAME(MSG_WAN_LINK_CHG_ACK);

        MSG_TYPE_NAME(MSG_LAN_IP_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_IP_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_LAN_MACLIST_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_MACLIST_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_LAN_DOMAIN_NAME_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_DOMAIN_NAME_UPDATE_ACK);

        MSG_TYPE_NAME(MSG_GET_DHOST_LIST);
        MSG_TYPE_NAME(MSG_GET_DHOST_LIST_ACK);
        MSG_TYPE_NAME(MSG_DNS_CHANGE);
        MSG_TYPE_NAME(MSG_DNS_CHANGE_ACK);
        MSG_TYPE_NAME(MSG_PROUTE_SET_DNS);
        MSG_TYPE_NAME(MSG_PROUTE_SET_DNS_ACK);
        MSG_TYPE_NAME(MSG_WLAN_ADD_DEVICE);
        MSG_TYPE_NAME(MSG_WLAN_ADD_DEVICE_ACK);
        MSG_TYPE_NAME(MSG_WLAN_DEL_DEVICE);
        MSG_TYPE_NAME(MSG_WLAN_DEL_DEVICE_ACK);

        MSG_TYPE_NAME(MSG_TR069_SET_PARKEY);
        MSG_TYPE_NAME(MSG_TR069_SET_PARKEY_ACK);
        MSG_TYPE_NAME(MSG_TR069_GET_EVENTS);
        MSG_TYPE_NAME(MSG_TR069_GET_EVENTS_ACK);
        MSG_TYPE_NAME(MSG_TR069_CLEAR_EVENTS);
        MSG_TYPE_NAME(MSG_TR069_CLEAR_EVENTS_ACK);
        MSG_TYPE_NAME(MSG_TR069_DOWNLOAD);
        MSG_TYPE_NAME(MSG_TR069_DOWNLOAD_ACK);
        MSG_TYPE_NAME(MSG_TR069_UPLOADLOAD);
        MSG_TYPE_NAME(MSG_TR069_UPLOADLOAD_ACK);
        MSG_TYPE_NAME(MSG_TR069_REBOOT);
        MSG_TYPE_NAME(MSG_TR069_REBOOT_ACK);
        MSG_TYPE_NAME(MSG_TR069_FACTORYRESET);
        MSG_TYPE_NAME(MSG_TR069_FACTORYRESET_ACK);
        MSG_TYPE_NAME(MSG_TR069_SCHEDULEINFORM);
        MSG_TYPE_NAME(MSG_TR069_SCHEDULEINFORM_ACK);
        MSG_TYPE_NAME(MSG_TR069_GET_TRANSFERCOMPLETEINFO);
        MSG_TYPE_NAME(MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK);

        MSG_TYPE_NAME(MSG_LBT_SET_STATE);
        MSG_TYPE_NAME(MSG_LBT_SET_STATE_ACK);
        MSG_TYPE_NAME(MSG_LBT_GET);
        MSG_TYPE_NAME(MSG_LBT_GET_ACK);

        MSG_TYPE_NAME(MSG_TR069_HTTPD_CONNECT);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CONNECT_ACK);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CREATE_UPFILE);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CREATE_UPFILE_ACK);

        MSG_TYPE_NAME(MSG_ETHNET_LINK_STATE);
        MSG_TYPE_NAME(MSG_ETHNET_LINK_STATE_ACK);
        MSG_TYPE_NAME(MSG_ETHWAN_INTF_NAME);
        MSG_TYPE_NAME(MSG_ETHWAN_INTF_NAME_ACK);

        MSG_TYPE_NAME(MSG_MON_INTF_REGISTER);
        MSG_TYPE_NAME(MSG_MON_INTF_REGISTER_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_UNREGISTER);
        MSG_TYPE_NAME(MSG_MON_INTF_UNREGISTER_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_QUERY);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_QUERY_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_INFORM);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_INFORM_ACK);

        MSG_TYPE_NAME(MSG_AUTH);
        MSG_TYPE_NAME(MSG_AUTH_ACK);

        MSG_TYPE_NAME(MSG_WAN_DEL_INST);
        MSG_TYPE_NAME(MSG_WAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_WAN_ADD_INST);
        MSG_TYPE_NAME(MSG_WAN_ADD_INST_ACK);
        MSG_TYPE_NAME(MSG_LAN_DEL_INST);
        MSG_TYPE_NAME(MSG_LAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_LAN_ADD_INST);
        MSG_TYPE_NAME(MSG_LAN_ADD_INST_ACK);
        MSG_TYPE_NAME(MSG_WLAN_DEL_INST);
        MSG_TYPE_NAME(MSG_WLAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_WLAN_ADD_INST);
        MSG_TYPE_NAME(MSG_WLAN_ADD_INST_ACK);

        MSG_TYPE_NAME(MSG_WLAN_ENABLE_CHANGE);
        MSG_TYPE_NAME(MSG_WLAN_ENABLE_CHANGE_ACK);
        MSG_TYPE_NAME(MSG_CMD);
        MSG_TYPE_NAME(MSG_CMD_ACK);
        MSG_TYPE_NAME(MSG_RESP);
        MSG_TYPE_NAME(MSG_RESP_ACK);
        MSG_TYPE_NAME(MSG_RESP_FRAGMENT);
        MSG_TYPE_NAME(MSG_RESP_FRAGMENT_ACK);
        MSG_TYPE_NAME(MSG_RESP_ERR);
        MSG_TYPE_NAME(MSG_RESP_ERR_ACK);

        MSG_TYPE_NAME(MSG_CMM_CLEAR_CFG);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_CFG_ACK);

        MSG_TYPE_NAME(MSG_BUTTON);
        MSG_TYPE_NAME(MSG_BUTTON_ACK);

        MSG_TYPE_NAME(MSG_SNTP_UPDATE_TIME);
        MSG_TYPE_NAME(MSG_SNTP_UPDATE_TIME_ACK);
        MSG_TYPE_NAME(MSG_SNTP_TIME_CHANGED);
        MSG_TYPE_NAME(MSG_SNTP_TIME_CHANGED_ACK);
        MSG_TYPE_NAME(MSG_NTPS_CHANGED);
        MSG_TYPE_NAME(MSG_NTPS_CHANGED_ACK);

        MSG_TYPE_NAME(MSG_TR111_DEVICE_ADD);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_ADD_ACK);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_DEL);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_DEL_ACK);

        MSG_TYPE_NAME(MSG_DIAGNOSTIC_COMPLETE);
        MSG_TYPE_NAME(MSG_DIAGNOSTIC_COMPLETE_ACK);

        MSG_TYPE_NAME(MSG_MSG4UDP_REGISTER);
        MSG_TYPE_NAME(MSG_MSG4UDP_REGISTER_ACK);
        MSG_TYPE_NAME(MSG_MSG4UDP_UNREGISTER);
        MSG_TYPE_NAME(MSG_MSG4UDP_UNREGISTER_ACK);
        MSG_TYPE_NAME(MSG_MSG4UDP_MESSAGE);
        MSG_TYPE_NAME(MSG_MSG4UDP_MESSAGE_ACK);

        MSG_TYPE_NAME(MSG_WAN_CONN_ENABLE_UPDATE);
        MSG_TYPE_NAME(MSG_WAN_CONN_ENABLE_UPDATE_ACK);

        MSG_TYPE_NAME(MSG_VLAN_ADD_BRIDGE);
        MSG_TYPE_NAME(MSG_VLAN_ADD_BRIDGE_ACK);
        MSG_TYPE_NAME(MSG_VLAN_DEL_BRIDGE);
        MSG_TYPE_NAME(MSG_VLAN_DEL_BRIDGE_ACK);
        MSG_TYPE_NAME(MSG_VLAN_BIND_PORT);
        MSG_TYPE_NAME(MSG_VLAN_BIND_PORT_ACK);
        MSG_TYPE_NAME(MSG_VLAN_UNBIND_PORT);
        MSG_TYPE_NAME(MSG_VLAN_UNBIND_PORT_ACK);


        MSG_TYPE_NAME(MSG_VDSL_BRIEF_STATUS_OF_PORT);
        MSG_TYPE_NAME(MSG_VDSL_BRIEF_STATUS_OF_PORT_ACK);
        MSG_TYPE_NAME(MSG_VDSL_EXTENDED_STATUS_OF_PORT);
        MSG_TYPE_NAME(MSG_VDSL_EXTENDED_STATUS_OF_PORT_ACK);
        MSG_TYPE_NAME(MSG_VDSL_STATUS);
        MSG_TYPE_NAME(MSG_VDSL_STATUS_ACK);
        MSG_TYPE_NAME(MSG_VDSL_SNR_STATUS);
        MSG_TYPE_NAME(MSG_VDSL_SNR_STATUS_ACK);
        MSG_TYPE_NAME(MSG_VDSL_BME_FIRMWARE_VERSION);
        MSG_TYPE_NAME(MSG_VDSL_BME_FIRMWARE_VERSION_ACK);

        MSG_TYPE_NAME(MSG_LAN_ETH_DEL_INST);
        MSG_TYPE_NAME(MSG_LAN_ETH_ADD_INST);

        MSG_TYPE_NAME(MSG_PORTOFF_TRIGGER);

        MSG_TYPE_NAME(MSG_CMM_INST_ADDED);
        MSG_TYPE_NAME(MSG_CMM_INST_DELED);

     	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MSGTYPE(%04x)", usMsgType);
			return s_szUnsupported;
		}
	}
}

/*
*	函数名称: 	tbsGetMIDName
*	函数功能: 	调试辅助函数，将数值形式MID转换为字符串可读形式。
*	输入参数: 	usMID - MID数值
*	返回值  :	对应的MID字符串名字。如果不在函数可识别范围中，返回
*				UNKNOWN_MID(ddd)格式。
*	创建    : 	匡素文 / 2008-11-10
*/
const char* tbsGetMIDName(unsigned short usMID)
{
	#define MID_NAME(name) case name: return #name
	switch (usMID)
    {
        MID_NAME(MID_CCP);
        MID_NAME(MID_AUTH);
        MID_NAME(MID_ATM);
        MID_NAME(MID_IGMP);
        MID_NAME(MID_CMM);
        MID_NAME(MID_LAN);
        MID_NAME(MID_IPT);
        MID_NAME(MID_ETHLAN);
        MID_NAME(MID_ETHWAN);
        MID_NAME(MID_PPPOE);
        MID_NAME(MID_WLAN);
        MID_NAME(MID_TR069BE);
        MID_NAME(MID_DGN);
        MID_NAME(MID_DHCPR);
        MID_NAME(MID_DHCPS);
        MID_NAME(MID_TIMER);
        MID_NAME(MID_IPCONN);
        MID_NAME(MID_FIREWALL);
        MID_NAME(MID_SNMPC);
        MID_NAME(MID_QOS);
        MID_NAME(MID_STATIC_ROUTING);
        MID_NAME(MID_VDSL);
        MID_NAME(MID_DNS);
        MID_NAME(MID_ALG);
        MID_NAME(MID_WAN);
        MID_NAME(MID_DROUTE);
        MID_NAME(MID_SNTP);
        MID_NAME(MID_VLAN);
        MID_NAME(MID_USB_MASS);
        MID_NAME(MID_LOG);
        MID_NAME(MID_FTPD);
        MID_NAME(MID_NATPRIO);
        MID_NAME(MID_WPS);
        MID_NAME(MID_ACL);
        MID_NAME(MID_UPNP);
        MID_NAME(MID_LSVLAN);
        MID_NAME(MID_PORTOFF);
        MID_NAME(MID_ANTIATTACK);
	    MID_NAME(MID_SPT);
        MID_NAME(MID_SUPP);
        MID_NAME(MID_TRACERT);
        MID_NAME(MID_WEBP);
        MID_NAME(MID_DIAG);
	    MID_NAME(MID_DEVCONFIG);	
	    MID_NAME(MID_PORTMAPPING);	
        MID_NAME(MID_URLFILTER);	
        MID_NAME(MID_WEB);
        MID_NAME(MID_LNB);
        MID_NAME(MID_CLI);
        MID_NAME(MID_TR069FE);
        MID_NAME(MID_DDNS);	
        MID_NAME(MID_CFG);	
        MID_NAME(MID_MACFILTER);
        MID_NAME(MID_BRIDGE_FILTER);
        MID_NAME(MID_IP_ACL);
        MID_NAME(MID_DEFAULTGW);
        MID_NAME(MID_SAMBA);
        MID_NAME(MID_TF_PORTMAPPING);
        MID_NAME(MID_TF_FIREWALL);	
        MID_NAME(MID_OS_INFO);	
        MID_NAME(MID_IPMACFILTER);	
        MID_NAME(MID_TR069_HTTPD);
        MID_NAME(MID_SNMPA);
		MID_NAME(MID_TR064FE);

        MID_NAME(MID_PC);
        MID_NAME(MID_ELM);
        MID_NAME(MID_UPG);
        MID_NAME(MID_VDSLD);
        MID_NAME(MID_DSL);
        MID_NAME(MID_TM);
		MID_NAME(MID_IPTUNNEL);
		MID_NAME(MID_GRE);
		MID_NAME(MID_MLD);
		MID_NAME(MID_TF_GUI);
        //MID_NAME(MID_MON);
        //MID_NAME(MID_LBT);
        MID_NAME(MID_UPGCGI);
        MID_NAME(MID_FTPUPG);
        MID_NAME(MID_PTI);
        MID_NAME(MID_MSG4UDP);
        MID_NAME(MID_TFTPUPG);
        MID_NAME(MID_DEVINFO);

        MID_NAME(MID_GRP_CCP);

        MID_NAME(MID_GRP_BASE);

        MID_NAME(MID_GRP_MACLIST_UPDATE);
        MID_NAME(MID_GRP_MON_LINK_UPDATE);
        MID_NAME(MID_GRP_WAN_LINK_UPDATE);
        MID_NAME(MID_GRP_WAN_CONN_UPDATE);
        MID_NAME(MID_GRP_DNS_UPDATE);
        MID_NAME(MID_GRP_WAN_DEL_INST);
        MID_NAME(MID_GRP_WAN_ADD_INST);
        MID_NAME(MID_GRP_BUTTON);
        MID_NAME(MID_GRP_TIME_CHANGED);
        MID_NAME(MID_GRP_WAN_CONN_ENABLE_UPDATE);
        MID_NAME(MID_GRP_LAN_DEL_INST);
        MID_NAME(MID_GRP_LAN_ADD_INST);

        MID_NAME(MID_GRP_WLAN_DEL_INST);
        MID_NAME(MID_GRP_WLAN_ADD_INST);
        MID_NAME(MID_GRP_LAN_ETH_INST_CHG);
        MID_NAME(MID_TR069DIAG);
        

        MID_NAME(MID_NULL);

	 	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MID(%04x)", usMID);
			return s_szUnsupported;
		}
	}
}


